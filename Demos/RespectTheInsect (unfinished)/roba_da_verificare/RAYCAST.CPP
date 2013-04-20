#include <math.h>
#include "fastmath.h"
#include "raycast.h"


// costruttori
AD_Raycast::AD_Raycast ()
{
  // termini quadratici puri (x^2, y^2, z^2)
  A.a[0][0]=0;
  A.a[1][1]=0;
  A.a[2][2]=0;

  // termini rettangolari (xy, xz, yz)
  A.a[0][1]=A.a[1][0]=0;
  A.a[0][2]=A.a[2][0]=0;
  A.a[1][2]=A.a[2][1]=0;

  // termini lineari (x, y, z)
  b.x=0;
  b.y=0;
  b.z=0;

  c=0;
}


AD_Raycast::AD_Raycast (float a11, float a22, float a33,
	                    float a12, float a13, float a23,
                        float b1,  float b2,  float b3,
						float cc)
{
  // termini quadratici puri (x^2, y^2, z^2)
  A.a[0][0]=a11;
  A.a[1][1]=a22;
  A.a[2][2]=a33;

  // termini rettangolari (xy, xz, yz)
  A.a[0][1]=A.a[1][0]=a12;
  A.a[0][2]=A.a[2][0]=a13;
  A.a[1][2]=A.a[2][1]=a23;

  // termini lineari (x, y, z)
  b.x=b1;
  b.y=b2;
  b.z=b3;

  c=cc;
}


void AD_Raycast::assign_quadric (float a11, float a22, float a33,
	                             float a12, float a13, float a23,
					             float b1,  float b2,  float b3,
								 float cc)
{
  // termini quadratici puri (x^2, y^2, z^2)
  A.a[0][0]=a11;
  A.a[1][1]=a22;
  A.a[2][2]=a33;

  // termini rettangolari (xy, xz, yz)
  A.a[0][1]=A.a[1][0]=a12;
  A.a[0][2]=A.a[2][0]=a13;
  A.a[1][2]=A.a[2][1]=a23;

  // termini lineari (x, y, z)
  b.x=b1;
  b.y=b2;
  b.z=b3;

  c=cc;
}


void AD_Raycast::assign_quadric (AD_Matrix *m, AD_Vect3D *v, float cc)
{
  mat_copy(m, &A);
  vect_copy(v, &b);
  c=cc;
}


void AD_Raycast::init()
{
  int nx, ny;

  mytex.load_pcx("METAL4.PCX", RGBINTERLEAVED);

  nx=(image->width  / X_INTERPOLATION_STEP) + 1;
  ny=(image->height / Y_INTERPOLATION_STEP) + 1;

  ray_shooted=new RayUV[nx*ny];
  nray_x=nx;
  nray_y=ny;
}


void AD_Raycast::trace_one_ray (AD_Ray *r, float k)
{
  float EPSILON_A = 1E-4f;
  float EPSILON_DELTA = 1E-4f;

  AD_Vect3D aux;
  float q_a, q_b, q_c, delta, t, t1, t2;

  // la quadratica in t e' data dall'intersezione delle
  //
  //  q(x) = x'Ax + b'x + c
  //  
  //  l(x) = x° + td
  //
  // dove A e' una matrice quadrata simmetrica, b vettore
  // dei termini lineari, c una costante; queste 3 entita'
  // definiscono la quadratica generale in R^n
  // La funzione lineare e' il raggio che si traccia, dato
  // da origine x° e direzione d
  // 
  // Con l'apice indico vettore trasposto
  //
  // q(t)=t²(d'Ad) + t(2x°'Ad + b'd) + (x°'Ax° + b'x° + c)
  //
  // questa e' una semplice equazione quadraica ad una sola
  // variabile (t) e si risolve con la solita formula che
  // si impara alle medie; l'equazione infatti si presenta
  // nella forma 
  //
  // q(t) = q_a * t²  +  q_b * t  +  q_c

  mat_mulvectaffine(&A, &r->direction, &aux);
  q_a=vect_dot(&aux, &r->direction);             // q_a ok!

  q_b=vect_dot(&b, &r->direction);
  q_b=q_b + 2.0f*vect_dot(&r->origin, &aux);     // q_b ok!

  q_c=c;
  q_c=q_c + vect_dot(&b, &r->origin);
  mat_mulvectaffine(&A, &r->origin, &aux);
  q_c=q_c + vect_dot(&aux, &r->origin);          // q_c ok!

  delta=q_b*q_b-4.0f*q_a*q_c;
  if (delta<=EPSILON_DELTA)
  {
	r->u=r->v=0;
	return;
  }

  if (fast_float_abs(q_a)<EPSILON_A) t=-q_c/q_b;
  else
  {
    t1=(-q_b+sqrtf(delta)/(2*q_a));
    t2=(-q_b-sqrtf(delta)/(2*q_a));

    // si prende l'intersezione piu' vicina
    if (t1<t2) t=t1;
    else t=t2;
/*
	if (t<0)
	{
	  r->u=r->v=0;
	  return;
	}
*/
  }

  // calcoliamo il punto di intersezione
  r->intersection.x=r->origin.x+t*r->direction.x;
  r->intersection.y=r->origin.y+t*r->direction.y;
  r->intersection.z=r->origin.z+t*r->direction.z;

  r->u=fabsf(r->intersection.z);
  r->v=k*fabsf(atan2f(r->intersection.y, r->intersection.x));
}


void AD_Raycast::do_efx2D(double pos)
{
  int x, y, mask_u, mask_v, tex_offset;
  int y_du_r, y_dv_r, y_du_l, y_dv_l;
  int x_du, x_dv, u_r, v_r, u_l, v_l, u, v;
  int i, j;
  int video_ofs, r, video_ofs0;
  AD_Ray ray;
  AD_Matrix m;
  AD_Vect3D vv;

  
  assign_quadric(fabsf(2*cos(6*pos+0.3)), fabsf(sin(8*pos+0.3)),0,
	             0,0,0, 0,pos,0, -100);

  ray.origin.x=0.0f;
  ray.origin.y=0.0f;
  ray.origin.z=(float)(256.0f+pos*256);
  mat_setmatrix_of_eulerrotation(&m, -2*pos, 3*pos, 0);
  mat_identity(&m);

  // CALCOLO DELLE INTERSEZIONI
  // --------------------------
  r=0;
  for (y=0; y<image->height; y+=Y_INTERPOLATION_STEP)
  {
	for (x=0; x<image->width; x+=X_INTERPOLATION_STEP)
	{
	  vv.x=(float)(x) - (float)(image->width / 2.0f);
	  vv.y=(float)(y) - (float)(image->height / 2.0f);
	  vv.z=ray.origin.z +256;
      mat_mulvectaffine(&m, &vv, &ray.direction);
      
	  vect_normalize(&ray.direction);

	  trace_one_ray(&ray, (float)(pos*128));

      ray_shooted[r].u=myfist16(ray.u);
      ray_shooted[r].v=myfist16(ray.v);
	  r++;
	}
  }


  // FASE DI RENDERING
  // -----------------
  mask_u=(mytex.dimx-1);
  mask_v=(mytex.dimy-1);
  video_ofs0=0;
  r=0;
  for (y=0; y<image->height; y+=Y_INTERPOLATION_STEP)
  {
	for (x=0; x<image->width; x+=X_INTERPOLATION_STEP)
	{
	  // disegno interpolato
	  y_du_l=(ray_shooted[r+nray_x-1].u   - ray_shooted[r].u)   >> Y_SHIFT;
	  y_dv_l=(ray_shooted[r+nray_x-1].v   - ray_shooted[r].v)   >> Y_SHIFT;
	  y_du_r=(ray_shooted[r+nray_x].u - ray_shooted[r+1].u) >> Y_SHIFT;
	  y_dv_r=(ray_shooted[r+nray_x].v - ray_shooted[r+1].v) >> Y_SHIFT;

      u_l=ray_shooted[r].u;
      v_l=ray_shooted[r].v;
      u_r=ray_shooted[r+1].u;
      v_r=ray_shooted[r+1].v;

	  video_ofs=video_ofs0;
	  for (i=0; i<Y_INTERPOLATION_STEP; i++)
	  {
		x_du=(u_r - u_l) >> X_SHIFT;
		x_dv=(v_r - v_l) >> X_SHIFT;
		u=u_l;
		v=v_l;
        for (j=0; j<X_INTERPOLATION_STEP; j++)
		{
          tex_offset=(((v >> 16) & mask_v) << mytex.xbits) + ((u >> 16) & mask_u);
	      image->uint32ptr[video_ofs]=mytex.map32[tex_offset];
	      video_ofs++;
		  u+=x_du;
		  v+=x_dv;
		}
        video_ofs+=(image->width - X_INTERPOLATION_STEP);
		u_l+=y_du_l;
		v_l+=y_dv_l;
		u_r+=y_du_r;
		v_r+=y_dv_r;
	  }

	  r++;
      video_ofs0+=X_INTERPOLATION_STEP;
	}
	video_ofs0+=((Y_INTERPOLATION_STEP-1)*image->width);
  }

}