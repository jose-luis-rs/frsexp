// -*- C++ -*-
#define VME_CAEN_V820_FRS VME_CAEN_V830_FRS

VME_CAEN_V830_FRS(geom)
{
  MEMBER(DATA32 data[32]);

  UINT32 header NOENCODE
    {
       0_15: trigger_number;
      16_17: 0;
      18_23: count;
      24_25: reserved;
         26: 1;
      27_31: geom = MATCH(geom);
    }

    list(0<=index<header.count)
    {
   UINT32 ch_data NOENCODE
        {
          0_31: value;
          ENCODE(data[index],(value=value));
	}
     }
}


