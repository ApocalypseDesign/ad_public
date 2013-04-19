unit chnlist;

interface
uses windows,extctrls,ADsystem,Graphics,sysutils,controls,classes;
type
   TChnList=class(TPaintBox)
   procedure Paint; override;
   procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
   procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
   procedure DblClick; override;
   private
   protected
   public
     selected:integer;
   end;

implementation
uses unit1,shared;

procedure TChnList.Paint;
var num,i:integer;
    name,desc:pchar;
begin
  width:=form1.app.width-25;
  num:=ADSYSTEM_Channel_Count;
  if num=0 then
  with canvas do
  begin
    Font.name:='Arial';
    font.Size:=8;
    height:=37;
    Pen.Color:=clBlack;
    Brush.Color:=clGray;
    Rectangle(0,0,width-1,height-1);
    font.size:=8;
    Textout(10,10,'Nessun canale definito');
  end
  else
  with canvas do
  begin
    height:=num*37+15;
    Pen.Color:=clBlack;
    Font.name:='Arial';
    Font.size:=7;
    for i:=0 to num-1 do
    begin
      if i=selected then begin
                           Brush.Color:=clADbarSelected;
                           Font.Color:=clADbarSelectedText
                         end
      else begin
             Brush.Color:=clADbar;
             Font.Color:=clADbarText
           end;
      Rectangle(0,i*37,width-1,i*37+36);
      Rectangle(1,i*37+1,34,i*37+36-1);
      Draw(1,i*37+2,Form1.SpeedButton1.Glyph);
      ADSYSTEM_Channel_SetCurrent(i);
      ADSYSTEM_Channel_GetInfo(name,desc);
      TextOut(38,i*37+2,name+' ('+desc+')');
    end;
    Brush.Color:=clAppWorkspace;
    Font.Color:=clBlack;
    Font.Size:=8;
    if num=1 then  Textout(0,num*37,inttostr(num)+' canale')
    else Textout(0,num*37,inttostr(num)+' canali');
  end;
  ADSYSTEM_Channel_SetCurrent(selected);
end;

procedure TChnList.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
begin
  selected:=y div 37;
  if selected<0 then selected:=0;
  if selected>ADSYSTEM_Channel_Count-1
  then selected:=ADSYSTEM_Channel_Count-1;
  ADSYSTEM_Channel_SetCurrent(selected);
  Paint;
end;

procedure TChnList.MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
var MousePos:TPoint;
begin
 if (ADSYSTEM_Channel_Count>0) then
  begin
    MousePos.x:=x;
    MousePos.y:=y;
    MousePos:=clienttoscreen(MousePos);
    if button=mbright then form1.CHNPopup.Popup(MousePos.x,MousePos.y);
  end;
end;

procedure TChnList.DblClick;
begin
  inherited DblClick;
  //ADSYSTEM_Channel_SetCurrent(selected);
  form1.pages.ActivePageIndex:=PAGES_LEVELS;
  form1.Pages.OnChange(self);
  form1.efxList.ChannelBox.ItemIndex:=selected;
  form1.efxList.ChannelBox.OnChange(self);
  //form1.efxlist.RefreshChannelBox(selected);
  //form1.efxList.ChannelBoxChange(self);
  //form1.Pages.OnChange(self);
end;

end.
