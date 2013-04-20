// Metodi della classe Camera
// By [Turbo]

#include <windows.h>
#include <math.h>
#include "..\utility.h"
#include "..\Math\vect3D.h"
#include "camera.h"


extern HWND miawin;
extern FILE *fdeb;

AD_Camera::AD_Camera()
{
  znear=2.0;
  zfar=2000.0;

  name=(char *)NULL;
  screenX=screenY=0;
  prospettivaX=prospettivaY=0;
  currentroll=0;
  currentfov=0;
  currenttgfov=0;
}


// trasforma il lens del 3ds in fov in radianti; copiata dal clax
float4 AD_Camera::lens_to_fov (float4 lens)
{
  int  i;
  struct {
    float lens, fov;
  } lens_table[] = {
    {15.0f,  115.0f}, {20.0f, 94.28571f}, {24.0f, 84.0f}, {28.0f,  76.36364f},
    {35.0f,  63.0f},  {50.0f, 46.0f},     {85.0f, 28.0f}, {135.0f, 18.0f},
    {200.0f, 12.0f}
  };

  for (i = 0; i < 9; i++)
     if (lens == lens_table[i].lens) return(lens_table[i].fov);

  return((float)(15.0 / lens * 160.0));  // ho controllato, questa formula deriva dall'aver imposto
			      // un sistema di minimi quadrati di approssimazione, sui dati
			      // della tabella lens_table[]
}


int AD_Camera::init(imagebuffer *videobuffer)
{
  int j;

  // faccio /2 cosi' a runtime si interpola direttamente
  // fov/2 (in radianti)
  for (j=0; j<fovtrack.numkey; j++) fovtrack.rollazioni[j].roll*=0.5f;

  if (positiontrack.init()==-1) return(-1);
  if (targettrack.init()==-1)   return(-1);
  if (rollingtrack.init()==-1)  return(-1);
  if (fovtrack.init()==-1)      return(-1);

  // una key viene sempre messa in pos 0 anche se non si setta !!! .3ds sux  
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }

  // una key viene sempre messa in pos 0 anche se non si setta !!! .3ds sux  
  if ((targettrack.numkey==1) && (targettrack.posizioni[0].posintrack==0))
  {
	currenttarget=targettrack.posizioni[0].p;
	targettrack.numkey=0;
  }

  // una key viene sempre messa in pos 0 anche se non si setta !!! .3ds sux
  if ((rollingtrack.numkey==1) && (rollingtrack.rollazioni[0].posintrack==0))
  {
	currentroll=rollingtrack.rollazioni[0].roll;
	rollingtrack.numkey=0;
  }

  // una key viene sempre messa in pos0 anche se non si setta !!! .3ds sux
  if ((fovtrack.numkey==1) && (fovtrack.rollazioni[0].posintrack==0))
  {
	currentfov=fovtrack.rollazioni[0].roll;
    currenttgfov=(float)tan(fabs(currentfov));
	fovtrack.numkey=0;
  }

  
  // costruzione del frustrum (le normali puntano all'interno)
  vect_set(&frustrum_left_normal, cosf(currentfov), 0, sinf(currentfov));
  vect_set(&frustrum_right_normal, -cosf(currentfov), 0, sinf(currentfov));
  vect_set(&frustrum_up_normal, 0, cosf(currentfov), sinf(currentfov));
  vect_set(&frustrum_down_normal, 0, -cosf(currentfov), sinf(currentfov));


  // si cerca l'ultima posizione (nella timeline) occupata da
  // una key; questo serve per il multicamera
  lastkey_pos=-1;

  if (positiontrack.numkey>0)
    if (positiontrack.posizioni[positiontrack.numkey-1].posintrack>lastkey_pos)
      lastkey_pos=positiontrack.posizioni[positiontrack.numkey-1].posintrack;

  if (targettrack.numkey>0)
    if (targettrack.posizioni[targettrack.numkey-1].posintrack>lastkey_pos)
      lastkey_pos=targettrack.posizioni[targettrack.numkey-1].posintrack;

  if (rollingtrack.numkey>0)
    if (rollingtrack.rollazioni[rollingtrack.numkey-1].posintrack>lastkey_pos)
      lastkey_pos=rollingtrack.rollazioni[rollingtrack.numkey-1].posintrack;

  if (fovtrack.numkey>0)
    if (fovtrack.rollazioni[fovtrack.numkey-1].posintrack>lastkey_pos)
      lastkey_pos=fovtrack.rollazioni[fovtrack.numkey-1].posintrack;
    
  
  screenX=(float)(videobuffer->width / 2);
  screenY=(float)(videobuffer->height / 2);
  return(0);
}


void AD_Camera::build_cameramatrix(float4 framepos) // numero frame
{
  AD_Matrix pretrans;
  AD_Vect3D cameradir, ptmp;
  float  ax, ay, az,focus;
  float  sinx, siny, sinz, cosx, cosy, cosz;

  // estrazione dei dati tramite il keyframer, niente di piu' facile !!!
  
  if (positiontrack.numkey>0)  positiontrack.get_data(framepos, &currentpos);
  if (targettrack.numkey>0)    targettrack.get_data(framepos, &currenttarget);
  if (rollingtrack.numkey>0)   rollingtrack.get_data(framepos, &currentroll);
  if (fovtrack.numkey>0)
  {
	  fovtrack.get_data(framepos, &currentfov);
	  currenttgfov=(float)tan(currentfov);
      // costruzione del frustrum (le normali puntano all'interno)
      vect_set(&frustrum_left_normal, cosf(currentfov), 0, sinf(currentfov));
      vect_set(&frustrum_right_normal, -cosf(currentfov), 0, sinf(currentfov));
      vect_set(&frustrum_up_normal, 0, cosf(currentfov), sinf(currentfov));
      vect_set(&frustrum_down_normal, 0, -cosf(currentfov), sinf(currentfov));
  }

  vect_neg(&currentpos, &ptmp);
  mat_setmatrix_of_pretraslation(&pretrans, &ptmp);
  vect_sub(&currenttarget, &currentpos, &cameradir);
  focus=vect_lenght(&cameradir);

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

  if (screenX>screenY)
  {
    prospettivaX=screenX/currenttgfov;
    prospettivaY=0.95f*screenX/currenttgfov;
  }
  else
  {
    prospettivaX=screenY/currenttgfov;
    prospettivaY=0.95f*screenY/currenttgfov;
  }

  ax = -atan2f (cameradir.x, cameradir.z);
  ay = asinf (cameradir.y / focus);
  az = -currentroll;
  sinx = sinf (ax); cosx = cosf (ax); siny = sinf (ay);
  cosy = cosf (ay); sinz = sinf (az); cosz = cosf (az);
  currentmatrix_rot.a[0][0] =  sinx * siny * sinz + cosx * cosz;
  currentmatrix_rot.a[0][1] =  cosy * sinz;
  currentmatrix_rot.a[0][2] =  sinx * cosz - cosx * siny * sinz;
  currentmatrix_rot.a[1][0] =  sinx * siny * cosz - cosx * sinz;
  currentmatrix_rot.a[1][1] =  cosy * cosz;
  currentmatrix_rot.a[1][2] = -cosx * siny * cosz - sinx * sinz;
  currentmatrix_rot.a[2][0] = -sinx * cosy;
  currentmatrix_rot.a[2][1] =  siny;
  currentmatrix_rot.a[2][2] =  cosx * cosy;
  
  mat_mul(&currentmatrix_rot, &pretrans, &currentmatrix);
}