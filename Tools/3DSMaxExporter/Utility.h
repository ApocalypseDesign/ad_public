#ifndef _UTILFUNC_H_
#define _UTILFUNC_H_

#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "interpik.h"
#include "iparamm2.h"
#include "modstack.h"

bool  IsNodeBone(INode *pNode);
bool  IsNodeBipedBone(INode *pNode);
Matrix3 GetBoneTM(INode *pNode, TimeValue t);

TriObject *GetTriObjectFromNode(INode *node, int &deleteIt);
TriObject *GetTriObjectFromObject(Object *obj, int &deleteIt);
Object *GetBaseObjectFromNode (INode *node);
Modifier* FindPhysiqueModifier (INode *node);
Modifier* FindSkinModifier (INode *node);
void GetPivotOffset(INode* node, Point3 *p);

BOOL TMNegParity(Matrix3 &m);
void write_matrix (Matrix3 *m, FILE *f);
void write_point3 (Point3 *v, FILE *f);
void write_bbox (Box3 *bb, FILE *f);

BOOL IsTCBControl(Control *cont);
BOOL IsBezierControl(Control *cont);
BOOL IsLinearControl(Control *cont);

void write_chunk_header(FILE *f, int id, char *no, int s);
void write_string0 (FILE *f, char *st);
void my_ftoa (float val, char *out);
#endif