// Classe di Bezièr by [AD]Turbo roxxxxxx

#ifndef _BEZIER_H_
#define _BEZIER_H_

#include "ADrender.h"
#include "triangle.h"
#include "vect3D.h"

float fattoriale (int k);
float binomial_coefficients (int n, int i);
float Bernstein_evaluate (int i, int n, float t);


void Bezier_prova_init(void);
void Bezier_prova(imagebuffer *ib, float tpos);

class Bezier_Curve3D
{
private: 
	 int paint_step, max_paint_step;
	 float t_step;

public:
     AD_Vect3D *control_points;
	 AD_Vect3D *control_points_tr;
     int num_control_points;

     // Costruttore #1
	 Bezier_Curve3D(int ncp);

	 // metodi della classe
	 void Set_MAXPaint_Detail(int ts);
	 void Change_Paint_Detail (int precision);
	 void Curve_Evaluate (float t, AD_Vect3D *vout);
	 void Paint (imagebuffer *ib);
};

     

class Bezier_Surface3D
{
private:
	Bezier_Curve3D *aux_curve;
	
    // le seguenti 8 variabili sono le stesse (stesso significato)
	// delle omonime che sono presenti nella classe AD_Object3D

	AD_Vertex3D *vertex3D;    // usati quando si poligonizza
	AD_Tria3D *tria;          // usati quando si poligonizza
	AD_Vertex2D *vertex2D;    // array di vertici 2D
    AD_Vect3D *points, *points_tr, *normals;
	int num_vertex, num_tria;
	
	
	// array ausialiario di coefficenti binomiali
	float *binomials;

	int paint_ustep, paint_vstep;
	float u_step, v_step;
	int max_paint_ustep, max_paint_vstep;

public:
  int grid_dimX, grid_dimY;  // indica il numero di punto
	                           // in x ed y che formano
	                           // la griglia
  
  // punti di controllo della griglia; sono messi in memoria
  // per righe, dove in ogni riga si "viaggia" nelle u;
  // passando da una riga a quella dopo si "viaggia" nelle v
  AD_Vect3D *control_points;
  AD_Vect3D *control_points_tr;
  material materiale;
  float u0, v0, u1, v1, u2, v2, u3, v3;  // coordinate texture
                                         // ai 4 angoli della griglia

  // Costruttore
  Bezier_Surface3D(int u_points, int v_points);
  
  void Set_MAXPaint_Detail(int us, int vs);
  void Change_Paint_Detail (int precision);
  void Surface_Evaluate (float u, float v, AD_Vect3D *vout);
  void Transform_Control_Points (float pos);
  void Poligonize (void);
  void Paint (imagebuffer *ib, floatbuffer *zbuffer);
};


class Striped_Surface3D
{
private:

    // le seguenti 8 variabili sono le stesse (stesso significato)
	// delle omonime che sono presenti nella classe AD_Object3D
	AD_Vertex3D *vertex3D;    // usati quando si poligonizza
	AD_Tria3D *tria;          // usati quando si poligonizza
	AD_Vertex2D *vertex2D;    // array di vertici 2D
    AD_Vect3D *points, *points_tr, *normals;
	int num_vertex, num_tria;

	int paint_ustep, paint_vstep;
	float u_step, v_step;
	int max_paint_ustep, max_paint_vstep;

public:
  int grid_dimX, grid_dimY;  // indica il numero di punti
	                         // in x ed y che formano
	                         // la griglia
  
  int num_upatch, num_vpatch;  // numero delle pezze nei 2 sensi
  
  // punti di controllo della griglia; sono messi in memoria
  // per righe, dove in ogni riga si "viaggia" nelle u;
  // passando da una riga a quella dopo si "viaggia" nelle v
  AD_Vect3D *control_points;  
  AD_Vect3D *control_points_tr;
  
  // Costruttore
  Striped_Surface3D(int u_points, int v_points);
  
  void Set_MAXPaint_Detail(int us, int vs);
  void Change_Paint_Detail (int precision);
  void Surface_Evaluate (float u, float v, AD_Vect3D *vout);
  void Transform_Control_Points (float pos);
  void Poligonize (void);
  void Paint (imagebuffer *ib, floatbuffer *zbuffer);
};


#endif
