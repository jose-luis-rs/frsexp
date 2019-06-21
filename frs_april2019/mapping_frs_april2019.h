
//SIGNAL(Trigg, master_tpat.tpat.tpat, DATA16);

//FRS scaler from Master Crate
SIGNAL(FreeTrig, vme0.scaler_0.data[0],DATA32);
SIGNAL(AccTrig, vme0.scaler_0.data[1],DATA32);
SIGNAL(Spill, vme0.scaler_0.data[2],DATA32);
SIGNAL(ClockoneHz, vme0.scaler_0.data[3],DATA32);
SIGNAL(ClocktenHz, vme0.scaler_0.data[4],DATA32);
SIGNAL(VetoDeadTime, vme0.scaler_0.data[5],DATA32);
SIGNAL(ClockhundredkHz, vme0.scaler_0.data[6],DATA32);

SIGNAL(SCI1L, vme0.scaler_0.data[16],DATA32);
SIGNAL(SCI2L, vme0.scaler_0.data[17],DATA32);
SIGNAL(SCI3L, vme0.scaler_0.data[18],DATA32);
SIGNAL(SCI4L, vme0.scaler_0.data[19],DATA32);
SIGNAL(SCI5L, vme0.scaler_0.data[20],DATA32);
SIGNAL(SCI1R, vme0.scaler_0.data[21],DATA32);
SIGNAL(SCI2R, vme0.scaler_0.data[22],DATA32);
SIGNAL(SCI3R, vme0.scaler_0.data[23],DATA32);
SIGNAL(SCI4R, vme0.scaler_0.data[24],DATA32);
SIGNAL(SCI5R, vme0.scaler_0.data[25],DATA32);

//FRS scaler from MW Crate
SIGNAL(ICNewCurrent, vme1.scaler_0.data[0],DATA32);
SIGNAL(Seeold, vme1.scaler_0.data[1],DATA32);
SIGNAL(Seenew, vme1.scaler_0.data[2],DATA32);
SIGNAL(ICOldCurrent, vme1.scaler_0.data[3],DATA32);
SIGNAL(ICCounter, vme1.scaler_0.data[4],DATA32);
SIGNAL(SCIN1, vme1.scaler_0.data[5],DATA32);
SIGNAL(SCIN2, vme1.scaler_0.data[6],DATA32);
SIGNAL(SCIN3, vme1.scaler_0.data[7],DATA32);
SIGNAL(StartExt, vme1.scaler_0.data[8],DATA32);
SIGNAL(StopExt, vme1.scaler_0.data[9],DATA32);
SIGNAL(BTDG, vme1.scaler_0.data[10],DATA32);

//Scintillators dE, QDC signals
SIGNAL(SC1LdE,vme0.qdc_0.data[2],DATA12);
SIGNAL(SC1RdE,vme0.qdc_0.data[3],DATA12);
SIGNAL(SC2LdE,vme0.qdc_0.data[0],DATA12);
SIGNAL(SC2RdE,vme0.qdc_0.data[1],DATA12);
SIGNAL(SC3LdE,vme0.qdc_0.data[4],DATA12);
SIGNAL(SC3RdE,vme0.qdc_0.data[5],DATA12);
SIGNAL(SC4LdE,vme0.qdc_0.data[11],DATA12);
SIGNAL(SC4RdE,vme0.qdc_0.data[12],DATA12);
SIGNAL(SC5LdE,vme0.qdc_0.data[13],DATA12);
SIGNAL(SC5RdE,vme0.qdc_0.data[14],DATA12);

//Scintillators time signals
//SIGNAL(SC1Lt,vme0.tdc_0.data[2],DATA24);
//SIGNAL(SC1Rt,vme0.tdc_0.data[3],DATA24);
//SIGNAL(SC2Lt,vme0.tdc_0.data[0],DATA24);
//SIGNAL(SC2Rt,vme0.tdc_0.data[1],DATA24);
//SIGNAL(SC3Lt,vme0.tdc_0.data[4],DATA24);
//SIGNAL(SC3Rt,vme0.tdc_0.data[5],DATA24);
//SIGNAL(SC4Lt,vme0.tdc_0.data[6],DATA24);
//SIGNAL(SC4Rt,vme0.tdc_0.data[7],DATA24);
//SIGNAL(SC5Lt,vme0.tdc_0.data[8],DATA24);
//SIGNAL(SC5Rt,vme0.tdc_0.data[9],DATA24);

//Tacs
SIGNAL(SC1Lt,vme1.adc_1.data[0],DATA12);
SIGNAL(SC1Rt,vme1.adc_1.data[1],DATA12);

SIGNAL(SC2Lt,vme1.adc_1.data[5],DATA12);
SIGNAL(SC2Rt,vme1.adc_1.data[6],DATA12);

//SIGNAL(SC3Lt,vme1.adc_1.data[4],DATA12);
//SIGNAL(SC3Rt,vme1.adc_1.data[5],DATA12);
//SIGNAL(SC4Lt,vme1.adc_1.data[6],DATA12);
//SIGNAL(SC4Rt,vme1.adc_1.data[7],DATA12);
//SIGNAL(SC5Lt,vme1.adc_1.data[9],DATA12);
//SIGNAL(SC5Rt,vme1.adc_1.data[10],DATA12);


//Musics adc signals
//Music41
SIGNAL(MUS1A1, vme1.adc_0.data[0],DATA12);
SIGNAL(MUS1A2, vme1.adc_0.data[1],DATA12);
SIGNAL(MUS1A3, vme1.adc_0.data[2],DATA12);
SIGNAL(MUS1A4, vme1.adc_0.data[3],DATA12);
SIGNAL(MUS1A5, vme1.adc_0.data[4],DATA12);
SIGNAL(MUS1A6, vme1.adc_0.data[5],DATA12);
SIGNAL(MUS1A7, vme1.adc_0.data[6],DATA12);
SIGNAL(MUS1A8, vme1.adc_0.data[7],DATA12);
//Music42
SIGNAL(MUS2A1, vme1.adc_0.data[8],DATA12);
SIGNAL(MUS2A2, vme1.adc_0.data[9],DATA12);
SIGNAL(MUS2A3, vme1.adc_0.data[10],DATA12);
SIGNAL(MUS2A4, vme1.adc_0.data[11],DATA12);
SIGNAL(MUS2A5, vme1.adc_0.data[12],DATA12);
SIGNAL(MUS2A6, vme1.adc_0.data[13],DATA12);
SIGNAL(MUS2A7, vme1.adc_0.data[14],DATA12);
SIGNAL(MUS2A8, vme1.adc_0.data[15],DATA12);

//Music43
SIGNAL(MUS3A1, vme1.adc_0.data[16],DATA12);
SIGNAL(MUS3A2, vme1.adc_0.data[17],DATA12);
SIGNAL(MUS3A3, vme1.adc_0.data[18],DATA12);
SIGNAL(MUS3A4, vme1.adc_0.data[19],DATA12);
SIGNAL(MUS3A5, vme1.adc_0.data[20],DATA12);
SIGNAL(MUS3A6, vme1.adc_0.data[21],DATA12);
SIGNAL(MUS3A7, vme1.adc_0.data[22],DATA12);
SIGNAL(MUS3A8, vme1.adc_0.data[23],DATA12);

//TPC cathodes
SIGNAL(TPC_1_DT_1, vme2.tdc_0.data[0],DATA12);
SIGNAL(TPC_1_DT_2, vme2.tdc_0.data[1],DATA12);
SIGNAL(TPC_1_DT_3, vme2.tdc_0.data[2],DATA12);
SIGNAL(TPC_1_DT_4, vme2.tdc_0.data[3],DATA12);
SIGNAL(TPC_1_LT_1, vme2.tdc_0.data[4],DATA12);
SIGNAL(TPC_1_RT_1, vme2.tdc_0.data[5],DATA12);
SIGNAL(TPC_1_LT_2, vme2.tdc_0.data[6],DATA12);
SIGNAL(TPC_1_RT_2, vme2.tdc_0.data[7],DATA12);

SIGNAL(TPC_2_DT_1, vme2.tdc_0.data[8],DATA12);
SIGNAL(TPC_2_DT_2, vme2.tdc_0.data[9],DATA12);
SIGNAL(TPC_2_DT_3, vme2.tdc_0.data[10],DATA12);
SIGNAL(TPC_2_DT_4, vme2.tdc_0.data[11],DATA12);
SIGNAL(TPC_2_LT_1, vme2.tdc_0.data[12],DATA12);
SIGNAL(TPC_2_RT_1, vme2.tdc_0.data[13],DATA12);
SIGNAL(TPC_2_LT_2, vme2.tdc_0.data[14],DATA12);
SIGNAL(TPC_2_RT_2, vme2.tdc_0.data[15],DATA12);

SIGNAL(TPC_3_DT_1, vme2.tdc_1.data[0],DATA12);
SIGNAL(TPC_3_DT_2, vme2.tdc_1.data[1],DATA12);
SIGNAL(TPC_3_DT_3, vme2.tdc_1.data[2],DATA12);
SIGNAL(TPC_3_DT_4, vme2.tdc_1.data[3],DATA12);
SIGNAL(TPC_3_LT_1, vme2.tdc_1.data[4],DATA12);
SIGNAL(TPC_3_RT_1, vme2.tdc_1.data[5],DATA12);
SIGNAL(TPC_3_LT_2, vme2.tdc_1.data[6],DATA12);
SIGNAL(TPC_3_RT_2, vme2.tdc_1.data[7],DATA12);

SIGNAL(TPC_4_DT_1, vme2.tdc_1.data[8],DATA12);
SIGNAL(TPC_4_DT_2, vme2.tdc_1.data[9],DATA12);
SIGNAL(TPC_4_DT_3, vme2.tdc_1.data[10],DATA12);
SIGNAL(TPC_4_DT_4, vme2.tdc_1.data[11],DATA12);
SIGNAL(TPC_4_LT_1, vme2.tdc_1.data[12],DATA12);
SIGNAL(TPC_4_RT_1, vme2.tdc_1.data[13],DATA12);
SIGNAL(TPC_4_LT_2, vme2.tdc_1.data[14],DATA12);
SIGNAL(TPC_4_RT_2, vme2.tdc_1.data[15],DATA12);

SIGNAL(TPC_5_DT_1, vme2.tdc_1.data[16],DATA12);
SIGNAL(TPC_5_DT_2, vme2.tdc_1.data[17],DATA12);
SIGNAL(TPC_5_DT_3, vme2.tdc_1.data[18],DATA12);
SIGNAL(TPC_5_DT_4, vme2.tdc_1.data[19],DATA12);
SIGNAL(TPC_5_LT_1, vme2.tdc_1.data[20],DATA12);
SIGNAL(TPC_5_RT_1, vme2.tdc_1.data[21],DATA12);
SIGNAL(TPC_5_LT_2, vme2.tdc_1.data[22],DATA12);
SIGNAL(TPC_5_RT_2, vme2.tdc_1.data[23],DATA12);

//TPC anodes
SIGNAL(TPC_1_A_1, vme2.adc_0.data[0],DATA12);
SIGNAL(TPC_1_A_2, vme2.adc_0.data[1],DATA12);
SIGNAL(TPC_1_A_3, vme2.adc_0.data[2],DATA12);
SIGNAL(TPC_1_A_4, vme2.adc_0.data[3],DATA12);
SIGNAL(TPC_1_AL_1, vme2.adc_0.data[4],DATA12);
SIGNAL(TPC_1_AR_1, vme2.adc_0.data[5],DATA12);
SIGNAL(TPC_1_AL_2, vme2.adc_0.data[6],DATA12);
SIGNAL(TPC_1_AR_2, vme2.adc_0.data[7],DATA12);

SIGNAL(TPC_2_A_1, vme2.adc_0.data[8],DATA12);
SIGNAL(TPC_2_A_2, vme2.adc_0.data[9],DATA12);
SIGNAL(TPC_2_A_3, vme2.adc_0.data[10],DATA12);
SIGNAL(TPC_2_A_4, vme2.adc_0.data[11],DATA12);
SIGNAL(TPC_2_AL_1, vme2.adc_0.data[12],DATA12);
SIGNAL(TPC_2_AR_1, vme2.adc_0.data[13],DATA12);
SIGNAL(TPC_2_AL_2, vme2.adc_0.data[14],DATA12);
SIGNAL(TPC_2_AR_2, vme2.adc_0.data[15],DATA12);

SIGNAL(TPC_3_A_1, vme2.adc_1.data[0],DATA12);
SIGNAL(TPC_3_A_2, vme2.adc_1.data[1],DATA12);
SIGNAL(TPC_3_A_3, vme2.adc_1.data[2],DATA12);
SIGNAL(TPC_3_A_4, vme2.adc_1.data[3],DATA12);
SIGNAL(TPC_3_AL_1, vme2.adc_1.data[4],DATA12);
SIGNAL(TPC_3_AR_1, vme2.adc_1.data[5],DATA12);
SIGNAL(TPC_3_AL_2, vme2.adc_1.data[6],DATA12);
SIGNAL(TPC_3_AR_2, vme2.adc_1.data[7],DATA12);

SIGNAL(TPC_4_A_1, vme2.adc_1.data[8],DATA12);
SIGNAL(TPC_4_A_2, vme2.adc_1.data[9],DATA12);
SIGNAL(TPC_4_A_3, vme2.adc_1.data[10],DATA12);
SIGNAL(TPC_4_A_4, vme2.adc_1.data[11],DATA12);
SIGNAL(TPC_4_AL_1, vme2.adc_1.data[12],DATA12);
SIGNAL(TPC_4_AR_1, vme2.adc_1.data[13],DATA12);
SIGNAL(TPC_4_AL_2, vme2.adc_1.data[14],DATA12);
SIGNAL(TPC_4_AR_2, vme2.adc_1.data[15],DATA12);

SIGNAL(TPC_5_A_1, vme2.adc_1.data[16],DATA12);
SIGNAL(TPC_5_A_2, vme2.adc_1.data[17],DATA12);
SIGNAL(TPC_5_A_3, vme2.adc_1.data[18],DATA12);
SIGNAL(TPC_5_A_4, vme2.adc_1.data[19],DATA12);
SIGNAL(TPC_5_AL_1, vme2.adc_1.data[20],DATA12);
SIGNAL(TPC_5_AR_1, vme2.adc_1.data[21],DATA12);
SIGNAL(TPC_5_AL_2, vme2.adc_1.data[22],DATA12);
SIGNAL(TPC_5_AR_2, vme2.adc_1.data[23],DATA12);


//MW Anodes
SIGNAL(MWAN_1, vme1.tdc_0.data[0],DATA12);
SIGNAL(MWAN_2, vme1.tdc_0.data[1],DATA12);
SIGNAL(MWAN_3, vme1.tdc_0.data[2],DATA12);
SIGNAL(MWAN_4, vme1.tdc_0.data[3],DATA12);
SIGNAL(MWAN_5, vme1.tdc_0.data[4],DATA12);
SIGNAL(MWAN_6, vme1.tdc_0.data[5],DATA12);
SIGNAL(MWAN_7, vme1.tdc_0.data[6],DATA12);
SIGNAL(MWAN_8, vme1.tdc_0.data[7],DATA12);
SIGNAL(MWAN_9, vme1.tdc_0.data[8],DATA12);
SIGNAL(MWAN_10, vme1.tdc_0.data[9],DATA12);
SIGNAL(MWAN_11, vme1.tdc_0.data[10],DATA12);
SIGNAL(MWAN_12, vme1.tdc_0.data[11],DATA12);
SIGNAL(MWAN_13, vme1.tdc_0.data[12],DATA12);


SIGNAL(MW_1_XR, vme1.tdc_0.data[16],DATA12);
SIGNAL(MW_1_XL, vme1.tdc_0.data[17],DATA12);
SIGNAL(MW_1_YU, vme1.tdc_0.data[18],DATA12);
SIGNAL(MW_1_YD, vme1.tdc_0.data[19],DATA12);

SIGNAL(MW_2_XR, vme1.tdc_0.data[20],DATA12);
SIGNAL(MW_2_XL, vme1.tdc_0.data[21],DATA12);
SIGNAL(MW_2_YU, vme1.tdc_0.data[22],DATA12);
SIGNAL(MW_2_YD, vme1.tdc_0.data[23],DATA12);

SIGNAL(MW_3_XR, vme1.tdc_0.data[24],DATA12);
SIGNAL(MW_3_XL, vme1.tdc_0.data[25],DATA12);
SIGNAL(MW_3_YU, vme1.tdc_0.data[26],DATA12);
SIGNAL(MW_3_YD, vme1.tdc_0.data[27],DATA12);

SIGNAL(MW_4_XR, vme1.tdc_0.data[28],DATA12);
SIGNAL(MW_4_XL, vme1.tdc_0.data[29],DATA12);
SIGNAL(MW_4_YU, vme1.tdc_0.data[30],DATA12);
SIGNAL(MW_4_YD, vme1.tdc_0.data[31],DATA12);

SIGNAL(MW_5_XR, vme1.tdc_1.data[0],DATA12);
SIGNAL(MW_5_XL, vme1.tdc_1.data[1],DATA12);
SIGNAL(MW_5_YU, vme1.tdc_1.data[2],DATA12);
SIGNAL(MW_5_YD, vme1.tdc_1.data[3],DATA12);

SIGNAL(MW_6_XR, vme1.tdc_1.data[4],DATA12);
SIGNAL(MW_6_XL, vme1.tdc_1.data[5],DATA12);
SIGNAL(MW_6_YU, vme1.tdc_1.data[6],DATA12);
SIGNAL(MW_6_YD, vme1.tdc_1.data[7],DATA12);

SIGNAL(MW_7_XR, vme1.tdc_1.data[8],DATA12);
SIGNAL(MW_7_XL, vme1.tdc_1.data[9],DATA12);
SIGNAL(MW_7_YU, vme1.tdc_1.data[10],DATA12);
SIGNAL(MW_7_YD, vme1.tdc_1.data[11],DATA12);


//SIGNAL(ZERO_SUPPRESS:MW_8_XR);
//SIGNAL(ZERO_SUPPRESS:MW_8_XL);
//SIGNAL(ZERO_SUPPRESS:MW_8_YU);
//SIGNAL(ZERO_SUPPRESS:MW_8_YD);


SIGNAL(MW_9_XR, vme1.tdc_1.data[12],DATA12);
SIGNAL(MW_9_XL, vme1.tdc_1.data[13],DATA12);
SIGNAL(MW_9_YU, vme1.tdc_1.data[14],DATA12);
SIGNAL(MW_9_YD, vme1.tdc_1.data[15],DATA12);

SIGNAL(MW_10_XR, vme1.tdc_1.data[16],DATA12);
SIGNAL(MW_10_XL, vme1.tdc_1.data[17],DATA12);
SIGNAL(MW_10_YU, vme1.tdc_1.data[18],DATA12);
SIGNAL(MW_10_YD, vme1.tdc_1.data[19],DATA12);

SIGNAL(MW_11_XR, vme1.tdc_1.data[20],DATA12);
SIGNAL(MW_11_XL, vme1.tdc_1.data[21],DATA12);
SIGNAL(MW_11_YU, vme1.tdc_1.data[22],DATA12);
SIGNAL(MW_11_YD, vme1.tdc_1.data[23],DATA12);

SIGNAL(MW_12_XR, vme1.tdc_1.data[24],DATA12);
SIGNAL(MW_12_XL, vme1.tdc_1.data[25],DATA12);
SIGNAL(MW_12_YU, vme1.tdc_1.data[26],DATA12);
SIGNAL(MW_12_YD, vme1.tdc_1.data[27],DATA12);

SIGNAL(MW_13_XR, vme1.tdc_1.data[28],DATA12);
SIGNAL(MW_13_XL, vme1.tdc_1.data[29],DATA12);
SIGNAL(MW_13_YU, vme1.tdc_1.data[30],DATA12);
SIGNAL(MW_13_YD, vme1.tdc_1.data[31],DATA12);

