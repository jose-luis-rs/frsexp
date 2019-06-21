
#ifdef TRIGGERS_CLOCK_TCAL_12_13

#define LAND_EVENT_PHYSICS       1  
#define LAND_EVENT_SPILL_BEGIN   6
#define LAND_EVENT_SPILL_END     7
#define LAND_EVENT_CLOCK         12
#define LAND_EVENT_TCAL_CYCLE    13

#define LAND_EVENT_START_ACQ     14
#define LAND_EVENT_STOP_ACQ      15

#else

#define LAND_EVENT_PHYSICS       1  
#define LAND_EVENT_OFFSPILL      2
#define LAND_EVENT_CLOCK         3 ///< Clock pulse
#define LAND_EVENT_TCAL_CYCLE    4 ///< Time calibrator starts again

#define LAND_EVENT_SPILL_BEGIN_KEEP_ALIVE \
                                 10
#define LAND_EVENT_SPILL_END_KEEP_ALIVE \
                                 11
#define LAND_EVENT_SPILL_BEGIN   12
#define LAND_EVENT_SPILL_END     13

#define LAND_EVENT_START_ACQ     14
#define LAND_EVENT_STOP_ACQ      15

#endif
