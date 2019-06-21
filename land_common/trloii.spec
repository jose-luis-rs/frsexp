TRLOII_LMU_SCALERS(id)
{
	MEMBER(DATA32 before_lmu[63] ZERO_SUPPRESS);
	MEMBER(DATA32 before_lmu_mux[31] ZERO_SUPPRESS);
	MEMBER(DATA32 before_lmu_aux[31] ZERO_SUPPRESS);
	MEMBER(DATA32 before_dt[127] ZERO_SUPPRESS);
	MEMBER(DATA32 after_dt[127] ZERO_SUPPRESS);
	MEMBER(DATA32 after_reduction[127] ZERO_SUPPRESS);

	UINT32 header {
		0_5:   before_lmu;
		6_10:  before_lmu_mux;
		11_15: before_lmu_aux;
		16_23: after_lmu;
		24_31: id = MATCH(id);
	}

	list(0 <= i < header.before_lmu) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(before_lmu[i], (value=value));
		}
	}
	list(0 <= i < header.before_lmu_mux) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(before_lmu_mux[i], (value=value));
		}
	}
	list(0 <= i < header.before_lmu_aux) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(before_lmu_aux[i], (value=value));
		}
	}
	list(0 <= i < header.after_lmu) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(before_dt[i], (value=value));
		}
	}
	list(0 <= i < header.after_lmu) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(after_dt[i], (value=value));
		}
	}
	list(0 <= i < header.after_lmu) {
		UINT32 scaler NOENCODE {
			0_31: value;
			ENCODE(after_reduction[i], (value = value));
		}
	}
}

TRLOII_SAMPLER(mark)
{
	MEMBER(DATA32 sampler[512] ZERO_SUPPRESS);

	UINT32 header {
		0_9:   word_num;
		10:    overflow;
		16_31: mark = MATCH(mark);
	}

	list(0 <= index < header.word_num) {
		UINT32 time_lo NOENCODE {
			0_29: time;
			30:   dunno; // == 0?
			31:   overflow;
			ENCODE(sampler[index], (value = time));
		}
	}
}

TRLOII_SAMPLER_TWO(mark)
{
	MEMBER(DATA32 sampler_hi[512] ZERO_SUPPRESS);
	MEMBER(DATA32 sampler_lo[512] ZERO_SUPPRESS);

	UINT32 header {
		0_9:   word_num;
		10:    overflow;
		16_31: mark = MATCH(mark);
	}

	list(0 <= index < (header.word_num) / 2) {
		UINT32 time_lo NOENCODE {
			0_29: time;
			30:   dunno; // == 0?
			31:   overflow;
			ENCODE(sampler_lo[index], (value = time));
		}
		UINT32 time_hi NOENCODE {
			0_29: time;
			30:   dunno; // == 1?
			31:   overflow;
			ENCODE(sampler_hi[index], (value = time));
		}
	}

	if ((header.word_num % 2) == 1) {
		UINT32 last_word NOENCODE;
	}

}

TRLOII_TPAT(id)
{
	MEMBER(DATA32 n);
	MEMBER(DATA16 tpat[170] NO_INDEX_LIST);
	MEMBER(DATA8 trig[170] NO_INDEX_LIST);
	MEMBER(DATA32 ts_lo[170] NO_INDEX_LIST);
	MEMBER(DATA32 ts_hi[170] NO_INDEX_LIST);

	UINT32 header NOENCODE {
		0_11:  uint32_num;
		12_23: event_num;
		24_31: id = MATCH(id);
		ENCODE(n, (value=uint32_num / 3));
	}

	list(0 <= index < header.uint32_num / 3) {
		UINT32 time_lo NOENCODE {
			0_31: t;
			ENCODE(ts_lo APPEND_LIST, (value=t));
		}
		UINT32 time_hi NOENCODE {
			0_30: t;
			31:   overflow;
			ENCODE(ts_hi APPEND_LIST, (value=t));
		}
		UINT32 trigger NOENCODE {
			 0_15: tpat;
			16_23: dunno;
			24_27: encoded;
			28_31: lec;
			ENCODE(tpat APPEND_LIST, (value=tpat));
			ENCODE(trig APPEND_LIST, (value=encoded));
		}
	}
}
