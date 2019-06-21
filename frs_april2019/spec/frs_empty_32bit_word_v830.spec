// -*- C++ -*-

EMPTY_32BIT_WORD_V830_FRS(geom)

{
  MEMBER(DATA32 data[32]);

  UINT32 header NOENCODE
    {
  
      0_26:  0x6000000;
      27_31: geom=MATCH(geom);
    }
  
}
