#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_animatable (Animatable *anim, int blanks)
{
   int w, k;

   if (!anim) return;

   for (k=0; k<blanks; k++) fprintf (fTXT, " ");
   fprintf (fTXT, "SubAnim = %d\n", anim->NumSubs());
   if (anim->NumKeys() > 0)
   {
      for (k=0; k<blanks; k++) fprintf (fTXT, " ");
      fprintf (fTXT, "NumKeys = %d\n", anim->NumKeys());
   }
   for (w=0; w<anim->NumSubs(); w++)
   {
      for (k=0; k<blanks; k++) fprintf (fTXT, " ");
	  fprintf(fTXT, "Anim%d: %s\n", w, anim->SubAnimName(w));
	  if ( (strcmp(anim->SubAnimName(w), "U Offset")==0) ||
           (strcmp(anim->SubAnimName(w), "V Offset")==0) ||
           (strcmp(anim->SubAnimName(w), "U Tiling")==0) ||
           (strcmp(anim->SubAnimName(w), "V Tiling")==0) ||
           (strcmp(anim->SubAnimName(w), "U Angle")==0)  ||
           (strcmp(anim->SubAnimName(w), "V Angle")==0))
	  {
         Control *c=GetControlInterface(anim->SubAnim(w));
         fprintf(fTXT, "Control pointer: %p\n", c);
	  }

      export_animatable(anim->SubAnim(w), blanks+3);
   }
}

//--------------------------------------------------------------------

BOOL SkeletonExporter::material_has_transparency_track(Mtl* mtl)
{
  Control *c;
  Animatable *track;

  if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    track=mtl->SubAnim(2)->SubAnim(0);
    c=GetControlInterface(track);
    if ((c) && (c->NumKeys()>0)) return(TRUE);
  }
  return(FALSE);
}

//--------------------------------------------------------------------

void SkeletonExporter::dump_material_tracks(Mtl* mtl)
{
  Control *c;
  int size_key;
  Animatable *track;

  if (!mtl) return;
  export_animatable(mtl, 1);

  if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
  {
    // opacity track
	track=mtl->SubAnim(2)->SubAnim(0);
    c=GetControlInterface(track);
    if ((c) && (c->NumKeys()>0))
	{
       if (IsTCBControl(c)) size_key=28;
	   else
	   if (IsBezierControl(c)) size_key=16;
	   else size_key=8;
	   fprintf(fTXT, "Material %s:  opacity track present.\n", mtl->GetName());
       write_chunk_header(fA3D, MATERIAL_OPACITY_TRACK_ID,
	                      mtl->GetName(), 1+2+4+c->NumKeys()*size_key);
	   export_float_track(c, 1.0f, fA3D);
	}
  }
  fprintf(fTXT, "\n\n");
}