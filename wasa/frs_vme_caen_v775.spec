// -*- C++ -*-

#define VME_CAEN_V792_FRS VME_CAEN_V775_FRS

VME_CAEN_V775_FRS(id,type,geom)
{
  MEMBER(DATA12_OVERFLOW data[32] ZERO_SUPPRESS);
  //MEMBER(DATA32 data[32] ZERO_SUPPRESS);


  UINT32 marker NOENCODE
  {
    8_31:     id = MATCH(id);
  }


  UINT32 marker1 NOENCODE
  {
    0_31:    0x00000022;
  }


  UINT32 header NOENCODE
    {
 
        0_5: count;
       14_15: 0;
       16_23: crate;
       24_26: 0b010;
       27_31: geom;
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
          27_31: geom;

	  	  ENCODE(data[channel],(value=value,overflow=overflow));
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
      27_31: geom;
     

      }

 

}

