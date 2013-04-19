unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ADsystem, preThread;

type
  TForm1 = class(TForm)
    Image: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
    pThread:previewThread;
    demolength:integer;
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.FormCreate(Sender: TObject);
var bmp:TBitmap;
begin
  if paramcount>=1 then form1.demolength:=strtoint(paramstr(1))
  else form1.demolength:=20;

  if paramcount=3 then
  begin
    image.Width:=strtoint(paramstr(2));
    image.Height:=strtoint(paramstr(3));
    width:=image.width+22;
    height:=image.height+46;
  end;

  ADSYSTEM_Init(image.Width,image.Height);
  ADSYSTEM_Channel_Create;
  ADSYSTEM_Channel_SetCurrent(0);
  //showmessage(inttostr(ADSYSTEM_CLevel_Count));
  if ADSYSTEM_CLevel_Count<=0 then
  begin
    showmessage('EFX not found ERROR');
    application.Terminate;
  end
  else begin
  ADSYSTEM_CLevel_SetCurrent(0);
  ADSYSTEM_Level_Create;
  bmp:=TBitmap.Create;
  bmp.Width:=image.Width;
  bmp.Height:=image.Height;
  bmp.PixelFormat:=pf32bit;
  image.Picture.Bitmap:=bmp;
  end;
end;

procedure TForm1.FormActivate(Sender: TObject);
var re:TRect;
begin
  re.Top:=0;
  re.Left:=0;
  re.Right:=Image.Width;
  re.Bottom:=Image.Height;
  Image.Picture.Bitmap.Canvas.Brush.Style:=bsSolid;
  Image.Picture.Bitmap.Canvas.Brush.Color:=clBlack;
  Image.Picture.Bitmap.Canvas.FillRect(re);
  Image.Picture.Bitmap.Canvas.TextOut(10,10,'Loading...');
  pThread:=previewThread.Create(true);
  pThread.dlength:=demolength;//form1.demolength;
  pThread.Resume;
end;

procedure TForm1.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  if pThread<>nil then
  begin
   pThread.FreeOnTerminate:=true;
   pThread.Terminate;
  end;
end;

end.
