// -*- C++ -*-

#define VME_CAEN_V792_FRS VME_CAEN_V775_FRS
#define VME_CAEN_V785_FRS VME_CAEN_V775_FRS

VME_CAEN_V775_FRS(geom)
{
  //MEMBER(DATA12_OVERFLOW data[32] ZERO_SUPPRESS);
  MEMBER(DATA12 data[32] ZERO_SUPPRESS);

  UINT32 header NOENCODE
    {
 
        8_13: count;
       14_15: 0;
       16_23: crate;
       24_26: 0b010;
       27_31: geom = MATCH(geom);
    }

    list(0<=index<header.count)
    {
      UINT32 ch_data NOENCODE
	{
	  0_11:  value;

          12:    overflow;
          13:    underflow;
          14:    valid;

          // 15: undefined;

          16_20: channel;

          24_26: 0b000;
          27_31: geom = MATCH(geom);

	  //ENCODE(data[channel],(value=value,overflow=overflow));
          ENCODE(data[channel],(value=value));
	}
    }



  
  UINT32 eob
    {
      // 0_31:  data;
      //24_26: 0b100;
      //27_31: geom = CHECK(geom);
      // NOENCODE;

      0_23:  event_number;
      24_26: 0b100;
      27_31: geom = MATCH(geom);
     

      }
}

