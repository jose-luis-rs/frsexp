// -*- C++ -*-


CAMAC_LECROY_2277_1CH_HACK(channel,edge,high_byte)
{
  MEMBER(DATA16 data[16] NO_INDEX_LIST /*ZERO_SUPPRESS_MULTI*/);

  UINT32 ch_data NOENCODE 
    {
      0_15:  value;
      16:    edge = CHECK(edge);  // 1=leading, 0=trailing
      17_21: channel = CHECK(channel);
      24_31: high = CHECK(high_byte);
      
      ENCODE(data APPEND_LIST,(value=value)); 
    }
}



SUBEVENT(LAND_CAMAC_PILEUP)
{
    select several
    {
	tdc = CAMAC_LECROY_2277_1CH_HACK(channel=0,edge=1,high_byte=0x00);
#ifdef MULTIHIT_TDC_2277_TCALMT
	tcal = CAMAC_LECROY_2277_1CH_HACK(channel=1,edge=1,high_byte=0x00);
#endif
    }
}

