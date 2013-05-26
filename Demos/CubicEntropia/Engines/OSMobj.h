#ifndef _OSMOBJ_H_
#define _OSMOBJ_H_

#include "keyframe.h"

class AD_OSMObject
{
public:
    AD_Vect3D center;
    postrack *center_track;
    // matrice di trasformazione dalla spazio oggetto allo
	// spazio dell'OSM
	AD_Matrix tm, invtm;
	// bounding box
	float bbx1, bby1, bbz1, bbx2, bby2, bbz2;


    AD_OSMObject()
	{
	   center.x=center.y=center.z=0;
	   center_track=(postrack *)NULL;
	}
	virtual void init (void) {}
	void set_bbox (float x1, float y1, float z1, float x2, float y2, float z2)
	{
	  bbx1=x1;
	  bby1=y1;
	  bbz1=z1;
	  bbx2=x2;
	  bby2=y2;
	  bbz2=z2;
	}
    virtual void update (float4 framepos) {}
	virtual void Map (AD_Vect3D *pos, AD_Vect3D *out) {}
};


// ***********************************************************
// ******************       TWIST      ***********************
// ***********************************************************
class AD_TwistModifier: public AD_OSMObject
{
public:
  float angle, bias, uplim, lowlim, height, angleOverHeight;
  int axis, dolim;

  rolltrack *angle_track;
  rolltrack *bias_track;
  rolltrack *uplim_track;
  rolltrack *lowlim_track;

  AD_TwistModifier()
  {
	angle=bias=uplim=lowlim=0;
	axis=dolim=0;
    angle_track=bias_track=uplim_track=lowlim_track=(rolltrack *)NULL;
  }

  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// ******************       RIPPLE     ***********************
// ***********************************************************
class AD_RippleModifier: public AD_OSMObject
{
public:
  float amp1, amp2, wlen, phase, decay;

  rolltrack *amp1_track;
  rolltrack *amp2_track;
  rolltrack *wlen_track;
  rolltrack *phase_track;
  rolltrack *decay_track;

  AD_RippleModifier()
  {
	amp1=amp2=wlen=phase=decay=0;
    amp1_track=amp2_track=wlen_track=phase_track=decay_track=(rolltrack *)NULL;
  }

  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// *******************       BEND      ***********************
// ***********************************************************
class AD_BendModifier: public AD_OSMObject
{
public:
  float angle, dir, uplim, lowlim, r;
  int axis, dolim;

  AD_Matrix tmBelow, tmAbove;
  rolltrack *angle_track;
  rolltrack *dir_track;
  rolltrack *uplim_track;
  rolltrack *lowlim_track;

  AD_BendModifier()
  {
	angle=dir=uplim=lowlim=r=0;
	axis=dolim=0;
    angle_track=dir_track=uplim_track=lowlim_track=(rolltrack *)NULL;
  }

  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// *******************      TAPER      ***********************
// ***********************************************************
class AD_TaperModifier: public AD_OSMObject
{
public:
  float amount, curve, uplim, lowlim, l;
  int axis, effectaxis, symmetry, dolim, doX, doY;

  rolltrack *amount_track;
  rolltrack *curve_track;
  rolltrack *uplim_track;
  rolltrack *lowlim_track;

  AD_TaperModifier()
  {
	amount=curve=uplim=lowlim=0;
	axis=effectaxis=dolim=symmetry=doX=doY=0;
    amount_track=curve_track=uplim_track=lowlim_track=(rolltrack *)NULL;
  }
  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// *******************      NOISE      ***********************
// ***********************************************************
class AD_NoiseModifier: public AD_OSMObject
{
public:
  int seed, fractal, animate, phase;
  float scale, rough, iterations, freq, time;
  AD_Vect3D strength;
  rolltrack noise[3];

  rolltrack *scale_track;
  postrack *strength_track;
  rolltrack *freq_track;
  rolltrack *phase_track;

  AD_NoiseModifier()
  {
     seed=fractal=animate=phase=0;
     scale=100;
	 rough=time=0;
	 iterations=6.0f;
	 freq=0.25f;
     vect_set(&strength, 0, 0, 0);
	 scale_track=freq_track=phase_track=(rolltrack *)NULL;
     strength_track=(postrack *)NULL;
  }
  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  float noise3(float f1, float f2, float f3);
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// ********************     STRETCH    ***********************
// ***********************************************************
class AD_StretchModifier: public AD_OSMObject
{
public:
  float stretch, amplify, uplim, lowlim;
  float heightMin, heightMax;
  int axis, dolim;

  rolltrack *stretch_track;
  rolltrack *amplify_track;
  rolltrack *uplim_track;
  rolltrack *lowlim_track;

  AD_StretchModifier()
  {
	stretch=amplify=uplim=lowlim=0;
    heightMin=heightMax=0;
	axis=dolim=0;
    stretch_track=amplify_track=uplim_track=lowlim_track=(rolltrack *)NULL;
  }

  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// *******************       MELT      ***********************
// ***********************************************************
class AD_MeltModifier: public AD_OSMObject
{
public:
  float cx, cy, cz;  // centro della bounding box
  float xsize, ysize, zsize, size;

  rolltrack *amount_track;
  rolltrack *spread_track;  
  rolltrack *solidity_track;

  int axis, negaxis;
  float amount, spread, solidity;

  AD_MeltModifier()
  {
	cx=cy=cz=xsize=ysize=zsize=0;
	amount=spread=solidity=0;
	axis=negaxis=0;
    amount_track=spread_track=solidity_track=(rolltrack *)NULL;
  }
  void init(void);
  void update (float4 framepos);

  // Metodo usato dai Object Space Modifiers
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
};


// ***********************************************************
// ********************       FFD      ***********************
// ***********************************************************
class AD_FFDModifier: public AD_OSMObject
{
public:
  float offset;
  int invol, dim1, dim2, dim3, num_cpoints;
  AD_Vect3D *cpoints, *cpoints_tr;
  AD_Vect3D currentpos;
  postrack *cpoints_tracks;  // tracce di posizione dei control points

  postrack positiontrack; // traccia delle posizioni del lattice
  rottrack rotationtrack; // traccia delle rotazioni del lattice
  postrack scaletrack;    // traccia di scaling del lattice
  // è la sola matrice dovuta ai movimenti (anche rotazioni
  // e scaling) del lattice nella sua globalità
  AD_Matrix latticematrix, inv_latticematrix;

  AD_FFDModifier()
  {
	 cpoints_tracks=(postrack *)NULL;
	 cpoints=cpoints_tr=(AD_Vect3D *)NULL;
	 invol=1;
  }
  void init(void);
  void update (float4 framepos);
  void Map (AD_Vect3D *pos, AD_Vect3D *out);
  void Map2 (AD_Vect3D *pos, AD_Vect3D *out);
  void Map3 (AD_Vect3D *pos, AD_Vect3D *out);
};

#endif
