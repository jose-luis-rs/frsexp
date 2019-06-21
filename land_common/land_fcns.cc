
#include "structures.hh"

#include "land_triggers.hh"

#include "event_base.hh"
#include "worker_thread.hh"

uint32 LAND_CAMAC_SCALER::has_timestamp() const
{
  if (CURRENT_EVENT->_unpack.trigger == LAND_EVENT_SPILL_BEGIN ||
      CURRENT_EVENT->_unpack.trigger == LAND_EVENT_SPILL_END ||
#ifdef LAND_EVENT_SPILL_BEGIN_KEEP_ALIVE
      CURRENT_EVENT->_unpack.trigger == LAND_EVENT_SPILL_BEGIN_KEEP_ALIVE ||
#endif
#ifdef LAND_EVENT_SPILL_END_KEEP_ALIVE
      CURRENT_EVENT->_unpack.trigger == LAND_EVENT_SPILL_END_KEEP_ALIVE ||
#endif
      CURRENT_EVENT->_unpack.trigger == LAND_EVENT_START_ACQ ||
      CURRENT_EVENT->_unpack.trigger == LAND_EVENT_STOP_ACQ)
    return 1;
  return 0;
}

uint32 LAND_CAMAC_CONVERTER::has_data() const
{
#ifdef LAND_EVENT_OFFSPILL
  if (CURRENT_EVENT->_unpack.trigger == LAND_EVENT_OFFSPILL)
    return 0;
#endif
  return 1;
}
