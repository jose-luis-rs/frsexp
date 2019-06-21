// -*- C++ -*-

VME_VUPROM_V35()
{
  MEMBER(DATA12 data[192] ZERO_SUPPRESS);

  UINT32 header NOENCODE
  {
    0_8:    word_number;
    9:      raw_marker;
    10_15:  0;
    16_23:  geom;
    24_31:  0xfe;
  }

  list(0<=index<header.word_number)
  {
    UINT32 ch_data NOENCODE
    {
      0_9:    value;
      10_15:  0;
      16_23:  channel;
      24_31:  geom;

      ENCODE(data[channel],(value=value));
    }
  }
}

