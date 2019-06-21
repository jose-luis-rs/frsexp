// -*- C++ -*-

TIMESTAMP_TITRIS(id)
{
	UINT32 header {
		0_11:  id = MATCH(id);
		12_32: 0;
	};
	UINT32 d1 {
		0_15:  t1;
		16_31: 0x00f7;
	};
	UINT32 d2 {
		0_15:  t2;
		16_31: 0x01f7;
	};
	UINT32 d3 {
		0_15:  t3;
		16_31: 0x02f7;
	};
}
