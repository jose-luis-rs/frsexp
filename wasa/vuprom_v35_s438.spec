// -*- C++ -*-

VME_VUPROM_scaler(id)
{
  MEMBER(DATA32 datas[192] ZERO_SUPPRESS);

  UINT32 header NOENCODE
  { 
    0_31:    id = MATCH(id);
  }

  UINT32 header2 NOENCODE
  {
    0_31:    word_number;
  }

  list(0<=index<header2.word_number)
  {
    UINT32 ch_data NOENCODE
    {
      0_31:    value;
      ENCODE(datas[index],(value=value));
    }
  }


}



VME_VUPROM_tdc(id)
{
  MEMBER(DATA24 data[192] ZERO_SUPPRESS);

  UINT32 marker NOENCODE
  {
    0_31:    id = MATCH(id);
  }

  UINT32 header NOENCODE
  {
    0_31:    word_number;
  }

  list(0<=index<header.word_number)
  {
    UINT32 ch_data NOENCODE
    {
      0_9:    value;
      10_20:  notused;
      //16:  1;
      21_22:  mult;
      23_29:  channel;
      30_31:  0x3;

      ENCODE(data[channel],(value=value));
    }
  }
}

