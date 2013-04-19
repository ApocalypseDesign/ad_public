unit ADsystem;

interface
uses windows;

const ADSYSTEMDLL='ADSYSTEM.DLL';

procedure ADSYSTEM_Init(width,height:integer); cdecl; external ADSYSTEMDLL index 1;
procedure ADSYSTEM_Update; cdecl; external ADSYSTEMDLL index 2;
function  ADSYSTEM_Paint(curpos:single):pointer; cdecl; external ADSYSTEMDLL index 3;
procedure ADSYSTEM_Close; cdecl; external ADSYSTEMDLL index 4;
procedure ADSYSTEM_LoadFromFile(filename:pchar); cdecl; external ADSYSTEMDLL index 5;
procedure ADSYSTEM_SaveToFile(filename:pchar); cdecl; external ADSYSTEMDLL index 6;
procedure ADSYSTEM_Reset; cdecl; external ADSYSTEMDLL index 10;
function  ADSYSTEM_ChunkWrite(ChunkName:pchar; data:pointer; size:integer):integer; cdecl; external ADSYSTEMDLL index 20;
function  ADSYSTEM_ChunkRead (ChunkName:pchar; var data:pointer; var size:integer):integer; cdecl; external ADSYSTEMDLL index 21;

procedure ADSYSTEM_SoundLoad; cdecl; external ADSYSTEMDLL index 30;
procedure ADSYSTEM_SoundPlay(pos:integer); cdecl; external ADSYSTEMDLL index 31;
procedure ADSYSTEM_SoundStop; cdecl; external ADSYSTEMDLL index 32;

procedure ADSYSTEM_Image_Import(filename:pchar); cdecl; external ADSYSTEMDLL index 50;
function  ADSYSTEM_Image_Count:integer; cdecl; external ADSYSTEMDLL index 51;
procedure ADSYSTEM_Image_SetCurrent(channelIndex:integer); cdecl; external ADSYSTEMDLL index 52;
procedure ADSYSTEM_Image_GetData(var buffer:pchar; var width,height:integer); cdecl; external ADSYSTEMDLL index 53;
procedure ADSYSTEM_Image_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 54;

function  ADSYSTEM_Channel_Count:integer; cdecl; external ADSYSTEMDLL index 100;
function  ADSYSTEM_Channel_Create:integer; cdecl; external ADSYSTEMDLL index 101;
procedure ADSYSTEM_Channel_SetCurrent(channelIndex:integer); cdecl; external ADSYSTEMDLL index 102;
procedure ADSYSTEM_Channel_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 103;
procedure ADSYSTEM_Channel_SetInfo(name,desc:pchar); cdecl; external ADSYSTEMDLL index 104;
procedure ADSYSTEM_Channel_Delete; cdecl; external ADSYSTEMDLL index 105;
function  ADSYSTEM_Channel_GetCurrent:integer; cdecl; external ADSYSTEMDLL index 106;

function  ADSYSTEM_CLevel_Count:integer; cdecl; external ADSYSTEMDLL index 200;
procedure ADSYSTEM_CLevel_SetCurrent(clevelIndex:integer); cdecl; external ADSYSTEMDLL index 202;
procedure ADSYSTEM_CLevel_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 203;

function  ADSYSTEM_CLevelControl_Count:integer; cdecl; external ADSYSTEMDLL index 220;
procedure ADSYSTEM_CLevelControl_SetCurrent(cLevelContolIndex:integer); cdecl; external ADSYSTEMDLL index 222;
procedure ADSYSTEM_CLevelControl_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 223;
procedure ADSYSTEM_CLevelControl_GetBase(var name:pchar); cdecl; external ADSYSTEMDLL index 227;

function  ADSYSTEM_CLevelChannel_Count:integer; cdecl; external ADSYSTEMDLL index 240;
procedure ADSYSTEM_CLevelChannel_SetCurrent(cLevelChannelIndex:integer); cdecl; external ADSYSTEMDLL index 242;
procedure ADSYSTEM_CLevelChannel_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 243;

function  ADSYSTEM_CLevelImage_Count:integer; cdecl; external ADSYSTEMDLL index 260;
procedure ADSYSTEM_CLevelImage_SetCurrent(cLevelChannelIndex:integer); cdecl; external ADSYSTEMDLL index 262;
procedure ADSYSTEM_CLevelImage_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 263;

function  ADSYSTEM_Level_Count:integer; cdecl; external ADSYSTEMDLL index 300;
function  ADSYSTEM_Level_Create:integer; cdecl; external ADSYSTEMDLL index 301;
procedure ADSYSTEM_Level_SetCurrent(levelIndex:integer); cdecl; external ADSYSTEMDLL index 302;
procedure ADSYSTEM_Level_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 303;
procedure ADSYSTEM_Level_SetInfo(name,desc:pchar); cdecl; external ADSYSTEMDLL index 304;
procedure ADSYSTEM_Level_Delete; cdecl; external ADSYSTEMDLL index 305;
function  ADSYSTEM_Level_GetClass:integer; cdecl; external ADSYSTEMDLL index 306;

procedure ADSYSTEM_LevelControl_SetCurrent(levelControlIndex:integer); cdecl; external ADSYSTEMDLL index 322;
function  ADSYSTEM_LevelControl_GetControl:integer; cdecl; external ADSYSTEMDLL index 328;
procedure ADSYSTEM_LevelControl_SetControl(controlIndex:integer); cdecl; external ADSYSTEMDLL index 329;
function  ADSYSTEM_LevelControl_GetClass:integer; cdecl; external ADSYSTEMDLL index 326;

procedure ADSYSTEM_LevelChannel_SetCurrent(levelChannelIndex:integer); cdecl; external ADSYSTEMDLL index 342;
function  ADSYSTEM_LevelChannel_GetChannel:integer; cdecl; external ADSYSTEMDLL index 348;
procedure ADSYSTEM_LevelChannel_SetChannel(channelIndex:integer); cdecl; external ADSYSTEMDLL index 349;

procedure ADSYSTEM_LevelImage_SetCurrent(levelImageIndex:integer); cdecl; external ADSYSTEMDLL index 362;
function  ADSYSTEM_LevelImage_GetImage:integer; cdecl; external ADSYSTEMDLL index 368;
procedure ADSYSTEM_LevelImage_SetImage(imageIndex:integer); cdecl; external ADSYSTEMDLL index 369;

function  ADSYSTEM_CControl_Count:integer; cdecl; external ADSYSTEMDLL index 400;
procedure ADSYSTEM_CControl_SetCurrent(ccontrolIndex:integer); cdecl; external ADSYSTEMDLL index 402;
procedure ADSYSTEM_CControl_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 403;
procedure ADSYSTEM_CControl_GetBase(var name:pchar); cdecl; external ADSYSTEMDLL index 407;

function  ADSYSTEM_Control_Count:integer; cdecl; external ADSYSTEMDLL index 500;
function  ADSYSTEM_Control_Create:integer; cdecl; external ADSYSTEMDLL index 501;
procedure ADSYSTEM_Control_SetCurrent(controlIndex:integer); cdecl; external ADSYSTEMDLL index 502;
procedure ADSYSTEM_Control_GetInfo(var name,desc:pchar); cdecl; external ADSYSTEMDLL index 503;
procedure ADSYSTEM_Control_SetInfo(name,desc:pchar); cdecl; external ADSYSTEMDLL index 504;
procedure ADSYSTEM_Control_Delete; cdecl; external ADSYSTEMDLL index 505;
function  ADSYSTEM_Control_GetClass:integer; cdecl; external ADSYSTEMDLL index 506;
procedure ADSYSTEM_Control_GetBase(var name:pchar); cdecl; external ADSYSTEMDLL index 507;
procedure ADSYSTEM_Control_LoadFromMemory(data:pointer; size:integer); cdecl; external ADSYSTEMDLL index 510;
procedure ADSYSTEM_Control_SaveToMemory(var data:pointer; var size:integer); cdecl; external ADSYSTEMDLL index 511;
procedure ADSYSTEM_Control_FreeSavedMemory; cdecl; external ADSYSTEMDLL index 513;
procedure ADSYSTEM_Control_GetValue(pos:single; var data:pointer; var size:integer); cdecl; external ADSYSTEMDLL index 512;

function  ADSYSTEM_ControlKey_Count:integer; cdecl; external ADSYSTEMDLL index 550;
function  ADSYSTEM_ControlKey_GetPos(keynum:integer):integer; cdecl; external ADSYSTEMDLL index 551;
procedure ADSYSTEM_ControlKey_Add(pos:integer); cdecl; external ADSYSTEMDLL index 552;
procedure ADSYSTEM_ControlKey_Move(num,newpos:integer); cdecl; external ADSYSTEMDLL index 553;
procedure ADSYSTEM_ControlKey_Delete(keynum:integer); cdecl; external ADSYSTEMDLL index 554;
procedure ADSYSTEM_ControlKey_Edit(keynum:integer; mainWin:HWND); cdecl; external ADSYSTEMDLL index 555;

implementation

end.
