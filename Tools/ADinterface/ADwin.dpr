library ADwin;

{ Important note about DLL memory management: ShareMem must be the
  first unit in your library's USES clause AND your project's (select
  Project-View Source) USES clause if your DLL exports any procedures or
  functions that pass strings as parameters or function results. This
  applies to all strings passed to and from your DLL--even those that
  are nested in records and classes. ShareMem is the interface unit to
  the BORLNDMM.DLL shared memory manager, which must be deployed along
  with your DLL. To avoid using BORLNDMM.DLL, pass string information
  using PChar or ShortString parameters. }

uses
  SysUtils,
  Classes,
  Controls,
  Unit1 in 'Unit1.pas' {Form1};

procedure AllocWindow; stdcall;
begin
  form1:=tform1.create(nil);
end;

procedure FreeWindow; stdcall;
begin
  form1.free;
end;

procedure AddDriver(dname:pchar;dguid:TGUID); stdcall;
begin
  SetLength(drivers,length(drivers)+1);
  with drivers[High(drivers)] do
  begin
    name:=dname;
    guid:=dguid;
  end;
end;

procedure AddDevice(dname:pchar;dguid:TGUID); stdcall;
begin
  with drivers[High(drivers)] do
  begin
    SetLength(devices,length(devices)+1);
    with devices[High(devices)] do
    begin
      name:=dname;
      guid:=dguid;
    end;
  end;
end;

procedure AddRes(mwidth,mheight,mbpp:integer); stdcall;
begin
  with drivers[High(drivers)] do
  with devices[High(devices)] do
  begin
    SetLength(modes,length(modes)+1);
    with modes[High(modes)] do
    begin
      width:=mwidth;
      height:=mheight;
      bpp:=mbpp;
    end;
  end;
end;

function ShowSettingsWindow:boolean; stdcall;
var i:integer;
begin
  for i:=low(drivers) to high(drivers)
  do form1.BoxDriver.Items.Add(drivers[i].name);

  form1.BoxDriver.ItemIndex:=0;
  form1.BoxDriver.OnChange(nil);
  form1.BoxDevice.OnChange(nil);

  if form1.BoxDriver.Items.count=1 then
  begin
    form1.boxdriver.enabled:=false;
    if form1.boxDevice.Items.count=1 then
    form1.boxdevice.enabled:=false;
  end;  

  if form1.showmodal=mrOk then result:=true
  else result:=false;

  if form1.BoxMode.ItemIndex=0 then gofullscreen:=true
  else gofullscreen:=false;
end;

function GetDriverGUID:pguid; stdcall;
var i,j:integer;
begin
with form1 do
begin
  j:=-1;
  for i:=low(drivers) to high( drivers) do
  if drivers[i].name=boxdriver.items[boxdriver.ItemIndex]
  then j:=i;
  if j>=0 then result:=@drivers[j].guid
  else result:=@zeroguid;
end;
end;

function GetDeviceGUID:pguid; stdcall;
var i,j,k:integer;
begin
with form1 do
begin
  j:=-1;
  for i:=low(drivers) to high( drivers) do
  if drivers[i].name=boxdriver.items[boxdriver.ItemIndex]
  then j:=i;
  if j>=0 then
  with drivers[j] do
  begin
    k:=-1;
    for i:=low(devices) to high( devices) do
    if devices[i].name=boxdevice.items[boxdevice.ItemIndex]
    then k:=i;
    if k>=0 then result:=@devices[k].guid
    else result:=@zeroguid;
  end else result:=@zeroguid;
end;
end;

procedure GetRes(var dwidth,dheight,dbpp:integer); stdcall;
var i,j,k,l:integer;
begin
with form1 do
begin
  j:=-1;
  for i:=low(drivers) to high( drivers) do
  if drivers[i].name=boxdriver.items[boxdriver.ItemIndex]
  then j:=i;
  if j>=0 then
  with drivers[j] do
  begin
    k:=-1;
    for i:=low(devices) to high( devices) do
    if devices[i].name=boxdevice.items[boxdevice.ItemIndex]
    then k:=i;
    if k>=0 then
     with devices[k] do
     begin
       l:=-1;
       for i:=low(modes) to high(modes) do
       if inttostr(modes[i].width)
       +' x '
       +inttostr(modes[i].height)
       +' x '
       +inttostr(modes[i].bpp)
       =boxres.items[boxres.ItemIndex]
       then l:=i;
       if l>=0 then
       with modes[l] do
       begin
         dwidth:=width;
         dheight:=height;
         dbpp:=bpp;
       end;
     end;
  end;
end;
end;

function GetFullscreen:boolean; stdcall;
begin
  result:=gofullscreen;
end;

exports
   AllocWindow,
   FreeWindow,
   ShowSettingsWindow,
   AddDriver,
   AddDevice,
   AddRes,
   GetDriverGUID,
   GetDeviceGUID,
   GetRes,
   GetFullscreen;
begin
end.
