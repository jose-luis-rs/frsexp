// -*- C++ -*-

#define chan_sis3316 2
#define max_sample_length 900

VME_STRUCK_SIS3316(id)
{
   MEMBER(DATA16 data[chan_sis3316][max_sample_length] ZERO_SUPPRESS);
   MEMBER(DATA32 maw[chan_sis3316][3] ZERO_SUPPRESS);
   MEMBER(DATA32 timeStamp[chan_sis3316][2] ZERO_SUPPRESS);
   MEMBER(DATA32 gate[chan_sis3316][2] ZERO_SUPPRESS);

      UINT32 header0
      {
         0_15:  chanNo;
         16_31: 0;
      }

      UINT32 header1
      {
         0_3:   format;
         4_15:  channelID ;
         16_31: timeS;
	 ENCODE(timeStamp[header0.chanNo][0],(value=timeS));
      }

      UINT32 header2
      {
         0_31:  timeS;
	 ENCODE(timeStamp[header0.chanNo][1],(value=timeS));
      }

      UINT32 gate7
      {
         0_27: sum;
         28_31: 0;
	 ENCODE(gate[header0.chanNo][0],(value=sum));
      }

      UINT32 gate8
      {
         0_27:  sum;
         28_31: 0;
	 ENCODE(gate[header0.chanNo][1],(value=sum));
      }

      UINT32 maw1
      {
         0_27:  max;
         28_31: 0;
	 ENCODE(maw[header0.chanNo][0],(value=max));
      }

      UINT32 maw2
      {
         0_27:  after;
         28_31: 0;
	 ENCODE(maw[header0.chanNo][1],(value=after));
      }

      UINT32 maw3
      {
         0_27:  before;
         28_31: 0;
      	 ENCODE(maw[header0.chanNo][2],(value=before));
      }

      UINT32 header3
      {
         0_25:  sampleLength;
         26:    0;
         27:    mawFlag;
         28_31: 0xE;
      }


      list(0<=index<header3.sampleLength)
      {
         UINT32 ch_data NOENCODE
         {
            0_15:  sample1;
	    16_31: sample2;

	    ENCODE(data[header0.chanNo][2*index],(value=sample1));
	    ENCODE(data[header0.chanNo][2*index+1],(value=sample2));
         }
      }
}

