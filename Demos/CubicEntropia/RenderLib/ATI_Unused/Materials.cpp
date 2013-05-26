#include "RenderLib.h"
#include "SetMaterial.h"

CMaterial::CMaterial()
{
  strcpy(p_Name, "");
  strcpy(p_FatherName, "");
  p_TransparencyAmount=128;
  p_TransparencyType=OFF;
  p_MapsMixType=MIXTYPE_BLEND3DS;
  p_AlphaMixFactor=128;
  p_LightMixType=MIXTYPE_BLEND3DS;
  p_BumpType=BUMP_EMBM;
  p_BumpUScale=1.0f;
  p_BumpVScale=1.0f;;
  p_BackFaceCulling=BACKFACE_CULLING;
  p_Map1=(CGraphicSurface *)NULL;
  p_Map2=(CGraphicSurface *)NULL;
  p_Map1Channel=TEXMAPPING1;
  p_Map2Channel=ENVMAPPING;
  p_BumpMap=(CGraphicSurface *)NULL;
  p_BumpType=0;
  p_AlphaMap=(CGraphicSurface *)NULL;
  p_NextMaterial=(CMaterial *)NULL;
  p_NumPasses=0;
  p_U1Offset=p_U2Offset=0;
  p_V1Offset=p_V2Offset=0;
  p_W1Angle=p_W2Angle=0;
  p_U1Tile=p_U2Tile=1;
  p_V1Tile=p_V2Tile=1;
  p_U1OffsetTrack=p_U2OffsetTrack=(CRolltrack *)NULL;
  p_V1OffsetTrack=p_V2OffsetTrack=(CRolltrack *)NULL;
  p_W1AngleTrack=p_W2AngleTrack=(CRolltrack *)NULL;
  p_U1TileTrack=p_U2TileTrack=(CRolltrack *)NULL;
  p_V1TileTrack=p_V2TileTrack=(CRolltrack *)NULL;
  mat_identity(&p_Map1Matrix);
  mat_identity(&p_Map2Matrix);
  p_TransparencyTrack=(CRolltrack *)NULL;
  p_Specular=OFF;
}

//--------------------------------------------------------------------

int32 CMaterial::m_Init(void *arg1)
{
  if (p_U1OffsetTrack)
    if (!p_U1OffsetTrack->m_Init(1)) return(0);
  if (p_U2OffsetTrack)
    if (!p_U2OffsetTrack->m_Init(1)) return(0);
  if (p_V1OffsetTrack)
    if (!p_V1OffsetTrack->m_Init(1)) return(0);
  if (p_V2OffsetTrack)
    if (!p_V2OffsetTrack->m_Init(1)) return(0);
  if (p_W1AngleTrack)
    if (!p_W1AngleTrack->m_Init(1)) return(0);
  if (p_W2AngleTrack)
    if (!p_W2AngleTrack->m_Init(1)) return(0);
  if (p_U1TileTrack)
    if (!p_U1TileTrack->m_Init(1)) return(0);
  if (p_U2TileTrack)
    if (!p_U2TileTrack->m_Init(1)) return(0);
  if (p_V1TileTrack)
    if (!p_V1TileTrack->m_Init(1)) return(0);
  if (p_V2TileTrack)
    if (!p_V2TileTrack->m_Init(1)) return(0);
  if (p_TransparencyTrack)
	if (!p_TransparencyTrack->m_Init(1)) return(0);

  p_NumPasses=1;
  if (p_NextMaterial) p_NumPasses++;
  m_BuildFVF();
  return(1);
}

//--------------------------------------------------------------------

void CMaterial::m_Update(float4 frame)
{
  AD_Matrix l_rot, l_trans, l_scale, aux, prePivotMatrix, postPivotMatrix;
  AD_Vect3D l_offs, l_prePivot, l_postPivot;
  int32 i;

  if (p_U1OffsetTrack)
    p_U1OffsetTrack->m_GetData(frame, &p_U1Offset);
  if (p_U2OffsetTrack)
    p_U2OffsetTrack->m_GetData(frame, &p_U2Offset);
  if (p_V1OffsetTrack)
    p_V1OffsetTrack->m_GetData(frame, &p_V1Offset);
  if (p_V2OffsetTrack)
    p_V2OffsetTrack->m_GetData(frame, &p_V2Offset);
  if (p_W1AngleTrack)
    p_W1AngleTrack->m_GetData(frame, &p_W1Angle);
  if (p_W2AngleTrack)
    p_W2AngleTrack->m_GetData(frame, &p_W2Angle);
  if (p_U1TileTrack)
    p_U1TileTrack->m_GetData(frame, &p_U1Tile);
  if (p_U2TileTrack)
    p_U2TileTrack->m_GetData(frame, &p_U2Tile);
  if (p_V1TileTrack)
    p_V1TileTrack->m_GetData(frame, &p_V1Tile);
  if (p_V2TileTrack)
    p_V2TileTrack->m_GetData(frame, &p_V2Tile);

  if (p_U1Tile<0) p_U1Tile=0;
  if (p_V1Tile<0) p_V1Tile=0;


  vect_set(&l_prePivot, -0.5f, -0.5f, 0);
  vect_set(&l_postPivot, 0.5f, 0.5f, 0);
  mat_setmatrix_of_pretraslation(&prePivotMatrix, &l_prePivot);
  mat_setmatrix_of_pretraslation(&postPivotMatrix, &l_postPivot);

  // costruiamo la matrice per la mappa1
  vect_set(&l_offs, -p_U1Offset, -p_V1Offset, 0);
  mat_setmatrix_of_pretraslation(&l_trans, &l_offs);
  mat_setmatrix_of_eulerrotationZ(&l_rot, -p_W1Angle);
  mat_setmatrix_of_scaling(&l_scale, p_U1Tile, p_V1Tile, 1);

  mat_identity(&aux);
  mat_mul(&l_trans, &aux, &aux);
  mat_mul(&prePivotMatrix, &aux, &aux);
  mat_mul(&l_rot, &aux, &aux);
  mat_mul(&l_scale, &aux, &aux);
  mat_mul(&postPivotMatrix, &aux, &aux);
  aux.a[0][2]=aux.a[0][3];  aux.a[0][3]=0;
  aux.a[1][2]=aux.a[1][3];  aux.a[1][3]=0;
  aux.a[2][0]=aux.a[2][1]=aux.a[2][3]==0; aux.a[2][1]=1;
  for (i=0; i<3; i++) aux.a[1][i]=-aux.a[1][i];
  mat_transpose(&aux, &p_Map1Matrix);

  // costruiamo la matrice per la mappa1
  vect_set(&l_offs, -p_U2Offset, -p_V2Offset, 0);
  mat_setmatrix_of_pretraslation(&l_trans, &l_offs);
  mat_setmatrix_of_eulerrotationZ(&l_rot, -p_W2Angle);
  mat_setmatrix_of_scaling(&l_scale, p_U2Tile, p_V2Tile, 1);
  mat_identity(&aux);
  mat_mul(&l_trans, &aux, &aux);
  mat_mul(&prePivotMatrix, &aux, &aux);
  mat_mul(&l_rot, &aux, &aux);
  mat_mul(&l_scale, &aux, &aux);
  mat_mul(&postPivotMatrix, &aux, &aux);
  aux.a[0][2]=aux.a[0][3];  aux.a[0][3]=0;
  aux.a[1][2]=aux.a[1][3];  aux.a[1][3]=0;
  aux.a[2][0]=aux.a[2][1]=aux.a[2][3]==0; aux.a[2][1]=1;
  for (i=0; i<3; i++) aux.a[1][i]=-aux.a[1][i];
  mat_transpose(&aux, &p_Map2Matrix);

  if (p_TransparencyTrack)
  {
     float4 v;
     p_TransparencyTrack->m_GetData(frame, &v);
	 p_TransparencyAmount=(int32)v;
  }
}

//--------------------------------------------------------------------

int32 CMaterial::m_MapNeedUVTransform(int32 mapIndex)
{
   if ((mapIndex<1) || (mapIndex>2)) return(0);
   if (mapIndex==1)
   {
      if (!p_Map1) return(0);
	  if ((p_U1OffsetTrack) || (p_V1OffsetTrack) ||
		  (p_W1AngleTrack)                       ||
		  (p_U1TileTrack)   || (p_U1TileTrack))
          return(1);
	  else
	  if ((p_U1Offset!=0) || (p_V1Offset!=0) ||
		  (p_W1Angle!=0)                     ||
		  (p_U1Tile!=1)   || (p_V1Tile!=1))
           return(1);
	  else return(0);
   }
   else
   {
      if (!p_Map2) return(0);
	  if ((p_U2OffsetTrack) || (p_V2OffsetTrack) ||
		  (p_W2AngleTrack)                       ||
		  (p_U2TileTrack)   || (p_U2TileTrack))
          return(1);
	  else
	  if ((p_U2Offset!=0) || (p_V2Offset!=0) ||
		  (p_W2Angle!=0)                     ||
		  (p_U2Tile!=1)   || (p_V2Tile!=1))
           return(1);
	  else return(0);
   }
}

//--------------------------------------------------------------------

int32 CMaterial::m_GetNumPasses(void)
{
  return(p_NumPasses);
}

//--------------------------------------------------------------------

void CMaterial::m_BuildFVF(void)
{
   int32 TWO_UV=TEXMAPPING1 | TEXMAPPING2;
   int32 size, ch;
   char8 msg[150];

   strcpy(msg, "m_BuildFVF()");
   strcat(msg, p_Name);
   strcat(msg, " ");

   // un materiale non puo' avere la Mappa2 se non ha anche la Mappa1
   if ((p_Map2) && (!p_Map1))
   {
      strcat(msg, "Map2 is NOT NULL, and Map1 IS NULL!");
      return;
   }

   // ------------------    costruzione del FVF   --------------------
   p_FVF=D3DFVF_XYZ | D3DFVF_NORMAL;
   size=6*sizeof(float4);

   // sommiamo i canali usati dal materiale in questione e dal suo
   // eventuale NextMaterial
   ch=0;
   if (p_Map1) ch=ch | p_Map1Channel;
   if (p_Map2) ch=ch | p_Map2Channel;
   if (p_NextMaterial)
   {
      if (p_NextMaterial->p_Map1) ch=ch | p_NextMaterial->p_Map1Channel;
      if (p_NextMaterial->p_Map2) ch=ch | p_NextMaterial->p_Map2Channel;
   }

   // due mappe con 2 canali espliciti diversi
   if ((ch & TWO_UV)==TWO_UV)
   {
      p_FVF=p_FVF | D3DFVF_TEX2;
      size+=4*sizeof(float4);
   }
   else
   if ((ch & TEXMAPPING1) || (ch & TEXMAPPING2))
   {
      p_FVF=p_FVF | D3DFVF_TEX1;
      size+=2*sizeof(float4);
   }

   p_FVFSize=size;
}

//--------------------------------------------------------------------

uint32 CMaterial::m_FVF2Size(uint32 FVF)
{
   uint32 r;

   r=0;
   if (FVF & D3DFVF_XYZ) r+=3*sizeof(float4);
   if (FVF & D3DFVF_NORMAL) r+=3*sizeof(float4);
   if (FVF & D3DFVF_DIFFUSE) r+=sizeof(DWORD);
   if (FVF & D3DFVF_TEX2) r+=2*sizeof(float4);
   if (FVF & D3DFVF_TEX1) r+=sizeof(float4);
   return(r);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap0(CMaterial *m)
{
   HRESULT hr;

   hr=p_Device->SetTexture(0, NULL);
   hr=p_Device->SetTexture(1, NULL);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap1(CMaterial *m)
{
  HRESULT hr;

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurface);
  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

  if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);
  }
  else hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
  if (m->p_Map1Channel & ENVMAPPING)
     hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                           D3DTSS_TCI_CAMERASPACENORMAL | 0);
  else
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap1DOT3(CMaterial *m)
{
  HRESULT hr;

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurface);
  if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);
  }
  else hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
  if (m->p_Map1Channel & ENVMAPPING)
  {
  }
  else
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);

  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap12(CMaterial *m)
{
  HRESULT hr;

  // texture stage 0: texture, trasformazione UV
  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurface);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  //hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);
  }
  else hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

  // texture stage 1: texture, trasformazione UV
  hr=p_Device->SetTexture(1, m->p_Map2->p_HWSurface);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
  if (m->m_MapNeedUVTransform(2))
  {
     hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
     hr=p_Device->SetTransform(D3DTS_TEXTURE1, (D3DMATRIX *)&m->p_Map2Matrix);
  }
  else hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);


  // -------------------  mixing delle due mappe  --------------------
  if (m->p_MapsMixType==MIXTYPE_ADD)
    hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_ADD);
  else
  if (m->p_MapsMixType==MIXTYPE_SUB)
    hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  else
    hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_MODULATE);

  if (m->p_Map1Channel & ENVMAPPING)
  {
     // la mappa1 usa un envmapping
     hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                           D3DTSS_TCI_CAMERASPACENORMAL);
     if (m->p_Map2Channel & ENVMAPPING)
       hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                             D3DTSS_TCI_CAMERASPACENORMAL);
	 else hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
  }
  else
  {
	// la mappa1 ha le uv esplicite nella prima coppia del FVF
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    if (m->p_Map2Channel & ENVMAPPING)
      hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                            D3DTSS_TCI_CAMERASPACENORMAL);
	else
	{
      if (m->p_Map2Channel & TEXMAPPING1)
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
	  else hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
	}
  }
}

//--------------------------------------------------------------------

inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }

void CRenderLib::m_SetMap12EMBM(CMaterial *m)
{
  HRESULT hr;

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurface);
  if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);

    hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE1, (D3DMATRIX *)&m->p_Map1Matrix);
  }
  else
  {
	hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  }
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);

  // bump map sullo stage 1
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT00, F2DW(m->p_BumpUScale));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT01, F2DW(0.0f));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT10, F2DW(0.0f));
  hr=p_Device->SetTextureStageState(1, D3DTSS_BUMPENVMAT11, F2DW(m->p_BumpVScale));
  hr=p_Device->SetTexture(1, m->p_BumpMap->p_HWSurface);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);

  hr=p_Device->SetTexture(2, m->p_Map2->p_HWSurface);
  if (m->m_MapNeedUVTransform(2))
  {
     hr=p_Device->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
     hr=p_Device->SetTransform(D3DTS_TEXTURE2, (D3DMATRIX *)&m->p_Map2Matrix);
  }
  else hr=p_Device->SetTextureStageState(2, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(2, D3DTSS_COLORARG2, D3DTA_CURRENT);

  // -------------------  mixing delle due mappe  --------------------
  if (m->p_MapsMixType==MIXTYPE_ADD)
    hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_ADD);
  else
  if (m->p_MapsMixType==MIXTYPE_SUB)
    hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
  else
    hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_MODULATE2X);

  if (m->p_Map1Channel & ENVMAPPING)
  {
     // la mappa1 usa un envmapping
     hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                           D3DTSS_TCI_CAMERASPACENORMAL);
     if (m->p_Map2Channel & ENVMAPPING)
       hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                             D3DTSS_TCI_CAMERASPACENORMAL);
	 else
       hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
  }
  else
  {
	// la mappa1 ha le uv esplicite nella prima coppia del FVF
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
    if (m->p_Map2Channel & ENVMAPPING)
      hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX,
			                            D3DTSS_TCI_CAMERASPACENORMAL);
	else
	{
      if (m->p_Map2Channel & TEXMAPPING1)
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 0);
	  else
        hr=p_Device->SetTextureStageState(2, D3DTSS_TEXCOORDINDEX, 1);
	}
  }
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap12DOT3(CMaterial *m)
{
   m_SetMap12(m);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMaterialProcedures (CMaterial *m)
{
/*
  int32 num_lights;

  // cosi' mi salvo sempre il culo almeno
  //m->myraster=*tria_raster_NULL;
  m->m_UseMe=*setNULL;

  if (m->p_Flags & PAINT_LIGHT) num_lights=1;
  else num_lights=0;

// ########################################################
// #############   in caso di nessuna luce   ##############
// ########################################################
   if ((num_lights<=0) || (m->p_LightMixType==MIXTYPE_NONE))
   {
	 if ((m->p_Map1==(CGraphicSurface *)NULL) &&
         (m->p_Map2==(CGraphicSurface *)NULL))
	 {
		m->m_UseMe=*setNULL;
		return;
	 }


	 if ((m->p_Map1!=(CGraphicSurface *)NULL) &&
         (m->p_Map2!=(CGraphicSurface *)NULL))
	 // texture + env di base, no luci, chekko i vari metodi
	 {
		m->m_UseMe=*setTexEnv_mBLEND3DS;

		if (m->p_TransparencyType!=OFF) 
		{ 
          if (m->p_TransparencyType==MIXTYPE_BLEND50) // caso di trasparenza mediata
		    switch (m->p_MapsMixType)
 		    {
		      case MIXTYPE_ADD: m->m_UseMe=*setTexEnv_mADD_tBLENDalpha;
				                m->p_TransparencyAmount=128;
                                //m->p_FVF |= D3DFVF_DIFFUSE;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MAPSMIXTYPE_ENVSUBTEX: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MIXTYPE_BLEND50: m->m_UseMe=*setTexEnv_mBLEND3DS_tBLENDalpha;
				                    m->p_TransparencyAmount=128;
                                    //m->p_FVF |= D3DFVF_DIFFUSE;
				                    break;
		      case MIXTYPE_ALPHABLEND: //m->p_FVF |= D3DFVF_DIFFUSE;
				                       break;
		    }
          else  // caso di blend alpha per la trasparenza
          if (m->p_TransparencyType==MIXTYPE_ALPHABLEND)
            switch (m->p_MapsMixType)
 		    {
		      case MIXTYPE_ADD: m->m_UseMe=*setTexEnv_mADD_tBLENDalpha;
				                //m->p_FVF |= D3DFVF_DIFFUSE;
				                break;
		      case MAPSMIXTYPE_TEXSUBENV: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MAPSMIXTYPE_ENVSUBTEX: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MIXTYPE_BLEND50: m->m_UseMe=*setTexEnv_mBLEND3DS_tBLENDalpha;
				                    //m->p_FVF |= D3DFVF_DIFFUSE;
									break;
		      case MIXTYPE_ALPHABLEND: //m->p_FVF |= D3DFVF_DIFFUSE;
				                       break;
		    }
          else  // caso di blend additivo per la trasparenza
          if (m->p_TransparencyType==MIXTYPE_ADD)
		    switch (m->p_MapsMixType)
 		    {
		      case MIXTYPE_ADD: m->m_UseMe=*setTexEnv_mADD_tADD;
				                //m->p_FVF |= D3DFVF_DIFFUSE;
								break;
		      case MAPSMIXTYPE_TEXSUBENV: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MAPSMIXTYPE_ENVSUBTEX: //m->p_FVF |= D3DFVF_DIFFUSE;
				                          break;
		      case MIXTYPE_BLEND50: m->m_UseMe=*setTexEnv_mBLEND3DS_tADD;
				                    //m->p_FVF |= D3DFVF_DIFFUSE;
									break;
		      case MIXTYPE_ALPHABLEND: //m->p_FVF |= D3DFVF_DIFFUSE;
				                       break;
		    }
          else  // caso di mappa ALPHA
          if (m->p_TransparencyType==ALPHA_MAP)
            switch (m->p_MapsMixType)
 		    {
		      case MIXTYPE_ADD: m->m_UseMe=*setTexEnv_mADD_tMAPalpha;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: return; break;
		      case MIXTYPE_BLEND50: m->m_UseMe=*setTexEnv_mBLEND3DS_tMAPalpha;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: return; break;		  		  
		    }
		}
        else  // caso di non trasparenza e no luci (solo tex+env)
          switch (m->p_MapsMixType)
		  {
		      case MIXTYPE_ADD: m->m_UseMe=*setTexEnv_mADD;
				                return; break;
		      case MAPSMIXTYPE_TEXSUBENV: m->m_UseMe=*setTexEnv_mTEXSUBENV;
				                          return; break;
		      case MAPSMIXTYPE_ENVSUBTEX: m->m_UseMe=*setTexEnv_mENVSUBTEX;
				                          return; break;
		      case MIXTYPE_BLEND50: m->m_UseMe=*setTexEnv_mBLEND3DS;
				                    return; break;
		      case MIXTYPE_ALPHABLEND: m->m_UseMe=*setTexEnv_mBLEND3DS;
				                       return; break;
		    }
       return;  // fine del caso tex + env no luci
	 }


     if ((m->p_Map1!=(CGraphicSurface *)NULL))
	 // texture di base, no luci, chekko i vari metodi
     {
		m->m_UseMe=*setTex;        

	    if (m->p_TransparencyType!=OFF)
		{
		  if (m->p_TransparencyType==MIXTYPE_BLEND50)
		  {
		     m->p_TransparencyAmount=127;
		     m->m_UseMe=*setTex_tBLENDalpha;
		  }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ADD)
		  {
		     m->m_UseMe=*setTex_tADD;
		  }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ALPHABLEND)
		      m->m_UseMe=*setTex_tBLENDalpha;
          else
		  if (m->p_TransparencyType==ALPHA_MAP)
		      m->m_UseMe=*setTex_tMAPalpha;
		}
	    return; // fine del caso solo texture no luci
	 }


     if ((m->p_Map2!=(CGraphicSurface *)NULL))
	 // envmap di base, no luci, chekko i vari metodi
     {
		//m->myraster=(*tria_raster_envmap);
        m->m_UseMe=*setEnv;
        m->p_FVF=D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

	    if (m->p_TransparencyType!=OFF)
		{
		  if (m->p_TransparencyType==MIXTYPE_BLEND50)
		  {
		    m->p_TransparencyAmount=127;
		    m->m_UseMe=*setEnv_tBLENDalpha;
		  }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ADD)
		  {
		    m->m_UseMe=*setEnv_tADD;
		  }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ALPHABLEND)
		      m->m_UseMe=*setEnv_tBLENDalpha;
		  else
		  if (m->p_TransparencyType==ALPHA_MAP)
		      m->m_UseMe=*setEnv_tMAPalpha;
		}
	 return; // fine del caso envmap no luci
	 }
   }


// ########################################################
// ###############  caso di una o piu' luci ###############
// ########################################################
   else
   {
	 if ((m->p_Map1==(CGraphicSurface *)NULL) &&
         (m->p_Map2==(CGraphicSurface *)NULL))
	 {
		m->m_UseMe=*setRGB;

	    if (m->p_TransparencyType!=OFF)
		{
		  switch (m->p_TransparencyType)
		  {
		    case MIXTYPE_BLEND50: break;
			case MIXTYPE_ALPHABLEND: break;
			case ALPHA_MAP: break;
			case MIXTYPE_ADD: 
		                      m->m_UseMe=*setRGB_tADD;
							  break;
		  }
		}
		else if (m->p_LightMixType==MIXTYPE_BLEND3DS)
			 m->m_UseMe=*setRGB_lBLEND3DS;
		return;
	 }


	 if ((m->p_Map1!=(CGraphicSurface *)NULL) &&
         (m->p_Map2!=(CGraphicSurface *)NULL))
	 // texture+env di base, si luci, chekko i vari metodi
	 {
        m->m_UseMe=*setTexEnv_mBLEND3DS;

	    switch (m->p_MapsMixType)
		{
          case MIXTYPE_ADD :
		                     switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setTexEnv_mADD_lADD;
						                          break;
                               case MIXTYPE_SUB : m->m_UseMe=*setTexEnv_mADD_lSUB;
						                               break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setTexEnv_mADD_lBLEND3DS;
								                      break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setTexEnv_mADD_lBLEND3DS;
						                                    break;
							 }
                             return;
							 break;

          case MAPSMIXTYPE_TEXSUBENV :
		                     switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : break;
                               case MIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 : break;
                               case MIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;

          case MAPSMIXTYPE_ENVSUBTEX :
		                     switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : break;
                               case MIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 :  break;
                               case MIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;

          case MIXTYPE_BLEND50 :
		                     switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setTexEnv_mBLEND3DS_lADD; break;
                               case MIXTYPE_SUB : m->m_UseMe=*setTexEnv_mBLEND3DS_lSUB; break;
                               case MIXTYPE_BLEND50 :  m->m_UseMe=*setTexEnv_mBLEND3DS_lBLEND3DS; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setTexEnv_mBLEND3DS_lBLEND3DS; break;
							 }
                             return;
							 break;

          case MIXTYPE_ALPHABLEND :
		                     switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : break;
                               case MIXTYPE_SUB : break;
                               case MIXTYPE_BLEND50 : break;
                               case MIXTYPE_BLEND3DS : break;
							 }
                             return;
							 break;
		}
		return; // fine caso tex+env+luci
     }


     if ((m->p_Map1!=(CGraphicSurface *)NULL))
	 // texture di base, si luci, chekko i vari metodi
     {
		//m->myraster=(*tria_raster_texRGB);
        m->m_UseMe=*setTex_lBLEND3DS;

		if (m->p_TransparencyType!=OFF)
		{
		  if (m->p_TransparencyType==MIXTYPE_BLEND50)
          // caso di trasparenza con blend di tipo BLEND50
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : 
                                                  m->p_TransparencyAmount=128;
												  m->m_UseMe=*setTex_lADD_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_SUB : m->p_TransparencyAmount=128;
												  m->m_UseMe=*setTex_lSUB_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_BLEND50 : m->p_TransparencyAmount=128;
													  m->m_UseMe=*setTex_lBLEND3DS_tBLENDalpha;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->p_TransparencyAmount=128;
													   m->m_UseMe=*setTex_lBLEND3DS_tBLENDalpha;
								                       return; break;
							 }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ADD)
          // caso di trasparenza con blend di tipo ADD
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setTex_lADD_tADD;
								                  return; break;
                               case MIXTYPE_SUB : m->m_UseMe=*setTex_lSUB_tADD;
								                  return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setTex_lBLEND3DS_tADD;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setTex_lBLEND3DS_tADD;
								                       return; break;
							 }
		  else
		  if (m->p_TransparencyType==ALPHA_MAP)
		  			  // caso di trasparenza con blend con mappa alpha
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setTex_lADD_tMAPalpha;
								                  return; break;
                               case MIXTYPE_SUB : m->m_UseMe=*setTex_lSUB_tMAPalpha;
								                       return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setTex_lBLEND3DS_tMAPalpha;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setTex_lBLEND3DS_tMAPalpha;
								                            return; break;
							 }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ALPHABLEND)
			  // caso di trasparenza con blend di tipo alpha
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setTex_lADD_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_SUB : m->m_UseMe=*setTex_lSUB_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setTex_lBLEND3DS_tBLENDalpha;
							                          return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setTex_lBLEND3DS_tBLENDalpha;
								                       return; break;
							 }
          return;  // fine caso tex+luci+trasparenza
		}
        else  // caso tex+luci no trasparenza
		{
   		  switch (m->p_LightMixType)
		  {
			case MIXTYPE_ADD : m->m_UseMe=*setTex_lADD;
			                   return; break;
            case MIXTYPE_SUB : m->m_UseMe=*setTex_lSUB;
				                    return; break;
            case MIXTYPE_BLEND50 : m->m_UseMe=*setTex_lBLEND3DS;
								   return; break;
            case MIXTYPE_BLEND3DS : m->m_UseMe=*setTex_lBLEND3DS;
								         return; break;
		  }
		}
	 }


     if ((m->p_Map2!=(CGraphicSurface *)NULL))
	 // envmap di base, si luci, chekko i vari metodi
     {
        m->m_UseMe=*setEnv_lBLEND3DS;

		if (m->p_TransparencyType != OFF)
		{
		  if (m->p_TransparencyType==MIXTYPE_BLEND50)
          // caso di trasparenza con blend di tipo BLEND50
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->p_TransparencyAmount=128;
								                  m->m_UseMe=*setEnv_lADD_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_SUB : m->p_TransparencyAmount=128;
								                  m->m_UseMe=*setEnv_lSUB_tBLENDalpha;
												  return; break;
                               case MIXTYPE_BLEND50 : m->p_TransparencyAmount=128;
								                      m->m_UseMe=*setEnv_lBLEND3DS_tBLENDalpha;
													  return; break;
                               case MIXTYPE_BLEND3DS : m->p_TransparencyAmount=128;
								                       m->m_UseMe=*setEnv_lBLEND3DS_tBLENDalpha;
													   return; break;
							 }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ADD)
          // caso di trasparenza con blend di tipo ADD
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setEnv_lADD_tADD;
								                  return; break;
                               case MIXTYPE_SUB : m->m_UseMe=*setEnv_lSUB_tADD;
								                  return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setEnv_lBLEND3DS_tADD;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setEnv_lBLEND3DS_tADD;
								                       return; break;
							 }
          else
		  if (m->p_TransparencyType==ALPHA_MAP)
	      // caso di trasparenza con blend con mappa alpha
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setEnv_lADD_tMAPalpha;
								                  return; break;
                               case MIXTYPE_SUB :m->m_UseMe=*setEnv_lSUB_tMAPalpha;
								                      return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setEnv_lBLEND3DS_tMAPalpha;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setEnv_lBLEND3DS_tMAPalpha;
								                            return; break;
							 }
		  else
		  if (m->p_TransparencyType==MIXTYPE_ALPHABLEND)
	      // caso di trasparenza con blend di tipo alpha
                             switch (m->p_LightMixType)
							 {
							   case MIXTYPE_ADD : m->m_UseMe=*setEnv_lADD_tBLENDalpha;
								                  return; break;
                               case MIXTYPE_SUB :m->m_UseMe=*setEnv_lSUB_tBLENDalpha;
								                 return; break;
                               case MIXTYPE_BLEND50 : m->m_UseMe=*setEnv_lBLEND3DS_tBLENDalpha;
								                      return; break;
                               case MIXTYPE_BLEND3DS : m->m_UseMe=*setEnv_lBLEND3DS_tBLENDalpha;
								                       return; break;
							 }
          return; // fine caso env + luci + trasparenza
		}
        else  // caso env + luci no trasparenza
   		  switch (m->p_LightMixType)
		  {
			case MIXTYPE_ADD : m->m_UseMe=*setEnv_lADD;
			                   return; break;
            case MIXTYPE_SUB : m->m_UseMe=*setEnv_lSUB;
						       return; break;
            case MIXTYPE_BLEND50 : m->m_UseMe=*setEnv_lBLEND3DS;
								   return; break;
            case MIXTYPE_BLEND3DS : m->m_UseMe=*setEnv_lBLEND3DS;
							        return; break;
		  }
	 }
   }*/
}

//--------------------------------------------------------------------

int32 CRenderLib::m_AddUpdate_Material (CMaterial *m)
{
  int32 i, xy;
  char8 msg[200];

  if (m==NULL) m_Error("AddUpdate_Material: material NULL(maybe not allocated)!");
  if (m->p_Name != (char8 *)NULL) strcpy(msg, m->p_Name);
  strcpy(msg, ": ");

  // la mappa bump è caricata da file
  if (m->p_BumpMap)
  {
     // le mappe bump devono essere delle 8bit a scale di grigio
	 if (m->p_BumpMap->p_Type != GREYSCALE)
	 {
       strcat(msg, "Bumpmap MUST be an 8-bit grayscale texture !!!");
       m_Error(msg);
	 }
	 if (!m->p_Map1)
	 {
        strcat(msg, "DOT3 and EMBM needs at least a base Map1 !!!");
        m_Error(msg);
	 }
  }
  else
  {
	// creiamo una texture per la mappa bump
    if (m->p_BumpType & AUTOMATIC_BUMPMAP)
	{
	   if (!m->p_Map1)
	   {
          strcat(msg, "DOT3 and EMBM needs at least a base Map1 !!!");
          m_Error(msg);
	   }
	   // allochiamo una 8 bit avente stessa risoluzione della
	   // mappa1 di base
	   m->p_BumpMap=new CGraphicSurface(m->p_Map1->p_Width,
                                        m->p_Map1->p_Height,
										8);
	   // creiamo la mappa delle altezze dalla texture base in Map1
	   BuildGreyMap(m->p_Map1, m->p_BumpMap);
	}
  }

  // a questo punto siamo sicuri che se si voleva una mappa bump
  // essa è gia' stata caricata o generata in automatico con la
  // luminosita' dei pixel di p_Map1; inoltre siamo sicuri che esiste
  // una mappa base in p_Map1.

  // l'environmental map bump mapping necessita di un'envmap
  if (m->p_BumpMap)
  {
	 if (m->p_Map1Channel==ENVMAPPING)
	 {
       strcat(msg, "Environmental Bump and DOT3 need map1channel not be envmapping !!!");
       m_Error(msg);
	 }
	 if ((!m->p_Map2) && (m->p_BumpType & BUMP_EMBM))
	 {
       strcat(msg, "Environmental Bump needs an envmap(Map2) texture!!");
       m_Error(msg);
	 }
  }

  // Ora il bump è aposto. Controllo le mappe alpha !!!!!!!!!!!!!!
  // Le mappe alpha devono essere delle 8bit a scale di grigio
  if ((m->p_AlphaMap) && (m->p_AlphaMap->p_Type != GREYSCALE))
  {
     strcat(msg, "Alpha map MUST be an 8-bit grayscale texture");
     m_Error(msg);
  }

  // la mappa alpha e la texture devono avere stesse dimensioni
  // TEX + MAPPA ALPHA
  if ((m->p_AlphaMap) && (m->p_Map1))
  {
     if ((m->p_AlphaMap->p_Width != m->p_Map1->p_Width) ||
         (m->p_AlphaMap->p_Height != m->p_Map1->p_Height))
	 {
        strcat(msg, "Map1 and Alpha map MUST have same dimensions (width and height)!");
        m_Error(msg);
	 }
	 else
	 {
        // mette nel canale alpha della texture il valore del
        // pixel corrispondente della mappa alpha
		xy=m->p_AlphaMap->p_Width*m->p_AlphaMap->p_Height;
		for (i=0; i<xy; i++)
		{
           uchar8 *pix8=(uchar8 *)m->p_AlphaMap->p_Pixels;
           uchar8 p=pix8[i];
           uint32 *pix32=(uint32 *)m->p_Map1->p_Pixels;
           pix32[i] = (pix32[i] & 0x00FFFFFF) | (((int32)p) << 24);
		}
	 }
     m_AddUpdate_AlphaTexture(m->p_Map1, m->p_Map1->p_MipMapLevels);
  }

  if ((!m->p_AlphaMap) && (m->p_Map1))
	  m_AddUpdate_Texture(m->p_Map1, m->p_Map1->p_MipMapLevels);

  if ((!m->p_AlphaMap) && (m->p_Map2))
	  m_AddUpdate_Texture(m->p_Map2, m->p_Map2->p_MipMapLevels);
  else
  if ((m->p_AlphaMap) && (m->p_Map1) && (m->p_Map2))
	  m_AddUpdate_Texture(m->p_Map2, m->p_Map2->p_MipMapLevels);

  // aggiungo la mappa di bump
  if (m->p_BumpMap)
  {
	  if ((m->p_BumpType & BUMP_EMBM) && (p_EMBMSupported))
	      m_AddUpdate_BumpTexture(m->p_BumpMap, BUMP_EMBM);
	  else
	  if ((m->p_BumpType & BUMP_DOT3) && (p_DOT3Supported))
	      m_AddUpdate_BumpTexture(m->p_BumpMap, BUMP_DOT3);
  }

  m_SetMaterialProcedures(m);

  // setto il materiale D3D
  m->p_D3DMaterial.Ambient.r=0.0f;
  m->p_D3DMaterial.Ambient.g=0.0f;
  m->p_D3DMaterial.Ambient.b=0.0f;
  m->p_D3DMaterial.Ambient.a=1.0f;
  m->p_D3DMaterial.Diffuse.r=1.0f;
  m->p_D3DMaterial.Diffuse.g=1.0f;
  m->p_D3DMaterial.Diffuse.b=1.0f;
  m->p_D3DMaterial.Diffuse.a=1.0f;
  m->p_D3DMaterial.Specular.r=1.0f;
  m->p_D3DMaterial.Specular.g=1.0f;
  m->p_D3DMaterial.Specular.b=1.0f;
  m->p_D3DMaterial.Specular.a=1.0f;
  m->p_D3DMaterial.Power=50.0f;
  m->p_D3DMaterial.Emissive.r=0.0f;
  m->p_D3DMaterial.Emissive.g=0.0f;
  m->p_D3DMaterial.Emissive.b=0.0f;
  m->p_D3DMaterial.Emissive.a=0.0f;
  if ((m->p_TransparencyType == MIXTYPE_ADD) ||
      (m->p_TransparencyType == MIXTYPE_ALPHABLEND))
  {
     m->p_D3DMaterial.Diffuse.a=(float4)(m->p_TransparencyAmount/255.0);
  }
  return(1);
}

//--------------------------------------------------------------------

