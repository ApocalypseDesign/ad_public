// Definizione di una classe matrice di tipo 4x4
// By [Turbo]

#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "vect3D.h"

// Definizione di una matrice 4x4

typedef struct
{
  float4 a[4][4];
} AD_Matrix;


void mat_set(AD_Matrix *m,
			 float4 b11, float4 b12, float4 b13, float4 b14,
             float4 b21, float4 b22, float4 b23, float4 b24,
             float4 b31, float4 b32, float4 b33, float4 b34,
             float4 b41, float4 b42, float4 b43, float4 b44);

void mat_copy (AD_Matrix *m1, AD_Matrix *m2);
void mat_identity(AD_Matrix *m);
void mat_transpose(AD_Matrix *m, AD_Matrix *out);
void mat_insert_row(AD_Matrix *m, int r, AD_Vect3D *v);
void mat_insert_column(AD_Matrix *m, int c, AD_Vect3D *v);
void mat_get_row(AD_Matrix *m, int r, AD_Vect3D *v);
void mat_get_column(AD_Matrix *m, int c, AD_Vect3D *v);
void mat_setmatrix_of_scaling(AD_Matrix *m, float4 sx, float4 sy, float4 sz);
void mat_setmatrix_of_pretraslation(AD_Matrix *m, AD_Vect3D *v);
void mat_setmatrix_of_eulerrotation(AD_Matrix *m, float4 angx, float4 angy, float4 angz);
void mat_setmatrix_of_eulerrotationX(AD_Matrix *m, float4 angx);
void mat_setmatrix_of_eulerrotationY(AD_Matrix *m, float4 angy);
void mat_setmatrix_of_eulerrotationZ(AD_Matrix *m, float4 angz);
void mat_swap_3DS(AD_Matrix *m);
void mat_print_to_file (AD_Matrix *m, FILE *dove);
void mat_add(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_sub(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_mul (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);

void mat_mulvect (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
/*
inline void mat_mulvect (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)

{
//  out->x=m->a[0][0]*v->x+m->a[0][1]*v->y+m->a[0][2]*v->z+m->a[0][3];
//  out->y=m->a[1][0]*v->x+m->a[1][1]*v->y+m->a[1][2]*v->z+m->a[1][3];
//  out->z=m->a[2][0]*v->x+m->a[2][1]*v->y+m->a[2][2]*v->z+m->a[2][3];
  _asm
  {
	
	mov esi, [m]
	mov edi, [v]
	mov edx, [vout]

    // step 1 (x)
	fldz

	fld dword ptr [esi]
	fmul dword ptr [edi]
	fld dword ptr [esi+4]
	fmul dword ptr [edi+4]

	fxch
	faddp st(2), st

	fld dword ptr [esi+8]
	fmul dword ptr [edi+8]

	fxch
	faddp st(2), st

    fadd
	fadd dword ptr [esi+12]

	fstp dword ptr [edx]

	// step 2 (y)
	fldz

	fld dword ptr [esi+16]
	fmul dword ptr [edi]
	fld dword ptr [esi+20]
	fmul dword ptr [edi+4]

	fxch
	faddp st(2), st

	fld dword ptr [esi+24]
	fmul dword ptr [edi+8]

	fxch
	faddp st(2), st

    fadd
	fadd dword ptr [esi+28]

	fstp dword ptr [edx+4]

	// step 3 (z)
	fldz

	fld dword ptr [esi+32]
	fmul dword ptr [edi]
	fld dword ptr [esi+36]
	fmul dword ptr [edi+4]

	fxch
	faddp st(2), st

	fld dword ptr [esi+40]
	fmul dword ptr [edi+8]

	fxch
	faddp st(2), st

    fadd
	fadd dword ptr [esi+44]

	fstp dword ptr [edx+8]
  }
}
*/

void mat_mulaffine (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_mulvect (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_mulvectenv (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_mulvectaffine (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);


// Definizione di una matrice multidimensionale

typedef struct
{
   int rows, columns;
   float *M;
} nMatrix;


#define NOT_SINGULAR 0
#define IS_SINGULAR 1
#define BAD_DIMENSION -1

void nmat_make(nMatrix *m, int r, int c);
void nmat_free(nMatrix *m);
void nmat_copy (nMatrix *m1, nMatrix *m2);
void nmat_identity(nMatrix *m, int dim);
void nmat_add(nMatrix *m1, nMatrix *m2, nMatrix *m3);
void nmat_sub(nMatrix *m1, nMatrix *m2, nMatrix *m3);
void nmat_swap_rows(nMatrix *m, int r1, int r2);
int  nmat_inverse (nMatrix *m1, nMatrix *m2);
void nmat_mulnvect (nMatrix *m, nVector *v, nVector *out); // out=m*v
void nmat_diade (nVector *v, nMatrix *m);  // dot(v, v')

#endif