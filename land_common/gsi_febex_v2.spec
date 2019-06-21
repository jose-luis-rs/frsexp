FEBEX_EVENTHEADER()
{
	select {
		good = FEBEX_GOOD_EVENTHEADER();
		bad = FEBEX_BAD_EVENTHEADER();
	}
}

FEBEX_BAD_EVENTHEADER()
{
	UINT32 badbad {
		0_31: 0xbad00bad;
	}
}

FEBEX_GOOD_EVENTHEADER()
{
	UINT32 febexcards {
		0_7:   n_febex_sfp0 = RANGE(0, 64);
		8_15:  n_febex_sfp1 = RANGE(0, 64);
		16_23: n_febex_sfp2 = RANGE(0, 64);
		24_31: n_febex_sfp3 = RANGE(0, 64);
	}
	UINT32 trig_length {
		0_15:  trace_length;
		16_31: trigger_delay;
	}
	UINT32 energy_filter {
		0_10:  sum_b;
		11_20: gap;
		21_31: sum_a;
	}
	UINT32 polarity1 {
		0_31: pola;
	}
	UINT32 polarity2 {
		0_31: pola;
	}
	UINT32 polarity3 {
		0_31: pola;
	}
	UINT32 polarity4 {
		0_31: pola;
	}
}

FEBEX_PADDING()
{
	UINT32 pads_data NOENCODE {
		0_11:  counter;
		12_19: index;
		20_31: 0xadd;
	}
}

FEBEX_NOTRACE(sfp, card)
{
	MEMBER(DATA8 n_hit[16]);
	MEMBER(DATA12 t[16] ZERO_SUPPRESS_MULTI(32));
	MEMBER(DATA32 e[16] ZERO_SUPPRESS_MULTI(32));

	UINT32 indicator NOENCODE {
		0_7:   0x34;
		8_11:  trigger_type;
		12_15: sfp =  MATCH(sfp);
		16_23: card = MATCH(card);
		24_31: 0xff;
	}

	UINT32 data_size NOENCODE {
		0_31: size;
	}

	UINT32 head NOENCODE {
		0_23:  unused;
		24_31: 0xaf;
	}

	UINT32 triggertime NOENCODE {
		0_31: trigger_time;
	}

	UINT32 time NOENCODE {
		0_31: time;
	}

	list (0 <= i < ((data_size.size - 16) >> 3)) {
		UINT32 time NOENCODE {
		         0_10: time;    // from go4.  before: 0_11:  time;
			11_14: unused1;
			15: sign;
			16_19: unused2;
			20_23: multi_hit;
			24_27: n_hit;
			28_31: channel_id;
			ENCODE(t[channel_id], (value = (sign * -time + (sign - 1) * -time) & 0xfff) );
			ENCODE(n_hit[channel_id], (value = n_hit));
		}

		UINT32 energy NOENCODE {
			0_21:  energy;
			22:    unused1;
			23:    sign;
			24_27: unused2;
			28_31: channel_id;
			ENCODE(e[channel_id], (value = sign * -energy + (sign - 1) * -energy ));
		}
	}

	UINT32 trailer NOENCODE {
		0_23:  unused;
		24_31: 0xbf;
	}
}

FEBEX_TRACE(sfp, card)
{
	MEMBER(DATA32 trace_length[16] ZERO_SUPPRESS);
	MEMBER(DATA16 trace[16][1000] ZERO_SUPPRESS);
        MEMBER(DATA32 filter[16][1000] ZERO_SUPPRESS);

	UINT32 header NOENCODE {
		  0_7: 0x34;
		 8_11: trigger_type;
		12_15: sfp =  MATCH(sfp);
		16_23: card = MATCH(card);
		24_31: channel_id = RANGE(0, 0xfe);
	}

	UINT32 trace_size NOENCODE {
		0_31: size;
		ENCODE(trace_length[header.channel_id], (value = size / 2 - 4));
	}

	UINT32 trace_header NOENCODE {
		 0_17: notused1;
		   18: filter_mode;
		   19: filter_onoff;
		20_22: notused2;
		   23: adc_type;
		24_31: 0xaa;
	}

	if (trace_header.filter_onoff == 0) {
		list (0 <= i < (trace_size.size / 4) - 2) {
			UINT32 channel_trace NOENCODE {
				 0_13: data1;
				14_15: nik_knows; // unknown data. !=0
				16_29: data2;
				30_31: nik_knows2; // unknown data. !=0

				ENCODE(trace[header.channel_id][2*i+0], (value = data1));
				ENCODE(trace[header.channel_id][2*i+1], (value = data2));
			}
		}
	} else {
		list (0 <= i < (trace_size.size / 8) - 1) {
			UINT32 channel_trace NOENCODE {
				 0_13: data1;
				14_31: nik_knows; // unknown data. !=0

				ENCODE(trace[header.channel_id][i], (value = data1));
			}
			UINT32 channel_filter NOENCODE {
				 0_22: data1;
				   23: sign;
				24_31: nik_knows2; // unknown data. !=0

				ENCODE(filter[header.channel_id][i], (value = data1*(1-2*sign)));
			}
		}
	}

	UINT32 trace_trailer NOENCODE {
		 0_23: notused;
		24_31: 0xbb;
	}
}
