// // Metodi e operatori della classe AD_Matrix
// By [Turbo]
#include <math.h>
#include "matrix.h"

#define NMAT_ELEMENT(a, i, j) \
   a->M[i*a->columns+j] \


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


void mat_copy (AD_Matrix *m1, AD_Matrix *m2)
{
   int i, j;

   for (i=0; i<4; i++)
	   for (j=0; j<4; j++)
		   m2->a[i][j]=m1->a[i][j];

/*
  _asm
  {
	push ecx
	mov esi, [m1]

	mov edi, [m2]
	mov ecx, 16

	rep movsd
	pop ecx
  }
*/
}


void mat_swap_3DS(AD_Matrix *m)
{
  float4 tmp;
  int colonne, righe;

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


void mat_print_to_file (AD_Matrix *m, FILE *dove)
{
   fprintf(dove, "[0][0]=%f [0][1]=%f [0][2]=%f [0][3]=%f \n", m->a[0][0],m->a[0][1],m->a[0][2],m->a[0][3]);
   fprintf(dove, "[1][0]=%f [1][1]=%f [1][2]=%f [1][3]=%f \n", m->a[1][0],m->a[1][1],m->a[1][2],m->a[1][3]);
   fprintf(dove, "[2][0]=%f [2][1]=%f [2][2]=%f [2][3]=%f \n", m->a[2][0],m->a[2][1],m->a[2][2],m->a[2][3]);
   fprintf(dove, "[3][0]=%f [3][1]=%f [3][2]=%f [3][3]=%f \n", m->a[3][0],m->a[3][1],m->a[3][2],m->a[3][3]);
}

// METODI
void mat_identity(AD_Matrix *m)
{
  m->a[0][0]=1; m->a[0][1]=0; m->a[0][2]=0; m->a[0][3]=0;
  m->a[1][0]=0; m->a[1][1]=1; m->a[1][2]=0; m->a[1][3]=0;
  m->a[2][0]=0; m->a[2][1]=0; m->a[2][2]=1; m->a[2][3]=0;
  m->a[3][0]=0; m->a[3][1]=0; m->a[3][2]=0; m->a[3][3]=1;
}


void mat_transpose(AD_Matrix *m, AD_Matrix *mout)
{

  int i, j;

  for (i=0; i<4; i++)
  {
    for(j=0; j<4; j++)
	{
		mout->a[i][j]=m->a[j][i];
	}
  }
/*

  _asm
  {
	  push ecx
	  push eax
	  push ebx

	  mov esi, [m]
	  mov edi, [mout]
	  mov ecx, 4

ciclo:

      mov eax, [esi]
	  mov ebx, [esi+4]
      mov [edi], eax
	  mov [edi+16], ebx

      mov eax, [esi+8]
	  mov ebx, [esi+12]
      mov [edi+32], eax
	  mov [edi+48], ebx

      add esi, 16
	  add edi, 4
	  
      dec ecx
	  jnz ciclo

	  pop ebx
	  pop eax
	  pop ecx
  }
*/
}


void mat_insert_row(AD_Matrix *m, int r, AD_Vect3D *v)
// inserisce il vettore v nella riga r-esima, con r appartentente a [0..3]
{
  m->a[r][0]=v->x;
  m->a[r][1]=v->y;
  m->a[r][2]=v->z;
}


void mat_insert_column(AD_Matrix *m, int c, AD_Vect3D *v)
// inserisce il vettore p nella colonna i-esima, con i che sta in [0..3]
{
  m->a[0][c]=v->x;
  m->a[1][c]=v->y;
  m->a[2][c]=v->z;
}


void mat_get_row(AD_Matrix *m, int r, AD_Vect3D *v)
{
  v->x=m->a[r][0];
  v->y=m->a[r][1];
  v->z=m->a[r][2];
}


void mat_get_column(AD_Matrix *m, int c, AD_Vect3D *v)
{
  v->x=m->a[0][c];
  v->y=m->a[1][c];
  v->z=m->a[2][c];
}

// costruisce una matrice di scaling
void mat_setmatrix_of_scaling(AD_Matrix *m, float4 sx, float4 sy, float4 sz)
{
  mat_identity(m);
  m->a[0][0]=sx;
  m->a[1][1]=sy;
  m->a[2][2]=sz;
}


void mat_setmatrix_of_pretraslation(AD_Matrix *m, AD_Vect3D *v)
// costruisce una matrice di pre-traslazione
{
  m->a[0][0]=1; m->a[0][1]=0; m->a[0][2]=0; m->a[0][3]=v->x;
  m->a[1][0]=0; m->a[1][1]=1; m->a[1][2]=0; m->a[1][3]=v->y;
  m->a[2][0]=0; m->a[2][1]=0; m->a[2][2]=1; m->a[2][3]=v->z;
  m->a[3][0]=0; m->a[3][1]=0; m->a[3][2]=0; m->a[3][3]=1;
}


void mat_setmatrix_of_eulerrotation(AD_Matrix *m, float4 angx, float4 angy, float4 angz)
{
  float4 c1, s1, c2, s2, c3, s3;

  c1=(float)cos(angx); s1=(float)sin(angx);
  c2=(float)cos(angy); s2=(float)sin(angy);
  c3=(float)cos(angz); s3=(float)sin(angz);

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


void mat_setmatrix_of_eulerrotationX(AD_Matrix *m, float4 angx)
{
  float4 c, s;

  c=(float)cos(angx); s=(float)sin(angx);
  mat_identity(m);
  m->a[1][1]=m->a[2][2]=c;
  m->a[2][1]=s;
  m->a[1][2]=-s;
}


void mat_setmatrix_of_eulerrotationY(AD_Matrix *m, float4 angy)
{
  float4 c, s;

  c=(float)cos(angy); s=(float)sin(angy);
  mat_identity(m);
  m->a[0][0]=m->a[2][2]=c;
  m->a[2][0]=s;
  m->a[0][2]=-s;
}


void mat_setmatrix_of_eulerrotationZ(AD_Matrix *m, float4 angz)
{
  float4 c, s;

  c=(float)cos(angz); s=(float)sin(angz);
  mat_identity(m);
  m->a[0][0]=m->a[1][1]=c;
  m->a[0][1]=s;
  m->a[1][0]=-s;
}


void mat_add(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  int i;

  for (i=0; i<4; i++)
  {
    m3->a[i][0]=m1->a[i][0] + m2->a[i][0];
    m3->a[i][1]=m1->a[i][1] + m2->a[i][1];
    m3->a[i][2]=m1->a[i][2] + m2->a[i][2];
    m3->a[i][3]=m1->a[i][3] + m2->a[i][3];
  }
}


void mat_sub(AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  int i;

  for (i=0; i<4; i++)
  {
    m3->a[i][0]=m1->a[i][0] - m2->a[i][0];
    m3->a[i][1]=m1->a[i][1] - m2->a[i][1];
    m3->a[i][2]=m1->a[i][2] - m2->a[i][2];
    m3->a[i][3]=m1->a[i][3] - m2->a[i][3];
  }
}


void mat_mul (AD_Matrix *m1, AD_Matrix *m2, AD_Matrix *m3)
{
  AD_Matrix mt;
  int i;

  for (i=0; i<4; i++)
  {
      // moltiplicazione della riga i di m1 con la colonna
      // j di m2
     mt.a[i][0]=m1->a[i][0]*m2->a[0][0]+
                m1->a[i][1]*m2->a[1][0]+
		        m1->a[i][2]*m2->a[2][0];
//				+m1->a[i][3]*m2->a[3][0];

	 mt.a[i][1]=m1->a[i][0]*m2->a[0][1]+
                m1->a[i][1]*m2->a[1][1]+
	            m1->a[i][2]*m2->a[2][1];
//				+m1->a[i][3]*m2->a[3][1];

	 mt.a[i][2]=m1->a[i][0]*m2->a[0][2]+
                m1->a[i][1]*m2->a[1][2]+
		        m1->a[i][2]*m2->a[2][2];
//				+m1->a[i][3]*m2->a[3][2];

	 mt.a[i][3]=m1->a[i][0]*m2->a[0][3]+
                m1->a[i][1]*m2->a[1][3]+
	            m1->a[i][2]*m2->a[2][3]+
		        m1->a[i][3];
  }

  mat_copy(&mt, m3);
}


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


void mat_mulvect (AD_Matrix *m, AD_Vect3D *v, AD_Vect3D *vout)
{
/*
  vout->x=m->a[0][0]*v->x+m->a[0][1]*v->y+m->a[0][2]*v->z+m->a[0][3];
  vout->y=m->a[1][0]*v->x+m->a[1][1]*v->y+m->a[1][2]*v->z+m->a[1][3];
  vout->z=m->a[2][0]*v->x+m->a[2][1]*v->y+m->a[2][2]*v->z+m->a[2][3];
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



// MATRICI MULTIDIMENSIONALI

void nmat_make(nMatrix *m, int r, int c)
{
  int t;

  if ((r<0) || (c<0)) return;

  m->rows=r;
  m->columns=c;
  m->M=new float[r*c];
  for (t=0; t<r*c; t++) m->M[t]=0;
}


void nmat_free(nMatrix *m)
{
  if (m->M!=(float *)NULL) delete [] m->M;
  m->rows=m->columns=0;
}


void nmat_copy (nMatrix *m1, nMatrix *m2)
{
  int t;

  for (t=0; t<m1->columns*m1->rows; t++)
  {
	  m2->M[t]=m1->M[t];
  }
}


void nmat_identity(nMatrix *m, int dim)
{
  int r, c;

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


void nmat_add(nMatrix *m1, nMatrix *m2, nMatrix *m3)
{
  int t;

  for (t=0; t<m1->columns*m1->rows; t++)
  {
	m3->M[t]=m2->M[t] + m1->M[t];
  }
}


void nmat_sub(nMatrix *m1, nMatrix *m2, nMatrix *m3)
{
  int t;

  for (t=0; t<m1->columns*m1->rows; t++)  
  {
	m3->M[t]=m1->M[t] - m2->M[t];
  }
}


void nmat_swap_rows(nMatrix *m, int r1, int r2)
{
  int g;
  float f;

  for (g=0; g<m->columns; g++)
  {
	f=NMAT_ELEMENT(m, r1, g);
    NMAT_ELEMENT(m, r1, g) = NMAT_ELEMENT(m, r2, g);
	NMAT_ELEMENT(m, r2, g)=f;
  }
}

	
int nmat_inverse (nMatrix *m1, nMatrix *m2)
{
  int r, c, k;
  int maxrow;
  float maxval, val;

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
    for(c=r+1; c<m1->rows; c++)
	{
      val = NMAT_ELEMENT(m1, c, r); 
      if(fabs(val) > fabs(maxval))
	  {
        maxval = val;
        maxrow = r;
	  }
	}
    
    // scambio la riga avente elemento massimo con la corrente
	// questo permettera' una stabilita' numerica molto maggiore
    if(maxrow != r)
	{
      nmat_swap_rows(m1, r, maxrow);
      nmat_swap_rows(m2, r, maxrow);
	}

    // Divido la riga corrente per il massimo valore per avere
    // un 1 sulla diagonale
    for(k=0; k<m1->rows; k++)
	{
      NMAT_ELEMENT(m1, r, k) /= maxval;
      NMAT_ELEMENT(m2, r, k) /= maxval;
	}

    // Faccio un'operazione di "pivotaggio" con pivot in
	// posizione (r,r), cosi' metto a zero tutti gli elementi
	// in posizione (c, r)  r+1 <= c <= dimensione-1
    for(c=r+1; c<m1->rows; c++)
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
  for(r=m1->rows-1; r>=0; r--)
  {
    for(c=r-1; c>=0; c--)
	{
      val = NMAT_ELEMENT(m1, c, r);
      for(k=0; k<m1->rows; k++)
	  {
        NMAT_ELEMENT(m1, c, k) -= NMAT_ELEMENT(m1, r, k)*val; 
        NMAT_ELEMENT(m2, c, k) -= NMAT_ELEMENT(m2, r, k)*val; 
      } 
    } 
  }

  return(NOT_SINGULAR);
}


void nmat_mulnvect (nMatrix *m, nVector *v, nVector *out)
{
  int t, w;
  float faux;

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


void nmat_diade (nVector *v, nMatrix *m)
// Costruisce la diade data da vv'
{
  int i, j;

  for (i=0; i<v->dimension; i++)
  {
	for (j=0; j<v->dimension; j++)
	{
	  NMAT_ELEMENT(m, i, j)=v->v[i]*v->v[j];
	}
  }
}