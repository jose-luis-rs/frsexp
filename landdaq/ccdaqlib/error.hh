/* -*- C++ -*- */

#ifndef __ERROR_HH__
#define __ERROR_HH__

#ifdef USE_LOCAL_ERROR_HANDLING
#define ERR__MSG_INFO  0
#define ERR__MSG_ERROR 0

#ifndef DONT_DECLARE_MBS_TYPES
typedef unsigned int  INTU4;
typedef int           INTS4;
typedef char          CHARS;
#endif

#define MASK__PRTT    0
#define MASK__PRTSLOG 0
#else
#include <errnum_def.h>
#include <err_mask_def.h>
#endif

#define LOG(str)   C_F_ERROR(ERR__MSG_INFO,MASK__PRTSLOG,str)
#define INFO(str)  C_F_ERROR(ERR__MSG_INFO,MASK__PRTT,str)
#define ERROR(str) C_F_ERROR(ERR__MSG_ERROR,MASK__PRTT,str)

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define LOG_FMT(str,__VA_ARGS__...)   C_F_ERROR_FMT(ERR__MSG_INFO,MASK__PRTSLOG,str,__VA_ARGS__)
#define INFO_FMT(str,__VA_ARGS__...)  C_F_ERROR_FMT(ERR__MSG_INFO,MASK__PRTT,str,__VA_ARGS__)
#define ERROR_FMT(str,__VA_ARGS__...) C_F_ERROR_FMT(ERR__MSG_ERROR,MASK__PRTT,str,__VA_ARGS__)
#else
#define LOG_FMT(str,...)   C_F_ERROR_FMT(ERR__MSG_INFO,MASK__PRTSLOG,str,__VA_ARGS__)
#define INFO_FMT(str,...)  C_F_ERROR_FMT(ERR__MSG_INFO,MASK__PRTT,str,__VA_ARGS__)
#define ERROR_FMT(str,...) C_F_ERROR_FMT(ERR__MSG_ERROR,MASK__PRTT,str,__VA_ARGS__)
#endif

#if DEBUG_MESSAGES // define it before including <error.hh>
#define DEBUG(str) INFO(str)
#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define DEBUG_FMT(str,__VA_ARGS__...)  INFO_FMT(str,__VA_ARGS__)
#else
#define DEBUG_FMT(str,...)  INFO_FMT(str,__VA_ARGS__)
#endif
#else
#define DEBUG(str) 
#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define DEBUG_FMT(str,__VA_ARGS__...)
#else
#define DEBUG_FMT(str,...)
#endif
/* Note: DO NOT put a DEBUG in the readout path and F_DEBUG and think
 * that a separate sprintf will not eat CPU time when compiled without
 * DEBUG_MESSAGES
 */
#endif


void C_F_ERROR(INTS4 errnum,INTU4 mask,const char *str);
void C_F_ERROR_FMT(INTS4 errnum,INTU4 mask,const char *str,...);

#endif /* __ERROR_HH__ */


