
#include "structures.hh"
#include "user.hh"
#include "land_user.hh"
#include "land_triggers.hh"

#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#include <limits.h>

int _show_pileup = 0;
int _show_spill_structure = 0;

#define SAMPLER_BINS 70

#include <math.h>

#define STRUCT_BINS 80*256

#define SPILL_STRUCT_POS          0
#define SPILL_STRUCT_POS_GOOD     1
#define SPILL_STRUCT_PHYSICS      2
#define SPILL_STRUCT_PHYSICS_GOOD 3
#define SPILL_STRUCT_OFFSPILL     4
#define SPILL_STRUCT_CALIB_ETC    5
#define NUM_SPILL_STRUCT_TYPES    6

#define PILEUP_TIME_BEFORE 10 // in 100 ns steps
#define PILEUP_TIME_AFTER  10 // in 100 ns steps

struct sampler
{
  int hits, lost;
  uint32_t prev;
  
  double hist[SAMPLER_BINS+1];
  double sum_diff[SAMPLER_BINS+1];

  uint64_t begin;
  uint64_t spill_on;

  uint64_t stamp_prev;
  uint64_t wrap_add;

  uint64_t stamp_prev31;
  uint64_t wrap_add31;

  // 

  uint64_t pos_prev;
  uint64_t pos_prev2;

  // 0=POS, 1=1, 2=2, 3=cal+etc

  double spill_t_struct[STRUCT_BINS][NUM_SPILL_STRUCT_TYPES];
  uint64_t spill_t_start;
  uint32_t spill_t_binticks;
};

sampler _sampler_pos;

void sampler_clear(sampler &s,bool keep_prev)
{
  s.hits = 0;
  s.lost = 0;
  if (!keep_prev)
    s.prev = 0xffffffff;

  s.begin = (uint64_t) -1;
  s.spill_on = (uint64_t) -1;
  if (!keep_prev)
    {
      s.stamp_prev = 0;
      s.wrap_add = 0;

      s.stamp_prev31 = 0;
      s.wrap_add31 = 0;
    }

  if (!keep_prev)
    {
      s.pos_prev  = (uint64_t) -1;
      s.pos_prev2 = (uint64_t) -1;
    }

  memset(&s.hist,0,sizeof(s.hist));
  memset(&s.sum_diff,0,sizeof(s.sum_diff));

  memset(&s.spill_t_struct,0,sizeof(s.spill_t_struct));
  s.spill_t_binticks = 1;
  s.spill_t_start = (uint64_t) -1;
}

void compact_spill_struct(sampler &s,uint64_t span)
{
  uint32_t factor = 
    (uint32_t) ((2*span-1) / (STRUCT_BINS * s.spill_t_binticks));

  s.spill_t_binticks *= factor;

  for (int m = 0; m < NUM_SPILL_STRUCT_TYPES; m++)
    {
      for (uint32_t i = 0, j = 0; i < STRUCT_BINS / factor; i++, j += factor)
	{
	  double sum = 0;
	  
	  for (uint32_t k = 0; k < factor; k++)
	    sum += s.spill_t_struct[j+k][m];
	  
	  s.spill_t_struct[i][m] = sum;
	}
      for (int i = STRUCT_BINS / factor; i < STRUCT_BINS; i++)
	s.spill_t_struct[i][m] = 0;
    }
}

void add_spill_entry(sampler &s,int i,uint64_t t)
{
  if (s.spill_t_start == (uint64_t) -1)
    s.spill_t_start = t;

  if (t < s.spill_t_start)
    {
      // This will at most loose the first trigger in the first handled
      // event.
      return;
    }

  if (t - s.spill_t_start+1 > STRUCT_BINS * s.spill_t_binticks)
    {
      compact_spill_struct(s,t - s.spill_t_start+1);
    }


  s.spill_t_struct[(t - s.spill_t_start) / s.spill_t_binticks][i]++;



}

void set_spill_start(sampler &s,uint64_t t)
{
  if (s.spill_t_start == (uint64_t) -1)
    s.spill_t_start = t;
}


template<typename T>
void sampler_accumulate(sampler &s,
			T *buf,
			int nbuf,
			uint64_t *stamp_this)
{
  int i;

  if (_sampler_pos.stamp_prev == (uint64_t) -1)
    {
      // We need the previous value to even roughly syncronise (right
      // bit 31 (0-based) on first handled event).  
      _sampler_pos.stamp_prev = *stamp_this;
      if (nbuf)
	s.stamp_prev31 = buf[nbuf-1] & 0x7fffffff;
      // Ignore the data from the first event.
      return;
    }

  uint64_t last_prev_buf = s.stamp_prev31 + s.wrap_add31;

  // If the buffer needs to wrap, do that.

  if (nbuf && (buf[0] & 0x7fffffff) < s.stamp_prev31)
    {
      s.wrap_add31 += (((uint64_t) 1) << 31);
      s.stamp_prev31 = (buf[0] & 0x7fffffff);
    }

  // The first time recorded, must be larger than the previous trigger
  // time-stamp, as we have read all previous time-stamps (of the last
  // event) after that point.

  while (nbuf && (buf[0] & 0x7fffffff) + s.wrap_add31 <=
	 _sampler_pos.stamp_prev + _sampler_pos.wrap_add)
    {
      s.wrap_add31 += ((uint64_t) 1) << 31;
    }

  // Did the trigger time stamp wrap?

  if (*stamp_this < _sampler_pos.stamp_prev)
    _sampler_pos.wrap_add += ((uint64_t) 1) << 32;
  
  _sampler_pos.stamp_prev = *stamp_this;

  // The trigger time recorded must be larger than the last previous
  // time-stamp, as we read that before the current trigger happened.

  if (*stamp_this + _sampler_pos.wrap_add < last_prev_buf)
    _sampler_pos.wrap_add += ((uint64_t) 1) << 32;

  *stamp_this += _sampler_pos.wrap_add;

  // We ought to be in sync now.

  for (i = 0; i < nbuf; i++)
    {
      uint32_t value = buf[i] & 0x7fffffff;

      if (value < s.stamp_prev31)
	s.wrap_add31 += ((uint64_t) 1) << 31;
      s.stamp_prev31 = value;

      if (s.prev == 0xffffffff)
	;
      else if (buf[i] & 0x80000000)
	s.lost++;
      else
	{
	  // Previous and we are adjacent

	  uint32_t diff = (buf[i] - s.prev) & 0x7fffffff;

	  if (diff > 0x3fffffff)
	    {
	      fprintf (stderr,
		       "Anomalous diff %d < 0. (this: 0x%08x, prev: 0x%08x)\n",
		       ((int) diff) - 0x80000000,(uint32_t) buf[i],s.prev);
	    }
	  else
	    {
	      int bin = 0;

	      if (diff > 0)
		bin = (int) (log10((double) diff) * 10.0);
	      if (bin >= SAMPLER_BINS)
		bin = SAMPLER_BINS;

	      s.hist[bin]++;
	      s.sum_diff[bin] += diff;
	    }      
	}

      s.prev = buf[i] & 0x7fffffff;
      s.hits++;

      uint64_t pos_this  = value + s.wrap_add31;

      add_spill_entry(s,SPILL_STRUCT_POS,pos_this);

      // Was the previous POS ion affected by pileup?

      if (s.pos_prev != (uint64_t) -1)
	{
	  bool good_before = true;
	  bool good_after  = true;

	  if (s.pos_prev2 != (uint64_t) -1)
	    if (s.pos_prev - s.pos_prev2 <= PILEUP_TIME_BEFORE)
	      good_before = false;

	  if (pos_this - s.pos_prev <= PILEUP_TIME_AFTER)
	    good_after = false;

	  if (good_before && good_after)
	    add_spill_entry(s,SPILL_STRUCT_POS_GOOD,s.pos_prev);
	}

      s.pos_prev2 = s.pos_prev;
      s.pos_prev  = pos_this;
    }
}

void sampler_show(sampler &s,uint64_t stamp_this)
{
  char array[10][SAMPLER_BINS+1];
  double max = 1;
  int cut_i = SAMPLER_BINS;
  int i;
  double total_t;
  double rate;
  double log_quot[SAMPLER_BINS], log_quot_err[SAMPLER_BINS];

  double total_hits = 0;
  double used_hits;

  for (i = 0; i < SAMPLER_BINS; i++)
    {
      total_hits += s.hist[i];
      if (s.hist[i] > max)
	{
	  max = s.hist[i];
	}
    }

  /* Find out what is an unnormally long time.  Since that may be
   * inter-arrival-times between junk hits.  We do the cut after we have
   * passed 50 % of the data, and then when a bin has twice the number
   * of counts than the previous.
   */

  total_t = 0.0;
  used_hits = 0;
  for (i = 0; i < SAMPLER_BINS; i++)
    {
      if (i &&
	  used_hits > 0.5 * total_hits &&
	  s.hist[i] > 2 * s.hist[i-1])
	{
	  cut_i = i;
	  break;
	} 
      /*
      printf ("%2d %6.0f %.3f %.3f  %.0f\n",
	      i,s.hist[i],s.sum_diff[i] * 1.e-7,total_t * 1.e-7,
	      used_hits);
      */
      total_t += s.sum_diff[i];
      used_hits += s.hist[i];
    }

  rate = used_hits / (total_t*1.e-7);
  (void) cut_i;
  
  uint64_t inspill = 0, offspill = 0;

  if (s.spill_on != (uint64_t) -1)
    {
      inspill = stamp_this - s.spill_on;
      offspill = s.spill_on - s.begin;
    }
  else
    offspill = stamp_this - s.begin;

  printf ("POS hits: %8d (lost: %3d%c) "
	  "Offspill: %6.2f s  Inspill: %6.2f s\n"
	  "Total_t: %.3f s  Rate: %.1f Hz\n",
	  s.hits,s.lost,s.lost ? '+' : ' ',
	  ((double) offspill) * 1.e-7,((double) inspill) * 1.e-7,
	  total_t * 1.e-7,rate);
  
  for (i = 0; i < 10; i++)
    {
      memset(array[i],' ',sizeof(array[i]));
      array[i][SAMPLER_BINS] = 0;
    }

  array[9][cut_i] = 'c';
  
  for (i = 0; i < SAMPLER_BINS; i++)
    {
      int y = (s.hist[i] == 0 ? 0 : 1 + (int) ((s.hist[i]*35) / max));
      char c[] = "_.-^";
      
      array[y / 4][i] = c[y%4];
    }
  printf ("\n");
  
  for (i = 0; i < 10; i++)
    {
      printf ("       " ":%s:\n",array[9-i]);
    }
  
#define SAMPLER_T_SCALE   "|        1|us       |         "		\
    /* */                 "|        1|ms       |         |        1|s\n"
  
  printf ("        " SAMPLER_T_SCALE);
  printf ("\n");

  for (i = 0; i < SAMPLER_BINS; i++)
    {
      double a = pow(10.,i/10.0)    *1.e-7;
      double b = pow(10.,(i+1)/10.0)*1.e-7;

      double estimate = used_hits * (exp(-rate*a) - exp(-rate*b));

      double quot     = s.hist[i] / estimate;
      double quot_err = sqrt(s.hist[i]) / estimate;

      log_quot[i]     = log(quot)/log(2.);
      log_quot_err[i] = 1/quot/log(2.)*quot_err;
      /*
      if (estimate > 1)
	{
	  printf ("%10.0f %10.1f (%10.1f +/- %10.1f) (%10.1f +/- %10.1f)\n",
		  s.hist[i],estimate,
		  quot,quot_err,
		  log_quot[i],log_quot_err[i]);
	}
      */
    }

  for (i = 0; i < 10; i++)
    {
      memset(array[i],' ',sizeof(array[i]));
      array[i][SAMPLER_BINS] = 0;
    }

  for (i = 0; i < SAMPLER_BINS; i++)
    {
      double yq  = 2.5+(log_quot[i]+2)/4*32;
      double yqe = (log_quot_err[i])/4*32;
      double yq1 = yq-yqe;
      double yq2 = yq+yqe;

      {
	/* y1 collides with function in math.h */
	int y1_ = (int) ((yq1+3) / 4); /* shrinking error bars :-) */
	int y2_ = (int) ( yq2    / 4);
	int y;
          
	if (y1_ < 0) y1_ = 0;
	if (y2_ > 9) y2_ = 9;
          
	for (y = y1_; y <= y2_; y++)
	  {
	    array[y][i] = 'X';
	  }
      }

      if (yq >= 0 && yq <= 35)
	{
	  int y = (int) yq;
	  char c[] = "_.-^";

	  array[y / 4][i] = c[y%4];
	}
    }

  const char *scale[10] = { "1/4 -", "", "1/2 -", "", "1 -",
			    "", "2 -", "", "4 -", "" };

  for (i = 0; i < 10; i++)
    {
      printf ("%6s " ":%s:\n",scale[9-i],array[9-i]);
    }

  printf ("        " SAMPLER_T_SCALE);
  printf ("\n");
}

#define SPILL_STRUCT_SHOW_ROWS 20

void sampler_spill_show(sampler &s,uint64_t stamp_this)
{
  uint64_t inspill = 0, offspill = 0;

  if (s.spill_on != (uint64_t) -1)
    {
      inspill = stamp_this - s.spill_on;
      offspill = s.spill_on - s.begin;
    }
  else
    offspill = stamp_this - s.begin;

  printf ("%12" PRIu64 " (%12" PRIu64 ") %12" PRIu64 " %12" PRIu64 " -> "
	  "%10" PRIu64 " %10" PRIu64 "\n",
	  s.begin,s.spill_t_start,s.spill_on,stamp_this,
	  offspill,inspill);

  // We always plot from s.begin to stamp_this

  double rebin[80][NUM_SPILL_STRUCT_TYPES];
  double max = 0, max2 = 0;

  memset(rebin,0,sizeof(rebin));

  double tspan = (double) (stamp_this - s.begin);
  double ticks_per_column = tspan / 80.;
  double bins_per_column = ticks_per_column / s.spill_t_binticks;

  for (int m = 0; m < NUM_SPILL_STRUCT_TYPES; m++)
    {
      double start_bin = (double) (s.begin - s.spill_t_start) / s.spill_t_binticks;
      /*
      printf ("%10.0f %d %10.2f %10.2f %10.2f/\n",
	      tspan,s.spill_t_binticks,
	      start_bin,ticks_per_column,bins_per_column);
      */
      for (int j = 0; j < 80; j++)
	{
	  double end_bin = start_bin + bins_per_column;
	
	  double first = start_bin;
	  double last  = end_bin;  
	  
	  if (first < 0)           first = 0;
	  if (first > STRUCT_BINS-1) first = STRUCT_BINS-1;
	  if (last < 0)            last = 0;
	  if (last > STRUCT_BINS-1)  last = STRUCT_BINS-1;
	  
	  int first_i = (int) first;
	  int last_i  = (int) last;
	  
	  double sum = 0;

	  sum += s.spill_t_struct[first_i][m] * (1 - (first - first_i));
	  sum += s.spill_t_struct[last_i][m]  * (last - last_i);
	  
	  for (int i = first_i+1; i < last_i; i++)
	    sum += s.spill_t_struct[i][m];
	  
	  rebin[j][m] = sum;

	  if (sum > max)
	    {
	      max2 = max;
	      max = sum;
	    }
	  else if (sum > max2)
	    max2 = sum;

	  /*
	  printf ("%2d/%d: %10.0f (%10.2f %10.2f) %d %d\n",
		  j,m,sum,first,last,first_i,last_i);
	  */
	  start_bin = end_bin;
	}
    }

  char array[SPILL_STRUCT_SHOW_ROWS][81];
  char good_pos_ratio[81];

  int spill_on_col = 
    (int) ((double) (s.spill_on - s.begin) / ticks_per_column);

  if (spill_on_col < 0)  spill_on_col = 0;
  if (spill_on_col > 79) spill_on_col = 79;

  for (int i = 0; i < SPILL_STRUCT_SHOW_ROWS; i++)
    {
      memset(array[i],' ',sizeof(array[i]));
      array[i][80] = 0;

      array[i][spill_on_col] = ':';
      array[i][0] = ':';
      array[i][79] = ':';
    }

  memset(good_pos_ratio,' ',sizeof(good_pos_ratio));
  good_pos_ratio[80] = 0;

  /*
  for (int i = 0; i < 80; i++)
    printf ("%2d: %10.0f %10.0f %10.0f %10.0f\n",
	    i,rebin[i][0],rebin[i][1],rebin[i][2],rebin[i][3]);
  */

  if (max > 1.15 * max2)
    max = max2 * 1.1;

  for (int x = 0; x < 80; x++)
    {
      {
	int y = (rebin[x][SPILL_STRUCT_POS] == 0 ? 0 : 
		 1 + (int) ((rebin[x][SPILL_STRUCT_POS]*
			     (SPILL_STRUCT_SHOW_ROWS*4-2)) / max));
	char c[] = "_.-^";

	if (y >= SPILL_STRUCT_SHOW_ROWS * 4)
	  array[SPILL_STRUCT_SHOW_ROWS-1][x] = '#';
	else      
	  array[y / 4][x] = c[y % 4];
      }

      if (rebin[x][SPILL_STRUCT_POS] > 0)
	{
	  double ratio = ((double) rebin[x][SPILL_STRUCT_POS_GOOD]) / 
	    (rebin[x][SPILL_STRUCT_POS]);

	  int goodness = (int) (ratio * 10 + 0.5);

	  if (goodness < 0) goodness = 0;
	  if (goodness > 9) goodness = 9;

	  good_pos_ratio[x] = (char) ('0' + goodness);
	}
    }

  for (int i = 0; i < SPILL_STRUCT_SHOW_ROWS; i++)
    printf ("%s\n",array[SPILL_STRUCT_SHOW_ROWS-1-i]);

  printf ("%s\n",good_pos_ratio);
}

#define HIST_DIFF_POS_TRIG_BINS 41

int _hist_expect_diff_pos_trig[HIST_DIFF_POS_TRIG_BINS];

int _calc_pos_trig_diff = 0;

void sampler_clear_expect_diff()
{
  memset(_hist_expect_diff_pos_trig,0,sizeof(_hist_expect_diff_pos_trig));
}

template<typename T>
void sampler_collect_expect_diff(T *buf,
			      int nbuf,
			      uint64_t *stamp_this)
{
  /* To make the difference work properly signed, we shift the values up
   * and thus do not use the lower bits.
   */

  uint32_t stamp_value = ((uint32_t) *stamp_this) << 1;

  for (int i = 0; i < nbuf; i++)
    {
      uint32_t value = buf[i] << 1;

      int32_t diff = stamp_value - value; 

      diff >>= 1;
      diff += HIST_DIFF_POS_TRIG_BINS/2;

      if (diff >= 0 && diff < HIST_DIFF_POS_TRIG_BINS)
	{
	  _hist_expect_diff_pos_trig[diff]++;
	}
    }
}

void sampler_analyse_expect_diff()
{
  int best_num[3] = { 0, 0, 0 }; // make compiler happy
  int best_i[3] = { -1, -1, -1 };

  for (int i = 0; i < HIST_DIFF_POS_TRIG_BINS; i++)
    {
      printf ("%3d: %d\n",
	      i-HIST_DIFF_POS_TRIG_BINS/2,
	      _hist_expect_diff_pos_trig[i]);

      if (_hist_expect_diff_pos_trig[i] >= best_num[0])
	{
	  for (int j = 2; j >= 1; j--)
	    {
	      best_i[j] = best_i[j-1];
	      best_num[j] = best_num[j-1];
	    }
	  best_i[0] = i - HIST_DIFF_POS_TRIG_BINS/2;
	  best_num[0] = _hist_expect_diff_pos_trig[i];
	}
    }

  // TODO: should a) require som substantial statistics, and
  // b) that the peak is much larger than the rest

  if (best_i[0] != -1)
    printf ("Expected TRIG-POS time difference: %.0f ns\n",best_i[0] * 100.0);
  else
    printf ("Cannot determine TRIG-POS time difference.");
}

int _expect_diff_pos_trig = 0;
int _max_diff_pos_trig = 0;

template<typename T>
void sampler_calc_tprev_tnext(T *buf,
			      int nbuf,
			      uint64_t *stamp_this,
			      float *tprev,
			      float *tnext)
{
  if (_max_diff_pos_trig <= 0)
    return;

  /* Plan is for the moment that the DAQ DT before and after the sampler
   * readout is large enough that we will always have the time before
   * and after the trigger recorded in the current event.  I.e. no need
   * to consider data from previous events.
   */

  /* We only need to consider that the stamped times may have wrapped.
   * But this fixes itself automatically by clamping the results after
   * subtraction.
   */

  /* First find the (most likely) POS signal that corresponds to our
   * trigger.
   */

  int best_diff = INT_MAX;
  int best_i = -1;

  uint32_t stamp_value = ((uint32_t) *stamp_this) << 1;

  for (int i = 0; i < nbuf; i++)
    {
      uint32_t value = buf[i] << 1;

      int32_t diff = stamp_value - value; 

      diff >>= 1;
      int abs_diff = abs((int) diff - _expect_diff_pos_trig);

      if (abs_diff < best_diff)
	{
	  best_diff = abs_diff;
	  best_i = i;
	}
    }

  if (best_diff < _max_diff_pos_trig)
    {
      if (best_i > 0)
	{
	  uint32_t diff_prev = (buf[best_i] - buf[best_i-1]) & 0x7fffffff;

	  if (diff_prev > 100*10) // 100 us
	    *tprev = INFINITY;
	  else
	    *tprev = (float) (diff_prev * 100.0);
	}

      if (best_i < nbuf-1)
	{
	  uint32_t diff_next = (buf[best_i+1] - buf[best_i]) & 0x7fffffff;

	  if (diff_next > 100*10) // 100 us
	    *tnext = INFINITY;
	  else
	    *tnext = (float) (diff_next * 100.0);
	}

      // printf ("%10.0f %10.0f\n",*tprev,*tnext);
    }
}

void land_pos_sampler_user_function(unpack_event *event,
				    MASTER_VME   &master_vme,
				    raw_event    *raw_event)
{
  uint64_t stamp_this = (uint64_t) -1;

  if (master_vme.vme.failure.has_clock_counter_stamp)
    stamp_this = master_vme.vme.clock_counter_stamp;

  if (stamp_this != (uint64_t) -1)
    {
      if (_calc_pos_trig_diff)
	sampler_collect_expect_diff(master_vme.sampler_pos.stamps._items,
				    master_vme.sampler_pos.stamps._num_items,
				    &stamp_this);
      
      sampler_calc_tprev_tnext(master_vme.sampler_pos.stamps._items,
			       master_vme.sampler_pos.stamps._num_items,
			       &stamp_this,
			       &raw_event->TPREV,
			       &raw_event->TNEXT);
    }

  if (_show_pileup && stamp_this != (uint64_t) -1)
    {
      sampler_accumulate(_sampler_pos,
			 master_vme.sampler_pos.stamps._items,
			 master_vme.sampler_pos.stamps._num_items,
			 &stamp_this);

      add_spill_entry(_sampler_pos,
		      event->trigger == LAND_EVENT_PHYSICS ? SPILL_STRUCT_PHYSICS :
		      (event->trigger == LAND_EVENT_OFFSPILL ? SPILL_STRUCT_OFFSPILL : SPILL_STRUCT_CALIB_ETC),
		      stamp_this);

      if (_sampler_pos.begin == (uint64_t) -1)
	_sampler_pos.begin = stamp_this;
      if (event->trigger == LAND_EVENT_SPILL_BEGIN)
	_sampler_pos.spill_on = stamp_this;
    }
      
  if (event->trigger == LAND_EVENT_SPILL_END ||
      event->trigger == LAND_EVENT_SPILL_END_KEEP_ALIVE ||
      event->trigger == LAND_EVENT_SPILL_BEGIN_KEEP_ALIVE)
    {
      if (_show_pileup)
	{
	  sampler_show(_sampler_pos,stamp_this);
	  sampler_spill_show(_sampler_pos,stamp_this);
	  sampler_clear(_sampler_pos,true);
	  _sampler_pos.begin = stamp_this;
	  set_spill_start(_sampler_pos,stamp_this);
	}
    }

}

void land_pos_sampler_user_init()
{
  sampler_clear(_sampler_pos,false);
  if (_calc_pos_trig_diff)
    sampler_clear_expect_diff();
}

void land_pos_sampler_user_exit()
{
  if (_calc_pos_trig_diff)
    sampler_analyse_expect_diff();
}

void land_pos_sampler_user_usage_command_line_options()
{
  //      "  --option          Explanation.\n"
  printf ("  --pileup          Show pile-up each spill.\n");
  printf ("  --spill-struct    Show spill structure.\n");
  printf ("  --pos-trig-diff=N  Expected time difference trig-POS (in ns).\n");
  printf ("  --calc-pos-trig-diff  Calculate expected trig-POS time difference.\n");
}

bool land_pos_sampler_user_handle_command_line_option(const char *arg)
{
  const char *post;

#define MATCH_PREFIX(prefix,post) (strncmp(arg,prefix,strlen(prefix)) == 0 && *(post = arg + strlen(prefix)) != '\0')
#define MATCH_ARG(name) (strcmp(arg,name) == 0)

  if (MATCH_ARG("--pileup")) {
    _show_pileup = 1;
    return true;
  }
  else if (MATCH_ARG("--spill-struct")) {
    _show_spill_structure = 1;
    return true;
  }
  else if (MATCH_PREFIX("--pos-trig-diff=",post)) {
    _expect_diff_pos_trig = atoi(post) / 100;
    _max_diff_pos_trig = 2;
    return true;
  }
  else if (MATCH_ARG("--calc-pos-trig-diff")) {
    _calc_pos_trig_diff = 1;
    return true;
  }
 return false;
}
