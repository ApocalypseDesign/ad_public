unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ComCtrls, ExtCtrls, Menus, StdCtrls, ImgList, unit2, ADsystem, Buttons, unitpl;

type
  TForm1 = class(TForm)
    Panel1: TPanel;
    nav: TTreeView;
    Panel2: TPanel;
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    Esci1: TMenuItem;
    Label1: TLabel;
    Panel3: TPanel;
    Panel4: TPanel;
    Image1: TImage;
    MainPanel: TPanel;
    Panel5: TPanel;
    TrackBar1: TTrackBar;
    StatusBar1: TStatusBar;
    ImageList1: TImageList;
    Panel6: TPanel;
    navlist: TListBox;
    labcur: TLabel;
    btplay: TSpeedButton;
    btstop: TSpeedButton;
    btpause: TSpeedButton;
    PanelEfx: TPanel;
    Label2: TLabel;
    setctbox: TComboBox;
    Panel8: TPanel;
    efxctbox: TListBox;
    Panel9: TPanel;
    Panel7: TPanel;
    Panel10: TPanel;
    SpeedButton1: TSpeedButton;
    Label4: TLabel;
    efxname: TLabel;
    Image2: TImage;
    ctPanel: TPanel;
    PaintBox1: TPaintBox;
    ctbox: TComboBox;
    Label3: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Image3: TImage;
    Panel11: TPanel;
    Label8: TLabel;
    Image4: TImage;
    ComboBox1: TComboBox;
    Panel12: TPanel;
    ListBox1: TListBox;
    Panel13: TPanel;
    Label9: TLabel;
    Image5: TImage;
    Label10: TLabel;
    Panel14: TPanel;
    Label11: TLabel;
    Panel15: TPanel;
    SpeedButton2: TSpeedButton;
    Label12: TLabel;
    SpeedButton3: TSpeedButton;
    Panel16: TPanel;
    Label13: TLabel;
    Label14: TLabel;
    modify: TSpeedButton;
    procedure Esci1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure navChange(Sender: TObject; Node: TTreeNode);
    procedure TrackBar1Change(Sender: TObject);
    procedure btplayClick(Sender: TObject);
    procedure btstopClick(Sender: TObject);
    procedure btpauseClick(Sender: TObject);
    procedure navlistDblClick(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure efxctboxClick(Sender: TObject);
    procedure PaintBox1Paint(Sender: TObject);
    procedure setctboxChange(Sender: TObject);
    procedure ListBox1Click(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure ComboBox1Change(Sender: TObject);
    procedure ctboxDropDown(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure ctboxChange(Sender: TObject);
    procedure modifyClick(Sender: TObject);

  private
    { Private declarations }
  public
    { Public declarations }
    play:Tplayer;
  end;

var
  Form1: TForm1;

implementation

uses Unit3, Unit4;

{$R *.DFM}

procedure TForm1.Esci1Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.FormCreate(Sender: TObject);
var bmp:tbitmap;
begin
  nav.FullExpand;
  bmp:=tbitmap.Create;
  bmp.Width:=image1.Width;
  bmp.Height:=image1.Height;
  bmp.PixelFormat:=pf32bit;
  image1.Picture.Bitmap:=bmp;
  ADSYSTEM_CreateImage('video','backbuffer',image1.Width,image1.Height);
  ADSYSTEM_Init('video');
  ADSYSTEM_Update;
  Trackbar1.OnChange(self);
end;


procedure TForm1.FormDestroy(Sender: TObject);
begin
  ADSYSTEM_Close;
end;

procedure TForm1.navChange(Sender: TObject; Node: TTreeNode);
var i:integer; name,desc:array[0..200] of char;
begin
  speedbutton1click(self);
  speedbutton2click(self);
  navlist.Clear;
  if node.Parent=nil then labcur.caption:=node.Text
  else labcur.caption:=node.parent.Text+'/'+node.text;

  if node.Text='Images'
  then   for i:=0 to ADSYSTEM_GetNum(SYS_IMAGES)-1
         do begin
              ADSYSTEM_Get(SYS_IMAGES,i,name,desc);
              navlist.Items.Add(name);
            end;

  if (node.Text='Ct') and (node.Parent.Text='Classes')
  then   for i:=0 to ADSYSTEM_GetNum(SYS_CLASSES_CT)-1
         do begin
              ADSYSTEM_Get(SYS_CLASSES_CT,i,name,desc);
              navlist.Items.Add(name);
            end;

  if (node.Text='Ct') and (node.Parent.Text='Objects')
  then   for i:=0 to ADSYSTEM_GetNum(SYS_OBJECTS_CT)-1
         do begin
              ADSYSTEM_Get(SYS_OBJECTS_CT,i,name,desc);
              navlist.Items.Add(name);
            end;

  if (node.Text='Efx') and (node.Parent.Text='Classes')
  then   for i:=0 to ADSYSTEM_GetNum(SYS_CLASSES_EFX)-1
         do begin
              ADSYSTEM_Get(SYS_CLASSES_EFX,i,name,desc);
              navlist.Items.Add(name);
            end;

  if (node.Text='Efx') and (node.Parent.Text='Objects')
  then   for i:=0 to ADSYSTEM_GetNum(SYS_OBJECTS_EFX)-1
         do begin
              ADSYSTEM_Get(SYS_OBJECTS_EFX,i,name,desc);
              navlist.Items.Add(name);
            end;

end;

procedure TForm1.TrackBar1Change(Sender: TObject);
var src,dest:PByteArray;
    x,y,x1,larg:integer;
begin
  x1:=0;
  ADSYSTEM_Paint(Trackbar1.Position/Trackbar1.Max);
  src:=ADSYSTEM_GetImagePtr('video');
  larg:=image1.picture.bitmap.width shl 2;
  for y:=0 to image1.Picture.Bitmap.Height-1 do
  begin
   dest:=image1.Picture.Bitmap.ScanLine[y];
   for x:=0 to larg-1 do dest[x]:=src[x+x1];
   inc(x1,larg);
  end;
  image1.Invalidate;
end;

procedure TForm1.btplayClick(Sender: TObject);
begin                              (*
  if play=nil then
  begin                          *)
  play:=Tplayer.create(false);
  Play.inizia;
  play.Priority:=tpIdle;
  play.FreeOnTerminate:=true;                  (*
  end
  else begin play.riprendi; play.Resume; end;*)
  btplay.Enabled:=false;
  btpause.Enabled:=true;
  btstop.Enabled:=true;
end;

procedure TForm1.btstopClick(Sender: TObject);
begin
  if play<>nil then begin
                      play.Terminate;
                      play:=nil;
                    end;
  btplay.Enabled:=true;
  btpause.Enabled:=false;
  btstop.Enabled:=false;
end;

procedure TForm1.btpauseClick(Sender: TObject);
begin
  if play<>nil then
  if play.Suspended then begin
                           play.riprendi;
                           play.Resume;
                           btplay.Enabled:=false;
                           btpause.Enabled:=true;
                           btstop.enabled:=true;
                         end
  else
  begin
  play.Suspend;
  btplay.Enabled:=false;
  btpause.Enabled:=true;
  btstop.enabled:=false;
  end;
end;

procedure TForm1.navlistDblClick(Sender: TObject);
var i:integer; name,desc:array[0..200] of char;
begin
  speedbutton1.Click;
  speedbutton2.Click;
  if (labcur.Caption='Classes/Ct') then
  begin
    form3.name.Text:='';
    form3.desc.Text:='';
    form3.cttype.Caption:=navlist.Items[navlist.itemindex];
    form3.showmodal;
  end;
  if (labcur.Caption='Classes/Efx') then
  begin
    form4.name.Text:='';
    form4.desc.Text:='';
    form4.cttype.Caption:=navlist.Items[navlist.itemindex];
    form4.showmodal;
  end;
  if (labcur.Caption='Objects/Ct') then
  begin
    label9.Caption:=navlist.Items[navlist.itemindex];
    if ADSYSTEM_ObjCtCanLoadAndSave(pchar(label9.caption)) then modify.visible:=true
    else modify.Visible:=false;
    listbox1.Clear;
    for i:=0 to ADSYSTEM_GetObjCtNum(SYS_OBJECTS_CT,pchar(label9.caption))-1
    do
    begin
      ADSYSTEM_GetObjCt(SYS_OBJECTS_CT,pchar(label9.caption),i,name,desc);
      listbox1.Items.Add(name);
    end;
    combobox1.Items.Clear;
    for i:=0 to ADSYSTEM_GetNum(SYS_OBJECTS_CT)-1
         do begin
              ADSYSTEM_Get(SYS_OBJECTS_CT,i,name,desc);
              combobox1.Items.Add(name);
            end;
    Panel11.Align:=alClient;
    Panel11.Visible:=true;
  end;
  if (labcur.Caption='Objects/Efx') then
  begin
    efxname.Caption:=navlist.Items[navlist.itemindex];
    efxctbox.Clear;
    for i:=0 to ADSYSTEM_GetObjCtNum(SYS_OBJECTS_EFX,pchar(efxname.caption))-1
    do
    begin
      ADSYSTEM_GetObjCt(SYS_OBJECTS_EFX,pchar(efxname.caption),i,name,desc);
      efxctbox.Items.Add(name);
    end;
    setctbox.Items.Clear;
    for i:=0 to ADSYSTEM_GetNum(SYS_OBJECTS_CT)-1
         do begin
              ADSYSTEM_Get(SYS_OBJECTS_CT,i,name,desc);
              setctbox.Items.Add(name);
            end;
    PanelEfx.Align:=alClient;
    PanelEfx.Visible:=true;
  end;

end;

procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  label5.Visible:=false;
  panelEfx.Align:=alNone;
  panelEfx.Visible:=false;
  setctbox.Visible:=false;
  setctbox.Enabled:=false;
  image3.Visible:=false;
end;

procedure TForm1.efxctboxClick(Sender: TObject);
begin
  label5.Visible:=true;
  setctbox.itemindex:=-1;
  setctbox.Visible:=true;
  setctbox.Enabled:=true;
  image3.Visible:=true;
end;


procedure TForm1.PaintBox1Paint(Sender: TObject);
begin
  with paintbox1.Canvas do
  begin
    brush.Style:=bssolid;
    brush.Color:=clsilver;
    rectangle(0,0,paintbox1.width-1,paintbox1.height-1);
  end;
end;

procedure TForm1.setctboxChange(Sender: TObject);
begin
  if setctbox.itemindex>=0 then
  begin
  ADSYSTEM_SetObjCt(SYS_OBJECTS_EFX,
                    pchar(navlist.items[navlist.itemindex]),
                    pchar(efxctbox.Items[efxctbox.itemindex]),
                    pchar(setctbox.Items[setctbox.itemindex]));
  ADSYSTEM_Update;
  label5.Visible:=false;
  setctbox.Visible:=false;
  setctbox.Enabled:=false;
  image3.Visible:=false;
  end;
end;

procedure TForm1.ListBox1Click(Sender: TObject);
begin
  label8.Visible:=true;
  combobox1.itemindex:=-1;
  combobox1.Visible:=true;
  combobox1.Enabled:=true;
  image4.Visible:=true;
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
begin
  label8.Visible:=false;
  panel11.Align:=alNone;
  panel11.Visible:=false;
  combobox1.Visible:=false;
  combobox1.Enabled:=false;
  image4.Visible:=false;
end;

procedure TForm1.ComboBox1Change(Sender: TObject);
begin
  if combobox1.itemindex>=0 then
  begin
  ADSYSTEM_SetObjCt(SYS_OBJECTS_CT,
                    pchar(navlist.items[navlist.itemindex]),
                    pchar(listbox1.Items[listbox1.itemindex]),
                    pchar(combobox1.Items[combobox1.itemindex]));
  ADSYSTEM_Update;
  label8.Visible:=false;
  combobox1.Visible:=false;
  combobox1.Enabled:=false;
  image4.Visible:=false;
  end;
end;

procedure TForm1.ctboxDropDown(Sender: TObject);
var name,desc:array[0..200] of char; i:integer;
begin
  ctbox.Items.Clear;
  for i:=0 to ADSYSTEM_GetNum(SYS_OBJECTS_CT)-1
         do begin
              ADSYSTEM_Get(SYS_OBJECTS_CT,i,name,desc);
              ctbox.Items.Add(name);
            end;
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
begin
  paintbox1.Invalidate;
end;

procedure TForm1.ctboxChange(Sender: TObject);
begin
  paintbox1.Invalidate;
end;

procedure TForm1.modifyClick(Sender: TObject);
type TEdit = function(var data,size:pointer):boolean;
var CName:pchar;
    Edit:TEdit;
    LibraryHandle:THandle;
    data,size:pointer;
    allocsize:^integer;
begin
  CName:=ADSYSTEM_ObjCtClassName(pchar(label9.caption));
  LibraryHandle:= LoadLibrary(pchar(string(CName)+'.edt'));
  new(allocsize);
  size:=allocsize;
  ADSYSTEM_ObjCtSaveToMemory(pchar(label9.caption),data,size);
  if LibraryHandle > HINSTANCE_ERROR then
  begin
    @Edit:= GetProcAddress(LibraryHandle,'Edit');
    if Edit(data,size)
    then begin
           allocsize:=size;
           ADSYSTEM_ObjCtLoadFromMemory(pchar(label9.caption),data,allocsize^);
         end;
    FreeLibrary(LibraryHandle);
  end;
end;

end.



