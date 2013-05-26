#include "RenderLib.h"

extern FILE *fdebug;

CMaterial::CMaterial()
{
  int32 i;

  strcpy(p_Name, "");
  strcpy(p_ChildrenName, "");
  p_TransparencyAmount=128;
  p_TransparencyType=OFF;
  p_MapsMixType=MIXTYPE_MODULATE;
  p_LightMixType=MIXTYPE_MODULATE;
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
  p_FogType=OFF;
  p_FogStart=0;
  p_FogEnd=1;
  p_FogDensity=0.5f;
  p_FogColor=0xFF8090FF;
  p_AlphaMap=(CGraphicSurface *)NULL;
  p_NextMaterial=(CMaterial *)NULL;
  p_NumPasses=0;
  p_U1Offset=p_U2Offset=0;
  p_V1Offset=p_V2Offset=0;
  p_W1Angle=p_W2Angle=0;
  p_U1Tile=p_U2Tile=1;
  p_V1Tile=p_V2Tile=1;
  p_U1OffsetTrack=p_U2OffsetTrack=(CRollTrack *)NULL;
  p_V1OffsetTrack=p_V2OffsetTrack=(CRollTrack *)NULL;
  p_W1AngleTrack=p_W2AngleTrack=(CRollTrack *)NULL;
  p_U1TileTrack=p_U2TileTrack=(CRollTrack *)NULL;
  p_V1TileTrack=p_V2TileTrack=(CRollTrack *)NULL;
  
  mat_identity(&p_Map1Matrix);
  for (i=0; i<3; i++) p_Map1Matrix.a[1][i]=-p_Map1Matrix.a[1][i];
  mat_identity(&p_Map2Matrix);
  for (i=0; i<3; i++) p_Map2Matrix.a[1][i]=-p_Map2Matrix.a[1][i];

  p_TransparencyTrack=(CRollTrack *)NULL;
  p_Specular=OFF;
  p_WireFrame=OFF;
  p_WireFrameColor=0x00FFFFFF;
  p_ToonGradient=(CGraphicSurface *)NULL;
  p_ToonScale=1.015f;
  p_ToonLightsMixType=MIXTYPE_ADD;
  p_ToonMapsMixType=MIXTYPE_MODULATE;
}

//--------------------------------------------------------------------

int32 CMaterial::m_Init(void *arg1)
{
  if (p_U1OffsetTrack) if (!p_U1OffsetTrack->m_Init(1.0f)) return(0);
  if (p_U2OffsetTrack) if (!p_U2OffsetTrack->m_Init(1.0f)) return(0);
  if (p_V1OffsetTrack) if (!p_V1OffsetTrack->m_Init(1.0f)) return(0);
  if (p_V2OffsetTrack) if (!p_V2OffsetTrack->m_Init(1.0f)) return(0);
  if (p_W1AngleTrack) if (!p_W1AngleTrack->m_Init(1.0f)) return(0);
  if (p_W2AngleTrack) if (!p_W2AngleTrack->m_Init(1.0f)) return(0);
  if (p_U1TileTrack) if (!p_U1TileTrack->m_Init(1.0f)) return(0);
  if (p_U2TileTrack) if (!p_U2TileTrack->m_Init(1.0f)) return(0);
  if (p_V1TileTrack) if (!p_V1TileTrack->m_Init(1.0f)) return(0);
  if (p_V2TileTrack) if (!p_V2TileTrack->m_Init(1.0f)) return(0);
  if (p_TransparencyTrack) if (!p_TransparencyTrack->m_Init(1.0f)) return(0);
  if (p_FogStart<0) p_FogStart=0;
  if (p_FogDensity<0) p_FogDensity=0;
  else
  if (p_FogDensity>1) p_FogDensity=1;
  if (p_ToonScale <= 1) p_ToonScale=1.015f;
  p_NumPasses=1;
  if (p_NextMaterial) p_NumPasses++;
  m_BuildVertexFormat();
  return(1);
}

//--------------------------------------------------------------------

void CMaterial::m_Update(float4 frame)
{
  AD_Matrix l_rot, l_trans, l_scale, aux, prePivotMatrix, postPivotMatrix;
  AD_Vect3D l_offs, l_prePivot, l_postPivot;
  int32 i;

  if (p_U1OffsetTrack) p_U1OffsetTrack->m_GetData(frame, &p_U1Offset);
  if (p_U2OffsetTrack) p_U2OffsetTrack->m_GetData(frame, &p_U2Offset);
  if (p_V1OffsetTrack) p_V1OffsetTrack->m_GetData(frame, &p_V1Offset);
  if (p_V2OffsetTrack) p_V2OffsetTrack->m_GetData(frame, &p_V2Offset);
  if (p_W1AngleTrack) p_W1AngleTrack->m_GetData(frame, &p_W1Angle);
  if (p_W2AngleTrack) p_W2AngleTrack->m_GetData(frame, &p_W2Angle);
  if (p_U1TileTrack) p_U1TileTrack->m_GetData(frame, &p_U1Tile);
  if (p_U2TileTrack) p_U2TileTrack->m_GetData(frame, &p_U2Tile);
  if (p_V1TileTrack) p_V1TileTrack->m_GetData(frame, &p_V1Tile);
  if (p_V2TileTrack) p_V2TileTrack->m_GetData(frame, &p_V2Tile);

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
  //aux.a[2][0]=aux.a[2][1]=aux.a[2][3]==0; aux.a[2][1]=1;
  for (i=0; i<3; i++) aux.a[1][i]=-aux.a[1][i];
  mat_transpose(&aux, &p_Map1Matrix);

  // costruiamo la matrice per la mappa2
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
  //aux.a[2][0]=aux.a[2][1]=aux.a[2][3]==0; aux.a[2][1]=1;
  for (i=0; i<3; i++) aux.a[1][i]=-aux.a[1][i];
  mat_transpose(&aux, &p_Map2Matrix);

  if (p_TransparencyTrack)
  {
     p_TransparencyTrack->m_GetData(frame, &p_TransparencyAmount);
     if (p_TransparencyAmount<0) p_TransparencyAmount=0;
	 else
     if (p_TransparencyAmount>1.0f) p_TransparencyAmount=1.0f;
     if ((p_TransparencyType == MIXTYPE_ADD) ||
         (p_TransparencyType == MIXTYPE_ALPHABLEND))
        p_D3DMaterial.Diffuse.a=p_TransparencyAmount;
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
		  (p_U1TileTrack)   || (p_U1TileTrack)) return(1);
	  else
	  if ((p_U1Offset!=0) || (p_V1Offset!=0) ||
		  (p_W1Angle!=0)                     ||
		  (p_U1Tile!=1)   || (p_V1Tile!=1)) return(1);
	  else return(0);
   }
   else
   {
      if (!p_Map2) return(0);
	  if ((p_U2OffsetTrack) || (p_V2OffsetTrack) ||
		  (p_W2AngleTrack)                       ||
		  (p_U2TileTrack)   || (p_U2TileTrack)) return(1);
	  else
	  if ((p_U2Offset!=0) || (p_V2Offset!=0) ||
		  (p_W2Angle!=0)                     ||
		  (p_U2Tile!=1)   || (p_V2Tile!=1)) return(1);
	  else return(0);
   }
}

//--------------------------------------------------------------------

void CMaterial::m_BuildVertexFormat(void)
{
   int32 TWO_UV=TEXMAPPING1 | TEXMAPPING2;
   int32 ch;
   
   p_NumPasses=1;

   // ------------------    costruzione del FVF   --------------------
   p_InputVertexFormat.FVF=D3DFVF_XYZ | D3DFVF_NORMAL;
   p_InputVertexFormat.vertexSize=6*sizeof(float4);
   p_InputVertexFormat.enumType=FVF_ONLY_GEOMETRY;

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

   // two maps with 2 distinct mapping channels
   if ((ch & TWO_UV)==TWO_UV)
   {
      p_InputVertexFormat.FVF |= D3DFVF_TEX2;
      p_InputVertexFormat.vertexSize+=4*sizeof(float4);
	  p_InputVertexFormat.enumType=FVF_GEOMETRY_UV2;
	  if (p_BumpType & BUMP_DOT3)
	  {
         p_InputVertexFormat.SecondaryFVF=p_InputVertexFormat.FVF;
		 p_InputVertexFormat.SecondaryVertexSize=p_InputVertexFormat.vertexSize;
         p_InputVertexFormat.SecondaryEnumType=p_InputVertexFormat.enumType;
		 // CreateVertexBuffer needs a 0 FVF if the vertex is
		 // not a standard FVF format
		 p_InputVertexFormat.FVF=0;
		 // add the bump space tangent T
		 p_InputVertexFormat.vertexSize+=3*sizeof(float4);
         p_InputVertexFormat.enumType=VS_GEOMETRY_UV2_DOT3;
	  }
   }
   else
   if (ch & TEXMAPPING1)
   {
      p_InputVertexFormat.FVF |= D3DFVF_TEX1;
      p_InputVertexFormat.vertexSize+=2*sizeof(float4);
      p_InputVertexFormat.enumType=FVF_GEOMETRY_UV1;
	  if (p_BumpType & BUMP_DOT3)
	  {
         p_InputVertexFormat.SecondaryFVF=p_InputVertexFormat.FVF;
		 p_InputVertexFormat.SecondaryVertexSize=p_InputVertexFormat.vertexSize;
         p_InputVertexFormat.SecondaryEnumType=p_InputVertexFormat.enumType;
         // CreateVertexBuffer needs a 0 FVF if the vertex is
		 // not a standard FVF format
		 p_InputVertexFormat.FVF=0;
		 // add the bump space tagent T
		 p_InputVertexFormat.vertexSize+=3*sizeof(float4);
         p_InputVertexFormat.enumType=VS_GEOMETRY_UV1_DOT3;
	  }
	  else
      if (p_LightMixType==MIXTYPE_TOONSHADE)
	  {
		 p_InputVertexFormat.FVF=0;
	  }
   }
   else
   {
     // solo toon shading senza mappe 
	 if (p_LightMixType==MIXTYPE_TOONSHADE)
	 {
	   p_InputVertexFormat.FVF=0;
       p_InputVertexFormat.enumType=FVF_ONLY_GEOMETRY;
	 }
   }
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap0(CMaterial *m)
{
   HRESULT hr;

   hr=p_Device->SetTexture(0, NULL);
   
   hr=p_Device->SetRenderState(D3DRS_TEXTUREFACTOR, m->p_WireFrameColor);
   hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
   hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
   hr=p_Device->SetTexture(1, NULL);
   hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap1(CMaterial *m)
{
  HRESULT hr;

  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurfaces[m->p_Map1->p_CurrentTextureIndex]);
  hr=p_Device->SetTexture(1, NULL);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

  //if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);
  } //else hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

  switch (m->p_Map1Channel)
  {
	 case ENVMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
	    break;
	 case SPECULARMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
	    break;
	 case TEXMAPPING1:
     default:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		break;
  }
}

//--------------------------------------------------------------------

void CRenderLib::m_SetMap12(CMaterial *m)
{
  HRESULT hr;

  // texture stage 0: texture, trasformazione UV
  hr=p_Device->SetTexture(0, m->p_Map1->p_HWSurfaces[m->p_Map1->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
  //if (m->m_MapNeedUVTransform(1))
  {
    hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
    hr=p_Device->SetTransform(D3DTS_TEXTURE0, (D3DMATRIX *)&m->p_Map1Matrix);
  } //else hr=p_Device->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);

  // texture stage 1: texture, UV transformation
  hr=p_Device->SetTexture(1, m->p_Map2->p_HWSurfaces[m->p_Map2->p_CurrentTextureIndex]);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG1, D3DTA_CURRENT);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLORARG2, D3DTA_TEXTURE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSU, D3DTADDRESS_WRAP);
  hr=p_Device->SetTextureStageState(1, D3DTSS_ADDRESSV, D3DTADDRESS_WRAP);
  //if (m->m_MapNeedUVTransform(2))
  {
     hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
     hr=p_Device->SetTransform(D3DTS_TEXTURE1, (D3DMATRIX *)&m->p_Map2Matrix);
  }//else hr=p_Device->SetTextureStageState(1, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
  hr=p_Device->SetTextureStageState(1, D3DTSS_COLOROP, m_MixType2ColorOp(m->p_MapsMixType));

  switch (m->p_Map1Channel)
  {
	 case ENVMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
	    break;

	 case SPECULARMAPPING:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
	    break;

	 case TEXMAPPING1:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		break;

	 case TEXMAPPING2:
        hr=p_Device->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 1);
		break;
  }

  switch (m->p_Map2Channel)
  {
	 case ENVMAPPING:
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACENORMAL | 0);
	    break;

	 case SPECULARMAPPING:
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX,
			                              D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR | 0);
	    break;

	 case TEXMAPPING1:
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 0);
		break;

	 case TEXMAPPING2:
        hr=p_Device->SetTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);
		break;
  }

  hr=p_Device->SetTextureStageState(2, D3DTSS_COLOROP, D3DTOP_DISABLE);
}

//--------------------------------------------------------------------

void CRenderLib::m_FixMaterialFVF(CMaterial *m)
{
  if ((m->p_BumpType & BUMP_DOT3) && (!p_DOT3Supported))
  {
	 m->p_InputVertexFormat.FVF=m->p_InputVertexFormat.SecondaryFVF;
	 m->p_InputVertexFormat.vertexSize=m->p_InputVertexFormat.SecondaryVertexSize;
     m->p_InputVertexFormat.enumType=m->p_InputVertexFormat.SecondaryEnumType;
  }
}

//--------------------------------------------------------------------

int32 CRenderLib::m_AddUpdate_Material (CMaterial *m)
{
  int32 i, xy, j;
  char8 msg[200];

  strcpy(msg, "");
  if (m==NULL) m_Error("AddUpdate_Material: material NULL(maybe not allocated)!");
  if (m->p_Name != (char8 *)NULL) strcpy(msg, m->p_Name);
  strcat(msg, ": ");

  // bumpmap is loaded from file
  if (m->p_BumpMap)
  {
     // loaded bumpmap MUST be 8bit greyscale bitmap
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
	// create a bumpmap
    if (m->p_BumpType & AUTOMATIC_BUMPMAP)
	{
	   if (!m->p_Map1)
	   {
          strcat(msg, "DOT3 and EMBM needs at least a base Map1 !!!");
          m_Error(msg);
	   }
	   // allocate an 8bit bitmap with the same Map1 dimensions 
	   m->p_BumpMap=new CGraphicSurface(m->p_Map1->p_Width,
                                        m->p_Map1->p_Height,
										8);
       m->p_BumpMap->p_HWSurfaces=new LPDIRECT3DTEXTURE8;
	   m->p_BumpMap->p_HWSurfaces[0]=(IDirect3DTexture8 *)NULL;
	   // build heightmap from Map1
	   m_BuildGreyMap(m->p_Map1, m->p_BumpMap);
	}
  }

  // now we're sure that if we wanted a bumpmap then it has been
  // loaded from file or generated from Map1.
  // And we're sure that p_Map1 exists!

  if (m->p_BumpMap)
  {
     // DOT3 and EMBM wants explicit uv coordinates in the Map1
     // channel
	 if (m->p_Map1Channel==ENVMAPPING)
	 {
       strcat(msg, "Environmental Bump and DOT3 need map1channel not be envmapping !!!");
       m_Error(msg);
	 }
     // environmental map bump mapping needs an envmap
	 if ((!m->p_Map2) && (m->p_BumpType & BUMP_EMBM))
	 {
       strcat(msg, "Environmental Bump needs a Map2 texture!!");
       m_Error(msg);
	 }
  }

   // a material can't have a Map2 without having also Map1 !!!
   if ((m->p_Map2) && (!m->p_Map1))
   {
      strcat(msg, "Map2 is NOT NULL, and Map1 IS NULL!");
      return(0);
   }

  // Now bump is OK! Let's check alpha map !!!
  if ((m->p_AlphaMap) && (m->p_AlphaMap->p_Type != GREYSCALE))
  {
     strcat(msg, "Alpha map MUST be an 8-bit grayscale texture");
     m_Error(msg);
  }

  if ((m->p_LightMixType==MIXTYPE_TOONSHADE) &&
	  (m->p_Map1) && (m->p_Map1Channel==ENVMAPPING))
  {
    strcat(msg, "Toonshade doesn't work if map1channel is set for envmapping!");
    m_Error(msg);
  }

  // ora che tutti i parametri hanno combinazioni valide vediamo
  // di fixare il FVF inbase ai supporti effettivi che ha la scheda
  // grafica
  m_FixMaterialFVF(m);

  // alpha map must have same dimension of first map
  // TEX + MAPPA ALPHA
  if ((m->p_AlphaMap) && (m->p_Map1) && (m->p_TransparencyType==ALPHA_MAP))
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
        uchar8 *pix8=(uchar8 *)m->p_AlphaMap->p_Pixels;
		for (j=0; j<m->p_Map1->p_NumTextures; j++)
		{
          uint32 *pixAux=(uint32 *)m->p_Map1->p_Pixels;
		  uint32 *pix32=(uint32 *)&pixAux[j*xy];
		  for (i=0; i<xy; i++)
            pix32[i] = (pix32[i] & 0x00FFFFFF) | (((int32)pix8[i]) << 24);
		}
	 }
     m_AddUpdate_AlphaTexture(m->p_Map1, m->p_Map1->p_MipMapLevels);
  }

  if ((!m->p_AlphaMap) && (m->p_Map1) && (m->p_TransparencyType!=ALPHA_MAP1))
	  m_AddUpdate_Texture(m->p_Map1, m->p_Map1->p_MipMapLevels);
  else
  if ((!m->p_AlphaMap) && (m->p_Map1) && (m->p_TransparencyType==ALPHA_MAP1))
	  m_AddUpdate_AlphaTexture(m->p_Map1, m->p_Map1->p_MipMapLevels);

  if (m->p_Map2)
	  m_AddUpdate_Texture(m->p_Map2, m->p_Map2->p_MipMapLevels);

  // aggiungo la mappa di bump
  if (m->p_BumpMap)
  {
	  if ((m->p_BumpType & BUMP_EMBM) && (p_EMBMSupported))
	      //m_AddUpdate_BumpTexture(m->p_BumpMap, BUMP_EMBM, m->p_Map1->p_MipMapLevels);
          m_AddUpdate_BumpTexture(m->p_BumpMap, BUMP_EMBM, 1);
	  else
	  if ((m->p_BumpType & BUMP_DOT3) && (p_DOT3Supported))
	      m_AddUpdate_BumpTexture(m->p_BumpMap, BUMP_DOT3, m->p_Map1->p_MipMapLevels);
  }

  if (!m->p_ToonGradient)
     m->p_ToonGradient=p_TOONGradient;
  else m_AddUpdate_Texture(m->p_ToonGradient, 1);

  // sets D3D8 material
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
     m->p_D3DMaterial.Diffuse.a=m->p_TransparencyAmount;
  return(1);
}

//--------------------------------------------------------------------

