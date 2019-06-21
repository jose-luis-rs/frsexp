// -*- C++ -*-

//          Specification file for WASA-FRS experiment
//
//(Jose Luis Rodriguez Sanchez, 24 September 2018) 
//

#include "vuprom_v35_s438.spec"
#include "frs_vme_caen_v775.spec"

SUBEVENT(WASA_VME)
{
	select several
	{
		scaler1 = VME_VUPROM_scaler(id=0xdc17bb00);
		scaler2 = VME_VUPROM_scaler(id=0xdc17bb01);
                qdc    = VME_CAEN_V792_FRS(id=0x0792aa,type=0x7,geom=5);
		tdc1    = VME_VUPROM_tdc(id=0xdc17aa00);
		tdc2    = VME_VUPROM_tdc(id=0xdc17aa01);
	}
}

EVENT
{
	wasa = WASA_VME(type=10, subtype=1, control=10);
}

#include "mapping.hh"
