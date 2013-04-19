unit ADsystem;

interface
uses windows;


const ADSYSTEMDLL='ADSYSTEM.DLL';

      SYS_CLASSES_CT=0;
      SYS_CLASSES_EFX=1;

      SYS_OBJECTS_CT=2;
      SYS_OBJECTS_EFX=3;

      SYS_IMAGES=4;


procedure ADSYSTEM_Init(defaultimagename:pchar); cdecl; external ADSYSTEMDLL index 1;
procedure ADSYSTEM_Update; cdecl; external ADSYSTEMDLL index 2;
procedure ADSYSTEM_Paint(curpos:single); cdecl; external ADSYSTEMDLL index 3;
procedure ADSYSTEM_Close; cdecl; external ADSYSTEMDLL index 4;

function ADSYSTEM_GetNum(what:integer):integer; cdecl; external ADSYSTEMDLL index 5;
procedure ADSYSTEM_Get(what,num:integer; name,desc:pchar); cdecl; external ADSYSTEMDLL index 6;

function ADSYSTEM_GetObjCtNum(what:integer; objname:pchar):integer; cdecl; external ADSYSTEMDLL index 7;
procedure ADSYSTEM_GetObjCt(what:integer; objname:pchar; num:integer; name,desc:pchar); cdecl; external ADSYSTEMDLL index 8;
procedure ADSYSTEM_SetObjCt(what:integer; objname:pchar; name,ctname:pchar); cdecl; external ADSYSTEMDLL index 9;

procedure ADSYSTEM_CreateObj(what:integer; classname,name,desc:pchar); cdecl; external ADSYSTEMDLL index 10;
procedure ADSYSTEM_CreateImage(name,desc:pchar; width,height:integer); cdecl; external ADSYSTEMDLL index 11;
function ADSYSTEM_GetImagePtr(name:pchar):pointer; cdecl; external ADSYSTEMDLL index 12;

function ADSYSTEM_ObjCtCanLoadAndSave(name:pchar):boolean; cdecl; external ADSYSTEMDLL index 13;
procedure ADSYSTEM_ObjCtLoadFromMemory(name:pchar;data:pointer;size:integer); cdecl; external ADSYSTEMDLL index 14;
procedure ADSYSTEM_ObjCtSaveToMemory(name:pchar;var data:pointer;size:pointer); cdecl; external ADSYSTEMDLL index 15;

function ADSYSTEM_ObjCtBaseClass(name:pchar):pchar; cdecl; external ADSYSTEMDLL index 16;
function ADSYSTEM_ObjCtClassName(name:pchar):pchar; cdecl; external ADSYSTEMDLL index 17;


implementation

end.
