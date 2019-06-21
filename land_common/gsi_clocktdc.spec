GSI_CLOCKTDC_BAD00BAD()
{
	UINT32 bad00bad NOENCODE {
		0_31: bad00bad = MATCH(0xbad00bad);
	}
}

GSI_CLOCKTDC_FUSER(a_sfp0_num, a_sfp1_num, a_sfp2_num, a_sfp3_num, ch_per_ctdc)
{
	UINT32 slave_num {
		  0_7: sfp0_num = MATCH(a_sfp0_num);
		 8_15: sfp1_num = MATCH(a_sfp1_num);
		16_23: sfp2_num = MATCH(a_sfp2_num);
		24_31: sfp3_num = MATCH(a_sfp3_num);
	}
	UINT32 trig_window;
	UINT32 channel_num {
		0_31: num = MATCH(ch_per_ctdc);
	}
}

GSI_CLOCKTDC_PADDING()
{
	UINT32 padding NOENCODE {
		 0_19: whatever;
		20_31: 0xadd;
	}
}

GSI_CLOCKTDC_ITEM(sfp, tdc)
{
	MEMBER(DATA32 event_counter_lo32);
	MEMBER(DATA32 event_counter_hi16);
	MEMBER(DATA32 trigger_coarse_lo22);
	MEMBER(DATA32 trigger_coarse_hi24);
	// Entry 0 = channel 1 leading.
	// Entry 1 = channel 1 trailing.
	// Entry 2 = channel 2 leading.
	// ...
	// Entry 256 = trigger leading.
	// Entry 257 = dummy.
	MEMBER(DATA12 time_coarse[258] ZERO_SUPPRESS_MULTI(80));
	MEMBER(DATA12 time_fine[258] ZERO_SUPPRESS_MULTI(80));

	UINT32 header {
		  0_7: _0x34 = MATCH(0x34);
		 8_11: trigger_type;
		12_15: sfp_id = MATCH(sfp);
		16_23: tdc_id = MATCH(tdc);
		24_31: channel_id;
	}

	MATCH_END;

	UINT32 channel_num;
	UINT32 event_counter_lo_ NOENCODE {
		0_31: value;
		ENCODE(event_counter_lo32, (value = value));
	}
	UINT32 event_counter_hi_ NOENCODE {
		0_15: value;
		ENCODE(event_counter_hi16, (value = value));
	}
	UINT32 trigger_coarse_lo_ NOENCODE {
		  0_9: ignore;
		10_31: value;
		ENCODE(trigger_coarse_lo22, (value = value));
	}
	UINT32 trigger_coarse_hi_ NOENCODE {
		 0_23: value;
		24_31: ignore;
		ENCODE(trigger_coarse_hi24, (value = value));
	}
	UINT32 channel_header {
		  0_7: _0x34 = MATCH(0x34);
		 8_11: trigger_type;
		12_15: sfp_id = MATCH(sfp);
		16_23: tdc_id = MATCH(tdc);
		24_31: channel_id = RANGE(0, 128);
	}
	UINT32 data_byte_num;
	list (0 <= i < data_byte_num / 4) {
		UINT32 data {
			  0_3: time_fine;
			 4_15: time_coarse;
			   16: time_coarse_ext;
			   17: edge;
			// No doc and not used in Go4.
			   18: ws;
			// No doc and not used in Go4.
			   19: time_coarse_overflow;
			// No doc and not used in Go4.
			   20: rb_overflow;
			// Status != 4 -> dummy data.
			21_23: status;
			// Channel == 128 -> trigger.
			// Channel > 128 -> dummy data.
			24_31: channel;

			ENCODE(time_coarse[
			    (((128 > channel) || (128 == channel && 0 ==
						  edge)) && (4 == status)) *
			    (channel * 2 + edge) +
			    !(((128 > channel) || (128 == channel && 0 ==
						    edge)) && (4 == status)) *
			    257], (value = time_coarse));
			ENCODE(time_fine[
			    (((128 > channel) || (128 == channel && 0 ==
						  edge)) && (4 == status)) *
			    (channel * 2 + edge) +
			    !(((128 > channel) || (128 == channel && 0 ==
						    edge)) && (4 == status)) *
			    257], (value = time_fine));
		}
	}
}
