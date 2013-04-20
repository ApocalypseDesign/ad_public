// Classe di Bezièr by [AD]Turbo roxxxxxx

#include <stdlib.h>
#include <math.h>
#include "bezier.h"
#include "ADrender.h"

float fattoriale (int k)
{
  int ck;
  float result=(float)k;
  float ftrick=result-1.0f;


  if (k<=1) return(1.0);
  for (ck=k-1; ck>=2; ck--)
  {
	 result=result*ftrick;
	 ftrick-=1.0;
  }

  return(result);
}


float binomial_coefficients (int n, int i)
{
   return(fattoriale(n)/(fattoriale(i)*fattoriale(n-i)));
}


float Bernstein_evaluate (int i, int n, float t)
{
// valutazione dell'i-esimo polinomio di Bernstein di grado n
// nel punto t
//
//       n      (n)  i        n-i
//      B (t) = ( ) t  * (1-t)
//       i      (i)

  return((float)(binomial_coefficients(n, i)*pow(t, (float)i)*pow(1.0f-t, (float)(n-i))));
}



// ###########################################################
// ###################  CURVE DI BEZIER  #####################
// ###########################################################

// Costruttore
Bezier_Curve3D::Bezier_Curve3D(int ncp)
{
  int i;

  num_control_points=ncp;
  control_points=new AD_Vect3D[ncp];
  for (i=0; i<ncp; i++)
  {
    control_points[i].x=(float)(rand()%320);
    control_points[i].y=(float)(rand()%240);
    control_points[i].z=(float)(rand()%500);
  }
}


void Bezier_Curve3D::Set_MAXPaint_Detail(int ts)
{
  // il parametro indica il numero di punti sulla curva da
  // calcolare (compresi i 2 estremi)
  // tale parametro deve essere almeno 2 (o superiore)

  // setto le variabili private che definiscono la precisione
  // di rendering
  
  if (ts<2) ts=2;
  max_paint_step=paint_step=ts;
  t_step=1.0f/(float)(paint_step-1);
}


void Bezier_Curve3D::Change_Paint_Detail (int precision)
{
  if (precision<0) precision=0;
  if (precision>100) precision=100;
  max_paint_step=(paint_step*precision)/100;
  t_step=1.0f/(float)(paint_step-1);
}


void Bezier_Curve3D::Curve_Evaluate(float t, AD_Vect3D *vout)
// Valuta la curva di Bezier definita dagli np punti di controllo
// per un valore del parametro pari a t in [0..1]
// Usa lo schema di Horner-Bezier per velocizzare i calcoli
{
  float n_su_i=1.0, t1=1.0f-t, fact=1.0, i=1;
  AD_Vect3D vaux;
  int int_i=1;
  float	float_n = (float)(num_control_points-1);

  vaux.x=control_points[0].x * t1;
  vaux.y=control_points[0].y * t1;
  vaux.z=control_points[0].z * t1;
  do
  {
	fact=fact*t;
	n_su_i=n_su_i*(float_n - i + 1.0f)/i;
    vaux.x=(vaux.x + control_points[int_i].x*fact*n_su_i)*t1;
    vaux.y=(vaux.y + control_points[int_i].y*fact*n_su_i)*t1;
    vaux.z=(vaux.z + control_points[int_i].z*fact*n_su_i)*t1;
	i+=1;
	int_i+=1;
  } while (int_i!=(num_control_points-1));

  vaux.x=vaux.x + control_points[num_control_points-1].x * fact*t;
  vaux.y=vaux.y + control_points[num_control_points-1].y * fact*t;
  vaux.z=vaux.z + control_points[num_control_points-1].z * fact*t;
  vect_copy(&vaux, vout);
}


void Bezier_Curve3D::Paint(imagebuffer *ib)
{
  int i, xs, ys;
  float t;
  AD_Vect3D p;

  t=0;
  for (i=0; i<paint_step; i++)
  {
     Curve_Evaluate(t, &p);
	 // eventualmente si possono x, y e z (proiettando) per
	 // curve 3D, e cmq la routine di paint puo' essere
	 // riscritta dentro agli effetti che usano le curve;
	 // io per adesso stampo solo dei punti bianchi
	 xs=(int)(p.x);
     ys=(int)(p.y);
     if ((xs>=0) && (xs<ib->width) && (ys>=0) && (ys<ib->height))
        ib->uint32ptr[ib->muly[ys]+xs]=0xFFFFFFFF;

	 t+=t_step;
  }

  
  // disegno i control point in un altro colore
  for (i=0; i<num_control_points; i++)
  {
	xs=(int)(control_points[i].x);
    ys=(int)(control_points[i].y);
    if ((xs>=0) && (xs<ib->width) && (ys>=0) && (ys<ib->height))
       ib->uint32ptr[ib->muly[ys]+xs]=0xFFFFFFFF;
  }
}





// ###########################################################
// #######  SUPERFICI DI BEZIER (prodotto tensoriale)  #######
// ###########################################################

// Costruttore
Bezier_Surface3D::Bezier_Surface3D(int u_points, int v_points)
{
  int i;

  grid_dimX=u_points;
  grid_dimY=v_points;
  control_points=new AD_Vect3D[u_points*v_points];
  control_points_tr=new AD_Vect3D[u_points*v_points];

  for (i=0; i<u_points*v_points; i++)
  {
	control_points[i].x=(float)(rand()%100)-50.0f;
    control_points[i].y=(float)(rand()%100)-50.0f;
    control_points[i].z=(float)(rand()%100)-50.0f;
	vect_copy(&control_points[i], &control_points_tr[i]);
  }
	
  aux_curve=new Bezier_Curve3D(u_points);
  binomials=new float[v_points];

  materiale.texture_ptr=new texture;
  materiale.texture_ptr->load_pcx("RUSTY.PCX", RGBINTERLEAVED);
  materiale.texture_pos=1;
//  materiale.init(0);

  u0 = 0.0f * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
  v0 = 0.0f * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
  u1 = 1.0f * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
  v1 = 0.0f * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
  u2 = 0.0f * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
  v2 = 1.0f * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
  u3 = 1.0f * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
  v3 = 1.0f * materiale.texture_ptr->dimy*TEXTURE_PRECISION;

}


void Bezier_Surface3D::Set_MAXPaint_Detail(int us, int vs)
{
  // i parametri indicano il numero di punti sulla curva da
  // calcolare (compresi i 2 estremi) nei due sensi
  // tali parametri devono essere almeno 2 (o superiore)

  int i;

  if (us<2) us=2;
  if (vs<2) vs=2;

  // setto le variabili private che definiscono la precisione
  // di rendering
  max_paint_ustep=paint_ustep=us;
  max_paint_vstep=paint_vstep=vs;
  u_step=1.0f/(float)(paint_ustep-1);
  v_step=1.0f/(float)(paint_vstep-1);
  
  // alloco le strutture necessarie al rendering
  num_vertex=max_paint_ustep*max_paint_vstep;
  vertex3D=new AD_Vertex3D[num_vertex];
  points=new AD_Vect3D[num_vertex];
  points_tr=new AD_Vect3D[num_vertex];
  normals=new AD_Vect3D[num_vertex];
  vertex2D=new AD_Vertex2D[num_vertex];
  num_tria=(paint_ustep-1)*(paint_vstep-1)*2;
  tria=new AD_Tria3D[num_tria];

  // collego i puntatori dentro la struttura AD_vertex3D
  // ai corrispettivi array
  for (i=0; i<num_vertex; i++)
  {
	vertex3D[i].normal=&normals[i];
    vertex3D[i].point=&points[i];
    vertex3D[i].tpoint=&points_tr[i];
	vertex3D[i].p2D=&vertex2D[i];
  }

}


void Bezier_Surface3D::Change_Paint_Detail (int precision)
{
  if (precision<0) precision=0;
  if (precision>100) precision=100;
  paint_ustep=(max_paint_ustep*precision)/100;
  paint_vstep=(max_paint_vstep*precision)/100;
  u_step=1.0f/(float)(paint_ustep-1);
  v_step=1.0f/(float)(paint_vstep-1);
}


void Bezier_Surface3D::Surface_Evaluate (float u, float v, AD_Vect3D *vout)
{
  int i, j;
  AD_Vect3D *cp, *cp0;

  cp=control_points;
  for (i=0; i<grid_dimY; i++)
    binomials[i]=Bernstein_evaluate(i, grid_dimY-1, v);

  // valutazione dei punti di controllo "in verticale"
  // per v fissato
  cp0=control_points;
  for (j=0; j<grid_dimX; j++)   // devo generare grid_dimX punti
  {                             // di controllo
    cp=cp0;
	vect_set(&aux_curve->control_points[j], 0, 0, 0);

	for (i=0; i<grid_dimY; i++)
	{
	   aux_curve->control_points[j].x+=(*cp).x * binomials[i];
       aux_curve->control_points[j].y+=(*cp).y * binomials[i];
       aux_curve->control_points[j].z+=(*cp).z * binomials[i];
       cp+=grid_dimX;
	}
	cp0+=1;
  }

  aux_curve->Curve_Evaluate(u, vout);
}


void Bezier_Surface3D::Poligonize(void)
// Algoritmo utilizzato:
//
// [inizializzazione] : si parte con la riga dei punti di
// controllo che corrispondono a v=0 e si generano tutti i
// punti muovendosi in u
//
// [ciclo] : 
//
//  - ci si sposta in v di v_step, si genera un nuovo
//    set di punti di controllo per quella riga
//
//  - si calcolano i punti spostandosi in u
//
//  - genero i triangoli
//
// Vengono inoltremappate le coordinate texture linearmente su
// tutta la superficie

{
  float u, v;
  float tu0, tu1, tv0, tv1;  // coordinate texture su linee
                             // isoparametriche in v
  AD_Vect3D *cp0, *cp;
  AD_Vertex3D *vertex_row_up, *vertex_row_down;
  AD_Vect3D *points_already_calculated;
  AD_Vect3D *points_to_calculate;
  int i, j, w;

  num_tria=0;
  points_already_calculated=&points[0];
  points_to_calculate=&points[paint_ustep];
  vertex_row_up=&vertex3D[0];
  vertex_row_down=&vertex3D[paint_ustep];

  // copio la prima riga (v=0) di punti di controllo dentro
  // la curva
  for (j=0; j<grid_dimX; j++) 
	 vect_copy(&control_points_tr[j], &aux_curve->control_points[j]);

  // (pre)calcolo la prima fila di punti sulla superfice (v=0)
  u=0;
  tu0=u0;
  tv0=v0;
  tu1=u1;
  tv1=v1;
  for (j=0; j<paint_ustep; j++)
  {
	aux_curve->Curve_Evaluate(u, &points_already_calculated[j]);
	vertex_row_up[j].p2D->u=u*(tu1-tu0)+tu0;
	vertex_row_up[j].p2D->v=u*(tv1-tv0)+tv0;
	u+=u_step;
  }


  v=0;
  // -----------------------------------------------
  // inizio del ciclo bestiale mangia FPS abbestia !
  // -----------------------------------------------

  for (w=0; w<paint_vstep-1; w++)
  {
    v+=v_step;
    tu0=u0+v*(u2-u0);
    tv0=v0+v*(v2-v0);
    tu1=u1+v*(u3-u1);
    tv1=v1+v*(v3-v1);

    // precalcolo della fila "verticale" dei coefficenti binomiali
    //  per costruire il poligono di controllo della curva in
    //  v
    for (i=0; i<grid_dimY; i++)
	  binomials[i]=Bernstein_evaluate(i, grid_dimY-1, v);

  
    // costruzione del poligono di controllo della curva in v
    cp0=control_points_tr;
    for (j=0; j<grid_dimX; j++)  // devo generare grid_dimX punti
	{                            // di controllo
	  cp=cp0;
	  vect_set(&aux_curve->control_points[j], 0, 0, 0);
	  for (i=0; i<grid_dimY; i++)
	  {
	    aux_curve->control_points[j].x+=(*cp).x * binomials[i];
        aux_curve->control_points[j].y+=(*cp).y * binomials[i];
        aux_curve->control_points[j].z+=(*cp).z * binomials[i];
	    cp+=grid_dimX;
	  }
      cp0+=1;
	}

    // calcolo della fila di punti sulla superfice in v
    u=0;
    for (j=0; j<paint_ustep; j++)
	{
	  aux_curve->Curve_Evaluate(u, &points_to_calculate[j]);
	  vertex_row_down[j].p2D->u=u*(tu1-tu0)+tu0;
	  vertex_row_down[j].p2D->v=u*(tv1-tv0)+tv0;
	  u+=u_step;
	}


    // costruzione dei triangoli; essendo una superficie i
	// triangoli devono essere double-faced cosi' mi risparmio
	// di calcolare la normale (e nel metodo paint mi risparmiero'
	// di fare il bfc)
	// TO DO: calcolare la normale ai vertici (in modo veloce!)
    for (i=0; i<paint_ustep-1; i++)
	{
	  tria[num_tria].v1=vertex_row_up;
      tria[num_tria].v2=&(*(vertex_row_up+1));
	  tria[num_tria].v3=&(*(vertex_row_down+1));
	  num_tria++;

	  tria[num_tria].v1=&(*(vertex_row_down+1));
      tria[num_tria].v2=vertex_row_down;
	  tria[num_tria].v3=vertex_row_up;
	  num_tria++;

	  vertex_row_up+=1;
	  vertex_row_down+=1;
	}
	vertex_row_up+=1;
	vertex_row_down+=1;

    // what a nice optimization, by [AD]Turbo of course 
	// (others SUX)
    points_already_calculated+=paint_ustep;
    points_to_calculate+=paint_ustep;
  }
}

void Bezier_Surface3D::Transform_Control_Points (float pos)
{
  int i;
  float ax, ay, az;
  AD_Matrix m;


//  u0 = cos(pos) * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
//  u0 =0;
//  v0 = (pos/10) * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
//  u1 = cos(pos/10) * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
//  v1 = pos * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
//  u2 = cos(pos) * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
//  v2 = pos * materiale.texture_ptr->dimy*TEXTURE_PRECISION;
//  u3 = pos * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
//  v3 = (1-pos) * materiale.texture_ptr->dimy*TEXTURE_PRECISION;

//  u0=(0 + 2*pos) * materiale.texture_ptr->dimx*TEXTURE_PRECISION;
//  v0=(0 + pos)   * materiale.texture_ptr->dimy*TEXTURE_PRECISION;


  u0=v0=0;

  u1=(float)(((1-pos) + (fabs(cos(30*pos)))*pos)  * materiale.texture_ptr->dimx*TEXTURE_PRECISION);
  v1=0;

  u2=0;
  v2=(float)(((1-pos) + (fabs(cos(20*pos)))*pos) * materiale.texture_ptr->dimy*TEXTURE_PRECISION);

  u3=u1;
  v3=v2;



  pos*=3;
  // potenza delle curve di Bezier: invece di trasformare i
  // punti sulla curva trasformo solo i punti della griglia
  // di controllo !!!

  for (i=0; i<grid_dimX*grid_dimY; i++)
  {
    ax=(float)((pos*(720-i))*(3.1415f/180.0f)+3.1415);
    ay=(float)((pos*(390-2*i))*(3.1415f/180.0f));
//	az=(pos*(180-i))*(3.1415f/180.0f);
//	ay=cosf(pos*(3-i/5));
    az=cosf(pos*(3-i/6));


    mat_setmatrix_of_eulerrotation(&m, ax, ay, az);

    mat_mulvect(&m, &control_points[i], &control_points_tr[i]);
//	control_points_tr[i].x*=(pos/10+1);
	control_points_tr[i].y*=(pos/12+1);
  }
}


void Bezier_Surface3D::Paint (imagebuffer *ib, floatbuffer *zbuffer)
{
  float invz;
  int i;

  zbuffer->clear(0);

  for (i=0; i<paint_ustep*paint_vstep; i++)
  {
	invz=1.0f/(points[i].z+130);
	vertex2D[i].dist=invz;
    vertex2D[i].xs=(300.0f*invz*points[i].x) + (ib->width/2);
    vertex2D[i].ys=(280.0f*invz*points[i].y) + (ib->height/2);
  }

//  current_scanline_proc=(*scanline_flat);
//  render_setcurrent_material(&materiale);
  for (i=0; i<num_tria; i++)
  {
//	color_flat=(i*255)/num_tria;
//    materiale.myraster(tria[i].v1->p2D, tria[i].v2->p2D, tria[i].v3->p2D);
  }
}



// ###########################################################
// ###################  SUPERFICI RIGATE  ####################
// ###########################################################

Striped_Surface3D::Striped_Surface3D(int u_points, int v_points)
{
  int i;

  grid_dimX=u_points;
  grid_dimY=v_points;
  num_upatch=grid_dimX-1;
  num_vpatch=grid_dimY-1;

  control_points=new AD_Vect3D[u_points*v_points];
  control_points_tr=new AD_Vect3D[u_points*v_points];
  for (i=0; i<u_points*v_points; i++)
  {
	control_points[i].x=(float)(rand()%100)-50.0f;
    control_points[i].y=(float)(rand()%100)-50.0f;
    control_points[i].z=(float)(rand()%100)-50.0f;
	vect_copy(&control_points[i], &control_points_tr[i]);
  }
}


void Striped_Surface3D::Set_MAXPaint_Detail(int us, int vs)
{
  // i parametri indicano il numero di punti sulla curva da
  // calcolare (compresi i 2 estremi) nei due sensi
  // PER OGNI PEZZA (nelle bezier i due parametri si riferiscono
  // alla curva intiera !)
  // tali parametri devono essere almeno 2 (o superiore)

  int i;

  if (us<2) us=2;
  if (vs<2) vs=2;

  // setto le variabili private che definiscono la precisione
  // di rendering
  max_paint_ustep=paint_ustep=us;
  max_paint_vstep=paint_vstep=vs;
  u_step=1.0f/(float)(paint_ustep-1);
  v_step=1.0f/(float)(paint_vstep-1);
  
  // alloco le strutture necessarie al rendering
  num_vertex=num_upatch*paint_ustep-(num_upatch-1)*
             num_vpatch*paint_vstep-(num_vpatch-1);

  vertex3D=new AD_Vertex3D[num_vertex];
  points=new AD_Vect3D[num_vertex];
  points_tr=new AD_Vect3D[num_vertex];
  normals=new AD_Vect3D[num_vertex];
  vertex2D=new AD_Vertex2D[num_vertex];
  num_tria=(num_upatch*num_vpatch)*((paint_ustep-1)*(paint_vstep-1)*2);
  tria=new AD_Tria3D[num_tria];

  // collego i puntatori dentro la struttura AD_vertex3D
  // ai corrispettivi array
  for (i=0; i<num_vertex; i++)
  {
	vertex3D[i].normal=&normals[i];
    vertex3D[i].point=&points[i];
    vertex3D[i].tpoint=&points_tr[i];
	vertex3D[i].p2D=&vertex2D[i];
  }
}


void Striped_Surface3D::Change_Paint_Detail (int precision)
{
  if (precision<0) precision=0;
  if (precision>100) precision=100;
  paint_ustep=(max_paint_ustep*precision)/100;
  paint_vstep=(max_paint_vstep*precision)/100;
  u_step=1.0f/(float)(paint_ustep-1);
  v_step=1.0f/(float)(paint_vstep-1);
}


void Striped_Surface3D::Surface_Evaluate (float u, float v, AD_Vect3D *vout)
{
  int i, j;
  float local_u, local_v;
  AD_Vect3D *a, *b, c;

  // trovo in che pezza sono
  if (u>=1.0f) i=grid_dimX-2;
  else i=(int)(u/u_step);
  if (v>=1.0f) j=grid_dimY-2;
  else j=(int)(v/v_step);

  a=vertex3D[i].point;
  b=vertex3D[j*grid_dimX+i].point;
  local_u=(u - u_step*i)/u_step;
  local_v=(v - v_step*j)/v_step;

  // vout=P00(1-u)(1-v) + P10(1-v)u + P01(1-u)v + P11uv
  vect_scale(a, (1.0f-local_u)*(1.0f-local_v), vout);
  vect_scale(&(*(a+1)), (1.0f-local_v)*local_u, &c);
  vect_add(vout, &c, vout);
  vect_scale(b, (1.0f-local_u)*local_v, &c);
  vect_add(vout, &c, vout);
  vect_scale(&(*(b+1)), local_u*local_v, &c);
  vect_add(vout, &c, vout);
}


void Striped_Surface3D::Transform_Control_Points (float pos)
{
  int i;
  float ax, ay, az;
  AD_Matrix m;

  // potenza delle curve bilineari: invece di trasformare i
  // punti sulla curva trasformo solo i punti della griglia
  // di controllo !!!

  pos*=2;
  for (i=0; i<grid_dimX*grid_dimY; i++)
  {
    ax=(float)((pos*(720-i))*(3.1415f/180.0f)+3.1415);
    ay=(float)((pos*(390-2*i))*(3.1415f/180.0f));
	az=(float)((pos*(180-i))*(3.1415f/180.0f));
    mat_setmatrix_of_eulerrotation(&m, ax, ay, az);

    mat_mulvect(&m, &control_points[i], &control_points_tr[i]);
  }
}


void Striped_Surface3D::Poligonize(void)
{
  int i, j, w, t;
  float u, v;

  // ci sono grid_dimY-1 pezze in verticale
  for (i=0; i<num_vpatch; i++)
  {
    // ci sono grid_dimX-1 pezze in orizzontale
	v=0;
	for (j=0; j<paint_vstep; j++)
	{
	  for (w=0; w<num_upatch; w++)
	  {
		u=0;
        for (t=0; t<paint_ustep; t++)
		{
		  u+=u_step;
		}
	    
		v+=v_step;
	  }
	}
  }
}