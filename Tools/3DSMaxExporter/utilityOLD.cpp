// ============================================================================
// UtilFunc.cpp
// Copyright ©2000, Discreet   -  www.discreet.com
// Developed by Simon Feltman  -  simon.feltman@autodesk.com
// ----------------------------------------------------------------------------
#include "UtilFunc.h"
#include "bipexp.h"

// ============================================================================
// FixCoordSys(Matrix3 &tm) - Similarity Transformation
// similarity transformation consists of swapping the 2nd and 3rd rows, then
// swapping the the 2nd and 3rd columns as above, and THEN negating all
// the elements of the 1st row, then negating all the elements of the 1st
// column (with the result that the 11 element is unchanged).
// ----------------------------------------------------------------------------
void FixCoordSys(Matrix3 &tm)
{
/*
	// swap 2nd and 3rd rows
	Point3 row = tm.GetRow(1);
	tm.SetRow(1, tm.GetRow(2));
	tm.SetRow(2, row);

	// swap 2nd and 3rd columns
	Point4 column = tm.GetColumn(1);
	tm.SetColumn(1, tm.GetColumn(2));
	tm.SetColumn(2, column);

	// negate 1rst row and column
	tm.SetRow(0, -(tm.GetRow(0)));
	tm.SetColumn(0, -(tm.GetColumn(0)));
*/
	return;
}

// ============================================================================
// Change a vector from Z up to Y up
void FixCoordSys(Point3 &pnt)
{
	float tmp = pnt.y;
	pnt.y = pnt.z;
	pnt.z = tmp;
	pnt.x = -pnt.x;
}

// ============================================================================
// Convert a MAX TM to an OpenGL TM
void TM3ToGLTM4(const Matrix3 &tm, float glTM[16])
{
	Matrix3 maxTM = tm;
	FixCoordSys(maxTM);

	Point3 row = maxTM.GetRow(0);
	glTM[0] = row.x;
	glTM[1] = row.y;
	glTM[2] = row.z;
	glTM[3] = 0.f;

	row = maxTM.GetRow(1);
	glTM[4] = row.x;
	glTM[5] = row.y;
	glTM[6] = row.z;
	glTM[7] = 0.f;

	row = maxTM.GetRow(2);
	glTM[8] = row.x;
	glTM[9] = row.y;
	glTM[10] = row.z;
	glTM[11] = 0.f;

	row = maxTM.GetRow(3);
	glTM[12] = row.x;
	glTM[13] = row.y;
	glTM[14] = row.z;
	glTM[15] = 1.f;
}

// ============================================================================
/*
void EulerToQuat(const Point3 &euler, Quat &quat)
{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	// calculate trig identities
	cr = cos(euler.x * 0.5f);
	cp = cos(euler.y * 0.5f);
	cy = cos(euler.z * 0.5f);

	sr = sin(euler.x * 0.5f);
	sp = sin(euler.y * 0.5f);
	sy = sin(euler.z * 0.5f);

	cpcy = cp * cy;
	spsy = sp * sy;

	quat.x = sr * cpcy - cr * spsy;
	quat.y = cr * sp * cy + sr * cp * sy;
	quat.z = cr * cp * sy - sr * sp * cy;
	quat.w = cr * cpcy + sr * spsy;
}
*/

// ============================================================================
int TicksToFPS(TimeValue t)
{
	return (TicksToSec(t) * GetFrameRate());
}

// ============================================================================
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
// Get the number of direct child bones of a node
int GetChildBoneCount(INode *pNode)
{
	int count = 0;

	for(int i = 0; i < pNode->NumberOfChildren(); i++)
	{
		if(IsNodeBone(pNode->GetChildNode(i)))
			count++;
	}
	return count;
}

// ============================================================================
// Get the total number of bones in the scene, this should get passed the root node
int GetBoneCount(INode *pRoot)
{
	int count = 0;
	if(IsNodeBone(pRoot))
		count += 1;

	for(int i = 0; i < pRoot->NumberOfChildren(); i++)
		count += GetBoneCount(pRoot->GetChildNode(i));
	return count;
}

// ============================================================================
// Recursive iterator to get a bone index, used with GetBoneIndex
int RecursiveGetBoneIndex(INode *pRoot, INode *pNodeTest, int &boneCount)
{
	int boneIdx = -1;

	if(IsNodeBone(pRoot))
	{
		boneIdx = boneCount;
		boneCount++;

		if(pRoot == pNodeTest)
			return boneIdx;
	}

	// recurse child nodes
	for(int i = 0, idx = 0; i < pRoot->NumberOfChildren(); i++)
	{
		int boneIdx = RecursiveGetBoneIndex(pRoot->GetChildNode(i), pNodeTest, boneCount);
		if(boneIdx >= 0)
			return boneIdx;
	}

	return -1;
}

// ============================================================================
// Get an index from a node pointer
int GetBoneIndex(INode *pRoot, INode *pNodeTest)
{
	if(!IsNodeBone(pNodeTest))
		return -1;

	int boneCount = 0;
	return RecursiveGetBoneIndex(pRoot, pNodeTest, boneCount);
}

// ============================================================================
// Set the scaling of a biped bone object
void SetBipedScale(INode *pNode, BOOL bUniform)
{
	if(IsNodeBipedBone(pNode))
	{
		Control *pCont = pNode->GetTMController();
		IBipedExport *pBipExp = (IBipedExport*)pCont->GetInterface(I_BIPINTERFACE);
		pBipExp->RemoveNonUniformScale(bUniform);
		Control *pMaster = (Control*)pCont->GetInterface(I_MASTER);
		pMaster->NotifyDependents(FOREVER, PART_TM, REFMSG_CHANGE);
		pCont->ReleaseInterface(I_MASTER, pMaster);
		pCont->ReleaseInterface(I_BIPINTERFACE, pBipExp);
	}
}

// ============================================================================
// Get the transformation of a bone taking biped bones into account.
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
void CheckBBox(const Point3 &pnt, Point3 &min, Point3 &max)
{
	if(pnt.x > max.x) max.x = pnt.x;
	if(pnt.y > max.y) max.y = pnt.y;
	if(pnt.z > max.z) max.z = pnt.z;

	if(pnt.x < min.x) min.x = pnt.x;
	if(pnt.y < min.y) min.y = pnt.y;
	if(pnt.z < min.z) min.z = pnt.z;
}