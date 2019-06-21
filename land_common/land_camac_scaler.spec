// -*- C++ -*-

SOFT_SCALER32(channels)
{
  MEMBER(DATA32 data[32] ZERO_SUPPRESS);

  list(0<=index<channels)
    {
      UINT32 ch_data NOENCODE
        {
          0_31:  value;
          
          ENCODE(data[index],(value=value));
        }
    }
}

SUBEVENT(LAND_CAMAC_SCALER)
{
  scaler0 = CAMAC_LECROY_4434(channels=32);
  scaler1 = CAMAC_LECROY_4434(channels=32);
  scaler2 = CAMAC_LECROY_4434(channels=32);
#ifdef SOFT_SCALER32_CHANNELS
  scaler3 = SOFT_SCALER32(channels=SOFT_SCALER32_CHANNELS);
#endif

  // On special conditions, the event ends here...

  if (EXTERNAL has_timestamp)
    {
      UINT32 timestamp;
      UINT32 endianess { 0_31: 0x87654321; }
    }
}

SUBEVENT(LAND_CAMAC_START_STOP_STAMP)
{
  UINT32 timestamp;
  UINT32 endianess { 0_31: 0x87654321; }
}
