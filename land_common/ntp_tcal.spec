// -*- C++ -*-

NTP64_TIME()
{
  UINT32 sec;  // seconds
  UINT32 frac; // fraction of seconds (in 1/2^32 s)
}

NTP_MESSAGE(id)
{
  UINT32 header
    {
      0_15:     word_count;
      // 16_24: flags
      16:       quick_burst;
      24_31:    id = MATCH(id);
    }

  MARK_COUNT(start);
  
  UINT32 server_id; // similar as refid

  // And here comes the NTP packet itself
  UINT32 sspp
    {
      0_7:      precision;
      8_15:     poll;
      16_19:    stratum;
      //20_23   high part of stratum, 0
      24_26:    mode;
      27_29:    version = 3;
      30_31:    leap;
    }

  UINT32 root_dly
    {
      16_31:    sec;
      0_15:     frac;
    }
  UINT32 root_disp
    {
      16_31:    sec;
      0_15:     frac;
    }
  UINT32 refid;

  reftime  = NTP64_TIME();
  origtime = NTP64_TIME();  // transmit by thread
  rectime  = NTP64_TIME();  // receive by server
  tmittime = NTP64_TIME();  // transmit by server
  // The NTP packet itself ends here

  rxtime   = NTP64_TIME();  // receive by thread

  if (header.quick_burst)
    {
      orig2time = NTP64_TIME();  // transmit by thread
      rec2time  = NTP64_TIME();  // receive by server
      tmit2time = NTP64_TIME();  // transmit by server
      rx2time   = NTP64_TIME();  // receive by thread
    }

  MARK_COUNT(end);

  CHECK_COUNT(header.word_count,start,end,0,4);
}



RANDOM_TCAL_TSTAMP_CLOCK()
{
  // number of times clock scaler has been latched (including current)
  UINT32   soft_latches; // by software 
  UINT32   hard_latches; // by hardware (trigger)
  UINT32   clock_ticks;  // count at latch
  before = NTP64_TIME();    // time before latch of clock_ticks
  after  = NTP64_TIME();    // time after latch of clock_ticks
}


RANDOM_TCAL(id)
{
  UINT32 header
    {
      0_7:      word_count; // for easy skip

      8_11:     pulses;     // pulses sent (many if doing resends on failures)
      12_17:    clock_bits; // number of bits (usually 24 or 32) in the clock

      18:       has_tstamp1;
      19:       has_tstamp2;
      
      24_31:    id = MATCH(id);
    }

  MARK_COUNT(start);

  if (header.has_tstamp1)
    {
      tstamp1 = RANDOM_TCAL_TSTAMP_CLOCK(); // usually before pulse
    }
  if (header.has_tstamp2)
    {
      tstamp2 = RANDOM_TCAL_TSTAMP_CLOCK(); // usually after  pulse
    }

  MARK_COUNT(end);

  CHECK_COUNT(header.word_count,start,end,0,4);
}

