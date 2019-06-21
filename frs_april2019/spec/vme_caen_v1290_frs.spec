// -*- C++ -*-

#ifndef VME_CAEN_V1x90_NO

#define VME_CAEN_V1x90_NAME_POSTFIX(postfix) VME_CAEN_V1290_FRS##postfix
#define VME_CAEN_V1x90_NO 1290
#include "vme_caen_v1290_frs.spec"
#undef VME_CAEN_V1x90_NAME_POSTFIX
#undef VME_CAEN_V1x90_NO

#define VME_CAEN_V1x90_NAME_POSTFIX(postfix) VME_CAEN_V1190_FRS##postfix
#define VME_CAEN_V1x90_NO 1190
#include "vme_caen_v1290_frs.spec"
#undef VME_CAEN_V1x90_NAME_POSTFIX
#undef VME_CAEN_V1x90_NO

#else

VME_CAEN_V1x90_NAME_POSTFIX(_SUBTDC)(tdc,event_id,data /* from parent */)
{
  // Should have our own data type...
  MEMBER(DATA24 data[32] ZERO_SUPPRESS_MULTI(128)); // comes from parent

  MARK_COUNT(tdc_start);

  UINT32 header
    {
      0_11:  bunch_id;
      12_23: event_id = CHECK(event_id);

      24_25: tdc = MATCH(tdc);
      27_31: 0b00001;
    }

  several UINT32 ch_data NOENCODE
    {
#if VME_CAEN_V1x90_NO == 1290
      0_20:  value;
      21_23: channel_low;
      24_25: tdc = CHECK(tdc);
#endif
#if VME_CAEN_V1x90_NO == 1190
      0_18:  value;
      19_23: channel_low;
      24_25: tdc = CHECK(tdc);
#endif
      26:    trailing;

      27_31: 0b00000;

#if VME_CAEN_V1x90_NO == 1290
      ENCODE(data[channel_low],(value=value));
#endif
#if VME_CAEN_V1x90_NO == 1190
      ENCODE(data[(tdc << 5) | channel_low],
	     (value=value/*,trailing=trailing*/));
#endif
    }

  optional UINT32 error_flags // error leads to namespace collision in the C++ code
    {
      0_14:  flags; // left as an exercise to sanitize this...

      24_25: tdc = CHECK(tdc);
      27_31: 0b00100;
    }

  MARK_COUNT(tdc_end);

  UINT32 trailer
    {
      0_11:  word_count = CHECK_COUNT(tdc_start,tdc_end,4,4);
      12_23: event_id = CHECK(header.event_id);

      24_25: tdc = CHECK(tdc);
      27_31: 0b00011;
    }
}

VME_CAEN_V1x90_NAME_POSTFIX()(geom)
{
  // Should have our own data type...
  MEMBER(DATA24 data[32] ZERO_SUPPRESS_MULTI(128));

  UINT32 ucesb_separator
    {
      0_4:   geom = MATCH(geom);
      24_31: 0x4c;
    }

  MARK_COUNT(v1190_start);

  UINT32 header
    {
      5_26:  event_number;
      27_31: 0b01000;
    }

  select several
    {
      tdc[0] = VME_CAEN_V1x90_NAME_POSTFIX(_SUBTDC)(tdc=0,event_id=(header.event_number&0xfff),data=data);
      tdc[1] = VME_CAEN_V1x90_NAME_POSTFIX(_SUBTDC)(tdc=1,event_id=(header.event_number&0xfff),data=data);
      tdc[2] = VME_CAEN_V1x90_NAME_POSTFIX(_SUBTDC)(tdc=2,event_id=(header.event_number&0xfff),data=data);
      tdc[3] = VME_CAEN_V1x90_NAME_POSTFIX(_SUBTDC)(tdc=3,event_id=(header.event_number&0xfff),data=data);
    }

  MARK_COUNT(v1190_end);

  UINT32 trailer
    {
      5_19:  word_count = CHECK_COUNT(v1190_start,v1190_end,4,4);
      24:    tdc_error;
      25:    buffer_overflow;
      26:    trigger_lost;
      27_31: 0b10000;
    }
}

// Data without TDC headers and footers.  Those headers and footers
// are bad, as they are emitted even if the TDC has no data!  2x4x4=32
// bytes useless!

VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC)(tdc,event_id,data /* from parent */)
{
  // Should have our own data type...
  MEMBER(DATA24 data[32] ZERO_SUPPRESS_MULTI(128)); // comes from parent

  UINT32 ch_data NOENCODE
    {
#if VME_CAEN_V1x90_NO == 1290
      0_20:  value;
      21_23: channel_low;
      24_25: tdc = CHECK(tdc);
#endif
#if VME_CAEN_V1x90_NO == 1190
      0_18:  value;
      19_23: channel_low;
      24_25: tdc = CHECK(tdc);
#endif
      26:    trailing;

      27_31: 0b00000;

#if VME_CAEN_V1x90_NO == 1290
      ENCODE(data[(tdc << 3) | channel_low],
	     (value=value/*,trailing=trailing*/));
#endif
#if VME_CAEN_V1x90_NO == 1190
      ENCODE(data[(tdc << 5) | channel_low],
	     (value=value/*,trailing=trailing*/));
#endif
    }
}

VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC_ERROR)(tdc)
{
  UINT32 error_flags // error leads to namespace collision in the C++ code
    {
      0_14:  flags; // left as an exercise to sanitize this...

      24_25: tdc = CHECK(tdc);
      27_31: 0b00100;
    }
}

VME_CAEN_V1x90_NAME_POSTFIX(_SHORT)(geom)
{
  // Should have our own data type...
  MEMBER(DATA24 data[32] ZERO_SUPPRESS_MULTI(128));

  MARK_COUNT(v1190_start);

  UINT32 header
    {
      0_4:   geom = MATCH(geom);
      5_26:  event_number;
      27_31: 0b01000;
    }

  select several
    {
      tdc[0] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC)(tdc=0,event_id=(header.event_number&0xfff),data=data);
      tdc[1] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC)(tdc=1,event_id=(header.event_number&0xfff),data=data);
      tdc[2] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC)(tdc=2,event_id=(header.event_number&0xfff),data=data);
      tdc[3] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC)(tdc=3,event_id=(header.event_number&0xfff),data=data);

      norevisit err[0] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC_ERROR)(tdc=0);
      norevisit err[1] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC_ERROR)(tdc=1);
      norevisit err[2] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC_ERROR)(tdc=2);
      norevisit err[3] = VME_CAEN_V1x90_NAME_POSTFIX(_SHORT_SUBTDC_ERROR)(tdc=3);
    }

  UINT32 trigger
    {
      0_26:  time_tag;
      27_31: 0b10001;
    }

  MARK_COUNT(v1190_end);

  UINT32 trailer
    {
      0_4:   geom = CHECK(geom);
      5_19:  word_count = CHECK_COUNT(v1190_start,v1190_end,4,4);
      24:    tdc_error;
      25:    buffer_overflow;
      26:    trigger_lost;
      27_31: 0b10000;
    }
}

#endif
