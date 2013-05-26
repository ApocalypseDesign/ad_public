// // Metodi e operatori della classe AD_Matrix
// By [Turbo]
#include <math.h>
#include "matrix.h"
#include "fastmath.h"

#define NMAT_ELEMENT(a, i, j) \
   a->M[i*a->columns+j] \

#define a_11 0
#define a_12 1*4
#define a_13 2*4
#define a_14 3*4
#define a_21 4*4
#define a_22 5*4
#define a_23 6*4
#define a_24 7*4
#define a_31 8*4
#define a_32 9*4
#define a_33 10*4
#define a_34 11*4
#define a_41 12*4
#define a_42 13*4
#define a_43 14*4
#define a_44 15*4

MatrixOP3 mat_mul;
MatrixVect mat_mulvect;

void mat_set(AD_Matrix *m,
			 float4 b11, float4 b12, float4 b13, float4 b14,
             float4 b21, float4 b22, float4 b23, float4 b24,
             float4 b31, float4 b32, float4 b33, float4 b34,
             float4 b41, float4 b42, float4 b43, float4 b44)
{
  m->a[0][0]=b11; m->a[0][1]=b12; m->a[0][2]=b13; m->a[0][3]=b14;
  m->a[1][0]=b21; m->a[1][1]=b22; m->a[1][2]=b23; m->a[1][3]=b24;
  m->a[2][0]=b31; m->a[2][1]=b32; m->a[2][2]=b33; m->a[2][3]=b34;
  m->a[3][0]=b41; m->a[3][1]=b42; m->a[3][2]=b43; m->a[3][3]=b44;
}

//--------------------------------------------------------------------

void mat_copy (AD_Matrix *m1, AD_Matrix *m2)
{
   int32 i;

   for (i=0; i<4; i++)
   {
      m2->a[i][0]=m1->a[i][0];
      m2->a[i][1]=m1->a[i][1];
      m2->a[i][2]=m1->a[i][2];
      m2->a[i][3]=m1->a[i][3];
   }
}

//--------------------------------------------------------------------

void mat_swap_3DS(AD_Matrix *m)
{
  float4 tmp;
  int32 colonne, righe;

  for (colonne=0; colonne<3; colonne++)
  {
    tmp = m->a[colonne][1];
    m->a[colonne][1] = m->a[colonne][2];
    m->a[colonne][2] = tmp;
  }

  for (righe=0; righe<3; righe++)
  {
    tmp = m->a[1][righe];
    m->a[1][righe] =m-> a[2][righe];
    m->a[2][righe] = tmp;
  }
}

//--------------------------------------------------------------------

void mat_print_to_file (AD_Matrix *m, FILE *dove)
{
   fprintf(dove, "[0][0]=%f [0][1]=%f [0][2]=%f [0][3]=%f \n", m->a[0][0],m->a[0][1],m->a[0][2],m->a[0][3]);
   fprintf(dove, "[1][0]=%f [1][1]=%f [1][2]=%f [1][3]=%f \n", m->a[1][0],m->a[1][1],m->a[1][2],m->a[1][3]);
   fprintf(dove, "[2][0]=%f [2][1]=%f [2][2]=%f [2][3]=%f \n", m->a[2][0],m->a[2][1],m->a[2][2],m->a[2][3]);
   fprintf(dove, "[3][0]=%f [3][1]=%f [3][2]=%f [3][3]=%f \n", m->a[3][0],m->a[3][1],m->a[3][2],m->a[3][3]);
}

//--------------------------------------------------------------------

void mat_identity(AD_Matrix *m)
{
  m->a[0][0]=1; m->a[0][1]=0; m->a[0][2]=0; m->a[0][3]=0;
  m->a[1][0]=0; m->a[1][1]=1; m->a[1][2]=0; m->a[1][3]=0;
  m->a[2][0]=0; m->a[2][1]=0; m->a[2][2]=1; m->a[2][3]=0;
  m->a[3][0]=0; m->a[3][1]=0; m->a[3][2]=0; m->a[3][3]=1;
}


//--------------------------------------------------------------------

void mat_transpose(AD_Matrix *m, AD_Matrix *mout)
{
  int32 i;

  for (i=0; i<4; i++)
  {
	mout->a[i][0]=m->a[0][i];
	mout->a[i][1]=m->a[1][i];
	mout->a[i][2]=m->a[2][i];
	mout->a[i][3]=m->a[3][i];
  }
}

//--------------------------------------------------------------------

void mat_insert_row(AD_Matrix *m, int32 r, AD_Vect3D *v)
// inserisce il vettore v nella riga r-esima, con r appartentente a [0..3]
{
  m->a[r][0]=v->x;
  m->a[r][1]=v->y;
  m->a[r][2]=v->z;
}

//--------------------------------------------------------------------

void mat_insert_column(AD_Matrix *m, int32 c, AD_Vect3D *v)
// inserisce il vettore p nella colonna i-esima, con i che sta in [0..3]
{
  m->a[0][c]=v->x;
  m->a[1][c]=v->y;
  m->a[2][c]=v->z;
}

//--------------------------------------------------------------------

void mat_get_row(AD_Matrix *m, int32 r, AD_Vect3D *v)
{
  v->x=m->a[r][0];
  v->y=m->a[r][1];
  v->z=m->a[r][2];
}

//--------------------------------------------------------------------

void mat_get_column(AD_Matrix *m, int32 c, AD_Vect3D *v)
{
  v->x=m->a[0][c];
  v->y=m->a[1][c];
  v->z=m->a[2][c];
}

//--------------------------------------------------------------------

// costruisce una matrice di scaling
void mat_setmatrix_of_scaling(AD_Matrix *m, float4 sx, float4 sy, float4 sz)
{
  mat_identity(m);
  m->a[0][0]=sx;
  m->a[1][1]=sy;
  m->a[2][2]=sz;
}

//--------------------------------------------------------------------

void mat_setmatrix_of_pretraslation(AD_Matrix *m, AD_Vect3D *v)
// costruisce una matrice di pre-traslazione
{
  m->a[0][0]=1; m->a[0][1]=0; m->a[0][2]=0; m->a[0][3]=v->x;
  m->a[1][0]=0; m->a[1][1]=1; m->a[1][2]=0; m->a[1][3]=v->y;
  m->a[2][0]=0; m->a[2][1]=0; m->a[2][2]=1; m->a[2][3]=v->z;
  m->a[3][0]=0; m->a[3][1]=0; m->a[3][2]=0; m->a[3][3]=1;
}

//--------------------------------------------------------------------

void mat_setmatrix_of_eulerrotation(AD_Matrix *m, float4 angx, float4 angy, float4 angz)
{
  float4 c1, s1, c2, s2, c3, s3;

  c1=fast_cosf(angx); s1=fast_sinf(angx);
  c2=fast_cosf(angy); s2=fast_sinf(angy);
  c3=fast_cosf(angz); s3=fast_sinf(angz);

  mat_identity(m);
  m->a[0][0] = c2*c3;
  m->a[0][1] = -c2*s3;
  m->a[0][2] = -s2;
  m->a[1][0] = c1*s3 - s1*s2*c3;
  m->a[1][1] = c1*c3 + s1*s2*s3;
  m->a[1][2] = -s1*c2;
  m->a[2][0] = c1*s2*c3 + s1*s3;
  m->a[2][1] = s1*c3 - c1*s2*s3;
  m->a[2][2] = c1*c2;
}

//--------------------------------------------------------------------

void mat_setmatrix_of_eulerrotationX(AD_Matrix *m, float4 angx)
{
  float4 c, s;

  c=fast_cosf(angx);
  s=fast_sinf(angx);
  mat_identity(m);
  m->a[1][1]=m->a[2][2]=c;
  m->a[2][1]=s;
  m->a[1][2]=-s;
}

//--------------------------------------------------------------------

void mat_setmatrix_of_eulerrotationY(AD_Matrix *m, float4 angy)
{
  float4 c, s;

  c=fast_cosf(angy);
  s=fast_sinf(angy);
  mat_identity(m);
  m->a[0][0]=m->a[2][2]=c;
  m->a[2][0]=s;
  m->a[0][2]=-s;
}

//--------------------------------------------------------------------

void mat_setmatrix_of_eulerrotationZ(AD_Matrix *m, float4 angz)
{
  float4 c, s;

  c=fast_cosf(angz);
  s=fast_sinf(angz);
  mat_identity(m);
  m->a[0][0]=m->a[1][1]=c;
  m->a[0][1]=s;
  m->a[1][0]=-s;
}

//--------------------------------------------------------------------

void mat_add(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  int32 i;

  for (i=0; i<4; i++)
  {
    m3->a[i][0]=m1->a[i][0] + m2->a[i][0];
    m3->a[i][1]=m1->a[i][1] + m2->a[i][1];
    m3->a[i][2]=m1->a[i][2] + m2->a[i][2];
    m3->a[i][3]=m1->a[i][3] + m2->a[i][3];
  }
}

//--------------------------------------------------------------------

void mat_auto_add(AD_Matrix *m1, AD_Matrix *m2)
{
  int32 i;

  for (i=0; i<4; i++)
  {
    m1->a[i][0]=m1->a[i][0] + m2->a[i][0];
    m1->a[i][1]=m1->a[i][1] + m2->a[i][1];
    m1->a[i][2]=m1->a[i][2] + m2->a[i][2];
    m1->a[i][3]=m1->a[i][3] + m2->a[i][3];
  }
}

//--------------------------------------------------------------------

void mat_sub(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  int32 i;

  for (i=0; i<4; i++)
  {
    m3->a[i][0]=m1->a[i][0] - m2->a[i][0];
    m3->a[i][1]=m1->a[i][1] - m2->a[i][1];
    m3->a[i][2]=m1->a[i][2] - m2->a[i][2];
    m3->a[i][3]=m1->a[i][3] - m2->a[i][3];
  }
}

//--------------------------------------------------------------------

void mat_auto_sub(AD_Matrix *m1, AD_Matrix *m2)
{
  int32 i;

  for (i=0; i<4; i++)
  {
    m1->a[i][0]=m1->a[i][0] - m2->a[i][0];
    m1->a[i][1]=m1->a[i][1] - m2->a[i][1];
    m1->a[i][2]=m1->a[i][2] - m2->a[i][2];
    m1->a[i][3]=m1->a[i][3] - m2->a[i][3];
  }
}

//--------------------------------------------------------------------

void mat_mul_C (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  AD_Matrix mt;
  int32 i;

  for (i=0; i<4; i++)
  {
      // moltiplicazione della riga i di m1 con la colonna
      // j di m2
     mt.a[i][0]=m1->a[i][0]*m2->a[0][0]+
                m1->a[i][1]*m2->a[1][0]+
		        m1->a[i][2]*m2->a[2][0]+
				m1->a[i][3]*m2->a[3][0];

	 mt.a[i][1]=m1->a[i][0]*m2->a[0][1]+
                m1->a[i][1]*m2->a[1][1]+
	            m1->a[i][2]*m2->a[2][1]+
				m1->a[i][3]*m2->a[3][1];

	 mt.a[i][2]=m1->a[i][0]*m2->a[0][2]+
                m1->a[i][1]*m2->a[1][2]+
		        m1->a[i][2]*m2->a[2][2]+
				m1->a[i][3]*m2->a[3][2];

	 mt.a[i][3]=m1->a[i][0]*m2->a[0][3]+
                m1->a[i][1]*m2->a[1][3]+
	            m1->a[i][2]*m2->a[2][3]+
		        m1->a[i][3]*m2->a[3][3];
  }
  mat_copy(&mt, m3);
}

//--------------------------------------------------------------------

#ifdef  _MSC_VER
void mat_mul_3DNow (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
    AD_Matrix tmp;
    __asm
    {
        femms
        mov         eax,[m2]			//;source a
        mov         ecx,[m1]			//;source b
        mov         edx,[m3]			//;result r

//        sub         esp,64				//;T_ local work space to store temp results

        movd        mm0,[eax + a_21]    //;       | a_21
        movd        mm1,[eax + a_11]    //;       | a_11
        movd        mm6,[eax + a_12]    //;       | a_12
        punpckldq   mm1,mm0             //; a_21  | a_11  
        movd        mm5,[eax + a_22]    //;       | a_22
        pfmul       mm1,[ecx]           //; a_21 * b_12 | a_11 * b_11
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_32]    //;       | a_32
        movd        mm5,[eax + a_42]    //;       | a_42
        pfmul       mm6,[ecx]           //; a_22 * b_12 | a_12 * b_11     
        movd        mm2,[eax + a_31]    //;       | a_31
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_41]    //;       | a_41
        pfmul       mm7,[ecx+8]         //; a_42 * b_14 | a_32 * b13
        punpckldq   mm2,mm0             //; a_41  | a_31
        pfadd       mm6,mm7				//; a_42 * b_14 + a_22 * b_12 | a_32 * b13 + a_12 * b_11
        pfmul       mm2,[ecx+8]         //; a_41 * b_14 | a_31 * b13
        pfacc       mm6,mm6				//;		| a_12 * b_11 + a_22 * b_12 + a_32 * b_13 + a_42 * b_14  
        pfadd       mm1,mm2				//; a_21 * b_12 + a_41 * b_14 | a_11 * b_11 + a_31 * b13
        movd        [tmp+4],mm6         //; T_12   
        pfacc       mm1,mm1				//;       |  a_21 * b_12 + a_41 * b_14 + a_11 * b_11 + a_31 * b13
        movd        [tmp],mm1           //; T_11

        movd        mm0,[eax + a_23]    //;       | a_23
        movd        mm1,[eax + a_13]    //;       | a_13
        movd        mm6,[eax + a_14]    //;       | a_14
        punpckldq   mm1,mm0             //; a_23  | a_13  
        movd        mm5,[eax + a_24]    //;       | a_24
        pfmul       mm1,[ecx]           //; a_23 * b_12 | a_13 * b_11     
        punpckldq   mm6,mm5             //; a_24  | a_14      
        movd        mm7,[eax + a_34]    //;       | a_34
        movd        mm5,[eax + a_44]    //;       | a_44
        pfmul       mm6,[ecx]           //; a_24 * b_12 | a_14 * b_11     
        movd        mm2,[eax + a_33]    //;       | a_33
        punpckldq   mm7,mm5             //; a_44  | a_34
        movd        mm0,[eax + a_43]    //;       | a_43
        pfmul       mm7,[ecx+8]         //; a_44 * b_14 | a_34 * b_13
        punpckldq   mm2,mm0             //; a_43  | a_33
        pfadd       mm6,mm7				//; a_44 * b_14 + a_24 * b_12 | a_34 * b_13 + a_14 * b_11
        pfmul       mm2,[ecx+8]         //; a_43 * b_12 | a_33 * b11
        pfacc       mm6,mm6				//;		| a_44 * b_14 + a_24 * b_12 + a_34 * b_13 + a_14 * b_11
        pfadd       mm1,mm2				//; a_43 * b_12 + a_23 * b_12 | a_33 * b11 + a_13 * b_11
        movd        [tmp+12],mm6		//; T_14
        pfacc       mm1,mm1				//;		| a_43 * b_12 + a_23 * b_12 + a_33 * b11 + a_13 * b_11
        movd        [tmp+8],mm1			//; T_13

        movd        mm0,[eax + a_21]    //;       | a_21
        movd        mm1,[eax + a_11]    //;       | a_11
        movd        mm6,[eax + a_12]    //;       | a_12
        punpckldq   mm1,mm0             //; a_21  | a_11  
        movd        mm5,[eax + a_22]    //;       | a_22
        pfmul       mm1,[ecx+16]        //; a_21 * b_22 | a_11 * b_21     
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_32]    //;       | a_32
        movd        mm5,[eax + a_42]    //;       | a_42
        pfmul       mm6,[ecx+16]        //; a_22 * b_22 | a_12 * b_21     
        movd        mm2,[eax + a_31]    //;       | a_31
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_41]    //;       | a_41
        pfmul       mm7,[ecx+24]        //; a_42 * b_24 | a_32 * b_23
		punpckldq   mm2,mm0             //; a_41  | a_31
        pfadd       mm6,mm7				//; a_42 * b_24 + a_22 * b_22 | a_32 * b_23 + a_12 * b_21
        pfmul       mm2,[ecx+24]        //; a_41 * b_24 | a_31 * b_23
        pfacc       mm6,mm6				//;       | a_42 * b_24 + a_22 * b_22 + a_32 * b_23 + a_12 * b_21
        pfadd       mm1,mm2				//; a_41 * b_24 + a_21 * b_22 | a_31 * b_23 + a_11 * b_21
        movd        [tmp+20],mm6		//; T_22
        pfacc       mm1,mm1				//;		|a_41 * b_24 + a_21 * b_22 + a_31 * b_23 + a_11 * b_21
        movd        [tmp+16],mm1		//; T_21

        movd        mm0,[eax + a_23]    //;       | a_23
        movd        mm1,[eax + a_13]    //;       | a_13
        movd        mm6,[eax + a_14]    //;       | a_14
        punpckldq   mm1,mm0             //; a_23  | a_13  
        movd        mm5,[eax + a_24]    //;       | a_24
        pfmul       mm1,[ecx+16]        //; a_23 * b_22 | a_13 * b_21 
        punpckldq   mm6,mm5             //; a_24  | a_14      
        movd        mm7,[eax + a_34]    //;       | a_34
        movd        mm5,[eax + a_44]    //;       | a_44
        pfmul       mm6,[ecx+16]        //; a_24 * b_22 | a_14 * b_21     
        movd        mm2,[eax + a_33]    //;       | a_33
        punpckldq   mm7,mm5             //; a_44  | a_34
        movd        mm0,[eax + a_43]    //;       | a_43
        pfmul       mm7,[ecx+24]        //; a_44 * b_24 | a_34 * b_23
        punpckldq   mm2,mm0             //; a_43  | a_33
        pfadd       mm6,mm7				//; a_24 * b_22 + a_44 * b_24 | a_14 * b_21 + a_34 * b_23
        pfmul       mm2,[ecx+24]        //; a_43 * b_24 | a_33 * b_23
        pfacc       mm6,mm6				//;		|a_24 * b_22 + a_44 * b_24 + a_14 * b_21 + a_34 * b_23
        pfadd       mm1,mm2				//; a_43 * b_24 + a_23 * b_22 | a_33 * b_23 + a_14 * b_21
        movd        [tmp+28],mm6		//; T_24
        pfacc       mm1,mm1				//;		| a_43 * b_24 + a_23 * b_22 + a_33 * b_23 + a_14 * b_21
        movd        [tmp+24],mm1		//; T_23

        movd        mm0,[eax + a_21]    //;       | a_21
        movd        mm1,[eax + a_11]    //;       | a_11
        movd        mm6,[eax + a_12]    //;       | a_12
        punpckldq   mm1,mm0             //; a_21  | a_11  
        movd        mm5,[eax + a_22]    //;       | a_22
        pfmul       mm1,[ecx+32]        //; a_21 * b_32 | a_11 * b_31     
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_32]    //;       | a_32
        movd        mm5,[eax + a_42]    //;       | a_42
        pfmul       mm6,[ecx+32]        //; a_22 * b_32 | a_12 * b_31 
        movd        mm2,[eax + a_31]    //;       | a_31
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_41]    //;       | a_41
        pfmul       mm7,[ecx+40]        //; a_42 * b_34 | a_32 * b33
        punpckldq   mm2,mm0             //; a_41  | a_31
        pfadd       mm6,mm7				//; a_42 * b_34 + a_22 * b_32 | a_32 * b33 + a_12 * b_31 
        pfmul       mm2,[ecx+40]        //; a_41 * b_34 | a_31 * b33
        pfacc       mm6,mm6				//;		|a_42 * b_34 + a_22 * b_32 + a_32 * b33 + a_12 * b_31 
        pfadd       mm1,mm2				//; a_41 * b_34 + a_21 * b_32 | a_31 * b33 + a_11 * b_31
        movd        [tmp+36],mm6		//; T_32
        pfacc       mm1,mm1				//;		|a_41 * b_34 + a_21 * b_32 + a_31 * b33 + a_11 * b_31
        movd        [tmp+32],mm1		//; T_31

        movd        mm0,[eax + a_23]    //;       | a_23
        movd        mm1,[eax + a_13]    //;       | a_13
        movd        mm6,[eax + a_14]    //;       | a_14
        punpckldq   mm1,mm0             //; a_23  | a_13  
        movd        mm5,[eax + a_24]    //;       | a_24
        pfmul       mm1,[ecx+32]        //; a_21 * b_32 | a_11 * b_31     
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_34]    //;       | a_34
        movd        mm5,[eax + a_44]    //;       | a_44
        pfmul       mm6,[ecx+32]        //; a_22 * b_32 | a_12 * b_31     
        movd        mm2,[eax + a_33]    //;       | a_33
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_43]    //;       | a_43
        pfmul       mm7,[ecx+40]        //; a_42 * b_34 | a_32 * b_33
        punpckldq   mm2,mm0             //; a_43  | a_33
        pfadd       mm6,mm7				//; a_42 * b_34 + a_22 * b_32 | a_32 * b_33 + a_12 * b_31
        pfmul       mm2,[ecx+40]        //; a_41 * b_34 | a_31 * b_33
        pfacc       mm6,mm6				//;		|a_42 * b_34 + a_22 * b_32 + a_32 * b_33 + a_12 * b_31
        pfadd       mm1,mm2				//; a_41 * b_34 + a_21 * b_32 | a_31 * b_33 + a_11 * b_31
        movd        [tmp+44],mm6		//; T_34
        pfacc       mm1,mm1				//;		|a_41 * b_34 + a_21 * b_32 + a_31 * b_33 + a_11 * b_31
        movd        [tmp+40],mm1		//; T_33

        movd        mm0,[eax + a_21]    //;       | a_21
        movd        mm1,[eax + a_11]    //;       | a_11
        movd        mm6,[eax + a_12]    //;       | a_12
        punpckldq   mm1,mm0             //; a_21  | a_11  
        movd        mm5,[eax + a_22]    //;       | a_22
        pfmul       mm1,[ecx+48]        //; a_21 * b_42 | a_11 * b_41     
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_32]    //;       | a_32
        movd        mm5,[eax + a_42]    //;       | a_42
        pfmul       mm6,[ecx+48]        //; a_22 * b_42 | a_12 * b_41
        movd        mm2,[eax + a_31]    //;       | a_31
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_41]    //;       | a_41
        pfmul       mm7,[ecx+56]        //; a_42 * b_44 | a_32 * b_43
        punpckldq   mm2,mm0             //; a_41  | a_31
        pfadd       mm6,mm7				//; a_42 * b_44 + a_22 * b_42 | a_32 * b_43 + a_12 * b_41
        pfmul       mm2,[ecx+56]        //; a_41 * b_44 | a_31 * b_43
        pfacc       mm6,mm6				//;		|a_42 * b_44 + a_22 * b_42 + a_32 * b_43 + a_12 * b_41
        pfadd       mm1,mm2				//; a_41 * b_44 + a_21 * b_42 | a_31 * b_43 + a_11 * b_41
        movd        [tmp+52],mm6		//; T_42
        pfacc       mm1,mm1				//;		| a_41 * b_44 + a_21 * b_42 + a_31 * b_43 + a_11 * b_41
        movd        [tmp+48],mm1		//; T_41

        movd        mm0,[eax + a_23]    //;       | a_23
        movd        mm1,[eax + a_13]    //;       | a_13
        movd        mm6,[eax + a_14]    //;       | a_14
        punpckldq   mm1,mm0             //; a_23  | a_13  
        movd        mm5,[eax + a_24]    //;       | a_24
        pfmul       mm1,[ecx+48]        //; a_21 * b_42 | a_11 * b_41     
        punpckldq   mm6,mm5             //; a_22  | a_12      
        movd        mm7,[eax + a_34]    //;       | a_34
        movd        mm5,[eax + a_44]    //;       | a_44
        pfmul       mm6,[ecx+48]        //; a_22 * b_42 | a_12 * b_41     
        movd        mm2,[eax + a_33]    //;       | a_33
        punpckldq   mm7,mm5             //; a_42  | a_32
        movd        mm0,[eax + a_43]    //;       | a_43
        pfmul       mm7,[ecx+56]        //; a_42 * b_44 | a_32 * b_43
        punpckldq   mm2,mm0             //; a_43  | a_33
        pfadd       mm6,mm7				//; a_42 * b_44 + a_22 * b_42 | a_32 * b_43 + a_12 * b_41
        pfmul       mm2,[ecx+56]        //; a_41 * b_44 | a_31 * b_43
        pfacc       mm6,mm6				//;		|a_42 * b_44 + a_22 * b_42 + a_32 * b_43 + a_12 * b_41
        pfadd       mm1,mm2				//; a_41 * b_44 + a_21 * b_42 | a_31 * b_43 + a_11 * b_41 
        movd        [tmp+60],mm6		//; T_44
        pfacc       mm1,mm1				//; a_41 * b_44 + a_21 * b_42 + a_31 * b_43 + a_11 * b_41 
        movd        [tmp+56],mm1		//; T_43


        movq        mm3,[tmp]			//;MOVE FROM LOCAL TEMP MATRIX TO ADDRESS OF RESULT
        movq        mm4,[tmp+8]
        movq        [edx],mm3
        movq        [edx+8],mm4

        movq        mm3,[tmp+16]
        movq        mm4,[tmp+24]
        movq        [edx+16],mm3
        movq        [edx+24],mm4

        movq        mm3,[tmp+32]
        movq        mm4,[tmp+40]
        movq        [edx+32],mm3
        movq        [edx+40],mm4

        movq        mm3,[tmp+48]
        movq        mm4,[tmp+56]
        movq        [edx+48],mm3
        movq        [edx+56],mm4

        femms
    }
}
#endif

//--------------------------------------------------------------------

void mat_mulaffine (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  AD_Matrix mt;
  int i, j;

  for (i=0; i<3; i++)
    for (j=0; j<3; j++)
      // moltiplicazione della riga i di m1 con la colonna
      // j di m2
      mt.a[i][j]=m1->a[i][0]*m2->a[0][j]+
                 m1->a[i][1]*m2->a[1][j]+
		         m1->a[i][2]*m2->a[2][j]+
		         m1->a[i][3]*m2->a[3][j];

  mat_copy(&mt, m3);
}

//--------------------------------------------------------------------

void mat_mulvect_C (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
  vout->x=m->a[0][0]*v->x+m->a[0][1]*v->y+m->a[0][2]*v->z+m->a[0][3];
  vout->y=m->a[1][0]*v->x+m->a[1][1]*v->y+m->a[1][2]*v->z+m->a[1][3];
  vout->z=m->a[2][0]*v->x+m->a[2][1]*v->y+m->a[2][2]*v->z+m->a[2][3];
}

//--------------------------------------------------------------------

void mat_mulvect_asm (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
  _asm
  {
    push edx
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
	pop edx
  }
}

//--------------------------------------------------------------------

#ifdef  _MSC_VER
void mat_mulvect_3DNow (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
	float uno=1.0f;

	_asm
	{
        femms
        mov         eax,[m]
        mov         ecx,[v]
        mov         edx,[vout]

        movq        mm0, [ecx]       //  mm0 = | y | x |
        movd        mm1, [ecx+8]
		
		punpckldq   mm1, [uno]       //  mm1 = | 1 | z |
        movq        mm2, [eax + a_11]

        movq        mm3, [eax + a_13]
		pfmul       mm2, mm0

        pfmul       mm3, mm1
        movq        mm4, [eax + a_21]

        movq        mm5, [eax + a_23]
		pfadd       mm2, mm3

        pfmul       mm4, mm0
        pfacc       mm2, mm2     // xout a posto

        pfmul       mm5, mm1
		movd        [edx], mm2

		pfadd       mm4, mm5
        movq        mm6, [eax + a_31]

        pfacc       mm4, mm4
        movq        mm7, [eax + a_33]

		movd        [edx+4], mm4
        pfmul       mm6, mm0

        pfmul       mm7, mm1
		pfadd       mm6, mm7

        pfacc       mm6, mm6
		movd        [edx+8], mm6

        femms
	}
}
#endif

//--------------------------------------------------------------------

void mat_mulvectenv (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
/*  vout->x=m->a[0][0]*v->x+m->a[0][1]*v->y+m->a[0][2]*v->z;
  vout->y=m->a[1][0]*v->x+m->a[1][1]*v->y+m->a[1][2]*v->z;
*/
  _asm
  {
	push edx
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
	fstp dword ptr [edx+4]
	pop edx
  }
}

//--------------------------------------------------------------------

void mat_mulvectaffine (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
/*
  vout->x=m->a[0][0]*v->x+m->a[0][1]*v->y+m->a[0][2]*v->z;
  vout->y=m->a[1][0]*v->x+m->a[1][1]*v->y+m->a[1][2]*v->z;
  vout->z=m->a[2][0]*v->x+m->a[2][1]*v->y+m->a[2][2]*v->z;
*/
  _asm
  {
	push edx
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
	fstp dword ptr [edx+8]
	pop edx
  }
}

//--------------------------------------------------------------------

void mat_swap_rows(AD_Matrix *m, int32 r1, int32 r2)
{
  int32 g;
  float4 f;

  for (g=0; g<4; g++)
  {
	f=m->a[r1][g];
    m->a[r1][g]=m->a[r2][g];
    m->a[r2][g]=f;
  }
}

//--------------------------------------------------------------------

void mat_swap_columns(AD_Matrix *m, int32 c1, int32 c2)
{
  int32 g;
  float4 f;

  for (g=0; g<4; g++)
  {
	f=m->a[g][c1];
    m->a[g][c1]=m->a[g][c2];
    m->a[g][c2]=f;
  }
}

//--------------------------------------------------------------------

int32 mat_inverse (AD_Matrix *m1, AD_Matrix *out)
{
  int32 r, c, k;
  int32 maxrow;
  float4 maxval, val;
  AD_Matrix M;

  if ((!m1) || (!out)) return(0);

  mat_copy(m1, &M);
  mat_identity(out);
  maxrow=0;
  maxval=0;
  for (r=0; r<4; r++)
  {
	// cerco la riga che ha l'elemento + grosso nella diagonale
	// ovvero la riga R per cui mat(R, r)=max
	maxval =  M.a[r][r];
    maxrow = r;
    for (c=r+1; c<4; c++)
	{
      val = M.a[c][r];
      if (fabs(val) > fabs(maxval))
	  {
        maxval = val;
        maxrow = r;
	  }
	}
    
    // scambio la riga avente elemento massimo con la corrente
	// questo permettera' una stabilita' numerica molto maggiore
    if (maxrow != r)
	{
      mat_swap_rows(&M, r, maxrow);
      mat_swap_rows(out, r, maxrow);
	}

    // Divido la riga corrente per il massimo valore per avere
    // un 1 sulla diagonale
	maxval=1.0f/maxval;
    for (k=0; k<4; k++)
	{
      M.a[r][k] *= maxval;
      out->a[r][k] *= maxval;
	}

    // Faccio un'operazione di "pivotaggio" con pivot in
	// posizione (r,r), cosi' metto a zero tutti gli elementi
	// in posizione (c, r)  r+1 <= c <= dimensione-1
    for (c=r+1; c<4; c++)
	{
      val = M.a[c][r];
      for (k=0; k<4; k++)
	  {
        M.a[c][k] -= M.a[r][k]*val;
        out->a[c][k] -= out->a[r][k]*val;
      }
    }
  }

  // ora la matrice e' triangolare superiore, renderla diagonale
  // e' facile, basta operare dal basso verso l'alto
  for (r=3; r>=0; r--)
  {
    for (c=r-1; c>=0; c--)
	{
      val = M.a[c][r];;
      for (k=0; k<4; k++)
	  {
        M.a[c][k] -= M.a[r][k]*val;
        out->a[c][k] -= out->a[r][k]*val;
      } 
    } 
  }
  return(NOT_SINGULAR);
}

//--------------------------------------------------------------------
// Calcola gli autovalori e autovettori della matrice (simmetrica) A
// Usa il metodo di Jacobi
#define ROTATE(aa, ii, jj, kk, ll) \
	g=aa[ii][jj]; \
	h=aa[kk][ll]; \
	aa[ii][jj]=g-s*(h+g*tau); \
	aa[kk][ll]=h+s*(g-h*tau); \

int32 mat_eigen (AD_Matrix *A, AD_Vect3D *eigenValues,
                 AD_Matrix *eigenVectors, int32 sort)
{
  AD_Matrix l_copyA;
  int32 j, iq, ip, i, nrot;
  float4 tresh, theta, tau, t, sm, s, h, g, c;
  float4 b[3], z[3];
  float4 *d;
  int32 n=3;

  mat_copy(A, &l_copyA);
  // inizializzazioni
  d=&eigenValues->x;
  mat_identity(eigenVectors);
  z[0]=z[1]=z[2]=0;
  b[0]=d[0]=A->a[0][0];
  b[1]=d[1]=A->a[1][1];
  b[2]=d[2]=A->a[2][2];
  nrot=0;

  for (i=0; i<50; i++)
  {
     sm=0;
	 for (ip=0; ip<n-1; ip++)
	 {
        for (iq=ip+1; iq<n; iq++)
           sm+=(float4)fabs(A->a[ip][iq]);
	 }
	 if (sm==0) goto sorting;
     if (i<4) tresh=0.2f*sm/(n*n);
	 else tresh=0;

	 for (ip=0; ip<n-1; ip++)
	 {
        for (iq=ip+1; iq<n; iq++)
		{
           g=(float4)(100.0f*fabs(A->a[ip][iq]));
		   if ( (i>4) && 
			    ((fabs(d[ip])+g) == fabs(d[ip])) &&
                ((fabs(d[iq])+g) == fabs(d[iq]))
              )
              A->a[ip][iq]=0;
		   else
           if (fabs(A->a[ip][iq]) > tresh)
		   {
              h=d[iq]-d[ip];
			  if ((fabs(h)+g) == fabs(h)) t=A->a[ip][iq]/h;
			  else {
				      theta=(float4)(0.5*h/A->a[ip][iq]);
					  t=(float4)(1.0f/(fabs(theta)+(float4)sqrt(1.0+theta*theta)));
					  if (theta<0) t=-t;
			  }
			  c=1.0f/(float4)sqrt(1+t*t);
			  s=t*c;
			  tau=s/(1.0f+c);
			  h=t*A->a[ip][iq];
			  z[ip]-=h;
			  z[iq]+=h;
			  d[ip]-=h;
			  d[iq]+=h;
			  A->a[ip][iq]=0;
			  for (j=0; j<ip-1; j++)
			  {
                 ROTATE(A->a, j, ip, j, iq)
			  }
			  for (j=ip+1; j<=iq-1; j++)
			  {
                 ROTATE(A->a, ip, j, j, iq)
			  }
			  for (j=iq+1; j<n; j++)
			  {
                 ROTATE(A->a, ip, j, iq, j)
			  }
			  for (j=0; j<n; j++)
			  {
                 ROTATE(eigenVectors->a, j, ip, j, iq)
			  }
			  nrot++;
		   }
		}
	 }
	 for (ip=0; ip<n; ip++)
	 {
        b[ip]+=z[ip];
		d[ip]=b[ip];
		z[ip]=0;
	 }
  }

sorting:
;
  if (!sort)
  {
     mat_copy(&l_copyA, A);
	 return(nrot);
  }
  else
  {
    // sorting degli autovalori e autovettori
    float4 p;
    int32 k;
    for (i=0; i<n-1; i++)
	{
      p=d[k=i];
	  for (j=i+1; j<n; j++) if (d[j] >= p) p=d[k=j];
      if (k != i)
	  {
         d[k]=d[i];
	     d[i]=p;
	     for (j=0; j<n; j++)
		 {
            p=eigenVectors->a[j][i];
		    eigenVectors->a[j][i]=eigenVectors->a[j][k];
		    eigenVectors->a[j][k]=p;
		 }
	  }
	}
  }
  mat_copy(&l_copyA, A);
  return(nrot);
}

//--------------------------------------------------------------------

//********************************************************************
//                      MATRICI MULTIDIMENSIONALI
//********************************************************************
void nmat_make(nMatrix *m, int r, int c)
{
  int32 t;

  if ((r<0) || (c<0)) return;

  m->rows=r;
  m->columns=c;
  m->M=new float4[r*c];
  for (t=0; t<r*c; t++) m->M[t]=0;
}

//--------------------------------------------------------------------

void nmat_free(nMatrix *m)
{
  if (m->M!=(float4 *)NULL) delete [] m->M;
  m->rows=m->columns=0;
}

//--------------------------------------------------------------------

void nmat_copy (nMatrix *m1, nMatrix *m2)
{
  int32 t;

  for (t=0; t<m1->columns*m1->rows; t++)
  {
	  m2->M[t]=m1->M[t];
  }
}

//--------------------------------------------------------------------

void nmat_identity(nMatrix *m, int32 dim)
{
  int32 r, c;

  if (m->columns!=m->rows) return;

  for (r=0; r<dim; r++)
  {
	for (c=0; c<dim; c++)
	{
	  if (c!=r) NMAT_ELEMENT(m, r, c)=0;
      else NMAT_ELEMENT(m, r, c)=1;
	}
  }
}

//--------------------------------------------------------------------

void nmat_add(nMatrix *m1, nMatrix *m2, nMatrix *m3)
{
  int32 t;

  for (t=0; t<m1->columns*m1->rows; t++)
  {
	m3->M[t]=m2->M[t] + m1->M[t];
  }
}

//--------------------------------------------------------------------

void nmat_sub(nMatrix *m1, nMatrix *m2, nMatrix *m3)
{
  int32 t;

  for (t=0; t<m1->columns*m1->rows; t++)  
  {
	m3->M[t]=m1->M[t] - m2->M[t];
  }
}

//--------------------------------------------------------------------

void nmat_swap_rows(nMatrix *m, int32 r1, int32 r2)
{
  int32 g;
  float4 f;

  for (g=0; g<m->columns; g++)
  {
	f=NMAT_ELEMENT(m, r1, g);
    NMAT_ELEMENT(m, r1, g) = NMAT_ELEMENT(m, r2, g);
	NMAT_ELEMENT(m, r2, g)=f;
  }
}

//--------------------------------------------------------------------
	
int32 nmat_inverse (nMatrix *m1, nMatrix *m2)
{
  int32 r, c, k;
  int32 maxrow;
  float4 maxval, val;

  // N.B. : la matrice m2 deve essere gia' stata allocata !!!
  if (m1->columns!=m1->rows) return(BAD_DIMENSION);

  nmat_identity(m2, m1->rows);
  maxrow=0;
  maxval=0;
  for (r=0; r<m1->rows; r++)
  {
	// cerco la riga che ha l'elemento + grosso nella diagonale
	// ovvero la riga R per cui mat(R, r)=max
	maxval = NMAT_ELEMENT(m1, r, r);
    maxrow = r;
    for (c=r+1; c<m1->rows; c++)
	{
      val = NMAT_ELEMENT(m1, c, r); 
      if (fabs(val) > fabs(maxval))
	  {
        maxval = val;
        maxrow = r;
	  }
	}
    
    // scambio la riga avente elemento massimo con la corrente
	// questo permettera' una stabilita' numerica molto maggiore
    if (maxrow != r)
	{
      nmat_swap_rows(m1, r, maxrow);
      nmat_swap_rows(m2, r, maxrow);
	}

    // Divido la riga corrente per il massimo valore per avere
    // un 1 sulla diagonale
	maxval=1.0f/maxval;
    for (k=0; k<m1->rows; k++)
	{
      NMAT_ELEMENT(m1, r, k) *= maxval;
      NMAT_ELEMENT(m2, r, k) *= maxval;
	}

    // Faccio un'operazione di "pivotaggio" con pivot in
	// posizione (r,r), cosi' metto a zero tutti gli elementi
	// in posizione (c, r)  r+1 <= c <= dimensione-1
    for (c=r+1; c<m1->rows; c++)
	{
      val = NMAT_ELEMENT(m1, c, r);
      for (k=0; k<m1->rows; k++)
	  {
        NMAT_ELEMENT(m1, c, k) -= NMAT_ELEMENT(m1, r, k)*val;
        NMAT_ELEMENT(m2, c, k) -= NMAT_ELEMENT(m2, r, k)*val;
      }
    }
  }

  // ora la matrice e' triangolare superiore, renderla diagonale
  // e' facile, basta operare dal basso verso l'alto
  for (r=m1->rows-1; r>=0; r--)
  {
    for (c=r-1; c>=0; c--)
	{
      val = NMAT_ELEMENT(m1, c, r);
      for (k=0; k<m1->rows; k++)
	  {
        NMAT_ELEMENT(m1, c, k) -= NMAT_ELEMENT(m1, r, k)*val; 
        NMAT_ELEMENT(m2, c, k) -= NMAT_ELEMENT(m2, r, k)*val; 
      } 
    } 
  }
  return(NOT_SINGULAR);
}

//--------------------------------------------------------------------

void nmat_mulnvect (nMatrix *m, nVector *v, nVector *out)
{
  int32 t, w;
  float4 faux;

  for (t=0; t<m->rows; t++)
  {
	faux=0;
	for (w=0; w<m->columns; w++)
	{
	  faux=faux + NMAT_ELEMENT(m, t, w)*v->v[w];
	}

	out->v[t]=faux;
  }
}

//--------------------------------------------------------------------

void nmat_diade (nVector *v, nMatrix *m)
// Costruisce la diade data da vv'
{
  int32 i, j;

  for (i=0; i<v->dimension; i++)
  {
	for (j=0; j<v->dimension; j++)
	{
	  NMAT_ELEMENT(m, i, j)=v->v[i]*v->v[j];
	}
  }
}