#ifndef _SETMATERIAL_H_
#define _SETMATERIAL_H_

// procedure di settaggio D3D per ogni materiale

void setNULL(void);
void setRGB(void);
void setRGB_tADD(void);
void setRGB_lBLEND3DS(void);

void setNULLMap(void);
void setMap1(void);
void setMap12(void);

// #############
// #  TEXTURE  #
// #############
void setTex (void);

void setTex_tBLENDalpha (void);    // OK!
void setTex_tMAPalpha (void);      // OK!
void setTex_tADD (void);           // OK!

void setTex_lBLEND3DS (void);      // OK!
void setTex_lADD (void);           // OK!
void setTex_lSUB (void);           // OK!

void setTex_lBLEND3DS_tBLENDalpha (void);   // OK!
void setTex_lBLEND3DS_tADD (void);          // OK!
void setTex_lBLEND3DS_tMAPalpha (void);     // OK! 

void setTex_lADD_tBLENDalpha (void);   // OK!
void setTex_lADD_tADD (void);          // OK!
void setTex_lADD_tMAPalpha (void);     // OK!

void setTex_lSUB_tBLENDalpha (void);   // OK!
void setTex_lSUB_tADD (void);          // OK!
void setTex_lSUB_tMAPalpha (void);     // OK!


// #############
// #  ENVMAP   #
// #############
void setEnv (void);

void setEnv_tBLENDalpha (void);   // OK!
void setEnv_tMAPalpha (void);     // OK!
void setEnv_tADD (void);          // OK!

void setEnv_lBLEND3DS (void);     // OK!
void setEnv_lADD (void);          // OK!
void setEnv_lSUB (void);          // OK!

void setEnv_lBLEND3DS_tBLENDalpha (void);    // OK!
void setEnv_lBLEND3DS_tADD (void);           // OK!
void setEnv_lBLEND3DS_tMAPalpha (void);      // OK!

void setEnv_lADD_tBLENDalpha (void);     // OK!
void setEnv_lADD_tADD (void);            // OK!
void setEnv_lADD_tMAPalpha (void);       // OK!

void setEnv_lSUB_tBLENDalpha (void);     // OK!
void setEnv_lSUB_tADD (void);            // OK!
void setEnv_lSUB_tMAPalpha (void);       // OK!


// #####################
// #  TEXTURE + ENVMAP #
// #####################
void setTexEnv_mBLEND3DS(void);          // OK!
void setTexEnv_mADD(void);               // OK!
void setTexEnv_mTEXSUBENV(void);         // OK!
void setTexEnv_mENVSUBTEX(void);         // OK!

// TEX + ENV + TRASPARENZA
void setTexEnv_mBLEND3DS_tBLENDalpha(void);   // OK!
void setTexEnv_mBLEND3DS_tADD(void);          // OK!
void setTexEnv_mBLEND3DS_tMAPalpha(void);     // OK!
void setTexEnv_mADD_tBLENDalpha(void);        // OK!
void setTexEnv_mADD_tADD(void);               // OK!
void setTexEnv_mADD_tMAPalpha(void);          // OK!

// TEX + ENV + LUCI
void setTexEnv_mADD_lADD(void);          // OK!
void setTexEnv_mADD_lSUB(void);          // OK!
void setTexEnv_mADD_lBLEND3DS(void);     // OK!
void setTexEnv_mBLEND3DS_lADD(void);       // OK!
void setTexEnv_mBLEND3DS_lSUB(void);       // OK!
void setTexEnv_mBLEND3DS_lBLEND3DS(void);  // OK!


//void setTexEnv_mTEXSUBENV(void);
//void setTexEnv_mENVSUBTEX(void);

#endif