#ifndef _OSMOBJ_H_
#define _OSMOBJ_H_

#include "Keyframe.h"

class CObjectSpaceModifier
{
public:
    AD_Vect3D p_Center;

    CPosTrack *p_GizmoPositionTrack;
    CRotTrack *p_GizmoRotationTrack;
    CPosTrack *p_GizmoScaleTrack;

	// matrice di trasformazione del gizmo/lattice
	AD_Matrix p_GizmoMatrix, p_InverseGizmoMatrix;

    // matrice di trasformazione dalla spazio oggetto allo
	// spazio dell'OSM
	AD_Matrix p_TM, p_InvTM;
	// bounding box
	float4 p_bbx1, p_bby1, p_bbz1, p_bbx2, p_bby2, p_bbz2;

    CObjectSpaceModifier();

	virtual void m_Init (void);
	void m_SetBBox (float4 x1, float4 y1, float4 z1, float4 x2, float4 y2, float4 z2)
	{
	  p_bbx1=x1;
	  p_bby1=y1;
	  p_bbz1=z1;
	  p_bbx2=x2;
	  p_bby2=y2;
	  p_bbz2=z2;
	}
    
	virtual void m_Update (float4 framepos, AD_Vect3D *pivot);
	virtual void m_Map (AD_Vect3D *pos, AD_Vect3D *out)
	{ return; }
};


// ***********************************************************
// ******************       TWIST      ***********************
// ***********************************************************
class CTwistModifier: public CObjectSpaceModifier
{
public:
  float4 p_Angle, p_Bias, p_UpLim, p_LowLim, p_Height, p_AngleOverHeight;
  int32 p_Axis, p_DoLim;

  CRollTrack *p_AngleTrack;
  CRollTrack *p_BiasTrack;
  CRollTrack *p_UpLimTrack;
  CRollTrack *p_LowLimTrack;

  CTwistModifier()
  {
	p_Angle=p_Bias=p_UpLim=p_LowLim=0;
	p_Axis=p_DoLim=0;
    p_AngleTrack=p_BiasTrack=p_UpLimTrack=p_LowLimTrack=(CRollTrack *)NULL;
  }

  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// ******************       RIPPLE     ***********************
// ***********************************************************
class CRippleModifier: public CObjectSpaceModifier
{
public:
  float4 p_Amp1, p_Amp2, p_WLen, p_Phase, p_Decay;

  CRollTrack *p_Amp1Track;
  CRollTrack *p_Amp2Track;
  CRollTrack *p_WLenTrack;
  CRollTrack *p_PhaseTrack;
  CRollTrack *p_DecayTrack;

  CRippleModifier()
  {
	p_Amp1=p_Amp2=p_WLen=p_Phase=p_Decay=0;
    p_Amp1Track=p_Amp2Track=p_WLenTrack=p_PhaseTrack=p_DecayTrack=(CRollTrack *)NULL;
  }

  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// *******************       BEND      ***********************
// ***********************************************************
class CBendModifier: public CObjectSpaceModifier
{
public:
  float4 p_Angle, p_Dir, p_UpLim, p_LowLim, r;
  int32 p_Axis, p_DoLim;

  AD_Matrix p_TMBelow, p_TMAbove;
  CRollTrack *p_AngleTrack;
  CRollTrack *p_DirTrack;
  CRollTrack *p_UpLimTrack;
  CRollTrack *p_LowLimTrack;

  CBendModifier()
  {
	p_Angle=p_Dir=p_UpLim=p_LowLim=r=0;
	p_Axis=p_DoLim=0;
    p_AngleTrack=p_DirTrack=p_UpLimTrack=p_LowLimTrack=(CRollTrack *)NULL;
  }

  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};



// ***********************************************************
// *******************      TAPER      ***********************
// ***********************************************************
class CTaperModifier: public CObjectSpaceModifier
{
public:
  float4 p_Amount, p_Curve, p_UpLim, p_LowLim, l;
  int32 p_Axis, p_EffectAxis, p_Symmetry, p_DoLim, p_DoX, p_DoY;

  CRollTrack *p_AmountTrack;
  CRollTrack *p_CurveTrack;
  CRollTrack *p_UpLimTrack;
  CRollTrack *p_LowLimTrack;

  CTaperModifier()
  {
	p_Amount=p_Curve=p_UpLim=p_LowLim=0;
	p_Axis=p_EffectAxis=p_DoLim=p_Symmetry=p_DoX=p_DoY=0;
    p_AmountTrack=p_CurveTrack=p_UpLimTrack=p_LowLimTrack=(CRollTrack *)NULL;
  }
  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// *******************      NOISE      ***********************
// ***********************************************************
class CNoiseModifier: public CObjectSpaceModifier
{
public:
  int32 p_Seed, p_Fractal, p_Animate, p_Phase;
  float4 p_Scale, p_Rough, p_Iterations, p_Freq, p_Time;
  AD_Vect3D p_Strength;
  CRollTrack p_Noise[3];

  CRollTrack *p_ScaleTrack;
  CPosTrack *p_StrengthTrack;
  CRollTrack *p_FreqTrack;
  CRollTrack *p_PhaseTrack;

  CNoiseModifier()
  {
     p_Seed=p_Fractal=p_Animate=p_Phase=0;
     p_Scale=100;
	 p_Rough=p_Time=0;
	 p_Iterations=6.0f;
	 p_Freq=0.25f;
     vect_set(&p_Strength, 0, 0, 0);
	 p_ScaleTrack=p_FreqTrack=p_PhaseTrack=(CRollTrack *)NULL;
     p_StrengthTrack=(CPosTrack *)NULL;
  }
  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// ********************     STRETCH    ***********************
// ***********************************************************
class CStretchModifier: public CObjectSpaceModifier
{
public:
  float4 p_Stretch, p_Amplify, p_UpLim, p_LowLim;
  float4 p_HeightMin, p_HeightMax;
  int32 p_Axis, p_DoLim;

  CRollTrack *p_StretchTrack;
  CRollTrack *p_AmplifyTrack;
  CRollTrack *p_UpLimTrack;
  CRollTrack *p_LowLimTrack;

  CStretchModifier()
  {
	p_Stretch=p_Amplify=p_UpLim=p_LowLim=0;
    p_HeightMin=p_HeightMax=0;
	p_Axis=p_DoLim=0;
    p_StretchTrack=p_AmplifyTrack=p_UpLimTrack=p_LowLimTrack=(CRollTrack *)NULL;
  }

  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};

// ***********************************************************
// *******************       MELT      ***********************
// ***********************************************************
class CMeltModifier: public CObjectSpaceModifier
{
public:
  float4 p_cx, p_cy, p_cz;  // centro della bounding box
  float4 p_XSize, p_YSize, p_ZSize, p_Size;

  CRollTrack *p_AmountTrack;
  CRollTrack *p_SpreadTrack;  
  CRollTrack *p_SolidityTrack;

  int32 p_Axis, p_NegAxis;
  float4 p_Amount, p_Spread, p_Solidity;

  CMeltModifier()
  {
	p_cx=p_cy=p_cz=p_XSize=p_YSize=p_ZSize=0;
	p_Amount=p_Spread=p_Solidity=0;
	p_Axis=p_NegAxis=0;
    p_AmountTrack=p_SpreadTrack=p_SolidityTrack=(CRollTrack *)NULL;
  }
  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);

  // Metodo usato dai Object Space Modifiers
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
};


// ***********************************************************
// ********************       FFD      ***********************
// ***********************************************************
class CFFDModifier: public CObjectSpaceModifier
{
public:
  float4 p_Offset;
  int32 p_Invol, p_Dim1, p_Dim2, p_Dim3, p_NumControlPoints;
  AD_Vect3D *p_ControlPoints, *p_ControlPointsTr;
  CPosTrack *p_ControlPointsTracks;  // tracce di posizione dei control points

  CFFDModifier()
  {
	 p_ControlPointsTracks=(CPosTrack *)NULL;
	 p_ControlPoints=p_ControlPointsTr=(AD_Vect3D *)NULL;
	 p_Invol=1;
  }
  void m_Init(void);
  void m_Update (float4 framepos, AD_Vect3D *pivot);
  void m_Map (AD_Vect3D *pos, AD_Vect3D *out);
  void m_Map2 (AD_Vect3D *pos, AD_Vect3D *out);
  void m_Map3 (AD_Vect3D *pos, AD_Vect3D *out);
};

#endif