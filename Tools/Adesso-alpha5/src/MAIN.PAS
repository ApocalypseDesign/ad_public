unit Main;

interface

uses Windows, SysUtils, Classes, Graphics, Forms, Controls, Menus,
  StdCtrls, Dialogs, Buttons, Messages, ExtCtrls, ComCtrls, Here3DS;

type
  TMainForm = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    FileNewItem: TMenuItem;
    FileOpenItem: TMenuItem;
    FileCloseItem: TMenuItem;
    Window1: TMenuItem;
    Help1: TMenuItem;
    N1: TMenuItem;
    FileExitItem: TMenuItem;
    WindowCascadeItem: TMenuItem;
    WindowTileItem: TMenuItem;
    WindowArrangeItem: TMenuItem;
    HelpAboutItem: TMenuItem;
    OpenDialog: TOpenDialog;
    FileSaveItem: TMenuItem;
    FileSaveAsItem: TMenuItem;
    Edit1: TMenuItem;
    CutItem: TMenuItem;
    CopyItem: TMenuItem;
    PasteItem: TMenuItem;
    WindowMinimizeItem: TMenuItem;
    SpeedPanel: TPanel;
    OpenBtn: TSpeedButton;
    SaveBtn: TSpeedButton;
    CutBtn: TSpeedButton;
    CopyBtn: TSpeedButton;
    PasteBtn: TSpeedButton;
    StatusBar: TStatusBar;
    SaveDialog1: TSaveDialog;
    NewBtn: TSpeedButton;
    ImpMatBtn: TSpeedButton;
    Imp3DS: TOpenDialog;
    insFile: TSpeedButton;
    insfiled: TOpenDialog;
    Image1: TImage;
    Bevel1: TBevel;
    Bevel2: TBevel;
    procedure FormCreate(Sender: TObject);
    procedure FileNewItemClick(Sender: TObject);
    procedure WindowCascadeItemClick(Sender: TObject);
    procedure UpdateMenuItems(Sender: TObject);
    procedure WindowTileItemClick(Sender: TObject);
    procedure WindowArrangeItemClick(Sender: TObject);
    procedure FileCloseItemClick(Sender: TObject);
    procedure FileOpenItemClick(Sender: TObject);
    procedure FileExitItemClick(Sender: TObject);
    procedure FileSaveItemClick(Sender: TObject);
    procedure FileSaveAsItemClick(Sender: TObject);
    procedure CutItemClick(Sender: TObject);
    procedure CopyItemClick(Sender: TObject);
    procedure PasteItemClick(Sender: TObject);
    procedure WindowMinimizeItemClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure HelpAboutItemClick(Sender: TObject);
    procedure NewBtnClick(Sender: TObject);
    procedure CutBtnClick(Sender: TObject);
    procedure CopyBtnClick(Sender: TObject);
    procedure PasteBtnClick(Sender: TObject);
    procedure ImpMatBtnClick(Sender: TObject);
    procedure insFileClick(Sender: TObject);
  private
    { Private declarations }
    procedure CreateMDIChild(const Name: string);
    procedure ShowHint(Sender: TObject);
  public
    { Public declarations }
  end;

var
  MainForm: TMainForm;

implementation

{$R *.DFM}

uses ChildWin, About;

procedure TMainForm.FormCreate(Sender: TObject);
begin
  Application.OnHint := ShowHint;
  Screen.OnActiveFormChange := UpdateMenuItems;
end;

procedure TMainForm.ShowHint(Sender: TObject);
begin
  StatusBar.SimpleText := Application.Hint;
end;

procedure TMainForm.CreateMDIChild(const Name: string);
var
  Child: TMDIChild;
begin
  { create a new MDI child window }
  Child := TMDIChild.Create(Application);
  Child.Caption := Name;
end;

procedure TMainForm.FileNewItemClick(Sender: TObject);
begin
  CreateMDIChild('Senza Nome ' + IntToStr(MDIChildCount + 1));
end;

procedure TMainForm.FileOpenItemClick(Sender: TObject);
begin
  if OpenDialog.Execute then
   begin
    CreateMDIChild(OpenDialog.FileName);
    TMDIChild(ActiveMDIChild).RE.Lines.LoadFromFile(Opendialog.Filename);
    TMDIChild(ActiveMDIChild).RE.modified:=false;
    TMDIChild(ActiveMDIChild).RE.Repaint;
   end;
end;

procedure TMainForm.FileCloseItemClick(Sender: TObject);
begin
  if ActiveMDIChild <> nil then
    ActiveMDIChild.Close;
end;

procedure TMainForm.FileSaveItemClick(Sender: TObject);
begin
  { save current file (ActiveMDIChild points to the window) }
  if TMDIChild(ActiveMDIChild).caption[2]<>':'
  then FileSaveAsItemClick(Sender)
  else begin
         TMDIChild(ActiveMDIChild).RE.Lines.SaveToFile(TMDIChild(ActiveMDIChild).caption);
         TMDIChild(ActiveMDIChild).RE.Modified:=false;
       end;  
end;

procedure TMainForm.FileSaveAsItemClick(Sender: TObject);
begin
  { save current file under new name }
  if savedialog1.execute then
  begin
   TMDIChild(ActiveMDIChild).caption:=savedialog1.filename;
   TMDIChild(ActiveMDIChild).RE.Lines.SaveToFile(savedialog1.FileName);
   TMDIChild(ActiveMDIChild).RE.Modified:=false;
  end;
end;

procedure TMainForm.FileExitItemClick(Sender: TObject);
begin
  Close;
end;

procedure TMainForm.CutItemClick(Sender: TObject);
begin
  {cut selection to clipboard}
  TMDIChild(ActiveMDIChild).RE.CutToClipboard;
end;

procedure TMainForm.CopyItemClick(Sender: TObject);
begin
  {copy selection to clipboard}
  TMDIChild(ActiveMDIChild).RE.CopyToClipboard;
end;

procedure TMainForm.PasteItemClick(Sender: TObject);
begin
  {paste from clipboard}
  TMDIChild(ActiveMDIChild).RE.PasteFromClipboard;
end;

procedure TMainForm.WindowCascadeItemClick(Sender: TObject);
begin
  Cascade;
end;

procedure TMainForm.WindowTileItemClick(Sender: TObject);
begin
  Tile;
end;

procedure TMainForm.WindowArrangeItemClick(Sender: TObject);
begin
  ArrangeIcons;
end;

procedure TMainForm.WindowMinimizeItemClick(Sender: TObject);
var
  I: Integer;
begin
  { Must be done backwards through the MDIChildren array }
  for I := MDIChildCount - 1 downto 0 do
    MDIChildren[I].WindowState := wsMinimized;
end;

procedure TMainForm.UpdateMenuItems(Sender: TObject);
begin
  FileCloseItem.Enabled := MDIChildCount > 0;
  FileSaveItem.Enabled := MDIChildCount > 0;
  FileSaveAsItem.Enabled := MDIChildCount > 0;
  CutItem.Enabled := MDIChildCount > 0;
  CopyItem.Enabled := MDIChildCount > 0;
  PasteItem.Enabled := MDIChildCount > 0;
  SaveBtn.Enabled := MDIChildCount > 0;
  CutBtn.Enabled := MDIChildCount > 0;
  CopyBtn.Enabled := MDIChildCount > 0;
  PasteBtn.Enabled := MDIChildCount > 0;
  ImpMatBtn.Enabled:= MDIChildCount > 0;
  insfile.enabled:=MDIChildCount>0;
  WindowCascadeItem.Enabled := MDIChildCount > 0;
  WindowTileItem.Enabled := MDIChildCount > 0;
  WindowArrangeItem.Enabled := MDIChildCount > 0;
  WindowMinimizeItem.Enabled := MDIChildCount > 0;
end;

procedure TMainForm.FormDestroy(Sender: TObject);
begin
  Screen.OnActiveFormChange := nil;
end;

procedure TMainForm.HelpAboutItemClick(Sender: TObject);
begin
  AboutBox.ShowModal;
end;

procedure TMainForm.NewBtnClick(Sender: TObject);
begin
  FileNewItemClick(sender);
end;

procedure TMainForm.CutBtnClick(Sender: TObject);
begin
  CutItemClick(sender);
end;

procedure TMainForm.CopyBtnClick(Sender: TObject);
begin
  CopyItemClick(sender);
end;

procedure TMainForm.PasteBtnClick(Sender: TObject);
begin
  PasteItemClick(sender);
end;

procedure TMainForm.ImpMatBtnClick(Sender: TObject);
var miofile:charp; mioofs,mioofs2,mioofs3,i,sz:integer;
    st:string;
    texfound,envfound:boolean;
begin
  if imp3ds.execute then
  begin
    savedialog1.DefaultExt:='adm';
    savedialog1.FilterIndex:=2;
    if TMDIChild(ActiveMDIChild).caption[2]<>':'
       then savedialog1.FileName:=copy(imp3ds.FileName,1,length(imp3ds.filename)-3)+'adm';
    TMDIChild(ActiveMDIChild).RE.Lines.Add('// *** Apocalypse Design Scripting System v1.0 ***');
    TMDIChild(ActiveMDIChild).RE.Lines.Add('// Material script auto-created by ADESSO from "'+extractfilename(imp3ds.filename)+'"');
    TMDIChild(ActiveMDIChild).RE.Lines.Add('// Design is simply all...');
    TMDIChild(ActiveMDIChild).RE.Lines.Add('');
    miofile:=alloc_3DS(imp3ds.filename);
    mioofs:=find_first_subchunk_oftype(miofile,0,EDIT3DS);
    sz:=get_chunk_fullsize(miofile,mioofs)+mioofs;
    mioofs:=find_first_subchunk_oftype(miofile,mioofs,EDIT_MATERIAL);
    while (mioofs<>-1) and (mioofs<sz) do
    begin
      texfound:=false;
      envfound:=false;
      mioofs2:=find_first_subchunk_oftype(miofile,mioofs,MAT_NAME);
      st:='';
      i:=0;
      while charp(integer(miofile)+mioofs2+6+i)^<>#0 do begin
                              st:=st+charp(integer(miofile)+mioofs2+6+i)^;
                              inc(i);
                            end;
      TMDIChild(ActiveMDIChild).RE.Lines.Add('  material "'+st+'"');
      TMDIChild(ActiveMDIChild).RE.Lines.Add('  {');
      mioofs2:=find_first_subchunk_oftype(miofile,mioofs,MAT_MAP1);
      if mioofs2=-1 then mioofs2:=find_first_subchunk_oftype(miofile,mioofs,MAT_MAP2);
      mioofs3:=mioofs2;
      if mioofs2<>-1 then mioofs2:=find_first_subchunk_oftype(miofile,mioofs2,MAP_FILE);
      if mioofs2<>-1 then
      begin
       st:='';
       i:=0;
       while charp(integer(miofile)+mioofs2+6+i)^<>#0 do begin
                              st:=st+charp(integer(miofile)+mioofs2+6+i)^;
                              inc(i);
                            end;
       TMDIChild(ActiveMDIChild).RE.Lines.Add('    texture="'+st+'";');
       texfound:=true;
      end;
      if mioofs3<>-1 then mioofs2:=find_first_subchunk_oftype(miofile,mioofs3,MAP_REFLECT);
      if mioofs2<>-1 then mioofs2:=find_first_subchunk_oftype(miofile,mioofs2,MAP_FILE);
      if mioofs2<>-1 then
      begin
       st:='';
       i:=0;
       while charp(integer(miofile)+mioofs2+6+i)^<>#0 do begin
                              st:=st+charp(integer(miofile)+mioofs2+6+i)^;
                              inc(i);
                            end;
       TMDIChild(ActiveMDIChild).RE.Lines.Add('    envmap="'+st+'";');
       envfound:=true;
      end;
      if texfound and envfound
         then TMDIChild(ActiveMDIChild).RE.Lines.Add('    shading="TEXENV";')
         else if texfound then TMDIChild(ActiveMDIChild).RE.Lines.Add('    shading="TEX";')
                          else if envfound then TMDIChild(ActiveMDIChild).RE.Lines.Add('    shading="ENV";')
                                           else TMDIChild(ActiveMDIChild).RE.Lines.Add('    shading="WIRE";');
      TMDIChild(ActiveMDIChild).RE.Lines.Add('  }');
      TMDIChild(ActiveMDIChild).RE.Lines.Add('');
      mioofs:=find_next_chunk_oftype(miofile,mioofs,EDIT_MATERIAL);
    end;
  end;
end;

function nibbletohex(n:byte):string;
begin
  if n<10 then result:=inttostr(n)
  else result:=chr(ord('A')+n-10);
end;

function inttohex(n:longint):string;
var s:string;
begin
  s:=nibbletohex((n shr 28) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 24) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 20) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 16) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 12) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 8) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex((n shr 4) and $F);
  if s='0' then s:='';
  s:=s+nibbletohex(n and $F);
  result:=s;
end;

function FileNameToArrayName(fn:string):string;
var s:string; c:char; i:integer;
begin
  s:='file_';
  for i:=1 to length(fn) do
   begin
     c:=upcase(fn[i]);
     if c=' ' then c:='_';
     if c='.' then c:='_';
     s:=s+c;
   end;
  result:=s;
end;

procedure TMainForm.insFileClick(Sender: TObject);
var f:file of byte; ts,shex:string; l:longint;
    ns:integer; b1,b2,b3,b4:byte;
begin
  if insfiled.execute then
  begin
    savedialog1.DefaultExt:='h';
    savedialog1.FilterIndex:=3;
    if TMDIChild(ActiveMDIChild).caption[2]<>':'
       then savedialog1.FileName:=copy(insfiled.FileName,1,length(insfiled.filename)-3)+'h';
    TMDIChild(ActiveMDIChild).RE.Lines.Add('');
    assignfile(f,insfiled.filename);
    reset(f);
    TMDIChild(ActiveMDIChild).RE.Lines.Add('#ifndef _'+
    filenametoarrayname(extractfilename(insfiled.filename))+'_');
    TMDIChild(ActiveMDIChild).RE.Lines.Add('#define _'+
    filenametoarrayname(extractfilename(insfiled.filename))+'_');

    TMDIChild(ActiveMDIChild).RE.Lines.Add(' unsigned long int size_'
    +filenametoarrayname(extractfilename(insfiled.filename))+' = '+
    inttostr(filesize(f))+'; //dim in bytes');

    TMDIChild(ActiveMDIChild).RE.Lines.Add(' unsigned long int '
    +filenametoarrayname(extractfilename(insfiled.filename))+'[] = { ');

    ts:='   ';
    ns:=0;
    read(f,b1);
    read(f,b2);
    read(f,b3);
    read(f,b4);
    l:=(b4 shl 24)+ (b3 shl 16)+ (b2 shl 8)+ b1;
    while not(eof(f)) do
    begin
      shex:='0x'+inttohex(l);
      if (length(shex)<=length(inttostr(l))) or (l<0) then ts:=ts+shex+','
         else ts:=ts+inttostr(l)+',';
      inc(ns);
      if length(ts)>=68 then begin
                      TMDIChild(ActiveMDIChild).RE.Lines.Add(ts);
                      ts:='   ';
                      ns:=0;
                    end;
      if not(eof(f)) then read(f,b1) else b1:=0;
      if not(eof(f)) then read(f,b2) else b2:=0;
      if not(eof(f)) then read(f,b3) else b3:=0;
      if not(eof(f)) then read(f,b4) else b4:=0;
      l:=(b4 shl 24)+ (b3 shl 16)+ (b2 shl 8)+ b1;
    end;
    closefile(f);
    if ns=0 then ts:='  ';
    ts:=ts+' 0 };';
    TMDIChild(ActiveMDIChild).RE.Lines.Add(ts);
    TMDIChild(ActiveMDIChild).RE.Lines.Add('#endif');
    TMDIChild(ActiveMDIChild).RE.Lines.Add('');
  end;
end;


end.
