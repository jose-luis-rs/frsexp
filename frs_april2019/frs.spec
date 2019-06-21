// -*- C++ -*-
//
// Specification file for FRS Commissioning 2019
//
// (Jose Luis Rodriguez Sanchez, 22nd March 2019) 
//
//-------------------------------------------------------------------
//
// Include common files for the standard modules
#include "spec/land_std_vme.spec"
#include "../land_common/barrier.spec"
#include "../land_common/trloii.spec"
#include "../land_common/whiterabbit.spec"

// Include specification files for the FRS electronic modules
#include "spec/frs_vme_caen_v830.spec"
#include "spec/frs_vme_caen_v775.spec"
#include "spec/vme_caen_v1290_frs.spec"

// Our event consists of 5 subevents which are obtained from three VME crates
// The subevents are separated by the VME processor ID and type
EVENT
{
  master_ts = wr_100(type=12, subtype=1, control=19);
  master_tpat = tpat_subev(type=36, subtype=3600, control=19);
  vme0 = FRS_MASTER(type=88, subtype=8800, procid=10);
  vme1 = FRS_CRATE(type=88,subtype=8800,procid=30);
  vme2 = TPC_CRATE(type=88,subtype=8800,procid=20);
  ignore_unknown_subevent;
}

// Information about trigger
SUBEVENT(tpat_subev)
{
	select several {
		tpat = TRLOII_TPAT(id=0xcf);
	}
}

// Time stamp
SUBEVENT(wr_100)
{
	ts100 = TIMESTAMP_WHITERABBIT(id=0x100);
	wr_multi = WR_MULTI();
}

// Master VME crate
SUBEVENT(FRS_MASTER)
{
  header = LAND_STD_VME();
  barrier = BARRIER();
  select several {
      scaler_0 = VME_CAEN_V830_FRS(geom=8);
      qdc_0 = VME_CAEN_V792_FRS(geom=11);
      tdc_0 = VME_CAEN_V1290_FRS(geom=16); 
  }

}

// FRS VME crate
SUBEVENT(FRS_CRATE)
{
  header = LAND_STD_VME();
  select several {
      scaler_0 = VME_CAEN_V830_FRS(geom=0);    
      tdc_0 = VME_CAEN_V775_FRS(geom=8);
      tdc_1 = VME_CAEN_V775_FRS(geom=9);
      adc_0 = VME_CAEN_V785_FRS(geom=3);
      adc_1 = VME_CAEN_V785_FRS(geom=12);
    }
}

// TPC VME crate
SUBEVENT(TPC_CRATE)
{
  header = LAND_STD_VME();
  select several
    { 
      tdc_0 = VME_CAEN_V775_FRS(geom=8);
      tdc_1 = VME_CAEN_V775_FRS(geom=9);
      adc_0 = VME_CAEN_V785_FRS(geom=3);
      adc_1 = VME_CAEN_V785_FRS(geom=16);
    }
}
#include "mapping_timestamp.hh"
#include "mapping_frs_april2019.h"
