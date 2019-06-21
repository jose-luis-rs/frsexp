
#ifndef __AUTO_PEDESTAL_ADJUST_CHANNEL__
#define __AUTO_PEDESTAL_ADJUST_CHANNEL__

#include "readout_buffer.hh"
#include "error.hh"
#include "util.hh"

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

/* Automatically determine the noise level in ADC/QDC channels.
 *
 * The idea is that we collect data from clock events, i.e. where
 * there would reasonably be no signals, only the zero-noise.
 *
 * We collect data for some time, and when there is enough statistics,
 * an quick analysis is made and new cut levels are determined.
 *
 * Note, our only purpose is to reduce the data-rate by letting
 * zero-suppression start at an reasonable level.  Preferably we
 * should deliver fairly stable values during the experiment,
 * otherwise there is some trouble.
 *
 * Note, when using such an adaptive scheme, it is HIGHLY recommended
 * to always have the ADC/QDCs store raw values, and not subtracted
 * values.  This way, to an first order, any analysis will not be
 * affected since we've not moved around the values.  However, if the
 * pedestals move, it actually means that the zero-level is moving.
 * We however for these case recommend that the analysis itself
 * investigate the clock events data to adapt to these changes.
 */

/* Our principle can be rather simple, as we'd like to cut away
 * e.g. 99% of the garbage values.  However, for safety, we also
 * require this cut level to be close to the main pedestal peak, and
 * not to be far up in case of something very broad.
 */

#ifndef NUM_PEDESTAL_DATA
#define NUM_PEDESTAL_DATA   128 // must be multiple of 2
#endif

inline int compare_uint16(const void *p1,const void *p2)
{
  int v1 = *((const uint16 *) p1);
  int v2 = *((const uint16 *) p2);
  
  return v1 - v2;
}

// This flag is never removed
#define AUTO_PED_INFO_DISABLED   0x0001 // Channel permanently disabled
// These flags are set depending on if the fitting works
#define AUTO_PED_INFO_NO_DATA    0x0002 // No or too little data (=> cut 0)
#define AUTO_PED_INFO_SMALL_DIFF 0x0004 // The differences found too small (=> cut 0)
#define AUTO_PED_INFO_TOO_SPREAD 0x0008 // The data is too spread around (=> cut 0)
#define AUTO_PED_INFO_PED_CALCED 0x0080 // Pedestal value was calculated
#define AUTO_PED_INFO_PED_LARGE  0x0010 // The pedestal is suspiciously high
#define AUTO_PED_INFO_PED_WIDE   0x0020 // The pedestal is suspiciously wide
#define AUTO_PED_INFO_PED_MOVED  0x0040 // The pedestal moved since last measurement

union apfc_info
{
  uint32 _data;
  void  *_ptr;
};

typedef int (*auto_pedestal_format_channel)(char *str,size_t size,apfc_info info);

#define APAC_CSCR_LOW_RANGE    1
#define APAC_CSCR_HIGH_RANGE   2
#define APAC_CSCR_AUTO_RANGE   3
#define APAC_CRATE_SLOT_CH_RNG(crate,slot,channel,range) ((crate << 24) | (slot << 16) | (channel << 8) | (range))

int apac_crate_slot_ch_rng(char *str,size_t size,apfc_info info);

struct auto_pedestal_info
{
public:
  float  _pedestal_mean;      // Pedestal mean
  float  _pedestal_mean_err2; // Error on the mean (_std/sqrt(n))
  float  _pedestal_std;       // Pedestal width
  uint16 _pedestal_cut;       // Cut to apply to remove garbage
  uint16 _flags;              // What kind of problems we have
};

struct auto_pedestal_control
{
public:
  float  _max_expect_ped_std; // Maximum expected std. deviation (good ADC=> ~2)

public:
  auto_pedestal_format_channel _ch_no_fmt;
  apfc_info _info;
};

#define APAC_FORMAT_CHANNEL_NO(buf,apc) {      \
  if (apc._ch_no_fmt)                          \
    apc._ch_no_fmt(buf,sizeof(buf),apc._info); \
  else                                         \
   strcpy(buf,"UNKNOWN");                      \
}

#if defined __GNUC__ && __GNUC__ < 3 // 2.95 do not do iso99 variadic macros
#define APAC_SHOW_NEW_ERROR_FMT(apac,flag,str,__VA_ARGS__...) { \
  if (!((apac)->_status_prev._flags & (flag)))             \
    {                                                      \
      char buf[64];                                        \
      APAC_FORMAT_CHANNEL_NO(buf,(apac)->_control);        \
      LOG_FMT("%s: " str,buf,__VA_ARGS__);               \
    }                                                      \
  (apac)->_status._flags |= (flag);                        \
}
#else
#define APAC_SHOW_NEW_ERROR_FMT(apac,flag,str,...) {       \
  if (!((apac)->_status_prev._flags & (flag)))             \
    {                                                      \
      char buf[64];                                        \
      APAC_FORMAT_CHANNEL_NO(buf,(apac)->_control);        \
      LOG_FMT("%s: " str,buf,__VA_ARGS__);               \
    }                                                      \
  (apac)->_status._flags |= (flag);                        \
}
#endif

struct auto_pedestal_adjust_channel
{
public:
  auto_pedestal_info _status;
  auto_pedestal_info _status_prev;
  
  auto_pedestal_control _control;

public:
  uint16 _data[NUM_PEDESTAL_DATA];
  int    _num_data;

public:
  void collect_data(uint16 data)
  {  
    _data[_num_data & (NUM_PEDESTAL_DATA-1)] = data;
    _num_data++;
  }
  

  void analyse()
  {
    uint16 sorted[NUM_PEDESTAL_DATA];
    uint16 diff[NUM_PEDESTAL_DATA];
    int accept_diff, unreasonable_diff;
    int i,j;
    int n;
    double var;

    int min_k;
    int max_k;

    double sum_x;
    double sum_x2;
    double sum;

    _status_prev = _status;

    memset(&_status,0,sizeof(_status));
    _status._flags     = _status_prev._flags & AUTO_PED_INFO_DISABLED;

    if (_status._flags & AUTO_PED_INFO_DISABLED)
      {
	_status._pedestal_cut = 0xffff; // put the cut at maximum value, clips all but 0xffff
	return ;
      }
      
    if (_num_data < NUM_PEDESTAL_DATA)
      {
	APAC_SHOW_NEW_ERROR_FMT(this,AUTO_PED_INFO_NO_DATA,
				"Too little pedestal data (%d < %d)",
				_num_data,NUM_PEDESTAL_DATA);
	_status._pedestal_cut = 0; // disable the cut
	return;
      }
    
    n = NUM_PEDESTAL_DATA;
    
    //#error HANDLE the case of: high range and low range
    //#error HANDLE overflows and underflows
    
    /* High/low range is handled in the following way.  (Is only an
     * issue when module is operated in auto-mode.)  The user is
     * expected to call us for three distinct data sets.  Pedestals in
     * low mode, pedestals in high mode and pedestals in
     * auto(production) mode.  Low and high is clear, will deliver data.
     * The data from auto mode should actually be data for low range
     * mode, since otherwise auto makes no sense.  If there by chance is
     * a high range value, we'll simply be ignoring that as noise.  If
     * there are many, the channel actually has a problem, and we'll
     * fail with a warning...
     *
     * Overflows are then easily handled in the same way.  We also will
     * complain when the pedestal is too high, such that constant
     * overflow is also considered bad.
     */
    
    memcpy (sorted,_data,sizeof(sorted));
    
    qsort(sorted,n,sizeof(sorted[0]),compare_uint16);
    
    // Now, since we might have noise, we cannot just put the cut at the
    // 99%th item, since we might then be cutting very far up.
    
    // We make another array with the differences
    
    for (i = 0; i < n-1; i++)
      {
	diff[i] = (uint16) (sorted[i+1] - sorted[i]);
      }
    
    qsort(diff,n-1,sizeof(sorted[0]),compare_uint16);
    
    // Now, for an ADC/QDC to be in working shape, the peak should
    // be fairly narrow, i.e. we certainly expect differnces of 0,1
    
    // Any difference which is larger than the previous by a factor 3
    // is expected to be connected to values far away
    
    // prevdiff 1 => 4 is bad, 2 => 7 is bad
    
    // First get passed all small differences
    
    accept_diff = 1;
    unreasonable_diff = 3;
    
    j = 1;

    for ( ; j < n-1; j++)
      if (diff[j] >= accept_diff)
	goto acceptable_diff;

    APAC_SHOW_NEW_ERROR_FMT(this,AUTO_PED_INFO_SMALL_DIFF,
			    "Pedestal, data differences suspiciously small (largest: %d, median: %d)",
			    diff[n-2],sorted[n/2]);
    return;

  acceptable_diff:

    // Then find the maximum difference we'll allow
    // (we're allowing diff[j]

    for ( ; j+2 < n; j++)
      {
	if ((diff[j+1] > (diff[j]) * 3))
	  break; // diff[j+1] too large
	if (diff[j+1] > unreasonable_diff)
	  break;
      }

    // If we reach the end of the loop, all differences are acceptable
    
#define TDCP_MAX_NOISE_RATIO 0.1 // must be < 0.5

    // Now find out which values are usable (correspond to acceptable
    // differnces)  (sorted[min_k] .. sorted[max_k-1])
    
    for (min_k = n/2; min_k > 0; min_k--)
      if ((sorted[min_k] - sorted[min_k-1]) > diff[j])
	break; // min_k-1 not acceptable
    
    for (max_k = n/2; max_k < n; max_k++)
      if ((sorted[max_k] - sorted[max_k-1]) > diff[j])
	break; // max_k not acceptable
    /*
    printf ("%4d %4d (%d..%d)\n",
	    j,diff[j],
	    min_k,max_k);
    */
    // We now expect to have contained at least 90% of
    // the data within our selected range
    
    if ((max_k - min_k) < (n * (1 - TDCP_MAX_NOISE_RATIO)))
      {
	APAC_SHOW_NEW_ERROR_FMT(this,AUTO_PED_INFO_TOO_SPREAD,
				"Pedestal, too few values correspond to acceptable differences (%d of %d)",
				(max_k - min_k),n);
	return;
      }

    // Note that this also guarantees than the sum and
    // sum-1 we divide by are larger than 0
    
    // We can now calculate a mean and std deviation of our
    // data...  (Note that since we're cutting the tails,
    // (particularly) the std deviation is not completely
    // correct
    
    sum_x  = 0.;
    sum_x2 = 0.;
    
    for (i = min_k; i < max_k; i++)
      {
	sum_x  += sorted[i];
	sum_x2 += sorted[i] * sorted[i];
      }
    
    sum = (max_k - min_k);
    
    _status._pedestal_mean = (float) (sum_x / sum);
    var = (sum_x2 - sum_x*sum_x / sum)/(sum-1);
    _status._pedestal_std  = (float) sqrt (var);
    _status._pedestal_mean_err2 = (float) (_status._pedestal_std / sum);

    if (_status._pedestal_std > _control._max_expect_ped_std)
      {
	APAC_SHOW_NEW_ERROR_FMT(this,AUTO_PED_INFO_PED_WIDE,
				"Pedestal, suspiciously large std.dev. (+/-%5.1f at %5.1f)",
				_status._pedestal_std,
				_status._pedestal_mean);
      }
    /*
    INFO_FMT("%6.1f %6.1f : %4.1f %4.1f %4.1f (%d..%d)\n",
	     _status._pedestal_mean,_status._pedestal_std,sum,sum_x,sum_x2,
	     min_k,max_k);
    */
    // We put the cut at 4 sigma or so
    
    _status._pedestal_cut = 
      (uint16) ((_status._pedestal_mean + 2*_status._pedestal_std) + 1); // +1 since 0 survives

    _status._flags |= AUTO_PED_INFO_PED_CALCED;
	
    /* Now check if we had any regressions since last measurement.
     */

    if (_status_prev._flags & AUTO_PED_INFO_PED_CALCED)
      {
	// Did the pedestal move? (say we have 3 sigma allowance)

	double diff = fabs(_status     ._pedestal_mean - 
			   _status_prev._pedestal_mean);
	double diff_err = sqrt(_status     ._pedestal_mean_err2 + 
			       _status_prev._pedestal_mean_err2);

	if (diff > diff_err * 3.0 &&
	    diff >= 1.0)
	  {
	    APAC_SHOW_NEW_ERROR_FMT(this,AUTO_PED_INFO_PED_MOVED,
				    "Pedestal moved %4.1f (%5.1f+/-%5.1f) to (%3.1f+/-%3.1f)",
				    _status._pedestal_mean-_status_prev._pedestal_mean,
				    _status_prev._pedestal_mean,
				    sqrt(_status_prev._pedestal_mean_err2),
				    _status._pedestal_mean,
				    sqrt(_status._pedestal_mean_err2));
	  }
      }
    
#if 0 // dump the spectrum
    {
      int i;
      char buf[64];                                        \
      char spectrum[NUM_PEDESTAL_DATA*(1+5+1+1)+1]; // " %5d:v"
      char *p = spectrum;
      int x, sum;
      int last_x = -10;

      APAC_FORMAT_CHANNEL_NO(buf,this->_control);

      for (i = 0; i < n; i++)
	{
	  x = sorted[i];
	  sum = 1;
	  while (i < n && sorted[i+1] == x)
	    {
	      sum++;
	      i++;
	    }

	  if (last_x < x - 5)
	    p += sprintf(p," %d:",x);
	  else
	    {
	      while (last_x < x-1)
		{
		  last_x++;
		  *(p++) = '.';
		}
	    }
	  *(p++) = hexilog2(sum);
	  last_x = x;
	}
      *p = 0;
      LOG_FMT("%s: %5.1f(%4.1f) %s",
	      buf,
	      _status._pedestal_mean,
	      _status._pedestal_std,
	      spectrum);   
    }
#endif

#if 0
    // Since we sorted the array in-place, we need to clear it,
    // otherwise, the circular filling would give rise to biased
    // analysis later
    
    _num_data = 0;
#endif
  }
    
};


#endif /* __AUTO_PEDESTAL_ADJUST_CHANNEL__ */
