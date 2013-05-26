#include "SceneExport.h"
#include "A3Dfile.h"

void SkeletonExporter::export_helper_object(INode *node)
{
  int type;
  Matrix3 mat;
  Point3 row;
  INode *padre=node->GetParentNode();
  ObjectState os = node->EvalWorldState(0);
  float width, height, length;

  // ------------------------  script ADD  ---------------------------
  //                   ed estrazioni delle dimensioni
  Mtl *materiale=node->GetMtl();
  if (os.obj->ClassID()==Class_ID(DUMMY_CLASS_ID, 0))
  {
	 fprintf(fTXT, "Dummy helper presente %s\n", node->GetName());
	 type=DUMMY_HELPER;
  }
  else
  if (os.obj->ClassID()==Class_ID(POINTHELP_CLASS_ID, 0))
  {
	 fprintf(fTXT, "Point helper presente %s\n", node->GetName());
	 type=POINT_HELPER;
  }
  else
  if (os.obj->ClassID()==BOXGIZMO_CLASSID)
  {
	 fprintf(fTXT, "BoxGizmo helper presente %s\n", node->GetName());
	 type=BOXGIZMO_HELPER;
     ((BoxGizmoObject *)(os.obj))->pblock->GetValue(PB_BOXGIZMO_WIDTH, 0, width, FOREVER);
	 ((BoxGizmoObject *)(os.obj))->pblock->GetValue(PB_BOXGIZMO_HEIGHT, 0, height, FOREVER);
	 ((BoxGizmoObject *)(os.obj))->pblock->GetValue(PB_BOXGIZMO_LENGTH, 0, length, FOREVER);
  }
  else
  if (os.obj->ClassID()==SPHEREGIZMO_CLASSID)
  {
	 fprintf(fTXT, "SphereGizmo helper presente %s\n", node->GetName());
	 type=SPHEREGIZMO_HELPER;
     ((SphereGizmoObject *)(os.obj))->pblock->GetValue(PB_GIZMO_RADIUS, 0, width, FOREVER);
	 height=width;
	 length=width;
  }
  else type=UNKNOWN_GEOMETRIC_OBJECT;

  export_geometric_object(node, type, NULL);
  if (makeADO) fprintf(fADO, "  }\n\n");
  fprintf(fTXT, "\n\n\n----------------------------------------------------\n");
}