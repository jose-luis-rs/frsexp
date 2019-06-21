TAMEX1_HEADER()
{
	UINT32 trigger_window
	{
		 0_15: post_trig_ns;
		16_31: pre_trig_ns;
	}
}

TAMEX1_PADDING()
{
	UINT32 padding NOENCODE
	{
		 0_11: counter;
		12_19: index;
		20_31: 0xadd;
	}
}

TAMEX1_SFP(sfp, card)
{
	//  0..15 = leading edge
	// 16..31 = trailing edge for ToT.
	// 32..47 = trailing edge for QTC.
	//     48 = will eventually be a time reference between DAQs.
	//     49 = scratch storage for epoch and error words.
	MEMBER(DATA12 time_coarse[50] ZERO_SUPPRESS_MULTI(200));
	MEMBER(DATA12 time_fine[50] ZERO_SUPPRESS_MULTI(200));

	UINT32 indicator NOENCODE
	{
		  0_7: 0x34;
		 8_11: trigger_type;
		12_15: sfp = MATCH(sfp);
		16_23: card = MATCH(card);
		24_31: 0;
	}

	MATCH_END;

	UINT32 data_size NOENCODE
	{
		0_31: bytes;
	}

	UINT32 tdc_header NOENCODE
	{
		 0_15: lec;
		   16: buf_no;
		//17_19: reserved;
		20_23: trigger_type;
		24_31: 0xaa;
	}

	list (0 <= i < data_size.bytes / 4 - 2)
	{
		UINT32 data NOENCODE
		{
			 0_10: coarse;
			   11: is_leading;
			// fine=0x3ff -> error, do not use.
			12_21: fine;
			22_28: channel_index;
			29_31: type;

			ENCODE(time_coarse[
			    (0x4 == type) * channel_index +
			    (0x4 != type) * 49
			    ], (value = coarse));
			ENCODE(time_fine[
			    (0x4 == type) * channel_index +
			    (0x4 != type) * 49
			    ], (value = fine));
		}
	}

	UINT32 trailer NOENCODE
	{
		 0_23: unused;
		24_31: 0xbb;
	}
}
