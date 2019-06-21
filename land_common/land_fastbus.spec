// -*- C++ -*-

SUBEVENT(LAND_FASTBUS_ERROR)
{
  UINT32 error_code
    {
      0:  timeout_waiting_smi;
      1:  illegal_block_transfer;
      2:  fifa_address_overflow;
      3:  fifa_register_error;
      16: init_error;
    }
  UINT32 error_num;
}

SUBEVENT(LAND_FASTBUS_ERROR_NGF)
{
  UINT32 magic
    {
      0_31: 0x10000000;
    };
  UINT32 error_code
    {
      0:  error;
      1:  uninitialized;
      2:  ss1;
      3:  wc;
      4:  mid;
      5:  fifo_empty;
      6:  fifo_not_empty;
      7:  not_finished;
      8:  bad_end_addr;
      9:  word_lost;
    };
}

SUBEVENT(LAND_FASTBUS1)
{
#ifdef LAND_FASTBUS_FIRST_WORD_0
  UINT32 zero NOENCODE { 0_31: 0; }
#endif

  select several
    {
#include "fastbus_c1.hh"
    }
}

#ifndef NO_LAND_FASTBUS2
SUBEVENT(LAND_FASTBUS2)
{
#ifdef LAND_FASTBUS_FIRST_WORD_0
  UINT32 zero NOENCODE { 0_31: 0; }
#endif

  select several
    {
#include "fastbus_c2.hh"
    }
}
#endif
