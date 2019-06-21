// -*- C++ -*-

EMPTY_32BIT_WORD_V775_FRS(geom)

{
  MEMBER(DATA32 data[32]);

  UINT32 header NOENCODE
    {
      0_23:  0x000000;
      24_26: 0b110; // invalid data marker
      27_31: geom=MATCH(geom);
    }
  
}
