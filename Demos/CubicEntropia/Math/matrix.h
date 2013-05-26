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


typedef void (*MatrixOP3)(AD_Matrix *, AD_Matrix *, AD_Matrix *);
typedef void (*MatrixVect)(AD_Matrix *, AD_Vect3D *, AD_Vect3D *);

extern MatrixOP3 mat_mul;
extern MatrixVect mat_mulvect;


void mat_set(AD_Matrix *m,
			 float4 b11, float4 b12, float4 b13, float4 b14,
             float4 b21, float4 b22, float4 b23, float4 b24,
             float4 b31, float4 b32, float4 b33, float4 b34,
             float4 b41, float4 b42, float4 b43, float4 b44);

void mat_copy (AD_Matrix *m1, AD_Matrix *m2);
void mat_identity(AD_Matrix *m);
void mat_transpose(AD_Matrix *m, AD_Matrix *out);
void mat_insert_row(AD_Matrix *m, int32 r, AD_Vect3D *v);
void mat_insert_column(AD_Matrix *m, int32 c, AD_Vect3D *v);
void mat_get_row(AD_Matrix *m, int32 r, AD_Vect3D *v);
void mat_get_column(AD_Matrix *m, int32 c, AD_Vect3D *v);
void mat_setmatrix_of_scaling(AD_Matrix *m, float4 sx, float4 sy, float4 sz);
void mat_setmatrix_of_scaling(AD_Matrix *m, AD_Vect3D *v);
void mat_setmatrix_of_pretraslation(AD_Matrix *m, AD_Vect3D *v);
void mat_setmatrix_of_eulerrotation(AD_Matrix *m, float4 angx, float4 angy, float4 angz);
void mat_setmatrix_of_eulerrotationX(AD_Matrix *m, float4 angx);
void mat_setmatrix_of_eulerrotationY(AD_Matrix *m, float4 angy);
void mat_setmatrix_of_eulerrotationZ(AD_Matrix *m, float4 angz);
void mat_swap_3DS(AD_Matrix *m);
void mat_print_to_file (AD_Matrix *m, FILE *dove);
void mat_add(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_sub(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_auto_add(AD_Matrix *m1, AD_Matrix *m2);
void mat_auto_sub(AD_Matrix *m1, AD_Matrix *m2);

// Sono possibili ottimizzazioni in 3DNow!
void mat_mul_C (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_mul_3DNow (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);

void mat_mulvect_C (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_mulvect_asm (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_mulvect_3DNow (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);

void mat_mulaffine (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3);
void mat_mulvectenv (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_mulvectaffine (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout);
void mat_swap_rows(AD_Matrix *m, int32 r1, int32 r2);
void mat_swap_columns(AD_Matrix *m, int32 c1, int32 c2);
int32 mat_inverse (AD_Matrix *m1, AD_Matrix *out);

// calcola gli autovalori e autovettori della matrice (simmetrica)
// A; il parametro sort se messo a 0 indica nessun sorting
int32 mat_eigen (AD_Matrix *A, AD_Vect3D *eigenValues,
				 AD_Matrix *eigenVectors, int32 sort);


// Definizione di una matrice multidimensionale
typedef struct
{
  int32 rows, columns;
  float4 *M;
} nMatrix;


#define NOT_SINGULAR 0
#define IS_SINGULAR 1
#define BAD_DIMENSION -1

void nmat_make(nMatrix *m, int32 r, int32 c);
void nmat_free(nMatrix *m);
void nmat_copy (nMatrix *m1, nMatrix *m2);
void nmat_identity(nMatrix *m, int32 dim);
void nmat_add(nMatrix *m1, nMatrix *m2, nMatrix *m3);
void nmat_sub(nMatrix *m1, nMatrix *m2, nMatrix *m3);
void nmat_swap_rows(nMatrix *m, int32 r1, int32 r2);
int32 nmat_inverse (nMatrix *m1, nMatrix *m2);
void nmat_mulnvect (nMatrix *m, nVector *v, nVector *out); // out=m*v
void nmat_diade (nVector *v, nMatrix *m);  // dot(v, v')

#endif