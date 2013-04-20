// Definizione delle dipendenze esterne per le variabili
// globali usate dal render; tutte queste variabili si trovano
// nel file GLOBREND.CPP

#ifndef _EXTREND_H_
#define _EXTREND_H_


// Constant slopes
extern int du, dv, envdu, envdv, dR, dG, dB;
extern int envdudv;
extern float dz;


// Edge slopes
extern int left_dR, left_dG, left_dB;
extern int left_du, left_dv, left_envdu, left_envdv;
extern int left_dx, right_dx;
extern int left_envdudv;
extern float left_dz;


// Valori correnti durante lo scorrere degli edge
extern int left_x, right_x;
extern int left_u, left_v, left_envu, left_envv;
extern int left_envuv;
extern int left_R, left_G, left_B;
extern int color_flat;
extern float left_z;


// Dati relativi alle texture, envmap e materiale corrente
// NB: la variabile _umask- necessita dei _ perche'
// il vc ha gia' una sua definizione di questa variabile
extern unsigned int *texmap32, *envmap32;
extern unsigned char *texmap8, *envmap8;
extern int vmask, vshift, _umask_, envvshift;


// Dati relativi al buffer video
extern unsigned int *uvideo32;
extern unsigned char *uvideo8;
extern int scrX, scrY, *muly;
extern int offset0;


// Dati relativi allo zbuffer e suo uso
extern float *zbuf;
extern float trick, trick_step;


// Valori correnti nel disegno di una scanline
extern float d;
extern int l, offset, u, v, envu, envv;
extern int fi, fi2;
extern int envuv;
extern int RR1, GG1, BB1;
extern int xx1, xx2;
extern int env_offset, tex_offset;


// Tabelle di mixing usate dalle scanline
extern unsigned char *ttable;  // tabella di tarsparenza corrente
extern unsigned char *atable;  // tabella di alphamixing corrente
extern unsigned char add_table[512], sub_table[512];


// Valori usati nel ciclo di disegno di un triangolo
extern void (*current_scanline_proc)(void);
extern int current_flags;
extern int j, paint_second_part;
extern int top_y, bottom_y;
extern float scan, temp_scan;

#endif