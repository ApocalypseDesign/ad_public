#ifndef _EXTRASDK_H_
#define _EXTRASDK_H_

#include "simpobj.h"
#include "simpmod.h"
#include "stdmat.h"
#include "texmaps.h"
#include "ffdmod.h"
#include "ffdui.h"

#define BOOL_CONTROL_CLASS_ID	Class_ID(0x984b8d27,0x938f3e43)

class BKey {
	public:
		TimeValue time;
		DWORD flags;

		BKey() {flags=0;}
		BKey(TimeValue t) {time=t; flags=0;}
	};

class SetKeyBuffer : public Tab<BKey> {
	public:
		BOOL oldOnOffVal;
		BOOL startOnOff;

		SetKeyBuffer() : Tab<BKey>() {}
		SetKeyBuffer(BOOL onOff, BOOL start) : Tab<BKey>() {oldOnOffVal=onOff; startOnOff=start;}
		SetKeyBuffer(BOOL onOff, BOOL start, Tab<BKey> &table) : Tab<BKey>(table) {
			oldOnOffVal = onOff;
			startOnOff  = start;
			}
		int GetKeyIndex(TimeValue t);
		SetKeyBuffer &operator=(SetKeyBuffer &from) {
			Tab<BKey>::operator=(from);
			oldOnOffVal = from.oldOnOffVal;
			startOnOff  = from.startOnOff;
			return *this;
			}
	};


class BoolControl : public StdControl {
	public:				
		Tab<BKey> keys;
		Interval range, valid;
		BOOL onOff, startOnOff, rangeLinked;		
		SetKeyBuffer *setkeyBuffer;

		BoolControl();
		~BoolControl();
				
		// Animatable methods		
		void DeleteThis() {delete this;}		
		int IsKeyable() {return 1;}
		BOOL IsAnimated() {return keys.Count()?TRUE:FALSE;}
		Class_ID ClassID() {return BOOL_CONTROL_CLASS_ID;} 
		SClass_ID SuperClassID() {return CTRL_FLOAT_CLASS_ID;}
		void GetClassName(TSTR& s) {s = "BOOL_CONTROL_CNAME";}

		// Set-key mode support
		void CommitSetKeyBuffer(TimeValue t);
		void RevertSetKeyBuffer();
		void RevertSetKeyBuffer(BOOL clearHeldFlag);
		BOOL SetKeyBufferPresent() {return setkeyBuffer? TRUE: FALSE;}
		void HoldSetKeyBuffer();

		// Reference methods
		RefResult NotifyRefChanged(Interval, RefTargetHandle, PartID&, RefMessage) {return REF_SUCCEED;}
		IOResult Save(ISave *isave);
		IOResult Load(ILoad *iload);
		RefTargetHandle Clone(RemapDir &remap = NoRemap());

		// Control methods				
		void Copy(Control *from);
		BOOL IsLeaf() {return TRUE;}
		void CommitValue(TimeValue t) {}
		void RestoreValue(TimeValue t) {}
		
		void HoldTrack();
		Interval GetTimeRange(DWORD flags);
		void EditTimeRange(Interval range,DWORD flags);
		void MapKeys(TimeMap *map,DWORD flags );
		
		int NumKeys() {return keys.Count();}
		TimeValue GetKeyTime(int index) {return keys[index].time;}
		int GetKeyIndex(TimeValue t);		
		void DeleteKeyAtTime(TimeValue t);
		BOOL IsKeyAtTime(TimeValue t,DWORD flags);		
		void DeleteTime(Interval iv, DWORD flags);
		void ReverseTime(Interval iv, DWORD flags);
		void ScaleTime(Interval iv, float s);
		void InsertTime(TimeValue ins, TimeValue amount);
		BOOL SupportTimeOperations() {return TRUE;}
		void DeleteKeys(DWORD flags);
		void DeleteKeyByIndex(int index);
		void SelectKeys(TrackHitTab& sel, DWORD flags);
		void SelectKeyByIndex(int i,BOOL sel);
		void FlagKey(TrackHitRecord hit);
		int GetFlagKeyIndex();
		int NumSelKeys();
		void CloneSelectedKeys(BOOL offset=FALSE);
		void AddNewKey(TimeValue t,DWORD flags);		
		BOOL IsKeySelected(int index);
		BOOL CanCopyTrack(Interval iv, DWORD flags) {return TRUE;}
		BOOL CanPasteTrack(TrackClipObject *cobj,Interval iv, DWORD flags) {return cobj->ClassID()==ClassID();}
		TrackClipObject *CopyTrack(Interval iv, DWORD flags);
		void PasteTrack(TrackClipObject *cobj,Interval iv, DWORD flags);
		int GetSelKeyCoords(TimeValue &t, float &val,DWORD flags);
		void SetSelKeyCoords(TimeValue t, float val,DWORD flags);
		int SetSelKeyCoordsExpr(ParamDimension *dim,TCHAR *timeExpr, TCHAR *valExpr, DWORD flags);

		int HitTestTrack(			
			TrackHitTab& hits,
			Rect& rcHit,
			Rect& rcTrack,			
			float zoom,
			int scroll,
			DWORD flags);
		int PaintTrack(
			ParamDimensionBase *dim,
			HDC hdc,
			Rect& rcTrack,
			Rect& rcPaint,
			float zoom,
			int scroll,
			DWORD flags);
		

		// StdControl methods
		void GetValueLocalTime(TimeValue t, void *val, Interval &valid, GetSetMethod method=CTRL_ABSOLUTE) {GetValue(t,val,valid,method);}
		void GetValue(TimeValue t, void *val, Interval &valid, GetSetMethod method=CTRL_ABSOLUTE);
		void SetValueLocalTime(TimeValue t, void *val, int commit, GetSetMethod method);
		void Extrapolate(Interval range,TimeValue t,void *val,Interval &valid,int type);		
		void *CreateTempValue() {return new float;}
		void DeleteTempValue(void *val) {delete (float*)val;}
		void ApplyValue(void *val, void *delta) {*((float*)val) += *((float*)delta);}
		void MultiplyValue(void *val, float m) {*((float*)val) *= m;}

		void SortKeys();
		
	};

// ***********************************************************
//             PARAMETRI DEI PARTICLE SPRAY/SNOW
// ***********************************************************
#define PB_VPTPARTICLES		0
#define PB_RNDPARTICLES		1
#define PB_DROPSIZE			2
#define PB_SPEED			3
#define PB_VARIATION		4
#define PB_DISPTYPE			5
#define PB_STARTTIME		6
#define PB_LIFETIME			7
#define PB_EMITTERWIDTH		8
#define PB_EMITTERHEIGHT	9
#define PB_HIDEEMITTER		10
#define PB_BIRTHRATE		11
#define PB_CONSTANT			12
#define PB_RENDER			13
#define PB_TUMBLE			14
#define PB_SCALE			15


// ***********************************************************
//             PARAMETRI DEL WIND MODIFIER/OBJECT
// ***********************************************************
#define WPB_STRENGTH        0
#define WPB_DECAY           1
#define WPB_TYPE            2
#define WPB_TURBULENCE      3
#define WPB_FREQUENCY       4
#define WPB_SCALE           5
#define WPB_HOOPSON	        7
/*#define WPB_STRENGTH        0
#define WPB_DECAY           1
#define WPB_TURBULENCE      2
#define WPB_FREQUENCY       3
#define WPB_SCALE           4*/


// ***********************************************************
//            PARAMETRI DEL GRAVITY MODIFIER/OBJECT
// ***********************************************************
#define GPB_STRENGTH     0
#define GPB_DECAY        1
#define GPB_TYPE         2
#define GPB_DISPLENGTH   3
#define GPB_HOOPSON      4


// ***********************************************************
//             DEFINIZIONE DEL BOMB MODIFIER/OBJECT
// ***********************************************************
/*
#define BPB_STRENGTH		0
#define BPB_GRAVITY		1
#define BPB_CHAOS		2
#define BPB_DETONATION	3
#define BPB_SPIN			4
#define BPB_FALLOFF		5
#define BPB_FALLOFFON	6
#define BPB_MINFRAG		7
#define BPB_MAXFRAG		8
#define BPB_SEED			9

class BombObject: public WSMObject
{
	public:	
		IParamBlock *pblock;		
		Mesh mesh;
		
		static HWND hParam, hSot;
		static IObjParam *ip;
		static ISpinnerControl *strengthSpin;	
		static ISpinnerControl *gravSpin;
		static ISpinnerControl *detSpin;
		static ISpinnerControl *chaosSpin;
		static ISpinnerControl *spinSpin;
		static ISpinnerControl *falloffSpin;
		static ISpinnerControl *minFragSpin;
		static ISpinnerControl *maxFragSpin;
		static ISpinnerControl *seedSpin;

		RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message);

		void UpdateUI(TimeValue t);				
		void BuildMesh(TimeValue t,Mesh &mesh);

	public:
		BombObject();
		~BombObject();

		void SetStrength(TimeValue t, float f);
		void SetGravity(TimeValue t, float f);
		void SetChaos(TimeValue t, float f);
		void SetDetonation(TimeValue t,TimeValue det);
		void SetSpin(TimeValue t, float f);
		void SetFalloff(TimeValue t, float f);
		void SetFalloffOn(TimeValue t,int onOff);
		void SetMinFrag(TimeValue t,int m);
		void SetMaxFrag(TimeValue t,int m);
		void SetSeed(TimeValue t,int m);

		float GetStrength(TimeValue t, Interval& valid = Interval(0,0));
		float GetGravity(TimeValue t, Interval& valid = Interval(0,0));
		float GetChaos(TimeValue t, Interval& valid = Interval(0,0));
		TimeValue GetDetonation(TimeValue t, Interval& valid = Interval(0,0));	
		float GetSpin(TimeValue t, Interval& valid = Interval(0,0));
		float GetFalloff(TimeValue t, Interval& valid = Interval(0,0));
		int GetFalloffOn(TimeValue t, Interval& valid = Interval(0,0));
		int GetMinFrag(TimeValue t, Interval& valid = Interval(0,0));
		int GetMaxFrag(TimeValue t, Interval& valid = Interval(0,0));
		int GetSeed(TimeValue t, Interval& valid = Interval(0,0));

		//  inherited virtual methods:

		// From Animatable
//		void GetClassName(TSTR& s) { s= GetString(IDS_RB_BOMBOBJECT_CLASS); }  
		Class_ID ClassID() { return Class_ID(BOMB_OBJECT_CLASS_ID,0); }  		
		void DeleteThis() {delete this;}
		void MapKeys(TimeMap *map,DWORD flags);

		// From ref
		int NumRefs() {return 1;}
		RefTargetHandle GetReference(int i) {return pblock;}
		void SetReference(int i, RefTargetHandle rtarg) {pblock=(IParamBlock*)rtarg;}		
		IOResult Load(ILoad *iload);

		// From BaseObject
		int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt);
		void Snap(TimeValue t, INode* inode, SnapInfo *snap, IPoint2 *p, ViewExp *vpt);
		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
		CreateMouseCallBack* GetCreateMouseCallBack();
		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev );
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next);
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
		void GetWorldBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box );
		void GetLocalBoundBox(TimeValue t, INode* inode, ViewExp* vpt, Box3& box );
//		TCHAR *GetObjectName() { return GetString(IDS_RB_BOMB);}
		
		// from object
		int IsRenderable() { return FALSE; }
		int DoOwnSelectHilite() { return TRUE; }
		ObjectHandle ApplyTransform(Matrix3& matrix) { return this; }
		Interval ObjectValidity(TimeValue time);
		ObjectState Eval(TimeValue time);
//		void InitNodeName(TSTR& s) {s = GetString(IDS_RB_BOMB);}

		// From WSMObject
		Modifier *CreateWSMMod(INode *node);

		
		// from Animatable
		int NumSubs() { return 1; }  
		Animatable* SubAnim(int i) { return pblock; }
//		TSTR SubAnimName(int i) { return TSTR(GetString(IDS_RB_PARAMETERS));}
};


class BombMod : public Modifier
{
	
	private:
		BombObject  *obRef;
		INode       *nodeRef;		

		RefResult NotifyRefChanged( Interval changeInt,RefTargetHandle hTarget, 
		   PartID& partID, RefMessage message );	

	public:
		BombMod();
		BombMod(INode *node,BombObject *obj);
		~BombMod();

		Interval LocalValidity(TimeValue t);
		ChannelMask ChannelsUsed()  { return PART_GEOM|PART_TOPO; }
		ChannelMask ChannelsChanged() { return PART_GEOM|PART_TOPO; }
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os,INode *node);
		Class_ID InputType() { return Class_ID(TRIOBJ_CLASS_ID,0); }

		//  inherited virtual methods:

		// From Animatable
//		void GetClassName(TSTR& s) { s= GetString(IDS_RB_BOMBMOD); }  
		SClass_ID SuperClassID() { return WSM_CLASS_ID; }
		Class_ID ClassID() { return Class_ID(BOMB_CLASS_ID,0); } 
		void DeleteThis() {delete this;}
		
		// From ref
		int NumRefs() { return 2; }
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);

		// From BaseObject
		void BeginEditParams( IObjParam  *ip, ULONG flags,Animatable *prev );
		void EndEditParams( IObjParam *ip, ULONG flags,Animatable *next );
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
//		TCHAR *GetObjectName() { return GetString(IDS_RB_BOMBBINDING); }
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;}

		BombObject *GetWSMObject(TimeValue t);
};
*/


// ***********************************************************
// * Classe base del modificatore FFD (Free Form Deformation)*
// ***********************************************************
class FFDMod : public IFFDMod<Modifier> {	
	public:
		MasterPointControl *masterCont;		// Master track controller		
		// An addition transformation from "Mod Space".
		// Let's the user move/rotate/scale the source box
		Control	*tmControl;
		
		// Parameter block to store parameters
		IParamBlock2 *pblock;		
		
		// This BitArray will be set to a length of 64. 1 bit for
		// each point indicating its selection state.
		BitArray sel;
		
		// A cache of the input object's bounding box.
		Box3 lbox;
		int	 selLevel;
		int loadRefVersion;

		FFDRightMenu<FFDMod >	*ffdMenu;			// Right-click menu handler		
		FFDActionCB<FFDMod >	*ffdActionCB;		// Actions handler 		
		

		// Class variables -- these are only used by one instance
		// of this class at a time while it is being edited in
		// the command panel.
		static IObjParam			*ip;		
		static MoveModBoxCMode		*moveMode;
		static RotateModBoxCMode	*rotMode;
		static UScaleModBoxCMode	*uscaleMode;
		static NUScaleModBoxCMode	*nuscaleMode;
		static SquashModBoxCMode	*squashMode;
		static SelectModBoxCMode	*selectMode;

		FFDMod();
		~FFDMod();
		
		// From Animatable
		void DeleteThis() {delete this;}		
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip,ULONG flags,Animatable *next);				
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;} 
		BOOL AssignController(Animatable *control,int subAnim);		
		int SubNumToRefNum(int subNum);
		BOOL SelectSubAnim(int subNum);
		int	NumParamBlocks() { return 1; }					// return number of ParamBlocks in this instance
		IParamBlock2* GetParamBlock(int i) { return pblock; } // return i'th ParamBlock
		IParamBlock2* GetParamBlockByID(BlockID id) { return (pblock->ID() == id) ? pblock : NULL; } // return id'd ParamBlock

		// From BaseObject/Object
		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags, ModContext *mc);
		void GetWorldBoundBox(TimeValue t,INode* inode, ViewExp *vpt, Box3& box, ModContext *mc);
		int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt, ModContext* mc);
		void SelectSubComponent(HitRecord *hitRec, BOOL selected, BOOL all, BOOL invert=FALSE);
		void ClearSelection(int selLevel);
		void SelectAll(int selLevel);
		void InvertSelection(int selLevel);
		void ActivateSubobjSel(int level, XFormModes& modes);
		void Move(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin);
		void Rotate(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Quat& val, BOOL localOrigin);
		void Scale(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin);
		void GetSubObjectCenters(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);
		void GetSubObjectTMs(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);
		void TransformStart(TimeValue t) {if (ip) ip->LockAxisTripods(TRUE);}
		void TransformFinish(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE);}
		void TransformCancel(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE);}

		// NS: New SubObjType API
		int NumSubObjTypes();
		ISubObjType *GetSubObjType(int i);

		// From Modifier
		ChannelMask ChannelsUsed()  {return PART_GEOM|PART_TOPO|PART_SELECT|PART_SUBSEL_TYPE;}
		ChannelMask ChannelsChanged() {return PART_GEOM;}
		Class_ID InputType() {return defObjectClassID;}		
		Interval LocalValidity(TimeValue t);
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);

		// From ReferenceTarget/Maker
		int NumRefs() {return 67;}
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);
		int NumSubs() {return 3;}
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);

		int		 RemapRefOnLoad(int iref);
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);
						
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message);

		int DrawControlPoints(TimeValue t,ViewExp *vpt,GraphicsWindow *gw,Box3 box,BOOL ht=FALSE,INode *inode=NULL,ModContext *mc=NULL,int flags=0);		
		Matrix3 CompTM(TimeValue t,INode *inode,ModContext *mc);
		void PlugControllers(TimeValue t, BOOL all=FALSE);
		
		
		virtual Point3& getPt(int i)=0;
		virtual Point3& getOffset(int i)=0;
		virtual Point3 *GetPtPtr()=0;
		virtual Point3 *GetOffsetPtr()=0;
		virtual int GridWidth()=0;
		virtual int GridIndex(int i,int j,int k)=0;
		virtual Point3 GetControlPoint(TimeValue t, int i, int src=FALSE, BOOL initVol=FALSE)=0;
		virtual	ClassDesc2* GetClassDesc()=0;

		// From IFFDMod
		Point3 GetPt(int i) { return getPt(i); }
		void SetPt(int i, Point3 p) { getPt(i) = p; }
		void Reset();
		void Conform();
		void AnimateAll();
		void SetGridDim(IPoint3 d) { Reset(); }
};

#endif
