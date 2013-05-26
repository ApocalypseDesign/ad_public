#include "Utility.h"
#include "iSkin.h"
#include "Phyexp.h"
#include "bipexp.h"


bool IsNodeBone(INode *pNode)
{
	if(pNode == NULL || pNode->IsRootNode())
		return false;

	ObjectState os = pNode->EvalWorldState(0);
	if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))
		return true;

	// we don't want biped end effectors
	if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
		return false;

	Control *cont = pNode->GetTMController();
	if(cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID ||
		cont->ClassID() == FOOTPRINT_CLASS_ID
	) return true;

	return false;
}

// ============================================================================

bool IsNodeBipedBone(INode *pNode)
{
	if(pNode == NULL || pNode->IsRootNode())
		return false;

	Control *cont = pNode->GetTMController();
	if(cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
		cont->ClassID() == BIPBODY_CONTROL_CLASS_ID
	) return true;

	return false;
}

// ============================================================================

Matrix3 GetBoneTM(INode *pNode, TimeValue t)
{
	Matrix3 tm(1);

	if(IsNodeBipedBone(pNode))
	{
//		SetBipedScale(pNode, TRUE);
		tm = pNode->GetNodeTM(t);
//		SetBipedScale(pNode, FALSE);
	}
	else
		tm = pNode->GetNodeTM(t);
//	tm.NoScale();
	return tm;
}


// ============================================================================

Modifier *FindPhysiqueModifier (INode* node)
{
  Object* ObjectPtr = node->GetObjectRef();
  if (!ObjectPtr) return NULL;

  while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
  {
    IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
    int ModStackIndex = 0;
    while (ModStackIndex < DerivedObjectPtr->NumModifiers())
	{
       Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
       if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
         return ModifierPtr;
       ModStackIndex++;
	}
    ObjectPtr = DerivedObjectPtr->GetObjRef();
  }
  return NULL;
}

// ============================================================================

Modifier *FindSkinModifier (INode *node)
{
  Object* ObjectPtr = node->GetObjectRef();
  if (!ObjectPtr) return NULL;

  while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID && ObjectPtr)
  {
    IDerivedObject *DerivedObjectPtr = (IDerivedObject *)(ObjectPtr);
    int ModStackIndex = 0;
    while (ModStackIndex < DerivedObjectPtr->NumModifiers())
	{
       Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
       if (ModifierPtr->ClassID() == SKIN_CLASSID)
         return ModifierPtr;
       ModStackIndex++;
	}
    ObjectPtr = DerivedObjectPtr->GetObjRef();
  }
  return NULL;
}

// ============================================================================

// Return a pointer to a TriObject given an INode or return NULL
// if the node cannot be converted to a TriObject
TriObject *GetTriObjectFromNode(INode *node, int &deleteIt)
{
  deleteIt = FALSE;
  Object *obj = node->EvalWorldState(0).obj;
  if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
  { 
    TriObject *tri = (TriObject *) obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
	// Note that the TriObject should only be deleted
	// if the pointer to it is not equal to the object
	// pointer that called ConvertToType()
	if (obj != tri) deleteIt = TRUE;
	return tri;
  }
  else return NULL;
}

// ============================================================================

// Return a pointer to a TriObject given an Object or return NULL
// if the object cannot be converted to a TriObject
TriObject *GetTriObjectFromObject(Object *obj, int &deleteIt)
{
  deleteIt = FALSE;

  if (!obj) return(NULL);
  if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
  { 
    TriObject *tri = (TriObject *) obj->ConvertToType(0, Class_ID(TRIOBJ_CLASS_ID, 0));
	// Note that the TriObject should only be deleted
	// if the pointer to it is not equal to the object
	// pointer that called ConvertToType()
	if (obj != tri) deleteIt = TRUE;
	return tri;
  }
  else return NULL;
}

// ============================================================================

void GetPivotOffset(INode* node, Point3 *p)
{
 // get the pivot offset and remove the rotational/scale component
    Matrix3 mat(TRUE);
    Quat qRot = node->GetObjOffsetRot();
    qRot.MakeMatrix(mat);
 // max scales the object if the pivot is scaled so skip.
 // m.SetScale( ((SceneEntry *)data)->node->GetObjOffsetScale().s );
    Point3 pivot = -node->GetObjOffsetPos();
    mat = Inverse(mat);
    Point3 pOff = VectorTransform(mat, pivot);
    p->x=pOff.x;
    p->y=pOff.y;
    p->z=pOff.z;
}

// ============================================================================

BOOL TMNegParity(Matrix3 &m)
{
  return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

// ============================================================================

Object *GetBaseObjectFromNode (INode *node)
{
  Object *ob;
  IDerivedObject *od;
  SClass_ID cid;
  
  ob=node->GetObjectRef();
  if (ob)
    cid=ob->SuperClassID();
  else return (NULL);
  while ((cid==GEN_DERIVOB_CLASS_ID) && (ob))
  {
	od=(IDerivedObject *)ob;
    ob=od->GetObjRef();
    cid=ob->SuperClassID();
  }
  return(ob);
}

// ============================================================================

void write_matrix (Matrix3 *m, FILE *f)
{
  Point3 row;

  row = m->GetRow(0);
  write_point3(&row, f);
  row = m->GetRow(1);
  write_point3(&row, f);
  row = m->GetRow(2);
  write_point3(&row, f);
  row = m->GetRow(3);
  write_point3(&row, f);
}

// ============================================================================

void write_point3 (Point3 *v, FILE *f)
{
  fwrite(&v->x, sizeof(float), 1, f);
  fwrite(&v->y, sizeof(float), 1, f);
  fwrite(&v->z, sizeof(float), 1, f);
}

// ============================================================================

void write_bbox (Box3 *bb, FILE *f)
{
  write_point3(&bb->pmin, f);
  write_point3(&bb->pmax, f);
}

// ============================================================================

BOOL IsTCBControl(Control *cont)
{
  return (
		   cont->ClassID()==Class_ID(TCBINTERP_FLOAT_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(TCBINTERP_POSITION_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(TCBINTERP_ROTATION_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(TCBINTERP_POINT3_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(TCBINTERP_SCALE_CLASS_ID,0)
		 );
}

// ============================================================================

BOOL IsBezierControl(Control *cont)
{
  return (
		   cont->ClassID()==Class_ID(HYBRIDINTERP_FLOAT_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(HYBRIDINTERP_POSITION_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(HYBRIDINTERP_POINT3_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(HYBRIDINTERP_SCALE_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(HYBRIDINTERP_COLOR_CLASS_ID,0)
		 );
}

// ============================================================================

BOOL IsLinearControl(Control *cont)
{
  return (
		   cont->ClassID()==Class_ID(LININTERP_FLOAT_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(LININTERP_POSITION_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(LININTERP_ROTATION_CLASS_ID,0) ||
//		   cont->ClassID()==Class_ID(LININTERP_POINT3_CLASS_ID,0) ||
		   cont->ClassID()==Class_ID(LININTERP_SCALE_CLASS_ID,0)
		 );
}

// ============================================================================

void write_string0 (FILE *f, char *st)
{
  int k;

  for (k=0; st[k]!='\0'; k++)
    fwrite(&st[k], sizeof(char), 1, f);
  fwrite(&st[k], sizeof(char), 1, f);  // scrivo anche il \0
}

// ============================================================================

void write_chunk_header(FILE *f, int id, char *no, int s)
{
  fwrite(&id, sizeof(int), 1, f);
  write_string0(f, no);
  fwrite(&s, sizeof(int), 1, f);
}

// ============================================================================

void my_ftoa (float val, char *out)
{
   int i, j, k;

   sprintf(out, "%f", val);

   j=-1;
   k=strlen(out);
   for (i=0; i<k; i++)
	 if (out[i]==',') j=i;

   if (j!=-1) out[j]='.';
}