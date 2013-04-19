unit efxinfo;
interface
uses windows,stdctrls,extctrls,ADsystem,Graphics,sysutils,controls,classes,dialogs;
type
   TEfxInfo=class(TPaintBox)
     constructor Create(AOwner: TComponent); override;
     procedure Paint; override;
     procedure VisibleChanging; override;
   private
   protected
     Fefxnum:integer;
     cellheight:integer;
     procedure setefxnum(value:integer);
   public
     property efxnum:integer read Fefxnum write setefxnum;
   end;

implementation
uses unit1,shared;


constructor TEfxInfo.Create(AOwner:TComponent);
var rect:Trect;
begin
  inherited Create(AOwner);
  rect.Top:=10; rect.Left:=10;
  rect.Bottom:=20; rect.Right:=200;
  cellheight:=19;
end;

procedure TEfxInfo.setefxnum(value:integer);
begin
  Fefxnum:=value;
end;


procedure TEfxInfo.Paint;

    procedure StaticCell(canvas:TCanvas; num:integer; text:string);
    var rect:TRect;
    begin
      with canvas do
      begin
        pen.Color:=clBtnShadow;
        brush.Style:=bsSolid;
        brush.Color:=clBtnHighLight;
        rect.Left:=1;rect.Top:=1+cellheight*num;
        rect.Right:=width-2;rect.Bottom:=1+cellheight*(num+1);
        rectangle(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1);
        font.color:=clBlack;
        if num=0 then font.Style:=[fsBold] else font.Style:=[];
        TextRect(rect,2,rect.top+1,text);
      end;
    end;



var name,desc:pchar;
    //pippo:integer;
begin
  inherited Paint;
  if (efxnum>=0) and (efxnum<ADSYSTEM_Level_Count) then
  begin
  ADSYSTEM_Level_SetCurrent(efxnum);
  with canvas do
  begin
    ADSYSTEM_Level_GetInfo(name,desc);
    StaticCell(canvas,0,name);
    StaticCell(canvas,1,desc);
  end;
  end;
end;

procedure TEfxInfo.VisibleChanging;
begin
  inherited VisibleChanging;
end;


end.
