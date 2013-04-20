#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "keyframe.h"

class AD_Camera
{
public:
       AD_Camera();

	   float4 lens_to_fov (float4 lens);    // trasforma il lens del 3ds in fov in radianti
       int init(imagebuffer *videobuffer);  // fa l'init delle varie tracce di key; restituisce -1
		                                    // in caso di inizializzazione errata
       void build_cameramatrix(float4); // parametro: numero frame


       int lastkey_pos;     // serve per il multicamera
       float4 znear, zfar;  // distanza dei piani di clipping dall'asse z

       AD_Vect3D currentpos;      // posizione corrente utile per clippare gli oggetti
       AD_Vect3D currenttarget;
       float4 currentroll;
       float4 currentfov;
       float4 currenttgfov;      // tangente dell'angolo di FOV, usata per il clip3d
			                     // di oggetti e triangoli

       AD_Matrix currentmatrix_rot;
       AD_Matrix currentmatrix;
	   AD_Matrix inverse_rotmatrix;
	   AD_Vect3D frustrum_left_normal;
	   AD_Vect3D frustrum_right_normal;
	   AD_Vect3D frustrum_up_normal;
	   AD_Vect3D frustrum_down_normal;

       float4 screenX, screenY;
	   float4 prospettivaX, prospettivaY;
       char *name;    // nome (dell'oggetto) telecamera

       postrack positiontrack;
       postrack targettrack;
       rolltrack rollingtrack;
	   rolltrack  fovtrack;

protected:
private:
};

#endif