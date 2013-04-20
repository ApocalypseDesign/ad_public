//  Disegno delle scanline, by [AD]Turbo (zio animals, ma i
//  lavoracci sporchi li devo sempre fare io ?)
//  ME COJONI! 109 procedure di disegno!!!!
//  69 procedure di 109 CODATE E FUNZIONANTI !
//  mancano le altre 50 che contemplano lo schema di mixing
//  tex+env+luci+trasparenza

#ifndef _SCANLINE_H_
#define _SCANLINE_H_


void scanline_flat(void);
void scanline_RGB(void); // RGB opaco
void scanline_RGB_tBLEND50(void); // RGB con blend 50% (media aritmetica)
void scanline_RGB_tBLENDalpha(void); // RGB con tarsparenza di tipo alpha


// ###########################################################
// *********************  SOLO TEXTURE  **********************
// ###########################################################

// solo texture opaca (ingloba anche il caso di lightmixtype=NONE)
void scanline_texture(void);

// texture trasparente con mixing additivo (saturato)
void scanline_texture_tADD(void);
// texture trasparente con blend 50% (ingloba anche il caso di lightmixtype=NONE)
void scanline_texture_tBLEND50(void);
// texture trasparente con blend di tipo alpha (ingloba anche il caso di lightmixtype=NONE)
void scanline_texture_tBLENDalpha(void);


// texture non trasparente che riceve luce additiva
void scanline_texture_lADD(void);
// texture non trasparente che riceve luce sottrativa
void scanline_texture_lSUB(void);
// texture non trasparente che riceve luce con mix 50%
void scanline_texture_lBLEND50(void);
// texture non trasparente che riceve luce che blenda come nel 3DS4
void scanline_texture_lBLEND3DS(void);


// texture trasparente (blend 50%) e luce additiva
void scanline_texture_tBLEND50_lADD(void);
// texture trasparente (blend 50%) e luce sottrativa
void scanline_texture_tBLEND50_lSUB(void);
// texture trasparente (blend 50%) e luce mediata 50%
void scanline_texture_tBLEND50_lBLEND50(void);
// texture trasparente (blend 50%) e luce che blenda come in 3DS4
void scanline_texture_tBLEND50_lBLEND3DS(void);


// texture trasparente (blend alpha) e luce additiva
void scanline_texture_tBLENDalpha_lADD(void);
// texture trasparente (blend alpha) e luce sottrativa
void scanline_texture_tBLENDalpha_lSUB(void);
// texture trasparente (blend alpha) e luce mediata 50%
void scanline_texture_tBLENDalpha_lBLEND50(void);
// texture trasparente (blend alpha) e luce che blenda come in 3DS4
void scanline_texture_tBLENDalpha_lBLEND3DS(void);




// ###########################################################
// **********************  SOLO ENVMAP  **********************
// ###########################################################

// solo envmap opaca (ingloba anche il caso di lightmixtype=NONE)
void scanline_envmap(void);


void scanline_envmap_tADD(void);
// envmap trasparente con blend 50% (ingloba anche il caso di lightmixtype=NONE)
void scanline_envmap_tBLEND50(void);
// envmap trasparente con blend di tipo alpha (ingloba anche il caso di lightmixtype=NONE)
void scanline_envmap_tBLENDalpha(void);


// envmap non trasparente che riceve luce additiva
void scanline_envmap_lADD(void);
// envmap non trasparente che riceve luce sottrativa
void scanline_envmap_lSUB(void);
// envmap non trasparente che riceve luce con mix 50%
void scanline_envmap_lBLEND50(void);
// envmap non trasparente che riceve luce che blenda come nel 3DS4
void scanline_envmap_lBLEND3DS(void);


// envmap trasparente (blend 50%) e luce additiva
void scanline_envmap_tBLEND50_lADD(void);
// envmap trasparente (blend 50%) e luce sottrativa
void scanline_envmap_tBLEND50_lSUB(void);
// envmap trasparente (blend 50%) e luce mediata 50%
void scanline_envmap_tBLEND50_lBLEND50(void);
// envmap trasparente (blend 50%) e luce che blenda come in 3DS4
void scanline_envmap_tBLEND50_lBLEND3DS(void);


// envmap trasparente (blend alpha) e luce additiva
void scanline_envmap_tBLENDalpha_lADD(void);
// envmap trasparente (blend alpha) e luce sottrativa
void scanline_envmap_tBLENDalpha_lSUB(void);
// envmap trasparente (blend alpha) e luce mediata 50%
void scanline_envmap_tBLENDalpha_lBLEND50(void);
// envmap trasparente (blend alpha) e luce che blenda come in 3DS4
void scanline_envmap_tBLENDalpha_lBLEND3DS(void);




// ###########################################################
// *******************  ENVMAP + TEXTURE  ********************
// ###########################################################

// tex+env (tutti i mixing), no trasparenza, no luci
// (ingloba anche il caso di lightmixtype=NONE)
void scanline_texenv_mADD(void);
void scanline_texenv_mTEXSUBENV(void);
void scanline_texenv_mENVSUBTEX(void);
void scanline_texenv_mBLEND50(void);
void scanline_texenv_mBLENDalpha(void);


// tex+env (tutti i mixing) trasparenti (blend 50%), no luci
// (ingloba anche il caso di lightmixtype=NONE)
void scanline_texenv_mADD_tBLEND50(void);
void scanline_texenv_mTEXSUBENV_tBLEND50(void);
void scanline_texenv_mENVSUBTEX_tBLEND50(void);
void scanline_texenv_mBLEND50_tBLEND50(void);
void scanline_texenv_mBLENDalpha_tBLEND50(void);


// tex+env (tutti i mixing) trasparenti (blend di tipo alpha), no luci
// (ingloba anche il caso di lightmixtype=NONE)
void scanline_texenv_mADD_tBLENDalpha(void);
void scanline_texenv_mTEXSUBENV_tBLENDalpha(void);
void scanline_texenv_mENVSUBTEX_tBLENDalpha(void);
void scanline_texenv_mBLEND50_tBLENDalpha(void);
void scanline_texenv_mBLENDalpha_tBLENDalpha(void);


// tex+env (tutti i mixing) non trasparenti con luce additiva
void scanline_texenv_mADD_lADD(void);
void scanline_texenv_mTEXSUBENV_lADD(void);
void scanline_texenv_mENVSUBTEX_lADD(void);
void scanline_texenv_mBLEND50_lADD(void);
void scanline_texenv_mBLENDalpha_lADD(void);

// tex+env (tutti i mixing) non trasparenti con luce sottrativa
void scanline_texenv_mADD_lSUB(void);
void scanline_texenv_mTEXSUBENV_lSUB(void);
void scanline_texenv_mENVSUBTEX_lSUB(void);
void scanline_texenv_mBLEND50_lSUB(void);
void scanline_texenv_mBLENDalpha_lSUB(void);


// tex+env (tutti i mixing) non trasparenti con luce mediata (blend 50%)
void scanline_texenv_mADD_lBLEND50(void);
void scanline_texenv_mTEXSUBENV_lBLEND50(void);
void scanline_texenv_mENVSUBTEX_lBLEND50(void);
void scanline_texenv_mBLEND50_lBLEND50(void);
void scanline_texenv_mBLENDalpha_lBLEND50(void);


// tex+env (tutti i mixing) non trasparenti con luce (blend 3DS4)
void scanline_texenv_mADD_lBLEND3DS(void);
void scanline_texenv_mTEXSUBENV_lBLEND3DS(void);
void scanline_texenv_mENVSUBTEX_lBLEND3DS(void);
void scanline_texenv_mBLEND50_lBLEND3DS(void);
void scanline_texenv_mBLENDalpha_lBLEND3DS(void);

// -----------------------------------------------------------


// NB: I METODI CHE SEGUONO NON SONO STATI CODATI ANCORA E
// FORSE MAI LO SARANNO !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// tex+env (tutti i mixing) trasparenti (blend 50%) con luce additiva
void scanline_texenv_mADD_tBLEND50_lADD(void);
void scanline_texenv_mTEXSUBENV_tBLEND50_lADD(void);
void scanline_texenv_mENVSUBTEX_tBLEND50_lADD(void);
void scanline_texenv_mBLEND50_tBLEND50_lADD(void);
void scanline_texenv_mBLENDalpha_tBLEND50_lADD(void);

// tex+env (tutti i mixing) trasparenti (blend 50%) con luce sottrativa
void scanline_texenv_mADD_tBLEND50_lSUB(void);
void scanline_texenv_mTEXSUBENV_tBLEND50_lSUB(void);
void scanline_texenv_mENVSUBTEX_tBLEND50_lSUB(void);
void scanline_texenv_mBLEND50_tBLEND50_lSUB(void);
void scanline_texenv_mBLENDalpha_tBLEND50_lSUB(void);


// tex+env (tutti i mixing) trasparenti (blend 50%) con luce mediata (blend 50%)
void scanline_texenv_mADD_tBLEND50_lBLEND50(void);
void scanline_texenv_mTEXSUBENV_tBLEND50_lBLEND50(void);
void scanline_texenv_mENVSUBTEX_tBLEND50_lBLEND50(void);
void scanline_texenv_mBLEND50_tBLEND50_lBLEND50(void);
void scanline_texenv_mBLENDalpha_tBLEND50_lBLEND50(void);


// tex+env (tutti i mixing) trasparenti (blend 50%) con luce (blend 3DS4)
void scanline_texenv_mADD_tBLEND50_lBLEND3DS(void);
void scanline_texenv_mTEXSUBENV_tBLEND50_lBLEND3DS(void);
void scanline_texenv_mENVSUBTEX_tBLEND50_lBLEND3DS(void);
void scanline_texenv_mBLEND50_tBLEND50_lBLEND3DS(void);
void scanline_texenv_mBLENDalpha_tBLEND50_lBLEND3DS(void);

//------------------------------------------------------------

// tex+env (tutti i mixing) trasparenti (blend alpha) con luce additiva
void scanline_texenv_mADD_tBLENDalpha_lADD(void);
void scanline_texenv_mTEXSUBENV_tBLENDalpha_lADD(void);
void scanline_texenv_mENVSUBTEX_tBLENDalpha_lADD(void);
void scanline_texenv_mBLEND50_tBLENDalpha_lADD(void);
void scanline_texenv_mBLENDalpha_tBLENDalpha_lADD(void);

// tex+env (tutti i mixing) trasparenti (blend alpha) con luce sottrativa
void scanline_texenv_mADD_tBLENDalpha_lSUB(void);
void scanline_texenv_mTEXSUBENV_tBLENDalpha_lSUB(void);
void scanline_texenv_mENVSUBTEX_tBLENDalpha_lSUB(void);
void scanline_texenv_mBLEND50_tBLENDalpha_lSUB(void);
void scanline_texenv_mBLENDalpha_tBLENDalpha_lSUB(void);


// tex+env (tutti i mixing) trasparenti (blend alpha) con luce mediata (blend 50%)
void scanline_texenv_mADD_tBLENDalpha_lBLEND50(void);
void scanline_texenv_mTEXSUBENV_tBLENDalpha_lBLEND50(void);
void scanline_texenv_mENVSUBTEX_tBLENDalpha_lBLEND50(void);
void scanline_texenv_mBLEND50_tBLENDalpha_lBLEND50(void);
void scanline_texenv_mBLENDalpha_tBLENDalpha_lBLEND50(void);


// tex+env (tutti i mixing) trasparenti (blend alpha) con luce (blend 3DS4)
void scanline_texenv_mADD_tBLENDalpha_lBLEND3DS(void);
void scanline_texenv_mTEXSUBENV_tBLENDalpha_lBLEND3DS(void);
void scanline_texenv_mENVSUBTEX_tBLENDalpha_lBLEND3DS(void);
void scanline_texenv_mBLEND50_tBLENDalpha_lBLEND3DS(void);
void scanline_texenv_mBLENDalpha_tBLENDalpha_lBLEND3DS(void);

#endif