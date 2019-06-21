// -*- C++ -*-

#ifndef MAX_TACQ
#define MAX_TACQ 30
#endif

TACQUILA_DATA(sam,gtb)
{
  MEMBER(DATA12 tac[MAX_TACQ][17] ZERO_SUPPRESS);
  MEMBER(DATA12 adc[MAX_TACQ][17] ZERO_SUPPRESS);
  MEMBER(DATA12 clk[MAX_TACQ][17] ZERO_SUPPRESS);
  
  UINT32 header
  {
    0_8:	count;
    9_11:	dummy1;					
    12_15:	trigger_tac = RANGE(0,1);
    16_19:	trigger_sam = RANGE(1,2);
    20_23:	lec;		// local event counter
    24_27:	gtb = MATCH(gtb);
    28_31:	sam = MATCH(sam);
  }
  
  list(0 <= index < header.count)
  {    
    if (!(index & 1))
    {
      UINT32 ch_data NOENCODE
      {
	0_11:	tac_value;
	12_17:	clock_cycle;
	18_19:	dummy2;
	20:	trigger;
	21: 	0b0;
	22_26: 	channel	= RANGE(0,16);
	27_31: 	module 	= RANGE(1,MAX_TACQ);
	
	ENCODE(tac[module-1][channel],(value=tac_value));
	ENCODE(clk[module-1][channel],(value=clock_cycle));
      } 
      UINT32 ch_data2 NOENCODE
      {
	0_11:  	adc_value;
	12_19: 	dummy3;
	20:    	trigger;
	21: 	0b1;
	22_26: 	channel = RANGE(0,16);
	27_31: 	module 	= RANGE(1,MAX_TACQ);
	
	ENCODE(adc[module-1][channel],(value=adc_value));
      }
    }
  }
}


SUBEVENT(TACQUILA_LAND_SUBEVENT)
{
  select several
  {
    data_sam5_gtb0 = TACQUILA_DATA(sam=5,gtb=0); 
    data_sam6_gtb0 = TACQUILA_DATA(sam=6,gtb=0); 
    data_sam6_gtb1 = TACQUILA_DATA(sam=6,gtb=1); 
  }
}

