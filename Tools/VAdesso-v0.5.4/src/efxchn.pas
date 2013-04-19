unit efxchn;
interface
uses windows,stdctrls,extctrls,ADsystem,Graphics,sysutils,controls,classes,dialogs;
type
   TEfxChn=class(TPaintBox)
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

procedure TEFXChn.selcomboChange(Sender:TObject);
var numcontrol,i,oldChannel:integer;
    name,desc:pchar;
begin
  numcontrol:=0;
  oldChannel:=ADSYSTEM_Channel_GetCurrent;

  for i:=0 to ADSYSTEM_Channel_Count-1 do
  begin
    ADSYSTEM_Channel_SetCurrent(i);
    ADSYSTEM_Channel_GetInfo(name,desc);
    if name=selcombo.Items[selcombo.ItemIndex] then numcontrol:=i;
  end;
  ADSYSTEM_Channel_SetCurrent(oldChannel);
  ADSYSTEM_Level_SetCurrent(efxnum);
  if selected>-1 then begin
  ADSYSTEM_LevelChannel_SetCurrent(selected);
  ADSYSTEM_LevelChannel_SetChannel(numcontrol);
  end;
  form1.efxlist.paint;
end;

constructor TEfxChn.Create(AOwner:TComponent);
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

procedure TEfxChn.setefxnum(value:integer);
begin
  Fefxnum:=value;
  selected:=-1;
end;

procedure TEfxChn.setSelected(value:integer);
var curstr:string; name,desc:pchar;
    numcontrol,i,oldChannel:integer;
begin
  oldChannel:=ADSYSTEM_Channel_GetCurrent;
  numcontrol:=-1;
  FSelected:=value;

 if (efxnum>=0) and (efxnum<ADSYSTEM_Level_Count)
 then begin
  ADSYSTEM_Level_SetCurrent(efxnum);
  ADSYSTEM_CLevel_SetCurrent(ADSYSTEM_Level_GetClass);
  if (FSelected>=0) and (FSelected<ADSYSTEM_CLevelChannel_Count)
  then begin
   ADSYSTEM_CLevelChannel_SetCurrent(FSelected);
   ADSYSTEM_LevelChannel_SetCurrent(FSelected);
   numcontrol:=ADSYSTEM_LevelChannel_GetChannel;
   selcombo.Items.Clear;
   for i:=0 to ADSYSTEM_Channel_Count-1 do
   begin
    ADSYSTEM_Channel_SetCurrent(i);
    ADSYSTEM_Channel_GetInfo(name,desc);
    selcombo.Items.Add(name);
   end;
  end;
       if numcontrol>=0 then
       begin
         //showmessage(inttostr(numcontrol));
         ADSYSTEM_Channel_SetCurrent(numcontrol);
         ADSYSTEM_Channel_GetInfo(name,desc);
         curstr:=name;


  selcombo.ItemIndex:=selcombo.Items.IndexOf(curstr);
  selcombo.Visible:=true
  end else selcombo.Visible:=false;
   end;
  ADSYSTEM_Channel_SetCurrent(oldChannel);
  Paint;
end;

procedure TEfxChn.Paint;
var i,numchannel:integer;

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
    oldChannel:integer;
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
    height:=(ADSYSTEM_CLevelChannel_Count+4)*cellheight;
    for i:=0 to ADSYSTEM_CLevelChannel_Count-1
    do begin
         ADSYSTEM_CLevelChannel_SetCurrent(i);
         ADSYSTEM_CLevelChannel_GetInfo(name,desc);
         st1:=name;
         ADSYSTEM_LevelChannel_SetCurrent(i);
         numchannel:=ADSYSTEM_LevelChannel_GetChannel;

         oldChannel:=ADSYSTEM_Channel_GetCurrent;

                ADSYSTEM_Channel_SetCurrent(numChannel);
                ADSYSTEM_Channel_GetInfo(name,desc);
                st2:=name;

         ADSYSTEM_Channel_SetCurrent(oldChannel);

         cell(canvas,i,st1,st2);
       end;
  end;
  end;
end;

procedure TEfxChn.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
begin
  inherited MouseDown(Button,Shift,X,Y);
  selected:=(y div cellheight);
end;

procedure TEfxChn.VisibleChanging;
begin
  inherited VisibleChanging;
end;


end.
