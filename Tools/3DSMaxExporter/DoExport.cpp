/*===========================================================================*\
 | 
 |  FILE:	DoExport.cpp
 |			A3D Scene Exporter 
 |			RAY Scene Exporter 
 |			for 3D Studio MAX R3.0
 | 
 |  AUTH:   Turbo / Apocalypse Design
 |
 |  HIST:	Started 10-8-99
 | 
\*===========================================================================*/

#include "SceneExport.h"
#include "A3Dfile.h"
#include "extrasdk.h"
#include "interpik.h"


HWND buddy;
/*===========================================================================*\
 |  Determine what options we support
\*===========================================================================*/

BOOL SkeletonExporter::SupportsOptions(int ext, DWORD options)
{
	switch(ext) {
		case 0:
			if(options & SCENE_EXPORT_SELECTED) return TRUE;
			else return FALSE;
			break;
		}
	return FALSE;
}



/*===========================================================================*\
 |  Preferences dialog handler
\*===========================================================================*/

static BOOL CALLBACK PrefsDlgProc(
		HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    BOOL err;
	SkeletonExporter *se = (SkeletonExporter*)GetWindowLong(hWnd,GWL_USERDATA);
	if (!se && msg!=WM_INITDIALOG) return FALSE;

	switch (msg) {
		case WM_INITDIALOG:	
			// Update class pointer
			se = (SkeletonExporter*)lParam;
			SetWindowLong(hWnd,GWL_USERDATA,lParam);

			// Setup preferences initial state
			SetCheckBox(hWnd, CHECK_ADM, se->makeADM);
			SetCheckBox(hWnd, CHECK_ADL, se->makeADL);
			SetCheckBox(hWnd, CHECK_ADO, se->makeADO);
			//SetCheckBox(hWnd, CHECK_ADD, se->makeADD);
			SetCheckBox(hWnd, CHECK_ADP, se->makeADP);
			SetCheckBox(hWnd, CHECK_ADS, se->makeADS);
			SetCheckBox(hWnd, CHECK_RAY, se->makeRAY);
			buddy=GetDlgItem(hWnd, IDC_CSRATE);
			SendDlgItemMessage(hWnd, IDC_SPIN1, UDM_SETBUDDY, (LONG)buddy, 0L);
			SendDlgItemMessage(hWnd, IDC_SPIN1, UDM_SETRANGE, 0L, MAKELONG(1, 1000));
            SendDlgItemMessage(hWnd, IDC_SPIN1, UDM_SETPOS, 0L, MAKELONG(20, 0));
			break;

		case WM_DESTROY:
			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) {
				case IDC_CANCEL:
					EndDialog(hWnd,1);
					break;

				case IDC_OK:
					// Retrieve preferences
					se->makeADM = GetCheckBox(hWnd, CHECK_ADM);
                    se->makeADL = GetCheckBox(hWnd, CHECK_ADL);
                    se->makeADO = GetCheckBox(hWnd, CHECK_ADO);
                    //se->makeADD = GetCheckBox(hWnd, CHECK_ADD);
                    se->makeADP = GetCheckBox(hWnd, CHECK_ADP);
                    se->makeADS = GetCheckBox(hWnd, CHECK_ADS);
                    se->makeRAY = GetCheckBox(hWnd, CHECK_RAY);
			        se->cs_rate=GetDlgItemInt(hWnd, IDC_CSRATE, &err, FALSE);
					EndDialog(hWnd,0);
				break;
			}
			break;

		default:
			return FALSE;
		}	
	return TRUE;
} 


void SkeletonExporter::GetSceneBoundingBox(INode *node, TimeValue t, Box3 *bb)
{
	ObjectState os;
	Box3 baux;
	Matrix3 m;
	
	os = node->EvalWorldState(0);
	if ( 
		 (!node->IsRootNode()) &&
         (os.obj->SuperClassID()!=CAMERA_CLASS_ID)
	   )
	{
	   m=node->GetObjectTM(0);
	   os.obj->GetDeformBBox(t, baux, &m, FALSE);
	   if (baux.pmin.x < (*bb).pmin.x) (*bb).pmin.x=baux.pmin.x;
	   if (baux.pmin.y < (*bb).pmin.y) (*bb).pmin.y=baux.pmin.y;
	   if (baux.pmin.z < (*bb).pmin.z) (*bb).pmin.z=baux.pmin.z;
	   if (baux.pmax.x > (*bb).pmax.x) (*bb).pmax.x=baux.pmax.x;
	   if (baux.pmax.y > (*bb).pmax.y) (*bb).pmax.y=baux.pmax.y;
	   if (baux.pmax.z > (*bb).pmax.z) (*bb).pmax.z=baux.pmax.z;
	}

	for (int c=0; c<node->NumberOfChildren(); c++)
	{
       GetSceneBoundingBox(node->GetChildNode(c), t, bb);
	}
}


// ###########################################################
// #######   EXPORTERs DI SINGOLE ENTITA' DELLA SCENA  #######
// ###########################################################
void SkeletonExporter::export_general_info(void)
{
	int ff, lf, fs;
	Interval range = ip->GetAnimRange();
	Box3 bbox;

	fprintf(fTXT, "[AD]Turbo ROX as usual !! \n\n");

	INode *root = ip->GetRootNode();
	bbox.pmin.x=bbox.pmin.y=bbox.pmin.z=(float)1E10;
	bbox.pmax.x=bbox.pmax.y=bbox.pmax.z=(float)-1E10;
    GetSceneBoundingBox(root, 0, &bbox);

	ff=range.Start() / GetTicksPerFrame();
	lf=range.End() / GetTicksPerFrame();
	fs=GetFrameRate();

	// Start with a file identifier and format version
	fprintf(fTXT, "First frame : %d\n", ff);
	fprintf(fTXT, "Last frame : %d\n", lf);
	fprintf(fTXT, "Frame speed : %d\n", fs);
	fprintf(fTXT, "Scene Bounding Box at time 0\n");
	fprintf(fTXT, "min : %f, %f, %f\n",
		    bbox.pmin.x, bbox.pmin.y, bbox.pmin.z);
	fprintf(fTXT, "max : %f, %f, %f\n",
		    bbox.pmax.x, bbox.pmax.y, bbox.pmax.z);

	write_chunk_header(fA3D, SCENE_GENERAL_INFO_ID, "Scene Root", 36);
	fwrite(&ff, sizeof(int), 1, fA3D);
	fwrite(&lf, sizeof(int), 1, fA3D);
	fwrite(&fs, sizeof(int), 1, fA3D);
	write_bbox(&bbox, fA3D);

	if (makeRAY) write_chunk_header(fRAY, SCENE_GENERAL_INFO_ID, "Scene Root", 36);
	if (makeRAY) fwrite(&ff, sizeof(int), 1, fRAY);
	if (makeRAY) fwrite(&lf, sizeof(int), 1, fRAY);
	if (makeRAY) fwrite(&fs, sizeof(int), 1, fRAY);
	if (makeRAY) write_bbox(&bbox, fRAY);
}


/*===========================================================================*\
 |  For every node we get, just print out its name and object ref's classID
\*===========================================================================*/

BOOL SkeletonExporter::nodeEnum(INode* node,Interface *ip) 
{
	if(exportSelected && node->Selected() == FALSE)
		return TREE_CONTINUE;

	// Se annullato dall'utente esci
    // if (ip->GetCancel()) return FALSE;

	// The ObjectState is a 'thing' that flows down the pipeline containing
	// all information about the object. By calling EvalWorldState() we tell
	// max to eveluate the object at end of the pipeline.
	ObjectState os = node->EvalWorldState(0);

	fprintf(fTXT, "Name: %s, Father Name: %s\n",
		    node->GetName(), node->GetParentNode()->GetName());
	fflush(fTXT);
	if (os.obj)
	{
	  fprintf(fTXT, "Super Class ID: %X, ClassID: %X, %X\n",
	        os.obj->SuperClassID(),
			os.obj->ClassID().PartA(),
            os.obj->ClassID().PartB());
	}


	// The obj member of ObjectState is the actual object we will export.
	if (os.obj)
	{
	  // We look at the super class ID to determine the type of the object.
	  switch(os.obj->SuperClassID())
	  {
	    case WSM_OBJECT_CLASS_ID:
			 if (os.obj->ClassID()==Class_ID(WINDOBJECT_CLASS_ID, 0))
				export_WSM_wind(node);
			 else
			 if (os.obj->ClassID()==Class_ID(BOMB_OBJECT_CLASS_ID, 0))
				export_WSM_bomb(node);
			 else
			 if (os.obj->ClassID()==Class_ID(GRAVITYOBJECT_CLASS_ID, 0))
				export_WSM_gravity(node);
             break;

		case GEOMOBJECT_CLASS_ID:
			 if (os.obj->ClassID()==Class_ID(0x9125, 0))
			    export_bipedbone(node);   // biped bone
			 else
			 if (os.obj->ClassID()==BONE_OBJ_CLASSID)
			    export_MAXbone(node);     // Max bone
			 else
			 if (os.obj->ClassID()==patchObjectClassID)
			    export_patch(node);       // Bezier patch
			 else 
			 if ((os.obj->ClassID()==Class_ID(RAIN_CLASS_ID, 0)) ||
                 (os.obj->ClassID()==Class_ID(SNOW_CLASS_ID, 0)))
				export_particle_spray(node);  // particle spray
			 else
			 {
                if (makeRAY)
                if (os.obj->ClassID()==Class_ID(BOXOBJ_CLASS_ID, 0))
					export_ray_box(node);
				if (makeRAY)
                if (os.obj->ClassID()==Class_ID(SPHERE_CLASS_ID, 0))
					export_ray_sphere(node);
				if (makeRAY)
                if (os.obj->ClassID()==Class_ID(CYLINDER_CLASS_ID, 0))
					export_ray_cylinder(node);
				if (makeRAY)
                if (os.obj->ClassID()==Class_ID(0x81f1dfc, 0x77566f65))
					export_ray_plane(node);  // plane (grid object)
				export_solid_object(node); 
			 }
		     break;

		case CAMERA_CLASS_ID:
			 export_camera(node); 
			 break;

		case LIGHT_CLASS_ID:
		     export_light(node); 
			 break;

		case SHAPE_CLASS_ID:
//			 ExportShapeObject(node); 
			 break;

		case HELPER_CLASS_ID:
			 // catena cinematica
			 if (os.obj->ClassID()==Class_ID(0x1C706482, 0x3E82994))
				 export_IKChain(node);
			 else
			 // helper(s) standard
			 export_helper_object(node);
			 break;
	  }
	}
	fflush(fA3D);
	fflush(fTXT);
/*
	// inizio bones
	Object *p; IDerivedObject *q; Modifier *m; int n,i;

	p=node->GetObjectRef();
	if(p->SuperClassID()==GEN_DERIVOB_CLASS_ID)
	{
		q=(IDerivedObject *)p;
		n=q->NumModifiers();
		for (i=0; i<n; i++)
		{
			m=q->GetModifier(i);
			Class_ID cidd = m->ClassID();
            fprintf(fTXT,"   modifier : %s Class_ID(%X,%X)\n",m->GetName(),cidd.PartA(),cidd.PartB());
		}
	}

	// fine bones
*/
	// Recurse through this node's children, if any
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!nodeEnum(node->GetChildNode(c), ip))
			return FALSE;
	}
	return TRUE;
}



/*===========================================================================*\
 |  Do the export to the file
\*===========================================================================*/

int	SkeletonExporter::DoExport(const TCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	// Set local interface pointer
	ip = i;

	// load the configuration from disk
	// so that it can be used in our dialog box
	if(!LoadExporterConfig()) return 0;

	if(!suppressPrompts)
	{

		// Show preferences setup dialog
		int res = DialogBoxParam(
			hInstance,
			MAKEINTRESOURCE(IDD_SKELETON_SCEXP),
			i->GetMAXHWnd(),
			PrefsDlgProc,
			(LPARAM)this);

		// User clicked 'Cancel'
		if(res!=0) return 0;

	}

	exportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;

	// Open a fA3D for writing out to
	fA3D = fopen (name, "wb+");
	if (!fA3D) return 0;

	strcpy(nomefileTXT, name);  // copio nella variabile della
	int sl=strlen(name); // classe il nome del file .A3D
	                            // (compresa estensione)
    nomefileTXT[sl-4]='.';
    nomefileTXT[sl-3]='T';
    nomefileTXT[sl-2]='X';
    nomefileTXT[sl-1]='T';
    nomefileTXT[sl-0]='\0';
	fTXT = _tfopen(nomefileTXT, _T("wt"));
	if (!fTXT) return 0;

    if (makeRAY)
	{
      strcpy(nomefileRAY, name);
      nomefileRAY[sl-4]='.';
      nomefileRAY[sl-3]='R';
      nomefileRAY[sl-2]='A';
      nomefileRAY[sl-1]='Y';
      nomefileRAY[sl-0]='\0';
	  fRAY = fopen(nomefileRAY, "wb+");
	  if (!fRAY) return 0;
	}

    strcpy(nomefileANI, name);
    nomefileANI[sl-4]='.';
    nomefileANI[sl-3]='A';
    nomefileANI[sl-2]='N';
    nomefileANI[sl-1]='I';
    nomefileANI[sl-0]='\0';
	//fANI = fopen(nomefileANI, "wb+");
	//if (!fANI) return 0;

    if (makeADM)
	{
      strcpy(nomefileADM, name);
      nomefileADM[sl-4]='.';
      nomefileADM[sl-3]='A';
      nomefileADM[sl-2]='D';
      nomefileADM[sl-1]='M';
      nomefileADM[sl-0]='\0';
      fADM=fopen(nomefileADM, "wt");
      fprintf(fADM, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADM, "// Material script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADM, "// Design is simply all...\n\n");
	}

    if (makeADL)
	{
      strcpy(nomefileADL, name);
      nomefileADL[sl-4]='.';
      nomefileADL[sl-3]='A';
      nomefileADL[sl-2]='D';
      nomefileADL[sl-1]='L';
      nomefileADL[sl-0]='\0';
      fADL=fopen(nomefileADL, "wt");
      fprintf(fADL, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADL, "// Light script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADL, "// Design is simply all...\n\n");
	}

    if (makeADO)
	{
      strcpy(nomefileADO, name);
      nomefileADO[sl-4]='.';
      nomefileADO[sl-3]='A';
      nomefileADO[sl-2]='D';
      nomefileADO[sl-1]='O';
      nomefileADO[sl-0]='\0';
      fADO=fopen(nomefileADO, "wt");
      fprintf(fADO, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADO, "// Object script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADO, "// Design is simply all...\n\n");
	}
/*
    if (makeADD)
	{
      strcpy(nomefileADD, name);
      nomefileADD[sl-4]='.';
      nomefileADD[sl-3]='A';
      nomefileADD[sl-2]='D';
      nomefileADD[sl-1]='D';
      nomefileADD[sl-0]='\0';
      fADD=fopen(nomefileADD, "wt");
      fprintf(fADD, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADD, "// Dummys script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADD, "// Design is simply all...\n\n");
	}
*/
    if (makeADP)
	{
      strcpy(nomefileADP, name);
      nomefileADP[sl-4]='.';
      nomefileADP[sl-3]='A';
      nomefileADP[sl-2]='D';
      nomefileADP[sl-1]='P';
      nomefileADP[sl-0]='\0';
      fADP=fopen(nomefileADP, "wt");
      fprintf(fADP, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADP, "// Particle systems script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADP, "// Design is simply all...\n\n");
	}

    if (makeADS)
	{
      strcpy(nomefileADS, name);
      nomefileADS[sl-4]='.';
      nomefileADS[sl-3]='A';
      nomefileADS[sl-2]='D';
      nomefileADS[sl-1]='S';
      nomefileADS[sl-0]='\0';
      fADS=fopen(nomefileADS, "wt");
      fprintf(fADS, "// *** Apocalypse Design Scripting System v1.0 ***\n");
      fprintf(fADS, "// Scene script auto-created by Turbo's MAX-plugin\n");
      fprintf(fADS, "// Design is simply all...\n\n");
	}

	export_general_info();
	export_materials();

	// Print out a title for the file header
	fprintf(fTXT, "***Elenco dei nodi***\n\n");

	// Simple root node -> children enumeration
	// This will get the root node, and then cycle through its children (ie, the basic scene nodes)
	// It will then recurse to search their children, and then their children, etc
	int numChildren = i->GetRootNode()->NumberOfChildren();
	
	for (int idx=0; idx<numChildren; idx++) {
//		if (i->GetCancel()) break;
		nodeEnum(i->GetRootNode()->GetChildNode(idx), i);
	}


    // scrittura del chunk di fine scena !!!
	int flag_end=1;
    write_chunk_header(fA3D, END_SCENE_ID, i->GetRootNode()->GetName(), 4);
	fwrite(&flag_end, sizeof(int), 1, fA3D);
    if (makeRAY)
	{
       write_chunk_header(fRAY, END_SCENE_ID, i->GetRootNode()->GetName(), 4);
       fwrite(&flag_end, sizeof(int), 1, fRAY);
	}
	//export_ANI (i->GetRootNode());	

	//fflush(fANI);
	fclose(fTXT);
	fclose(fA3D);
	//fclose(fANI);
    if (makeADM) fclose(fADM);
    if (makeADL) fclose(fADL);
    if (makeADO) fclose(fADO);
    //if (makeADD) fclose(fADD);
    if (makeADP) fclose(fADP);
    if (makeADS) fclose(fADS);
    if (makeRAY) fclose(fRAY);

	// Save the current configuration back out to disk
	// for use next time the exporter is run
	SaveExporterConfig();
	return 1;
}