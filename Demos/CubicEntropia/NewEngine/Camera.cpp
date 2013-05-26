// Metodi della classe Camera
// By [Turbo]

#include <windows.h>
#include "Camera.h"
#include "BoundPrimitives.h"
#include "FastMath.h"

extern HWND miawin;

CCamera::CCamera()
{
   p_LastKeyPos=0;
   p_Znear=2.0f;
   p_Zfar=2000.0f;  // distanza dei piani di clipping dall'asse z
   p_AspectRatio=640.0f/480.0f;
   p_CurrentAngX=0;
   p_CurrentAngY=0;
   p_CurrentAngZ=0;

   vect_set(&p_CurrentTargetPosition, 0, 0, 0);
   p_CurrentRoll=0;
   p_CurrentFov=(Pi__/2.0f);
   p_CurrentTanFov=1.0f;

   vect_set(&p_LeftFrustrumNormal, 0, 0, 0);
   vect_set(&p_RightFrustrumNormal, 0, 0, 0);
   vect_set(&p_UpFrustrumNormal, 0, 0, 0);
   vect_set(&p_DownFrustrumNormal, 0, 0, 0);

   p_TargetTrack=(CPosTrack *)NULL;
   p_RollTrack=(CRollTrack *)NULL;
   p_FovTrack=(CRollTrack *)NULL;

   mat_identity(&p_ViewMatrix);
   mat_identity(&p_InverseViewMatrix);

   p_Type=TARGETED_CAMERA;
}

//--------------------------------------------------------------------

void CCamera::m_SetViewportDimension(int32 width, int32 height)
{
  p_AspectRatio=(float4)fabs((float4)width/(float4)height);
}

//--------------------------------------------------------------------

int32 CCamera::m_Init(void *arg1)
{
  int32 res;
  float4 c, s;

  // faccio /2 cosi' a runtime si interpola direttamente
  // fov/2 (in radianti)
  if (p_FovTrack)
  {
     //for (int32 j=0; j<p_FovTrack->p_NumKeys; j++)
	 //  p_FovTrack->p_Keys[j].roll*=0.5f;
  }

  if (p_TargetTrack)
    if (!p_TargetTrack->m_Init(1)) return(0);

  if (p_RollTrack)
    if (!p_RollTrack->m_Init(1))  return(0);

  if (p_FovTrack)
    if (!p_FovTrack->m_Init(1))   return(0);


  // costruzione del frustrum (le normali puntano all'interno)
  s=fast_sinf(p_CurrentFov*0.5f);
  c=fast_cosf(p_CurrentFov*0.5f);
  vect_set(&p_LeftFrustrumNormal, c, 0, s);
  vect_set(&p_RightFrustrumNormal, -c, 0, s);
  vect_set(&p_UpFrustrumNormal, 0, c, s);
  vect_set(&p_DownFrustrumNormal, 0, -c, s);


  // si cerca l'ultima posizione (nella timeline) occupata da
  // una key; questo serve per il multicamera
  p_LastKeyPos=-1;

  if (p_PosTrack)
    if (p_PosTrack->p_Keys[p_PosTrack->p_NumKeys-1].posintrack > p_LastKeyPos)
      p_LastKeyPos=p_PosTrack->p_Keys[p_PosTrack->p_NumKeys-1].posintrack;

  if (p_TargetTrack)
    if (p_TargetTrack->p_Keys[p_TargetTrack->p_NumKeys-1].posintrack>p_LastKeyPos)
      p_LastKeyPos=p_TargetTrack->p_Keys[p_TargetTrack->p_NumKeys-1].posintrack;

  if (p_RollTrack)
    if (p_RollTrack->p_Keys[p_RollTrack->p_NumKeys-1].posintrack > p_LastKeyPos)
      p_LastKeyPos=p_RollTrack->p_Keys[p_RollTrack->p_NumKeys-1].posintrack;

  if (p_FovTrack)
    if (p_FovTrack->p_Keys[p_FovTrack->p_NumKeys-1].posintrack>p_LastKeyPos)
      p_LastKeyPos=p_FovTrack->p_Keys[p_FovTrack->p_NumKeys-1].posintrack;

  res=CGeometricObject::m_Init(arg1);
  // estraggo i dati al tempo 0 per un settaggio iniziale
  if (res) m_Update(0);
  return(res);
}

//--------------------------------------------------------------------

void CCamera::m_BuildWorldMatrix(void)
{
  float4 sinx, cosx, siny, cosy, sinz, cosz;
  AD_Matrix pretrans, irot, swapXY;
  AD_Vect3D ptmp;

  sinx = fast_sinf(p_CurrentAngX);
  cosx = fast_cosf(p_CurrentAngX);
  siny = fast_sinf(p_CurrentAngY);
  cosy = fast_cosf(p_CurrentAngY);
  sinz = fast_sinf(p_CurrentAngZ);
  cosz = fast_cosf(p_CurrentAngZ);

  vect_neg(&p_CurrentPosition, &ptmp);
  mat_setmatrix_of_pretraslation(&pretrans, &ptmp);

  mat_identity(&p_CurrentRotationMatrix);
  p_CurrentRotationMatrix.a[0][0] =  sinx * siny * sinz + cosx * cosz;
  p_CurrentRotationMatrix.a[0][1] =  cosy * sinz;
  p_CurrentRotationMatrix.a[0][2] =  sinx * cosz - cosx * siny * sinz;
  p_CurrentRotationMatrix.a[1][0] =  sinx * siny * cosz - cosx * sinz;
  p_CurrentRotationMatrix.a[1][1] =  cosy * cosz;
  p_CurrentRotationMatrix.a[1][2] = -cosx * siny * cosz - sinx * sinz;
  p_CurrentRotationMatrix.a[2][0] = -sinx * cosy;
  p_CurrentRotationMatrix.a[2][1] =  siny;
  p_CurrentRotationMatrix.a[2][2] =  cosx * cosy;
  
  mat_mul(&p_CurrentRotationMatrix, &pretrans, &p_ViewMatrix);

  // costruzione matrice inversa
  mat_transpose(&p_CurrentRotationMatrix, &irot);
  mat_setmatrix_of_pretraslation(&pretrans, &p_CurrentPosition);
  mat_mul(&pretrans, &irot, &p_InverseViewMatrix);


  // costruzione matrice per il linking con oggetti
  // 1) matrice di swap YZ
  mat_identity(&swapXY);
  swapXY.a[1][1]=0;
  swapXY.a[2][2]=0;
  swapXY.a[1][2]=1;
  swapXY.a[2][1]=-1;

  mat_mul(&p_InverseViewMatrix, &swapXY, &p_WorldMatrix);
  //mat_mul(&swapXY, &p_InverseViewMatrix, &p_WorldMatrix);
  //mat_copy(&p_InverseViewMatrix, &p_WorldMatrix);
}

//--------------------------------------------------------------------

void CCamera::m_Update(float4 frame)
{
  AD_Vect3D cameradir;
  const float4 EPS=0.2f;
  float4 focus, c, s;

  // estrazione dei dati tramite il keyframer, niente di piu' facile !!!
  
  if (p_PosTrack)
	  p_PosTrack->m_GetData(frame, &p_CurrentPosition);

  if (p_TargetTrack)
	  p_TargetTrack->m_GetData(frame, &p_CurrentTargetPosition);

  if (p_RollTrack)
	  p_RollTrack->m_GetData(frame, &p_CurrentRoll);

  if (p_FovTrack)
  {
	  p_FovTrack->m_GetData(frame, &p_CurrentFov);
	  p_CurrentTanFov=(float4)tan(p_CurrentFov);
      // costruzione del frustrum (le normali puntano all'interno)
      s=fast_sinf(p_CurrentFov*0.5f);
      c=fast_cosf(p_CurrentFov*0.5f);
      vect_set(&p_LeftFrustrumNormal, c, 0, s);
      vect_set(&p_RightFrustrumNormal, -c, 0, s);
      vect_set(&p_UpFrustrumNormal, 0, c, s);
      vect_set(&p_DownFrustrumNormal, 0, -c, s);
  }

  vect_sub(&p_CurrentTargetPosition, &p_CurrentPosition, &cameradir);
  focus=vect_length(&cameradir);

/*
  cr=currentroll;
  vect_set(&up, (float)sin(cr), (float)(-cos(cr)), 0.0);
  vect_cross(&cameradir, &up, &right);
  vect_normalize(&right);
  vect_cross(&right, &cameradir, &up);

  mat_insert_row(&currentmatrix_rot, 0, &right);
  mat_insert_row(&currentmatrix_rot, 1, &up);
  mat_insert_row(&currentmatrix_rot, 2, &cameradir);
  mat_mul(&currentmatrix_rot, &pretrans, &currentmatrix);
*/


/*  vect_normalize(&cameradir, &ptmp);
  if (fabs(ptmp.z) < EPS)
  {
	 int32 fdfd=0;
	 //if (cameradir.x >= 0)
	  ax=0;
	  az=0;
	  ay=0;
     //else
	   //ax=-1.5708f;
  }
  else*/
  {
    p_CurrentAngX = (float4)-atan2 (cameradir.x, cameradir.z);
    p_CurrentAngY = (float4)asin (cameradir.y / focus);
    p_CurrentAngZ = -p_CurrentRoll;
  }

  m_BuildWorldMatrix();
}

//--------------------------------------------------------------------

void CCamera::m_Ray (float4 screenX, float4 screenY, int32 space, Ray *out)
{
   // screenX e screenY vanno nel range [-1;1]
   AD_Vect3D cam_x, cam_y, cam_z;
   float4 fx, fy;

   if (!out) return;

   fx=(float4)tan(p_CurrentFov*0.5);
   fy=(float4)tan(p_CurrentFov*0.5);

   if (space==WORLDSPACE_RAY)
   {
      vect_copy(&p_CurrentPosition, &out->base);
      mat_get_row(&p_CurrentRotationMatrix, 0, &cam_x);
      mat_get_row(&p_CurrentRotationMatrix, 1, &cam_y);
      mat_get_row(&p_CurrentRotationMatrix, 2, &cam_z);
      vect_auto_scale(&cam_x, screenX*fx);
      vect_auto_scale(&cam_y, screenY*fy*480.0f/640.0f);
      vect_add(&cam_x, &cam_y, &out->direction);
      vect_auto_add(&out->direction, &cam_z);
      vect_auto_normalize(&out->direction);
   }
   else
   if (space==CAMERASPACE_RAY)
   {
      vect_set(&out->base, 0, 0, 0);
      vect_set(&out->direction, screenX*fx, screenY*fy, 1);
   }
   vect_auto_normalize(&out->direction);
}

//--------------------------------------------------------------------

void CCamera::m_Paint(void *arg1, void *arg2, void *arg3)
{
   return;
}

//--------------------------------------------------------------------