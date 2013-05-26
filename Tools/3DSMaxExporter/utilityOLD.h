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

void  FixCoordSys(Point3 &pnt);
void  FixCoordSys(Matrix3 &tm);
void  TM3ToGLTM4(const Matrix3 &tm, float glTM[16]);
//void  EulerToQuat(const Point3 &euler, Quat &quat);
int   TicksToFPS(TimeValue t);
bool  IsNodeBone(INode *pNode);
bool  IsNodeBipedBone(INode *pNode);
int   GetChildBoneCount(INode *pNode);
int   GetBoneIndex(INode *pRoot, INode *pNodeTest);
int   GetBoneCount(INode *pRoot);
int   RecursiveGetBoneIndex(INode *pRoot, INode *pNodeTest, int &boneCount);
long  ClassIDToFlag(Class_ID cid);
Modifier* FindPhysiqueModifier(INode *pNode);
void SetBipedScale(INode *pNode, BOOL bUniform);
Matrix3 GetBoneTM(INode *pNode, TimeValue t);
Matrix3 GetRelativeMatrix(INode *node, int t);
Matrix3 UniformMatrix(Matrix3 orig_cur_mat);
static DWORD WINAPI DummyProgressFunction(LPVOID arg) { return(0); }
void CheckBBox(const Point3 &pnt, Point3 &min, Point3 &max);

#endif