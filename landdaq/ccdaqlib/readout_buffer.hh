/* -*- C++ -*- */

#ifndef __READOUT_BUFFER_H__
#define __READOUT_BUFFER_H__

/* Convenience type definitions.
 */

typedef unsigned char      uint8;
typedef unsigned short     uint16;
typedef unsigned int       uint32;
typedef unsigned long long uint64;

/* A pointer to memory, accessible for different sizes.
 */

union mem_ptr
{
public:
  uint32 *p32;
  uint16 *p16;
  uint8  *p8;

public:
  inline void align16(uint8* relative = 0)
  {
    switch ((p8 - relative) & 3L)
      {
      case 1: *(p8++)  = 0x21;
	break;
      }
  }
  
  inline void align32(uint8* relative = 0)
  {
    switch ((p8 - relative) & 3L)
      {
      case 3: *(p8++)  = 0x65;
      case 2: *(p16++) =   0x4321;
	break;
      case 1: *(p8++)  =     0x21;
	break;
      }
  }
  
  inline void align64(uint8* relative = 0)
  {
    switch ((p8 - relative) & 3L)
      {
      case 7: *(p8++)  = 0xed;
      case 6: *(p16++) =   0xcba9;
	/**/  *(p32++) =       0x87654321;
      break;
      case 5: *(p8++)  =     0xa9;
      case 4: *(p32++) =       0x87654321;
	break;
      case 3: *(p8++)  =         0x65;
      case 2: *(p16++) =           0x4321;
	break;
      case 1: *(p8++)  =             0x21;
	break;
      }
  }
  
};

#endif /* __READOUT_BUFFER_H__ */

