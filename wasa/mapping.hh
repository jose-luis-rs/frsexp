
//SIGNAL(ZERO_SUPPRESS:CALIFA_CH_1);
SIGNAL(ZERO_SUPPRESS:WASA_ENE_1);


// ENERGY SIGNAL
SIGNAL(WASA_ENE_1,wasa.tdc1.data[0],WASA_ENE_192,wasa.tdc1.data[191],DATA24);

SIGNAL(WASA_ENE_193,wasa.tdc2.data[0],WASA_ENE_384,wasa.tdc2.data[191],DATA24);