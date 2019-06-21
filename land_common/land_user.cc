
#include "structures.hh"

#include "user.hh"

#ifdef USE_CROS3_REWRITE
#include "threshold_cros3.c"
#endif

#include "land_triggers.hh"

#include "land_type_subtype.spec"

#include "lmd_output.hh"

#include <curses.h> // needed for the COLOR_ below

watcher_type_info land_watch_types[NUM_WATCH_TYPES] = 
  { 
    { COLOR_GREEN,   "Physics" }, 
    { COLOR_MAGENTA, "Offspill" }, 
    { COLOR_YELLOW,  "Tcal" }, 
    { COLOR_RED,     "Other" }, 
  };

/* code taken from land02
 */

#define PHILLIPS7120_PLUS__DC_OUT_10V   0x80 // else 5V
#define PHILLIPS7120_MINUS_DC_OUT_10V   0x40 // else 5V
#define PHILLIPS7120_CHARGE_MODE_MANUAL 0x20 // else CAMAC
#define PHILLIPS7120_PRIMARY_TDC        0x10 // else charge (Note: our module is broken, it always says 0x10, which is where the setting is!)
#define PHILLIPS7120_TIME_MODE_MANUAL   0x08 // else CAMAC
#define PHILLIPS7120_CHARGE_RANGE_300pC 0x04 // else 600 pC
#define PHILLIPS7120_TIME_RANGE_10us    0x01
#define PHILLIPS7120_TIME_RANGE_512ns   0x02
#define PHILLIPS7120_TIME_RANGE_1024ns  0x03
#define PHILLIPS7120_TIME_RANGE_MASK    0x03

#define D_TIME_NS(x) (x)

void recover_tcal_info(uint32 tcal_time_charge_info,
		       double &tcal_time,
		       double &tcal_charge)
{
  tcal_time = 0;
  tcal_charge = 0;

  if (tcal_time_charge_info == 0)
    return; // info was missing, e.g. due to separated DAQ

  // The 1st order byte is the setting
  // The 2nd byte is the secondary value
  // The high order word is the primary value (16 bits)

  // By masking the setting out from the 2nd byte, we get
  // both primary and secondary in the range 0 - 0x10000.

  uint8  setting   = (uint8)  (tcal_time_charge_info);
  uint16 secondary = (uint16) (tcal_time_charge_info & 0xff00);
  uint16 primary   = (uint16) (tcal_time_charge_info >> 16);

  // The plus/minus we do not care about

  uint32 time;
  uint32 charge;

  if (setting & PHILLIPS7120_PRIMARY_TDC)
    {
      time   = primary;
      charge = secondary;
    }
  else
    {
      time   = secondary;
      charge = primary;
    }

  // If charge mode manual, we have no idea, leave it as NAN

  if (!(setting & PHILLIPS7120_CHARGE_MODE_MANUAL))
    {
      // Charge mode is camac

      tcal_charge = (charge *
        (setting & PHILLIPS7120_CHARGE_RANGE_300pC ?
         300.0 / 0x10000 : 600.0 / 0x10000));
    }

  // If time mode manual, we have no idea, leave it as NAN

  if (!(setting & PHILLIPS7120_TIME_MODE_MANUAL))
    {
      switch(setting & PHILLIPS7120_TIME_RANGE_MASK)
        {
        case PHILLIPS7120_TIME_RANGE_10us:
          tcal_time = D_TIME_NS(10*1000.0) * (((double) time) / 0x10000);
          break;
        case PHILLIPS7120_TIME_RANGE_512ns:
          tcal_time = D_TIME_NS(512.0) * (((double) time) / 0x10000);
          break;
        case PHILLIPS7120_TIME_RANGE_1024ns:
          tcal_time = D_TIME_NS(1024.0) * (((double) time) / 0x10000);
#ifdef TIME_CALIBRATOR_CYCLES
          if (time %  (10*64) == 0 &&
              time >= 200*64 &&
              time <= 300*64)
            _tcal_is_peak = true;
#endif
          break;
        default:
          ERROR("Invalid tcal time range value.");
          break;
        }
    }

  /*
  INFO(0,"Tcal (%08x setting: %02x pri: %04x sec: %02x)  "
       "Time is: %5d %6.1f  Charge is: %5d %6.1f",
       _tcal_time_charge_info,
       setting,primary,secondary,
       time,_tcal_time,
       charge,_tcal_charge);
  */
}

uint64 ntp_t0 = 0;

void analyse_random_tcal_tstamp(RANDOM_TCAL_TSTAMP_CLOCK &tstamp,
				uint32 clock_bits,
				uint64 &t1, uint64 &t2,
				uint32 &cnt, uint32 &latch)
{
  /*
  printf ("s:%10d h:%10d c:%8d ",
	  tstamp.soft_latches,
	  tstamp.hard_latches,
	  tstamp.clock_ticks);
  */

  uint64 tbefore = (((uint64) tstamp.before.sec) << 32) + tstamp.before.frac;
  uint64 tafter  = (((uint64) tstamp.after.sec) << 32)  + tstamp.after.frac;

  uint64 dt = tafter - tbefore;

  double ddt = ((double) dt) * (1.0 / 0x100000000ll);

  printf (" %4.1fus",ddt*1000000.0);

  t1 = tbefore;
  t2 = tafter;
  cnt = tstamp.clock_ticks;
  latch = tstamp.soft_latches + tstamp.hard_latches;
}

void analyse_random_tcal(const char *host,RANDOM_TCAL &rtcal)
{
  if (!rtcal.header.u32)
    return;

  printf ("%s: p:%2d cb:%2d",
	  host,rtcal.header.pulses,rtcal.header.clock_bits);

  // zero-init to make compiler happy
  uint64 tA1 = 0, tA2 = 0, tB1 = 0, tB2 = 0;
  uint32 cntA = 0, cntB = 0;
  uint32 latchA = 0, latchB = 0;

  if (rtcal.header.has_tstamp1)
    analyse_random_tcal_tstamp(rtcal.tstamp1,rtcal.header.clock_bits,
			       tA1,tA2,cntA,latchA);
  if (rtcal.header.has_tstamp2)
    analyse_random_tcal_tstamp(rtcal.tstamp2,rtcal.header.clock_bits,
			       tB1,tB2,cntB,latchB);

  if (rtcal.header.has_tstamp1 && rtcal.header.has_tstamp2)
    {
      uint64 dt      = ((tB1 - tA1) + (tB2 - tA2)) / 2;
      uint32 cnts    = cntB - cntA;
      uint32 latches = latchB - latchA;

      if (ntp_t0 == 0)
	ntp_t0 = tA1;

      double ddt = ((double) dt) * (1.0 / 0x100000000ll);
      
      printf (" dt=%5.1fus cnt=%3d latch=%3d",
	      ddt*1000000.0,
	      cnts,
	      latches);
    }

  if (rtcal.header.has_tstamp1)
    {
      
      uint64 tnow = ((tA1-ntp_t0)+(tA2-ntp_t0))/2;
      double dtnow = ((double) tnow) * (1.0 / 0x100000000ll);

      printf (" t=%12.6f cnt=%10u latch=%10u",dtnow,cntA,latchA);
    }

  printf ("\n");
}

void analyse_ntp_message(const char *host,NTP_MESSAGE &msg)
{
  if (!msg.header.u32)
    return;

  char server_id[64];
  sprintf (server_id,"%d.%d.%d.%d",
	   (msg.server_id >> 24) & 0xff,
	   (msg.server_id >> 16) & 0xff,
	   (msg.server_id >>  8) & 0xff,
	   (msg.server_id      ) & 0xff);

  char ref_id[64];
  sprintf (ref_id,"%d.%d.%d.%d",
	   (msg.refid >> 24) & 0xff,
	   (msg.refid >> 16) & 0xff,
	   (msg.refid >>  8) & 0xff,
	   (msg.refid      ) & 0xff);

  printf ("%s: %15s(%15s): l:%d v:%d m:%d st:%2d p:%2d pr:%3d\n",
	  host,
	  server_id,ref_id,
	  msg.sspp.leap,
	  msg.sspp.version,
	  msg.sspp.mode,
	  msg.sspp.stratum,
	  msg.sspp.poll,
	  (sint8) msg.sspp.precision);

  double root_delay = msg.root_dly.sec + 
    msg.root_dly.frac * (1.0 / 0x10000ll);
  double root_disp = msg.root_dly.sec + 
    msg.root_dly.frac * (1.0 / 0x10000ll);

  printf ("%s: rdly:%6.3fms rdsp:%6.3fms",
	  host,
	  root_delay*1000.0,
	  root_disp*1000.0);

  uint64 reft  = (((uint64) msg.reftime.sec)  << 32) + msg.reftime.frac;
  uint64 csend = (((uint64) msg.origtime.sec) << 32) + msg.origtime.frac;
  uint64 srecv = (((uint64) msg.rectime.sec ) << 32) + msg.rectime.frac;
  uint64 ssend = (((uint64) msg.tmittime.sec) << 32) + msg.tmittime.frac;
  uint64 crecv = (((uint64) msg.rxtime.sec  ) << 32) + msg.rxtime.frac;

  uint64 refold = (srecv-reft);

  uint64 delay = (crecv - csend) - (ssend - srecv);
  uint64 offset = ((csend - srecv) + (crecv - ssend));
  
  double drefold = ((double) refold) * (1.0 / 0x100000000ll);
  double ddelay  = ((double) delay)  * (1.0 / 0x100000000ll);
  double doffset = ((sint32) offset) * (1.0 / 0x100000000ll) * 0.5;
  
  printf (" ref:%7.3fs dly:%6.3fms off:%6.3fms ",
	  drefold,ddelay*1000.0,doffset*1000.0);

  uint64 tnow = ((csend-ntp_t0)+(crecv-ntp_t0))/2;
  double dtnow = ((double) tnow) * (1.0 / 0x100000000ll);
  
  printf (" t=%12.6f \n",dtnow);

}








/* end code lifted */

void land_watcher_event_info(watcher_event_info *info,
			     unpack_event *event)
{
  info->_type = LAND_WATCH_TYPE_OTHER;


  // not from buffer time (when that gets fixed... for now: no-op)
  info->_info &= ~WATCHER_DISPLAY_INFO_TIME; 
  if (event->camac_scalers.timestamp)
    {
      info->_time = event->camac_scalers.timestamp;
      info->_info |= WATCHER_DISPLAY_INFO_TIME;
    }

  if (event->trigger == LAND_EVENT_PHYSICS)
    {
      if (event->camac.tpat & 0x00ff)
	{
	  info->_type = LAND_WATCH_TYPE_PHYSICS;
	  info->_display |= WATCHER_DISPLAY_SPILL_ON;
	}
      else
	{
	  info->_type = LAND_WATCH_TYPE_OFFSPILL;
	  info->_display |= WATCHER_DISPLAY_SPILL_OFF;
	}
    }
#ifdef LAND_EVENT_OFFSPILL
  else if (event->trigger == LAND_EVENT_OFFSPILL)
    {
      info->_type = LAND_WATCH_TYPE_OFFSPILL;
      info->_display |= WATCHER_DISPLAY_SPILL_OFF;
    }
#endif
  else if (event->trigger == LAND_EVENT_TCAL_CYCLE)
    {
      info->_type = LAND_WATCH_TYPE_TCAL;

#ifndef NO_TCAL_INFO
      double tcal_time, tcal_charge;

      recover_tcal_info(event->camac_tcal_info.info,
			tcal_time,tcal_charge);

      info->_range_loc = tcal_time;
      info->_info |= WATCHER_DISPLAY_INFO_RANGE;
#endif
    }
  else if (event->trigger == LAND_EVENT_SPILL_BEGIN)
    {
      /* We do not fire the EOS spill display on keep-alive events,
       * as the spill soon should go away anyhow, and then we loose
       * the statistics.
       */

      info->_display |= WATCHER_DISPLAY_SPILL_BOS;
    }
  else if (event->trigger == LAND_EVENT_SPILL_END
#ifdef LAND_EVENT_SPILL_END_KEEP_ALIVE
	   || 
	   event->trigger == LAND_EVENT_SPILL_END_KEEP_ALIVE
#endif
      )
    {
      info->_display |= WATCHER_DISPLAY_SPILL_EOS;
    }
}

bool land_correlation_event_info(unpack_event *event)
{
  if (event->trigger == LAND_EVENT_PHYSICS
#ifdef LAND_EVENT_OFFSPILL
      || event->trigger == LAND_EVENT_OFFSPILL
#endif
      )
    return true;

  return false;
}

#if defined(USE_CROS3_REWRITE)
void copy_output_cros3_event(lmd_event_out *event_out,
			    FILE_INPUT_EVENT *file_event,
			    unpack_event *event,
			    const select_event *sel)
{
  size_t data_length = 0;
  
  for (int i = 0; i < 2; i++)
    {
      size_t length = ((char *) event->cros3.ccb[i]._dest_end) -
        ((char *) event->cros3.ccb[i]._dest_buffer);

      data_length += length;
    }

  if (!data_length) // only write the subevent if there is some payload
    return;

  // First a subevent header
  
  // This must be global...  Or at least, the memory for it must be
  // available when the data is written to the output
  static lmd_subevent_10_1_host _header_add;
  
  _header_add._header.l_dlen =
    (sint32) SUBEVENT_DLEN_FROM_DATA_LENGTH(data_length);
  _header_add._header.i_type    = SUBEVENT_CROS3;
  _header_add._header.i_subtype = SUBEVENT_CROS3_DATA_REWRITE;
#if defined(__i386__) || defined(__x86_64__) || (__BYTE_ORDER == __LITTLE_ENDIAN)
  // choose the X86 marker, little endian
  _header_add.i_procid          = 9;  // hmm, this is a marker of the producer
#elif defined(__powerpc__) || (__BYTE_ORDER == __BIG_ENDIAN)
  // choose the RIO2 marker, big endian
  _header_add.i_procid          = 10; // hmm, this is a marker of the producer
#else
#error Do not know what to use as procid marker
#endif
  _header_add.h_subcrate        = 0;
  _header_add.h_control         = 5;  
  
  // obey command line selection
  if (!sel->accept_subevent(&_header_add))
    return;

  // add_chunk just remembers the pointers.  The data is not copied until
  // the actual buffer formatting is performed (after we've returned).
  event_out->add_chunk(&_header_add,sizeof(_header_add),false);
  
  for (int i = 0; i < 2; i++)
    {
      size_t length = ((char *) event->cros3.ccb[i]._dest_end) -
	((char *) event->cros3.ccb[i]._dest_buffer);
      
      event_out->add_chunk(event->cros3.ccb[i]._dest_buffer,length,false);
    }
}
#endif

#if defined(USE_DIAMOND_REWRITE)
void copy_output_diamond_event(lmd_event_out *event_out,
			    FILE_INPUT_EVENT *file_event,
			    unpack_event *event,
			    const select_event *sel)
{
  // We copy the data from

  // event->diamond.begin_tdc to event->diamond.end_tdc
  // event->diamond.begin_adc to event->diamond.end_adc
  // event->diamond.begin_apv25 to event->diamond.end_apv25

  // for the tdc or adc, we have to rewrite the geom such that the
  // tdc and adc can be differentiated.

  size_t data_length = 0;

  data_length += (event->diamond.end_tdc - event->diamond.begin_tdc);
  data_length += (event->diamond.end_adc - event->diamond.begin_adc);
  data_length += (event->diamond.end_apv25 - event->diamond.begin_apv25);

  if (!data_length) // only write the subevent if there is some payload
    return;

  // Add the 'std' failure/has_bits marker

  data_length += sizeof(uint32);

  // First a subevent header
  
  // This must be global...  Or at least, the memory for it must be
  // available when the data is written to the output
  static lmd_subevent_10_1_host _header_add;
  static uint32 _failure;

  _header_add._header.l_dlen = (sint32) SUBEVENT_DLEN_FROM_DATA_LENGTH(data_length);
  _header_add._header.i_type    = SUBEVENT_VME;
  _header_add._header.i_subtype = SUBEVENT_VME_DATA8800;

  // For big endian, use procid 10 (RIO2),
  // for little endian, use procid 9 (X86),

  if ((__BYTE_ORDER == __BIG_ENDIAN) ^
      event->diamond.subevent_info._swapping)
    {
      // For little endian, choose the X86 marker, little endian
      _header_add.i_procid = 9;
    }
  else
    {
      // For big endian, choose the RIO2 marker, big endian
      _header_add.i_procid          = 10;
    }
   
  _header_add.h_subcrate        = 1;
  _header_add.h_control         = 5;  

  // obey command line selection
  if (!sel->accept_subevent(&_header_add))
    return;

  // add_chunk just remembers the pointers.  The data is not copied until
  // the actual buffer formatting is performed (after we've returned).
  event_out->add_chunk(&_header_add,sizeof(_header_add),false);
  
  // Add the LAND 'std' VME failure marker

  _failure = 0;
  
  event_out->add_chunk(&_failure,sizeof(uint32),false);
 
  // Since we want to rewrite the data for the tdc (or the adc), we
  // have to make a copy of it (rewriting in-place is not nice, as it
  // could cause some major mess).  Note, due to the statically
  // allocated headers, we're anyhow not multi-event at a time safe,
  // so a static pointer to allocated memory is ok.

  static uint32 *_tdc_rewrite_data = NULL;
  static size_t  _tdc_rewrite_alloc = 0;

  size_t tdc_size = (event->diamond.end_tdc - event->diamond.begin_tdc);

  if (tdc_size > _tdc_rewrite_alloc)
    {
      uint32* np = (uint32*) realloc(_tdc_rewrite_data,tdc_size);

      if (!np)
	ERROR("Memory allocation failure");

      _tdc_rewrite_data = np;
      _tdc_rewrite_alloc = tdc_size;
    }
  
  // Rewrite the TDC geom field from 0x1f to 0x1c

  uint32 *src = (uint32*) event->diamond.begin_tdc;
  uint32 *dest = _tdc_rewrite_data;

  if (!event->diamond.subevent_info._swapping)
    for (int i = (int) (tdc_size / sizeof(uint32)); i; --i)
      *(dest++) = (*(src++) & 0x07ffffff) | (0x1c << 27);
  else
    for (int i = (int) (tdc_size / sizeof(uint32)); i; --i)
      *(dest++) = (*(src++) & 0xffffff07) | (0x1c << (27-24));

  // Add the TDC data to the output

  event_out->add_chunk(_tdc_rewrite_data,tdc_size,
		       event->diamond.subevent_info._swapping);
 
  // Just add the ADC and APV25 data

  event_out->add_chunk(event->diamond.begin_adc,
		       (event->diamond.end_adc - event->diamond.begin_adc),
		       event->diamond.subevent_info._swapping);
  event_out->add_chunk(event->diamond.begin_apv25,
		       (event->diamond.end_apv25 - event->diamond.begin_apv25),
		       event->diamond.subevent_info._swapping);
}
#endif

#if defined(USE_CROS3_REWRITE) || defined(DIAMOND_REWRITE)
bool copy_output_file_event(lmd_event_out *event_out,
			    FILE_INPUT_EVENT *file_event,
			    unpack_event *event,
			    const select_event *sel,
			    bool combine_event)	
{
  event_out->copy(file_event,sel,combine_event);

  // Then add a subevent for the rewritten data

#if defined(USE_CROS3_REWRITE)
  copy_output_cros3_event(event_out,file_event,event,sel);
#endif
#if defined(USE_DIAMOND_REWRITE)
  copy_output_diamond_event(event_out,file_event,event,sel);
#endif

  return true;
}
#endif
