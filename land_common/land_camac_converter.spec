// -*- C++ -*-

SUBEVENT(LAND_CAMAC_CONVERTER)
{
  UINT16 tpat;
  UINT16 tprev;
  UINT16 tnext; 
  UINT16 tprev2;

  if (EXTERNAL has_data)
    {
#include "camac_c2.hh"
#include "camac_c3.hh"
    }
}

#include "ntp_tcal.spec"

SUBEVENT(LAND_CAMAC_TCAL_INFO)
{
  UINT32 info;

  select several
    {
      rtcal = RANDOM_TCAL(id = 0xe7);
      ntp   = NTP_MESSAGE(id = 0xef);
    }
}
