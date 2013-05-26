#include <windows.h>
#include <math.h>
#include "..\..\utility.h"
#include "..\camera.h"
#include "..\keyframe.h"
#include "..\lights.h"
#include "types.h"

extern HWND miawin;  // finestra principale; usata qui per le debug_error
extern FILE *fdeb;
extern AD_Tria3D **list_to_paint_trasparent;  // lista dei triangoli trasparenti

// ####################################################################
// ######################  LUCI OMNIDIREZIONALI  ######################
// ####################################################################

void AD_Omnilight::init(void)
{
  int kc;
  float swap;

  if (near_radius>far_radius)
  {
	swap=near_radius;
	near_radius=far_radius;
	far_radius=swap;
  }
  if (near_radius<1E-9f) near_radius =1E-9f;
  if (far_radius<1E-9f) far_radius =1E-9f;
  inv_near_radius=1.0f/near_radius;
  inv_far_radius=1.0f/far_radius;

  if (colortrack.numkey>0)
  {
     // questo swapping serve perche' essendo la colortrack una traccia di posizione (ovvero
     // tripletta di float, che pero' in questo caso sono intesi come RGB) la yz vengono
     // swappate dal keyframer reader, e quindi essendo questi colori non devono essere
     // swappati  ==> serve uno swap per tornare ai dati giusti
     for (kc=0; kc<colortrack.numkey; kc++)
     {
       vect_swap_3DS(&colortrack.posizioni[kc].p);
       vect_swap_3DS(&colortrack.posizioni[kc].incomtg);
       vect_swap_3DS(&colortrack.posizioni[kc].outcomtg);
       vect_scale(&colortrack.posizioni[kc].p, RGB_MAXVALUE, &colortrack.posizioni[kc].p);
	   vect_scale(&colortrack.posizioni[kc].incomtg, RGB_MAXVALUE, &colortrack.posizioni[kc].incomtg);
       vect_scale(&colortrack.posizioni[kc].outcomtg, RGB_MAXVALUE, &colortrack.posizioni[kc].outcomtg);
     }
  }
  colortrack.init();
  positiontrack.init();

  // una key viene sempre messa in pos 0 anche se non si setta !!! .3ds sux  
  if ((positiontrack.numkey==1) && (positiontrack.posizioni[0].posintrack==0))
  {
	currentpos=positiontrack.posizioni[0].p;
	positiontrack.numkey=0;
  }
  
  // una key viene sempre messa in pos 0 anche se non si setta !!! .3ds sux  
  if ((colortrack.numkey==1) && (colortrack.posizioni[0].posintrack==0))
  {
	currentcolor=colortrack.posizioni[0].p;
	colortrack.numkey=0;
  }

  // sistemo il flare connesso; devo creare i 2 triangoli,
  // i vertici 2D, 3D (solo per linkare i v2D), il materiale
  // trasparente
  if (flare!=(texture *)NULL)
  {
	latoX=(float)flare->dimx;
	latoY=(float)flare->dimy;

	// setto le uv dei vertici per il flare; i vertici sono cosi'
	// disposti
	// 0----1
	// |    |
	// |    |
	// 2----3

	v2D[0].u=0.0f*latoX*TEXTURE_PRECISION;
	v2D[0].v=0.0f*latoY*TEXTURE_PRECISION;
	v2D[1].u=0.99f*latoX*TEXTURE_PRECISION;
	v2D[1].v=0.0f*latoY*TEXTURE_PRECISION;
	v2D[2].u=0.0f*latoX*TEXTURE_PRECISION;
	v2D[2].v=0.99f*latoY*TEXTURE_PRECISION;
	v2D[3].u=0.99f*latoX*TEXTURE_PRECISION;
	v2D[3].v=0.99f*latoY*TEXTURE_PRECISION;

	// i vertici 3D li setto sempre da non clippare visto
	// che quando devo disegnare i flare sono sicuro che non
	// devo clipparli e solo quando disegno li inserisco nella
	// lista dei triangoli trasparenti
	v3D[0].flags=0;
	v3D[1].flags=0;
	v3D[2].flags=0;
	v3D[3].flags=0;
	
	// setto il materiale
    matflare.texture_ptr=flare;
	matflare.flags=(PAINT_TEXTURE | IS_TRASPARENT);
	matflare.trasparencytype=MIXTYPE_ADD;
	AddUpdate_Material(&matflare);

	tria[0].materiale=&matflare;
	tria[0].v1=&v3D[0]; tria[0].sp1=&v2D[0];
	tria[0].v2=&v3D[2]; tria[0].sp2=&v2D[2];
	tria[0].v3=&v3D[3]; tria[0].sp3=&v2D[3];

	tria[1].materiale=&matflare;
	tria[1].v1=&v3D[3]; tria[1].sp1=&v2D[3];
	tria[1].v2=&v3D[1]; tria[1].sp2=&v2D[1];
	tria[1].v3=&v3D[0]; tria[1].sp3=&v2D[0];
  }
}


void AD_Omnilight::update (float4 pos, AD_Camera *cam)
{
  float nlX, nlY, invz;

  if (positiontrack.numkey>0) positiontrack.get_data(pos, &currentpos);
  if (colortrack.numkey>0)    colortrack.get_data(pos, &currentcolor);

  if (currentcolor.x<0)	currentcolor.x=0;
  if (currentcolor.x>RGB_MAXVALUE)	currentcolor.x=RGB_MAXVALUE;
  if (currentcolor.y<0) currentcolor.y=0;
  if (currentcolor.y>RGB_MAXVALUE)	currentcolor.y=RGB_MAXVALUE;
  if (currentcolor.z<0) currentcolor.z=0;
  if (currentcolor.z>RGB_MAXVALUE)	currentcolor.z=RGB_MAXVALUE;

  // mi calcolo la posizione della luce in camera-space
  mat_mulvect(&cam->currentmatrix, &currentpos, &currentpos_incamera);
  
  if ( (flare!=(texture *)NULL) &&
	   (currentpos_incamera.z > cam->znear) &&
	   (currentpos_incamera.z < cam->zfar) )
  {
    // proietto in 2D su schermo la luce
	invz=1.0f/currentpos_incamera.z;
    currentpos_incamera.x=cam->prospettivaX*(currentpos_incamera.x*invz) + (cam->screenX);
    currentpos_incamera.y=(cam->screenY) - cam->prospettivaY*(currentpos_incamera.y*invz);

	nlX=(flare_scale_x*latoX)/(currentpos_incamera.z - cam->znear + 1.0f);
    nlY=(flare_scale_y*latoY)/(currentpos_incamera.z - cam->znear + 1.0f);

	// mi calcolo i punti dei 2 triangoli
    v2D[0].xs=currentpos_incamera.x - (nlX/2.0f);
	if (v2D[0].xs > cam->screenX*2) return;
    v2D[0].ys=currentpos_incamera.y - (nlY/2.0f);
	if (v2D[0].ys > cam->screenY*2) return;
    v2D[0].dist=invz;


    v2D[1].xs=currentpos_incamera.x + (nlX/2.0f);
	if (v2D[1].xs < 0) return;
    v2D[1].ys=currentpos_incamera.y - (nlY/2.0f);
    v2D[1].dist=invz;


    v2D[2].xs=currentpos_incamera.x - (nlX/2.0f);
    v2D[2].ys=currentpos_incamera.y + (nlY/2.0f);
	if (v2D[2].ys < 0) return;
    v2D[2].dist=invz;

    v2D[3].xs=currentpos_incamera.x + (nlX/2.0f);
    v2D[3].ys=currentpos_incamera.y + (nlY/2.0f);
    v2D[3].dist=invz;

    tria[0].mid_z=currentpos_incamera.z;
    tria[1].mid_z=currentpos_incamera.z;
	*list_to_paint_trasparent++=&tria[0];
	*list_to_paint_trasparent++=&tria[1];
  }
}


// ####################################################################
// ###########################  LUCI SPOT  ############################
// ####################################################################

void AD_Spotlight::init(void)
{
  int kc;
  float swap;

  if (near_radius>far_radius)
  {
	swap=near_radius;
	near_radius=far_radius;
	far_radius=swap;
  }
  if (near_radius<1E-9f) near_radius =1E-9f;
  if (far_radius<1E-9f) far_radius =1E-9f;
  inv_near_radius=1.0f/near_radius;
  inv_far_radius=1.0f/far_radius;
  
  currentfalloff=(float)(currentfalloff*M_PI/360.0);
  cos_currentfalloff=(float)cos(currentfalloff);
  currenthotspot=(float)(currenthotspot*M_PI/360.0);
  if (colortrack.numkey>0)
  {
     // questo swapping serve perche' essendo la colortrack una traccia di posizione (ovvero
     // tripletta di float, che pero' in questo caso sono intesi come RGB) la yz vengono
     // swappate dal keyframer reader, e quindi essendo questi colori non devono essere
     // swappati  ==> serve uno swap per tornare ai dati giusti
     for (kc=0; kc<colortrack.numkey; kc++)
     {
        vect_swap_3DS(&colortrack.posizioni[kc].p);
        vect_swap_3DS(&colortrack.posizioni[kc].incomtg);
        vect_swap_3DS(&colortrack.posizioni[kc].outcomtg);
        vect_scale(&colortrack.posizioni[kc].p, RGB_MAXVALUE, &colortrack.posizioni[kc].p);
	    vect_scale(&colortrack.posizioni[kc].incomtg, RGB_MAXVALUE, &colortrack.posizioni[kc].incomtg);
        vect_scale(&colortrack.posizioni[kc].outcomtg, RGB_MAXVALUE, &colortrack.posizioni[kc].outcomtg);
     }
  }
  colortrack.init();
  positiontrack.init();
  targettrack.init();

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
  if ((colortrack.numkey==1) && (colortrack.posizioni[0].posintrack==0))
  {
	currentcolor=colortrack.posizioni[0].p;
	colortrack.numkey=0;
  }
}


void AD_Spotlight::update (float4 pos, AD_Camera *cam)
{
  if (positiontrack.numkey>0) positiontrack.get_data(pos, &currentpos);
  if (targettrack.numkey>0)   targettrack.get_data(pos, &currenttarget);
  if (colortrack.numkey>0) colortrack.get_data(pos, &currentcolor);

  if (currentcolor.x<0)	currentcolor.x=0;
  if (currentcolor.x>RGB_MAXVALUE)	currentcolor.x=RGB_MAXVALUE;
  if (currentcolor.y<0) currentcolor.y=0;
  if (currentcolor.y>RGB_MAXVALUE)	currentcolor.y=RGB_MAXVALUE;
  if (currentcolor.z<0) currentcolor.z=0;
  if (currentcolor.z>RGB_MAXVALUE)	currentcolor.z=RGB_MAXVALUE;
}