unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, ComCtrls, ExtCtrls, Menus, StdCtrls, ADsystem, EfxList, ChnList,
  rImgList, efxinfo, efxchn, preview, shared, uAbout, Trackv,
  ExtDlgs, efximg, jpeg;

type
  TEfxButton=class(TSpeedButton)
       public
       efxName,efxDesc:string;
       efxNum:integer;
       procedure Click; override;
     end;

     (*
  TCtrButton=class(TSpeedButton)
       public
       ctrName,ctrDesc:string;
       ctrNum:integer;
       procedure Click; override;
     end;*)

  TForm1 = class(TForm)
    MainMenu1: TMainMenu;
    File1: TMenuItem;
    Modifica1: TMenuItem;
    Visualizza1: TMenuItem;
    Aiuto1: TMenuItem;
    Nuovo1: TMenuItem;
    Apri1: TMenuItem;
    Salva1: TMenuItem;
    N1: TMenuItem;
    Esci1: TMenuItem;
    MainPanel: TPanel;
    Pages: TPageControl;
    MainSheet: TTabSheet;
    EfxSheet: TTabSheet;
    SpeedButton1: TSpeedButton;
    FPanel: TPanel;
    SpeedButton3: TSpeedButton;
    SpeedButton4: TSpeedButton;
    SpeedButton5: TSpeedButton;
    Bevel1: TBevel;
    SpeedButton6: TSpeedButton;
    Elimina1: TMenuItem;
    SaveDialog1: TSaveDialog;
    OpenDialog1: TOpenDialog;
    Principale: TTabSheet;
    Panelx2: TPanel;
    Label9: TLabel;
    Label11: TLabel;
    Label12: TLabel;
    Informazioni1: TMenuItem;
    Anteprima1: TMenuItem;
    N2: TMenuItem;
    Mostraanteprimaimmediata1: TMenuItem;
    Spostasu1: TMenuItem;
    Spostagiu1: TMenuItem;
    N3: TMenuItem;
    Images: TTabSheet;
    ImportImg: TSpeedButton;
    OpenPictureDialog1: TOpenPictureDialog;
    Panel4: TPanel;
    App: TScrollBox;
    MainPn: TPanel;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Label6: TLabel;
    Label7: TLabel;
    Image1: TImage;
    Edit1: TEdit;
    Edit2: TEdit;
    Edit3: TEdit;
    Edit4: TEdit;
    Edit5: TEdit;
    memo1: TMemo;
    CheckBox1: TCheckBox;
    Properties: TPanel;
    SProp: TScrollBox;
    Splitter1: TSplitter;
    PanelTrack: TPanel;
    ScrollBoxTrack: TScrollBox;
    Panel3: TPanel;
    SBkeyDELETE: TSpeedButton;
    SBkeyMOVE: TSpeedButton;
    SBkeyADD: TSpeedButton;
    StatusBar: TStatusBar;
    AniPanel: TPanel;
    TimeBar: TTrackBar;
    CTPopup: TPopupMenu;
    NewCT: TMenuItem;
    ImportCT: TMenuItem;
    ExportCT: TMenuItem;
    ExportCTDialog: TSaveDialog;
    ImportCTDialog: TOpenDialog;
    EFXPopup: TPopupMenu;
    EFXmoveUP: TMenuItem;
    EFXmoveDOWN: TMenuItem;
    N4: TMenuItem;
    EFXdelete: TMenuItem;
    N5: TMenuItem;
    CHNPopup: TPopupMenu;
    CHNmoveUP: TMenuItem;
    CHNmoveDOWN: TMenuItem;
    N6: TMenuItem;
    CHNdelete: TMenuItem;
    Modifica2: TMenuItem;
    N7: TMenuItem;
    Panel1: TPanel;
    Panel2: TPanel;
    Image2: TImage;
    SpeedButton2: TSpeedButton;
    Label8: TLabel;
    OpenAudioDialog: TOpenDialog;
    Button1: TButton;
    procedure FormCreate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure PagesChange(Sender: TObject);
    procedure Esci1Click(Sender: TObject);
    procedure SpeedButton1Click(Sender: TObject);
    procedure SpeedButton6Click(Sender: TObject);
    procedure Salva1Click(Sender: TObject);
    procedure SpeedButton5Click(Sender: TObject);
    procedure Nuovo1Click(Sender: TObject);
    procedure SpeedButton3Click(Sender: TObject);
    procedure Apri1Click(Sender: TObject);
    procedure SpeedButton4Click(Sender: TObject);
    procedure TimeBarChange(Sender: TObject);
    procedure SpeedButton2Click(Sender: TObject);
    procedure TimeBarStartDrag(Sender: TObject;
      var DragObject: TDragObject);
    procedure Edit1Change(Sender: TObject);
    procedure Edit2Change(Sender: TObject);
    procedure Edit3Change(Sender: TObject);
    procedure Edit5Change(Sender: TObject);
    procedure Edit4Change(Sender: TObject);
    procedure Edit4KeyPress(Sender: TObject; var Key: Char);
    procedure Anteprima1Click(Sender: TObject);
    procedure Informazioni1Click(Sender: TObject);
    procedure AudioTrackSelect(Sender: TObject);
    procedure SpeedButton8Click(Sender: TObject);
    procedure SpeedButton9Click(Sender: TObject);
    procedure ImportImgClick(Sender: TObject);
    procedure NewCTClick(Sender: TObject);
    procedure ExportCTClick(Sender: TObject);
    procedure ImportCTClick(Sender: TObject);
    procedure EFXdeleteClick(Sender: TObject);
    procedure EFXmoveUPClick(Sender: TObject);
    procedure EFXmoveDOWNClick(Sender: TObject);
    procedure EFXPopupPopup(Sender: TObject);
    procedure CHNPopupPopup(Sender: TObject);
    procedure CHNmoveUPClick(Sender: TObject);
    procedure CHNmoveDOWNClick(Sender: TObject);
    procedure CHNdeleteClick(Sender: TObject);
    procedure Modifica2Click(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure SBkeyADDClick(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    efxList:TEfxList;
    chnList:TChnList;
    efxInfo:TEfxInfo;
    efxChn:TEfxChn;
    efxImg:TEfxImg;
    trackv:TTrackv;
    rimgList:TrImgList;
    PreviewShowing:boolean;
    demolength:integer;
    EfxBtnArray:array of TEfxButton;
  end;

var
  Form1: TForm1;

implementation

uses UnitCTset;

{$R *.DFM}

procedure TEfxButton.Click;
begin
  ADSYSTEM_CLevel_SetCurrent(efxNum);
  ADSYSTEM_Level_Create;
  ADSYSTEM_Level_SetCurrent(ADSYSTEM_Level_Count-1);
  form1.efxList.selected:=ADSYSTEM_Level_Count-1;
  form1.efxList.Invalidate;
  form1.efxChn.efxnum:=form1.efxList.selected;
  form1.efxChn.Invalidate;
  form1.efxImg.efxnum:=form1.efxList.selected;
  form1.efxImg.Invalidate;
  form1.efxInfo.efxnum:=form1.efxList.selected;
  form1.efxInfo.Invalidate;
  form1.trackv.Invalidate;
  ADSYSTEM_Update;
end;

(*
procedure TCtrButton.Click;
begin
  ADSYSTEM_CControl_SetCurrent(ctrNum);
  ADSYSTEM_LevelControl_Create;
end;             *)

procedure TForm1.FormCreate(Sender: TObject);
var efxNum,i:integer;
    rect:Trect;
    pname,pdesc:pchar;
    name,desc:string;
    bmp:tbitmap;
begin
  demolength:=20;
  SpeedButton1.Flat:=FlatButtons;
  image2.ControlStyle:= [csOpaque];
  chdir(extractfiledir(paramstr(0))); // forza la directory corrente

  //carico la DLL ADsystem.dll
  //chdir('efx');
  ADSYSTEM_Init(320,240);
  //chdir('..');

  chdir('chn');
  if fileexists('chn.bmp') then
  begin
    bmp:=TBitmap.Create;
    bmp.LoadFromFile('chn.bmp');
    SpeedButton1.Glyph:=bmp;
  end;

  chdir('..');

  //vado nella dir degli Efx
  chdir('efx');
  //aggiungo i pulsanti degli efx
  efxNum:=ADSYSTEM_CLevel_Count;
  SetLength(EfxBtnArray,efxNum);
  for i:=0 to efxNum-1 do
  begin
    efxBtnArray[i]:=TEfxButton.Create(EfxSheet);
    //efxBtn.Flat:=true;
    rect.Top:=1;
    rect.Left:=1+i*36;
    rect.Right:=37+i*36;
    rect.Bottom:=37;
    ADSYSTEM_CLevel_SetCurrent(i);
    ADSYSTEM_CLevel_GetInfo(pname,pdesc);
    name:=pname;
    desc:=pdesc;
    desc[1]:=upcase(desc[1]);
    efxBtnArray[i].Hint:=desc;
    efxBtnArray[i].ShowHint:=true;
    efxBtnArray[i].efxName:=name;
    efxBtnArray[i].efxDesc:=desc;
    efxBtnArray[i].efxNum:=i;
    efxBtnArray[i].Flat:=FlatButtons;
    if FileExists(name+'.bmp') then
    begin
      bmp:=Tbitmap.Create;
      bmp.LoadFromFile(name+'.bmp');
      efxBtnArray[i].Glyph:=bmp;
    end else efxBtnArray[i].Caption:=name;
    efxBtnArray[i].Dock(EfxSheet,rect);
  end;
  //vado nella dir principale
  chdir('..');
  (*
  //vado nella dir dei Ctr
  chdir('ctr');
  //aggiungo i pulsanti dei ctr
  ctrNum:=ADSYSTEM_CControl_Count;
  SetLength(CtrBtnArray,ctrNum);
  for i:=0 to ctrNum-1 do
  begin
    ctrBtnArray[i]:=TCtrButton.Create(Controllers);
    //efxBtn.Flat:=true;
    rect.Top:=1;
    rect.Left:=1+i*36;
    rect.Right:=37+i*36;
    rect.Bottom:=37;
    ADSYSTEM_CControl_SetCurrent(i);
    ADSYSTEM_CControl_GetInfo(pname,pdesc);
    name:=pname;
    desc:=pdesc;
    desc[1]:=upcase(desc[1]);
    ctrBtnArray[i].Hint:=desc;
    ctrBtnArray[i].ShowHint:=true;
    ctrBtnArray[i].ctrName:=name;
    ctrBtnArray[i].ctrDesc:=desc;
    ctrBtnArray[i].ctrNum:=i;
    ctrBtnArray[i].Flat:=FlatButtons;
    if FileExists(name+'.bmp') then
    begin
      bmp:=Tbitmap.Create;
      bmp.LoadFromFile(name+'.bmp');
      ctrBtnArray[i].Glyph:=bmp;
    end else ctrBtnArray[i].Caption:=name;
    ctrBtnArray[i].Dock(Controllers,rect);
  end;
  //torno nella dir principale
  chdir('..');
  *)
  //metto davanti la pagina principale
  Pages.ActivePageIndex:=PAGES_MAIN;
  //creo la efxlist
  EfxList:=TEfxList.Create(App);
  rect.top:=10; rect.Left:=10;
  rect.right:=135; rect.Bottom:=30;
  EfxList.Dock(App,rect);
  EfxList.Hide;

  //creo la chnlist
  ChnList:=TChnList.Create(App);
  rect.top:=10; rect.Left:=10;
  rect.right:=600; rect.Bottom:=300;
  ChnList.Dock(App,rect);
  ChnList.Hide;
  //creo la imglist
  rImgList:=TrImgList.Create(App);
  rect.top:=10; rect.Left:=10;
  rect.right:=600; rect.Bottom:=300;
  rImgList.Dock(App,rect);
  rImgList.Hide;

   //creo la efxInfo
  efxInfo:=TEfxInfo.Create(SProp);
  rect.top:=5; rect.Left:=5;
  rect.Right:=Properties.Width-5;
  rect.Bottom:=50;
  efxInfo.Dock(SProp,rect);

  //creo la efxChn
  efxChn:=TEfxChn.Create(SProp);
  rect.top:=60; rect.Left:=5;
  rect.Right:=Properties.Width-5;
  rect.Bottom:=Properties.Height-20;
  efxChn.Dock(SProp,rect);

  //creo la efxImg
  efxImg:=TEfxImg.Create(SProp);
  rect.top:=160; rect.Left:=5;
  rect.Right:=Properties.Width-5;
  rect.Bottom:=Properties.Height-20;
  efxImg.Dock(SProp,rect);

  //creo la TrackView
  trackv:=TTrackv.Create(ScrollBoxTrack);
  rect.top:=10; rect.Left:=10;
  rect.right:=611; rect.Bottom:=300;
  Trackv.Dock(ScrollBoxTrack,rect);
  //Trackv.Hide;

  SpeedButton2.Click;

  bmp:=TBitmap.Create;
  bmp.Width:=image2.Width;
  bmp.Height:=image2.Height;
  bmp.PixelFormat:=pf32bit;
  bmp.Canvas.Brush.Color:=clBlack;
  rect.Left:=1;rect.Top:=1;
  rect.Right:=319;rect.Bottom:=239;
  bmp.Canvas.Brush.Style:=bsSolid;
  bmp.Canvas.FillRect(rect);
  bmp.Canvas.Font.Color:=clWhite;
  bmp.Canvas.TextOut(10,10,'Anteprima Immediata');
  image2.Picture.Bitmap:=bmp;
  pages.OnChange(Self);
end;

procedure TForm1.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  ADSYSTEM_Close;
end;

procedure TForm1.PagesChange(Sender: TObject);
begin
  //OnResize(self);
  form1.invalidate;
  case Pages.ActivePageIndex of
  PAGES_CHANNELS:
     begin
       statusbar.SimplePanel:=false;
       panelx2.Visible:=false;
       splitter1.Visible:=false;
       if efxlist.Visible then chnList.selected:=efxList.ChannelBox.ItemIndex;
       mainpn.Visible:=false;
       efxlist.Visible:=false;
       chnlist.Visible:=true;
       rimglist.Visible:=false;
       paneltrack.Visible:=false;
       properties.Visible:=false;
       app.Color:=clAppWorkspace;
       anipanel.Visible:=true;
       panel1.Visible:=true;
     end;
  PAGES_LEVELS:
     begin
       statusbar.SimplePanel:=false;
       //statusbar.Align:=alBottom;
       anipanel.Visible:=true;
       paneltrack.Visible:=true;
       panelx2.Visible:=true;
       mainpn.Visible:=false;
       efxlist.RefreshChannelBox(chnlist.selected);
       efxlist.Visible:=true;
       chnlist.Visible:=false;
       rimglist.Visible:=false;
       properties.Visible:=true;
       app.Color:=clAppWorkspace;
       panel1.Visible:=true;
       splitter1.Visible:=true;
     end;
  PAGES_MAIN:
     begin
       statusbar.SimplePanel:=true;
       panelx2.Visible:=false;
       splitter1.Visible:=false;
       mainpn.Visible:=true;
       efxlist.Visible:=false;
       chnlist.Visible:=false;
       rimglist.Visible:=false;
       paneltrack.Visible:=false;
       properties.Visible:=false;
       app.Color:=clGray;
       anipanel.Visible:=false;
       panel4.align:=alClient;
       panel1.Visible:=false;
     end;
  PAGES_IMAGES:
     begin
       statusbar.SimplePanel:=true;
       panelx2.Visible:=false;
       splitter1.Visible:=false;
       mainpn.Visible:=false;
       efxlist.Visible:=false;
       chnlist.Visible:=false;
       rimglist.Visible:=true;
       paneltrack.Visible:=false;
       properties.Visible:=false;
       app.Color:=clAppWorkspace;
       anipanel.Visible:=false;
       panel1.Visible:=false;
     end;
  end;
end;

procedure TForm1.Esci1Click(Sender: TObject);
begin
  Close;
end;

procedure TForm1.SpeedButton1Click(Sender: TObject);
begin
  ADSYSTEM_Channel_Create;
  ChnList.Paint;
end;

procedure TForm1.SpeedButton6Click(Sender: TObject);
begin
  Prewin.showmodal;
end;

procedure TForm1.Salva1Click(Sender: TObject);
begin
 if savedialog1.Execute then ADSYSTEM_SaveToFile(pchar(savedialog1.filename));
end;

procedure TForm1.SpeedButton5Click(Sender: TObject);
begin
  Salva1Click(Sender);
end;

procedure TForm1.Nuovo1Click(Sender: TObject);
begin
  form1.Caption:='Visual ADESSO - SenzaNome.ad';
  savedialog1.FileName:='SenzaNome.ad';
  ADSYSTEM_Reset;
  ADSYSTEM_Channel_Create;
  ADSYSTEM_Channel_SetCurrent(0);
  pages.ActivePageIndex:=PAGES_MAIN;
  pages.OnChange(self);
  chnlist.selected:=0;
  Edit1.Text:='Nuova Animazione';
  Edit2.Text:='';
  Edit3.Text:='';
  Edit4.Text:='20';
  Edit5.Text:='';
  CheckBox1.Checked:=false;
  Memo1.Lines.Clear;
  timebar.Position:=0;
  app.repaint;
  edit1.SetFocus;
end;

procedure TForm1.SpeedButton3Click(Sender: TObject);
begin
  Nuovo1Click(sender);
end;

procedure TForm1.Apri1Click(Sender: TObject);
var chunkdata:pointer; chunksize:integer;
    chunkpchar:pchar;
    chunkinteger:^integer;
    chunkresult:integer;
begin
  if opendialog1.Execute then
  begin
    Nuovo1Click(self);
    Caption:='Visual ADESSO - '+opendialog1.FileName;
    savedialog1.FileName:=opendialog1.FileName;
    ADSYSTEM_LoadFromFile(pchar(opendialog1.filename));
    if ADSYSTEM_Channel_Count=0 then ADSYSTEM_Channel_Create;
    ADSYSTEM_Channel_SetCurrent(0);
    pages.ActivePageIndex:=PAGES_MAIN;
    pages.OnChange(self);
    chnlist.selected:=0;

    //chunks
    chunkresult:=ADSYSTEM_ChunkRead('NAME',chunkdata,chunksize);
    if chunkresult>-1 then
    begin
      chunkpchar:=chunkdata;
      Edit1.Text:=chunkpchar;
    end;
    chunkresult:=ADSYSTEM_ChunkRead('AUTHOR',chunkdata,chunksize);
    if chunkresult>-1 then
    begin
      chunkpchar:=chunkdata;
      Edit2.Text:=chunkpchar;
    end;
    chunkresult:=ADSYSTEM_ChunkRead('COPYRIGHT',chunkdata,chunksize);
    if chunkresult>-1 then
    begin
      chunkpchar:=chunkdata;
      Edit3.Text:=chunkpchar;
    end;
    chunkresult:=ADSYSTEM_ChunkRead('SOUNDTRACK',chunkdata,chunksize);
    if chunkresult>-1 then
    begin
      chunkpchar:=chunkdata;
      Edit5.Text:=chunkpchar;
    end;
    chunkresult:=ADSYSTEM_ChunkRead('LENGTH',chunkdata,chunksize);
    if chunkresult>-1 then
    begin
      chunkinteger:=chunkdata;
      Edit4.Text:=inttostr(chunkinteger^);
      demolength:=chunkinteger^;
    end;

    Timebar.Position:=0;
    app.repaint;
  end;
end;

procedure TForm1.SpeedButton4Click(Sender: TObject);
begin
  Apri1Click(Sender);
end;

procedure TForm1.TimeBarChange(Sender: TObject);
var src,dest:pchar;
y:integer;
  procedure memcpyd(dest,sorg:pointer; dwords:integer); assembler;
  asm
    push edi
    //push ecx   in delphi eax ecx edx non vanno preservati
    push esi
    mov edi,dest
    mov esi,sorg
    mov ecx,dwords
    rep movsd
    pop esi
    //pop ecx
    pop edi
  end;
begin
  if efxlist.Visible then efxlist.paint;
  if not(PreviewShowing) then
  begin
   src:=ADSYSTEM_Paint(Timebar.Position/TimeBar.Max);
   with Image2.Picture.Bitmap do
   begin
    for y:=0 to Height-1 do
    begin
     dest:=ScanLine[y];
     memcpyd(dest,src,width);
     src:=src+(width shl 2);
    end;
   end;
   Image2.Refresh;
  end;
  if Timebar.max=0 then timebar.max:=0;
  statusbar.Panels[0].Text:=floattostr(Timebar.Position*100.0/TimeBar.Max)+'%';
  statusbar.Panels[1].Text:=floattostr(Timebar.Position*demolength/TimeBar.Max)+'s';
end;

procedure TForm1.SpeedButton2Click(Sender: TObject);
const numpassi=18;
var i:integer;
begin
  if not(PreviewShowing) then //panel1.height:=16 //TSpeedButton(self).height
  begin
   for i:=numpassi-1 downto 0 do
   begin
    panel1.Height:=16+round((262.0-19.0)*i/numpassi);
    properties.Width:=188+round(138.0*i/numpassi);
   end;
   efxinfo.Width:=175;
   efxchn.Width:=175;
   efximg.Width:=175;
  end
  else
  begin
   for i:=1 to numpassi do
   begin
    panel1.Height:=16+round((262.0-19.0)*i/numpassi);
    properties.Width:=188+round(138.0*i/numpassi);
   end;
   efxinfo.Width:=295;
   efxchn.Width:=295;
   efximg.Width:=295;
  end;
  form1.resize;
  PreviewShowing:=not(PreviewShowing);
end;

procedure TForm1.TimeBarStartDrag(Sender: TObject;
  var DragObject: TDragObject);
begin
    ADSYSTEM_Update;
end;

procedure TForm1.Edit1Change(Sender: TObject);
var data:pchar;
begin
  data:=pchar(string(TEdit(Sender).text));
  ADSYSTEM_ChunkWrite('NAME',data,length(data)+1);
end;

procedure TForm1.Edit2Change(Sender: TObject);
var data:pchar;
begin
  data:=pchar(string(TEdit(Sender).text));
  ADSYSTEM_ChunkWrite('AUTHOR',data,length(data)+1);
end;

procedure TForm1.Edit3Change(Sender: TObject);
var data:pchar;
begin
  data:=pchar(string(TEdit(Sender).text));
  ADSYSTEM_ChunkWrite('COPYRIGHT',data,length(data)+1);
end;

procedure TForm1.Edit5Change(Sender: TObject);
var data:pchar;
begin
  data:=pchar(string(TEdit(Sender).text));
  ADSYSTEM_ChunkWrite('SOUNDTRACK',data,length(data)+1);
end;


procedure TForm1.Edit4Change(Sender: TObject);
var data:^integer;
begin
  new(data);
  if TEdit(Sender).text='' then data^:=0
  else data^:=strtoint(TEdit(Sender).text);
  demolength:=data^;
  ADSYSTEM_ChunkWrite('LENGTH',data,4);
  dispose(data);
end;

procedure TForm1.Edit4KeyPress(Sender: TObject; var Key: Char);
begin
    if (not((key>='0') and (key<='9'))) and (key<>#8) then key:=#0;
end;

procedure TForm1.Anteprima1Click(Sender: TObject);
begin
  SpeedButton6Click(Sender);
end;

procedure TForm1.Informazioni1Click(Sender: TObject);
begin
  Uabout.About.ShowModal;
end;

procedure TForm1.AudioTrackSelect(Sender: TObject);
begin
  if OpenAudioDialog.execute then
        Edit5.Text:=OpenAudioDialog.FileName;
end;

procedure TForm1.SpeedButton8Click(Sender: TObject);
begin
  Trackv.mode:=move;
end;

procedure TForm1.SpeedButton9Click(Sender: TObject);
begin
  Trackv.mode:=delete;
end;

procedure TForm1.ImportImgClick(Sender: TObject);
begin
  if OpenPictureDialog1.execute then
  begin
    ADSYSTEM_Image_Import(pchar(OpenpictureDialog1.filename));
    rimglist.Invalidate;
  end;
end;

procedure TForm1.NewCTClick(Sender: TObject);
var name,desc:pchar;
    i:integer;
begin
  ADSYSTEM_LevelControl_GetBase(name);
  CTSelForm.BaseCtName:=name;
  CTSelForm.ShowModal;
  if CTSelForm.ModalResult=mrOk
  then begin
    for i:=0 to ADSYSTEM_CControl_Count-1
    do begin
      ADSYSTEM_CControl_SetCurrent(i);
      ADSYSTEM_CControl_GetInfo(name,desc);
      if name=CTSelForm.BaseCtName then break;
    end;
    ADSYSTEM_LevelControl_Create;
    Trackv.Invalidate;
  end;
end;

procedure TForm1.ExportCTClick(Sender: TObject);
var f:integer;
    ftypestr:pchar;
    name,desc:pchar;
    dataptr:pointer;
    size:integer;
begin
  ftypestr:='CTf-01';
  if ExportCTDialog.Execute then
  begin
    f:=FileCreate(ExportCTDialog.Filename);
    ADSYSTEM_CControl_SetCurrent(ADSYSTEM_LevelControl_GetClass);
    ADSYSTEM_CControl_GetInfo(name,desc);
    FileWrite(f,ftypestr^,length(ftypestr)+1);
    FileWrite(f,name^,length(name)+1);
    ADSYSTEM_LevelControl_SaveToMemory(dataptr,size);
    FileWrite(f,size,sizeof(size));
    FileWrite(f,dataptr^,size);
    ADSYSTEM_LevelControl_FreeSavedMemory;
    FileClose(f);
  end;
end;

procedure TForm1.ImportCTClick(Sender: TObject);
var f:integer;
    c:char;
    ftypestr:string;
    name,desc:pchar;
    dataptr:pointer;
    i,size:integer;
begin
  if ImportCTDialog.Execute then
  begin
    f:=FileOpen(ImportCTDialog.Filename,fmOpenRead);
    ftypestr:='';
    repeat
        FileRead(f,c,1);
        ftypestr:=ftypestr+c;
    until c=#0;
    if ftypestr<>'CTf-01'+#0 then
        showmessage('Errore: File non valido')
    else begin
            ftypestr:='';
            repeat
                FileRead(f,c,1);
                if c<>#0 then ftypestr:=ftypestr+c;
            until c=#0;
            for i:=0 to ADSYSTEM_CControl_Count-1 do
            begin
              ADSYSTEM_CControl_SetCurrent(i);
              ADSYSTEM_CControl_GetInfo(name,desc);
              if name=ftypestr then break;
            end;
            ADSYSTEM_CControl_GetBase(name);
            ADSYSTEM_LevelControl_GetBase(desc);
            if name=desc then
            begin
              FileRead(f,size,sizeof(size));
              getmem(dataptr,size);
              FileRead(f,dataptr^,size);
              ADSYSTEM_LevelControl_Create;
              ADSYSTEM_LevelControl_LoadFromMemory(dataptr,size);
              freemem(dataptr);
            end
            else showmessage('Errore: Controllo incompatibile');
         end;
    FileClose(f);
  end;
  ADSYSTEM_Update;
  Trackv.Invalidate;
end;

procedure TForm1.EFXdeleteClick(Sender: TObject);
begin
  ADSYSTEM_Level_Delete;
  if EfxList.selected>=ADSYSTEM_Level_Count
  then EfxList.selected:=ADSYSTEM_Level_Count-1;
  EfxList.Invalidate;
  efxInfo.efxNum:=EfxList.selected;
  efxInfo.Invalidate;
  efxChn.efxNum:=EfxList.selected;
  efxChn.Invalidate;
  efxImg.efxNum:=EfxList.selected;
  efxImg.Invalidate;
  Trackv.Invalidate;
end;

procedure TForm1.EFXmoveUPClick(Sender: TObject);
begin
  ADSYSTEM_Level_MoveUp;
  if EfxList.selected>0 then EfxList.selected:=EfxList.selected-1;
  ADSYSTEM_Level_SetCurrent(EfxList.selected);
  EfxList.Invalidate;
  efxInfo.efxNum:=EfxList.selected;
  efxInfo.Invalidate;
  efxChn.efxNum:=EfxList.selected;
  efxChn.Invalidate;
  efxImg.efxNum:=EfxList.selected;
  efxImg.Invalidate;
  Trackv.Invalidate;
end;

procedure TForm1.EFXmoveDOWNClick(Sender: TObject);
begin
  ADSYSTEM_Level_MoveDown;
  if EfxList.selected<ADSYSTEM_Level_Count-1 then EfxList.selected:=EfxList.selected+1;
  ADSYSTEM_Level_SetCurrent(EfxList.selected);
  EfxList.Invalidate;
  efxInfo.efxNum:=EfxList.selected;
  efxInfo.Invalidate;
  efxChn.efxNum:=EfxList.selected;
  efxChn.Invalidate;
  efxImg.efxNum:=EfxList.selected;
  efxImg.Invalidate;
  Trackv.Invalidate;
end;

procedure TForm1.EFXPopupPopup(Sender: TObject);
begin
  if efxlist.selected=0 then EFXmoveUp.Enabled:=false
  else EFXmoveUp.Enabled:=true;

  if efxlist.selected=ADSYSTEM_Level_Count-1 then EFXmoveDown.Enabled:=false
  else EFXmoveDown.Enabled:=true;
end;

procedure TForm1.CHNPopupPopup(Sender: TObject);
begin
  if chnlist.selected=0 then CHNmoveUP.Enabled:=false
  else CHNmoveUP.Enabled:=true;

  if chnlist.selected=ADSYSTEM_Channel_Count-1 then CHNmoveDOWN.Enabled:=false
  else CHNmoveDOWN.Enabled:=true;

  if ADSYSTEM_Channel_Count<=1 then CHNdelete.Enabled:=false
  else CHNdelete.Enabled:=true;
end;

procedure TForm1.CHNmoveUPClick(Sender: TObject);
begin
  ADSYSTEM_Channel_MoveUp;
  if chnList.selected>0 then chnlist.selected:=chnlist.selected-1;
  ADSYSTEM_Channel_SetCurrent(chnList.selected);
  chnList.Invalidate;
end;

procedure TForm1.CHNmoveDOWNClick(Sender: TObject);
begin
  ADSYSTEM_Channel_MoveDown;
  if chnList.selected<ADSYSTEM_Channel_Count-1 then chnlist.selected:=chnlist.selected+1;
  ADSYSTEM_Channel_SetCurrent(chnList.selected);
  chnList.Invalidate;
end;

procedure TForm1.CHNdeleteClick(Sender: TObject);
begin
  ADSYSTEM_Channel_Delete;
  if chnList.selected>=ADSYSTEM_Channel_Count
  then chnList.selected:=ADSYSTEM_Channel_Count-1;
  chnList.Invalidate;
end;

procedure TForm1.Modifica2Click(Sender: TObject);
begin
  ADSYSTEM_Level_SetCurrent(efxlist.selected);
  ADSYSTEM_Level_Edit(form1.handle);
end;

procedure TForm1.FormResize(Sender: TObject);
begin
  efxlist.Width:=app.Width-25;
  chnlist.width:=app.width-25;
end;

procedure TForm1.SBkeyADDClick(Sender: TObject);
begin
  Trackv.mode:=add;
end;

end.
