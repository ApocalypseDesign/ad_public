unit preview;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, preThread;

type
  Tprewin = class(TForm)
    Image: TImage;
    procedure FormCreate(Sender: TObject);
    procedure FormActivate(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
    pThread:previewThread;
  end;

var
  prewin: Tprewin;

implementation

uses unit1;

{$R *.DFM}

procedure Tprewin.FormCreate(Sender: TObject);
var bmp:TBitmap;
begin
  bmp:=TBitmap.Create;
  bmp.Width:=image.Width;
  bmp.Height:=image.Height;
  bmp.PixelFormat:=pf32bit;
  image.Picture.Bitmap:=bmp;
  self.ControlStyle:= [csOpaque];
end;

procedure Tprewin.FormActivate(Sender: TObject);
var re:Trect;
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
  pThread.dlength:=form1.demolength;
  pThread.Resume;
end;

procedure Tprewin.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  if pThread<>nil then
  begin
   pThread.FreeOnTerminate:=true;
   pThread.Terminate;
  end; 
end;

end.
