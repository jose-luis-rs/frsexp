// -*- C++ -*-

VME_CAEN_V259_FRS(geom)
{
  MEMBER(DATA32 data[32] ZERO_SUPPRESS);

  UINT32 header NOENCODE
    {
      
     
       0_5:  count;
       24_26: 0b010;
       27_31:  geom = MATCH(geom);
      
    }

 list(0<=index<2)
   {
  
UINT32 ch_data_pattern NOENCODE
        {
        
        0_26:  value;
        27_31: geom = MATCH(geom);

	          ENCODE(data[index],(value=value));
	}
   }
 
     
  
   UINT32 eob
    {
      // 0_23:  event_number;
      24_26: 0x4;
      27_31: geom = MATCH(geom);
      
      //  0_26: 0b100000000000000000000000000;
      //  27_31: geom = MATCH(geom);
    }
  
}


