// -*- C++ -*-

VME_STRUCK_SIS3302(id,fw)
{
  MEMBER(DATA32 data[8] ZERO_SUPPRESS);
  MEMBER(DATA32 base[8] ZERO_SUPPRESS);
  MEMBER(DATA8  pileup[8] ZERO_SUPPRESS);
  MEMBER(DATA8  triggered[8] ZERO_SUPPRESS);

  UINT32 header_gamma3 NOENCODE
  {
	0_7:  word_number;
	8_11: id = MATCH(id);
	12_15: channel;
	16_31: fw = MATCH(fw);
  }

  UINT32 header_1 NOENCODE
  {
	0_15: id;
	16_31: timestamp_hi;
  }

  UINT32 header_2 NOENCODE
  {
	0_15: timestamp_lo;
	16_31: timestamp_mid;
  }

  UINT32 ch_data NOENCODE
  {
	0_31:  value;

	ENCODE(data[header_gamma3.channel],(value=value));
  }

  UINT32 dummy NOENCODE;
  UINT32 dummy0 NOENCODE;

  UINT32 ch_base NOENCODE
  {
	0_31: value;
	ENCODE(base[header_gamma3.channel],(value=value));
  }

  UINT32 ch_info NOENCODE
  {
	0: trigger_flag;
	1_23: 0b00000000000000000000000;
	24_27: fast_trigger_counter;
	28: n_minus_one_trigger_flag;
	29: n_plus_one_trigger_flag;
	30: retrigger_flag;
	31: pileup_flag;
	ENCODE(pileup[header_gamma3.channel],(value=pileup_flag));
	ENCODE(triggered[header_gamma3.channel],(value=trigger_flag));
  }

  UINT32 ch_trailer NOENCODE
  {
	0_31: trailer = MATCH(0xdeadbeef);
  }
}
