unit efxlist;

interface
uses windows,stdctrls,extctrls,ADsystem,Graphics,sysutils,controls,classes;
type
   TEfxList=class(TPaintBox)
     constructor Create(AOwner: TComponent); override;
     procedure Paint; override;
     procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
     procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
     procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
     procedure DblClick; override;
     procedure VisibleChanging; override;
   private
   protected
   public
     selected:integer;
     selectedIsBoolRange:boolean;
     selectedBoolRangeNum:integer;
     ChannelBox:TComboBox;
     procedure PaintTimeLine;
     procedure ChannelBoxChange(Sender:TObject);
     procedure RefreshChannelBox(numchannel:integer);
   end;

implementation
uses unit1,dialogs,shared;

procedure TEfxList.ChannelBoxChange(Sender: TObject);
begin
  ADSYSTEM_Channel_SetCurrent(ChannelBox.itemindex);
  selected:=0;
  Invalidate;
  if form1.efxChn<>nil then
  begin
    form1.efxInfo.efxnum:=0;
    form1.efxInfo.Invalidate;
    form1.efxChn.efxnum:=0;
    form1.efxImg.efxnum:=0;
    form1.efxChn.Invalidate;
    form1.efxImg.Invalidate;
    form1.trackv.Invalidate;
  end;
  selected:=ADSYSTEM_Level_Count-1;
  if form1.efxInfo<>nil then
  begin
   form1.efxInfo.efxnum:=selected;
   form1.efxInfo.Invalidate;
   form1.efxChn.efxnum:=selected;
   form1.efxChn.Invalidate;
   form1.efxImg.efxnum:=selected;
   form1.efxImg.Invalidate;
  end;
end;

constructor TEfxList.Create(AOwner:TComponent);
var rect:Trect;
begin
  inherited Create(AOwner);
  ChannelBox:=TComboBox.Create(Self);
  rect.Top:=10+2; rect.Left:=10;
  rect.Bottom:=20+2; rect.Right:=200;
  ChannelBox.Visible:=false;
  ChannelBox.Dock(form1.app,rect);
  ChannelBox.Style:=csDropDownList;
  ChannelBox.OnChange:=ChannelBoxChange;
  RefreshChannelBox(0);
end;

procedure TEfxList.RefreshChannelBox(numchannel:integer);
var i:integer; name,desc:pchar;
begin
  ChannelBox.Items.Clear;
  for i:=0 to ADSYSTEM_Channel_Count-1 do
  begin
    ADSYSTEM_Channel_SetCurrent(i);
    ADSYSTEM_Channel_GetInfo(name,desc);
    ChannelBox.Items.Add(name);
  end;
  ChannelBox.ItemIndex:=numchannel;
  ChannelBoxChange(self);
  selected:=ADSYSTEM_Level_Count-1;
end;

procedure TEfxList.Paint;
var num,i,x,size,lastx:integer;
    name,desc:pchar;
    data:pointer;
    booldata:^boolean;
    lastbooldata:boolean;
begin
  width:=form1.app.width-25;
  num:=ADSYSTEM_Level_Count;
  if num=0 then
  with canvas do
  begin
    height:=37+35;
    Pen.Color:=clBlack;
    Brush.Color:=clGray;
    Rectangle(0,30,width-1,65);
    Textout(10,40,'Nessun livello nel canale selezionato');
  end
  else
  with canvas do
  begin
    height:=num*37+15+30;
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
      ADSYSTEM_Level_SetCurrent(i);

      //pen.color:=clnavy;
      ADSYSTEM_LevelControl_SetCurrent(0);
      ADSYSTEM_LevelControl_GetValue(0,data,size);
      booldata:=data;
      lastx:=36;
      for x:=36 to width-1 do
      begin
        lastbooldata:=booldata^;
        ADSYSTEM_LevelControl_GetValue((x-36)/(width-1-36),data,size);
        booldata:=data;

        if (booldata^<>lastbooldata) or (x=width-1)then
        begin
         if (lastbooldata) and (i=selected) then brush.color:=clred;
         if (lastbooldata) and (i<>selected) then brush.color:=clmaroon;
         if not(lastbooldata) and (i=selected) then brush.color:=clADbarSelected;
         if not(lastbooldata) and (i<>selected) then brush.color:=clADbar;

         Rectangle(lastx,i*37+30+1,x,(i+1)*37+30-2);
         lastx:=x;
        end;
        //moveto(x,i*37+30+1);
        //lineto(x,(i+1)*37+30-2);
      end;

      pen.Color:=clBlack;
      brush.Style:=bsClear;
      Rectangle(34,i*37+30,width,i*37+36+30);
      ADSYSTEM_Level_GetInfo(name,desc);
      TextOut(38,i*37+2+30,name+' ('+desc+')');

      pen.color:=clAppWorkspace;
      moveto(0,i*37+30+36);
      lineto(width-1,i*37+30+36);
      pen.color:=clBlack;

      Draw(2,i*37+2+30,Form1.EfxBtnArray[ADSYSTEM_Level_GetClass].Glyph);

      brush.Style:=bsClear;
      Rectangle(0,i*37+30,36,i*37+36+30);
      Rectangle(1,i*37+1+30,35,i*37+36-1+30);
      brush.Style:=bsSolid;

    end;
    font.Color:=clBlack;
    font.size:=8;
    Brush.Color:=clAppWorkspace;
    if num=1 then  Textout(0,num*37+30,inttostr(num)+' livello')
    else Textout(0,num*37+30,inttostr(num)+' livelli');

    PaintTimeLine;
  end;
end;

procedure TEfxList.PaintTimeLine;
var x:integer;
begin
  with canvas do
  begin
    pen.Width:=2;
    pen.Color:=clWhite;
    x:=34+round((width-36)*form1.timebar.position/form1.timebar.Max);
    moveto(x,32);
    lineto(x,height-17);
    pen.Width:=1;
  end;
end;

procedure TEfxList.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
var oldselected:integer;
begin
  oldselected:=selected;
  selected:=(y-30) div 37;
  if selected<0 then selected:=0;
  if selected>=ADSYSTEM_Level_Count then selected:=ADSYSTEM_Level_Count-1;
  if selected<>oldselected then paint;
  Paint;
  form1.efxInfo.efxNum:=selected;
  form1.efxInfo.Invalidate;
  form1.efxChn.efxNum:=selected;
  form1.efxChn.Invalidate;
  form1.efxImg.efxNum:=selected;
  form1.efxImg.Invalidate;
  form1.trackv.invalidate;
  inherited MouseDown(Button,Shift,X,Y);
end;

procedure TEfxList.MouseMove(Shift: TShiftState; X, Y: Integer);
begin
  inherited MouseMove(Shift,x,y);
end;

procedure TEfxList.MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
var MousePos:TPoint;
begin
  inherited MouseUp(Button,Shift,X,Y);
  if (ADSYSTEM_Level_Count>0) then
  begin
    MousePos.x:=x;
    MousePos.y:=y;
    MousePos:=clienttoscreen(MousePos);
    if button=mbright then form1.EFXPopup.Popup(MousePos.x,MousePos.y);
  end;
end;

procedure TEfxList.DblClick;
begin
  ADSYSTEM_Level_SetCurrent(selected);
  ADSYSTEM_Level_Edit(form1.handle);
end;

procedure TEfxList.VisibleChanging;
begin
  inherited VisibleChanging;
  ChannelBox.Visible:=not(Visible);
  if form1.efxChn<>nil then
  begin
   form1.efxInfo.efxNum:=selected;
   form1.efxInfo.Invalidate;
   form1.efxChn.efxNum:=selected;
   form1.efxChn.Invalidate;
   form1.efxImg.efxNum:=selected;
   form1.efxImg.Invalidate;
  end;
end;

end.
