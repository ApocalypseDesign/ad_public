unit efximg;
interface
uses windows,stdctrls,extctrls,ADsystem,Graphics,sysutils,controls,classes,dialogs;
type
   TEfxImg=class(TPaintBox)
     constructor Create(AOwner: TComponent); override;
     procedure Paint; override;
     procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
     procedure VisibleChanging; override;
   private
   protected
     Fselected,Fefxnum:integer;
     cellheight:integer;
     procedure setefxnum(value:integer);
     procedure setSelected(value:integer);
     procedure selcomboChange(Sender:TObject);
   public
     selcombo:TComboBox;
     property efxnum:integer read Fefxnum write setefxnum;
     property selected:integer read FSelected write setSelected;
   end;

implementation
uses unit1,shared;

procedure TEFXImg.selcomboChange(Sender:TObject);
var numcontrol,i:integer;
    name,desc:pchar;
begin
  numcontrol:=0;
  if ADSYSTEM_Image_Count>=1 then selcombo.Visible:=true
  else begin selcombo.Visible:=false; end;

  for i:=0 to ADSYSTEM_Image_Count-1 do
  begin
    ADSYSTEM_Image_SetCurrent(i);
    ADSYSTEM_Image_GetInfo(name,desc);
    if name=selcombo.Items[selcombo.ItemIndex] then numcontrol:=i;
  end;
  ADSYSTEM_Level_SetCurrent(efxnum);
  if selected>-1 then begin
  ADSYSTEM_LevelImage_SetCurrent(selected);
  //showmessage(inttostr(numcontrol));
  ADSYSTEM_LevelImage_SetImage(numcontrol);
  end;

  form1.efxlist.paint;
end;

constructor TEfxImg.Create(AOwner:TComponent);
var rect:Trect;
begin
  inherited Create(AOwner);
  rect.Top:=10; rect.Left:=10;
  rect.Bottom:=20; rect.Right:=200;
  selcombo:=TComboBox.Create(self);
  selcombo.Visible:=false;
  selcombo.Dock(form1.SProp,rect);
  selcombo.Style:=csDropDownList;
  selcombo.Font.Name:='Arial';
  selcombo.Font.Size:=7;
  selcombo.OnChange:=selcomboChange;
  cellheight:=19;
  FSelected:=-1;
end;

procedure TEfxImg.setefxnum(value:integer);
begin
  Fefxnum:=value;
  selected:=-1;
end;

procedure TEfxImg.setSelected(value:integer);
var curstr:string; name,desc:pchar;
    numcontrol,i:integer;
begin
  numcontrol:=-1;
  FSelected:=value;

 if (efxnum>=0) and (efxnum<ADSYSTEM_Level_Count)
 then begin
  ADSYSTEM_Level_SetCurrent(efxnum);
  ADSYSTEM_CLevel_SetCurrent(ADSYSTEM_Level_GetClass);
  if (FSelected>=0) and (FSelected<ADSYSTEM_CLevelChannel_Count)
  then begin
   ADSYSTEM_CLevelImage_SetCurrent(FSelected);
   ADSYSTEM_LevelImage_SetCurrent(FSelected);
   numcontrol:=ADSYSTEM_LevelImage_GetImage;
   selcombo.Items.Clear;
   for i:=0 to ADSYSTEM_Image_Count-1 do
   begin
    ADSYSTEM_Image_SetCurrent(i);
    ADSYSTEM_Image_GetInfo(name,desc);
    selcombo.Items.Add(name);
   end;
  end;
       if numcontrol>=0 then
       begin
         //showmessage(inttostr(numcontrol));
         ADSYSTEM_Image_SetCurrent(numcontrol);
         ADSYSTEM_Image_GetInfo(name,desc);
         curstr:=name;


  selcombo.ItemIndex:=selcombo.Items.IndexOf(curstr);
  selcombo.Visible:=true
  end else selcombo.Visible:=false;
   end;
  Paint;
end;

procedure TEfxImg.Paint;
var i,numimg:integer;

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

    procedure cell(canvas:TCanvas; num:integer; text1,text2:string);
    var rect:TRect;
    begin
      with canvas do
      begin
        pen.Color:=clBtnShadow;
        if num=selected then begin
                           Brush.Color:=clADbarSelected;
                           Font.Color:=clADbarSelectedText
                         end
        else begin
             brush.Color:=clBtnHighLight;
             font.Color:=clBlack;
           end;
        rect.Left:=1;rect.Top:=1+cellheight*(num);
        rect.Right:=(width-2) div 2;rect.Bottom:=1+cellheight*(1+num);
        rectangle(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1);
        TextRect(rect,2,rect.top+1,text1);
        rect.left:=rect.Right; rect.Right:=width-2;
        if num=selected then
        begin
          rectangle(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1);
          selcombo.Left:=left+rect.Left-1;
          selcombo.Top:=top+rect.Top-1;
          selcombo.Width:=rect.Right-rect.Left+2;
        end
        else begin
               rectangle(rect.left-1,rect.top-1,rect.right+1,rect.bottom+1);
               font.Color:=clNavy;
               TextRect(rect,1+rect.left,rect.top+1,text2);
             end;
      end;
    end;

var name,desc:pchar;
    st1,st2:string;
begin
  inherited Paint;

  if (efxnum>=0) and (efxnum<ADSYSTEM_Level_Count) then
  begin
  ADSYSTEM_Level_SetCurrent(efxnum);
  with canvas do
  begin
    //ADSYSTEM_Level_GetInfo(name,desc);
    //StaticCell(canvas,0,name);
    //StaticCell(canvas,1,desc);

    ADSYSTEM_CLevel_SetCurrent(ADSYSTEM_Level_GetClass);
    height:=(ADSYSTEM_CLevelImage_Count+4)*cellheight;
    for i:=0 to ADSYSTEM_CLevelImage_Count-1
    do begin
         ADSYSTEM_CLevelImage_SetCurrent(i);
         ADSYSTEM_CLevelImage_GetInfo(name,desc);
         st1:=name;
         ADSYSTEM_LevelImage_SetCurrent(i);
         numimg:=ADSYSTEM_LevelImage_GetImage;

                ADSYSTEM_Image_SetCurrent(numimg);
                ADSYSTEM_Image_GetInfo(name,desc);
                st2:=name;
                //showmessage(st2);

         cell(canvas,i,st1,st2);
       end;
  end;
  end;
end;

procedure TEfxImg.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
begin
  inherited MouseDown(Button,Shift,X,Y);
  selected:=(y div cellheight);
  if (selected>=0) and (selected<ADSYSTEM_CLevelImage_Count)
  and (ADSYSTEM_Image_Count>0) then selcombo.OnChange(self);
end;

procedure TEfxImg.VisibleChanging;
begin
  inherited VisibleChanging;
end;


end.
