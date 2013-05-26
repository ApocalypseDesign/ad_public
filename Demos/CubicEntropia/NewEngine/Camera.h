#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "RenderLib.h"
#include "GeometricObject.h"

#define TARGETED_CAMERA 1
#define FREE_CAMERA 2

class CCamera: public CGeometricObject
{
public:
    int32 p_LastKeyPos;     // serve per il multicamera

    float4 p_Znear, p_Zfar;  // distanza dei piani di clipping dall'asse z
    AD_Vect3D p_CurrentTargetPosition;
    float4 p_CurrentRoll;
    float4 p_CurrentFov;
    float4 p_CurrentTanFov;   // tangente dell'angolo di FOV, usata per il clip3d
			                     // di oggetti e triangoli

	AD_Vect3D p_LeftFrustrumNormal;
	AD_Vect3D p_RightFrustrumNormal;
	AD_Vect3D p_UpFrustrumNormal;
	AD_Vect3D p_DownFrustrumNormal;

    float4 p_AspectRatio;
	float4 p_CurrentAngX, p_CurrentAngY, p_CurrentAngZ;

    CPosTrack *p_TargetTrack;
    CRollTrack *p_RollTrack;
	CRollTrack *p_FovTrack;

	AD_Matrix p_ViewMatrix, p_InverseViewMatrix;

    CCamera();
	void m_SetViewportDimension(int32 width, int32 height);

    // derivati da CGeometricObject
	int32 m_Init(void *arg1);
    void m_BuildWorldMatrix(void);
    void m_Update(float4 frame);
    void m_Ray (float4 screenX, float4 screenY, int32 space, Ray *out);
    void m_Paint(void *arg1, void *arg2, void *arg3);
};

#endif