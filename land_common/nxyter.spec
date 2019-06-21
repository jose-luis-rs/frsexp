// -*- C++ -*-

NXYTER_PADDING()
{
	UINT32 pads_data NOENCODE
	{
		0_11:	counter;
		12_19:	0x01;
		20_31:	0xadd;
	}
}

NXYTER(gem,nxy)
{
	MEMBER(DATA8 gemex);
	MEMBER(DATA8 nxyter);
	MEMBER(DATA8 err);
	MEMBER(DATA16 trigger_counter_head);
	MEMBER(DATA16 trigger_counter_foot);
	//MEMBER(DATA24 ts_epoch_trigger);
	MEMBER(DATA16 ts_epoch_trigger);

	//for nxyter words
//	MEMBER(DATA12 adc[128] ZERO_SUPPRESS_MULTI(100));
//	MEMBER(DATA24 ts_epoch_hit[128] ZERO_SUPPRESS_MULTI(100));
//	MEMBER(DATA16 ts_nxy[128] ZERO_SUPPRESS_MULTI(100));


//	MEMBER(DATA12 channel[10000] ZERO_SUPPRESS);
//	MEMBER(DATA12 adc[10000] ZERO_SUPPRESS);
//	MEMBER(DATA24 ts_epoch_hit[10000] ZERO_SUPPRESS);
//	MEMBER(DATA16 ts_nxy[10000] ZERO_SUPPRESS);

	//use these for multi-hits
	MEMBER(DATA12 channel[10000] NO_INDEX_LIST);
	MEMBER(DATA12 adc[10000] NO_INDEX_LIST);
	MEMBER(DATA16 ts_epoch_hit[10000] NO_INDEX_LIST);
	MEMBER(DATA16 ts_nxy[10000] NO_INDEX_LIST);

	UINT32 header1 NOENCODE
	{
		0_7:    0b00110100;
		8_15:   trigger_tp;
		16_23:  id_gemex=MATCH(gem); //MATCH(gem)  RANGE(0,3)
		24_31:	id_nxyter=MATCH(nxy); //MATCH(nxy)  RANGE(0,1)

		ENCODE(gemex,(value=id_gemex));
		ENCODE(nxyter,(value=id_nxyter));
	}

	UINT32 header2 NOENCODE
	{
		0_31:	packet_data_size;
	}

	UINT32 header_data NOENCODE
	{
		0_15:	trigger_counter1;
		16_19:	rob;
		20_23:	empty_trt1;
		24_31:	0b10101010;

		ENCODE(trigger_counter_head,(value=trigger_counter1));
	}

	UINT32 local_epoche_ts1 NOENCODE
	{
		0_15:	epoche_time_stamp_a;
		16_23:	0b00000000;
		24_31:	0b00000000;
	}

	UINT32 local_epoche_ts2 NOENCODE // local in exploder/gemex
	{
		//24 bit trigger timestamps
//		0_23:	epoche_time_stamp_b;
//		24_31:	epoche_time_stamp_ab;

		//11 bit trigger timestamps
		0_10:	epoche_time_stamp_b;
		11_31:	epoche_time_stamp_ab;

		ENCODE(ts_epoch_trigger,(value=epoche_time_stamp_b));
	}

	several UINT64 nxyter_data NOENCODE
	{

		//11 bit word for epoche_ts = 16us = nxyter_ts length
		0_10:	nxyter_epoche_time_stamp;
		11_23:	nxyter_epoche_time_stamp2;
		24_30:	id_ch=RANGE(0,127); // MATCH(chan_id)   RANGE(0,127)
		31:		0b0;
		32_45:	nxyter_time_stamp;
		46:		of;
		47:		pu;
		48_59:	adc_data;  //=RANGE(0,3000)
		60_61:	0b0000;

//		ENCODE(adc[id_ch],(value=nxyter_data.adc_data));
//		ENCODE(ts_epoch_hit[id_ch],(value=nxyter_epoche_time_stamp));
//		ENCODE(ts_nxy[id_ch],(value=nxyter_data.nxyter_time_stamp));

		//use these for multi-hits
		ENCODE(channel APPEND_LIST,(value=id_ch));
		ENCODE(adc APPEND_LIST,(value=adc_data));
		ENCODE(ts_epoch_hit APPEND_LIST,(value=nxyter_epoche_time_stamp));
		ENCODE(ts_nxy APPEND_LIST,(value=nxyter_time_stamp));
	}

	//MARK_COUNT(adc_end);

	UINT32 errors NOENCODE
	{
		0_15:	trigger_counter_not_processed;
		16_23:	error_flags;
		24_31:	0b11101110; //0xee

		ENCODE(err,(value=error_flags));
	}

	UINT32 data_folder NOENCODE
	{
		0_15:	trigger_counter2;
		16_19:	rob;
		20_23:	empty_trt2;
		24_31:	0b10111011;

		ENCODE(trigger_counter_foot,(value=trigger_counter2));
	}
}
