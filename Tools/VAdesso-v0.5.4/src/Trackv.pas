unit Trackv;

interface
uses controls,classes,extctrls,dialogs,graphics,windows,sysutils,ADsystem;

const LineHeight=20;
      LineWidth=500;
      NameWidth=100;

type
   TTrackMode=(Add,Move,Delete);

   TTrackv=class(TPaintBox)
   procedure Paint; override;
   procedure MouseMove(Shift: TShiftState; X, Y: Integer); override;
   procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
   procedure MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
   //procedure DblClick; override;
   private
     function xToPos(x:integer):integer;
     function posToX(pos:integer):integer;
     function yToNum(y:integer):integer;
     function numToY(num:integer):integer;
     procedure PaintKey(canvas:Tcanvas; x,y:integer; selected:boolean);
     procedure PaintTrack(canvas:Tcanvas; ctnum,linenum:integer);
   protected
   public
     mode:TTrackMode;
     moving:boolean;
     selectedCt:integer;
     selectedKey:integer;
   end;


implementation
uses unit1;

function TTrackv.xToPos(x:integer):integer;
var pos:integer;
begin
  pos:=(x-NameWidth)*2;
  if pos<0 then pos:=0;
  if pos>1000 then pos:=1000;
  result:=pos;
end;

function TTrackv.posToX(pos:integer):integer;
begin
  result:=NameWidth+pos div 2;
end;

function TTrackv.yToNum(y:integer):integer;
begin
  result:=y div lineHeight;
end;

function TTrackv.numToY(num:integer):integer;
begin
  result:=num * lineHeight;
end;

procedure TTrackv.MouseMove(Shift: TShiftState; X, Y: Integer);
var numCt,numKey,i,dist:integer;
    oldSelKey,oldSelCt:integer;
begin
 inherited MouseMove(Shift,X,Y);
 if ADSYSTEM_Level_Count>0 then
 begin
  if moving then begin
                   ADSYSTEM_Level_SetCurrent(form1.efxlist.selected);
                   ADSYSTEM_LevelControl_SetCurrent(selectedCt);
                   ADSYSTEM_LevelControlKey_Move(selectedKey,xToPos(x));
                   PaintTrack(canvas,selectedCt,selectedCt);
                   if selectedCt=0 then form1.efxList.Paint;
                 end
  else
  begin
   oldSelKey:=selectedKey;
   oldSelCt:=selectedCt;
   numCt:=ADSYSTEM_LevelControl_Count;
   selectedCt:=yToNum(y);
   if (selectedCt>=0) and (selectedCt<numCt)
   then begin
         ADSYSTEM_LevelControl_SetCurrent(selectedCt);
         numKey:=ADSYSTEM_LevelControlKey_Count;
         dist:=999999;
         if numkey>0 then dist:=abs(xToPos(x)-ADSYSTEM_LevelControlKey_GetPos(0));
         selectedKey:=0;
         for i:=0 to numKey-1
         do if abs(xToPos(x)-ADSYSTEM_LevelControlKey_GetPos(i))<dist
         then begin
                selectedKey:=i;
                dist:=abs(xToPos(x)-ADSYSTEM_LevelControlKey_GetPos(i));
              end;
         if numkey<=0 then selectedkey:=-1;
         if (oldSelKey<>selectedKey) or (oldSelCt<>selectedCt) then paint;
       end
   else begin
          selectedCt:=-1;
          if (oldSelKey<>selectedKey) or (oldSelCt<>selectedCt) then paint;
        end;
  end;
 end; 
end;


procedure TTrackv.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
var numCt:integer;
begin
 inherited MouseDown(Button,Shift,X,Y);
if (ADSYSTEM_Level_Count>0) and (x>=NameWidth) then
begin
 if button=mbLeft then
 begin
  numCt:=ADSYSTEM_LevelControl_Count;
  case mode of
  add:  if (yToNum(y)>=0) and (yToNum(y)<numCt)
        then begin
          ADSYSTEM_LevelControl_SetCurrent(yToNum(Y));
          ADSYSTEM_LevelControlKey_Add(round(xToPos(x)));
          //showmessage(inttostr(xtoPos(x)));
          paint;
        end;
  move:  if (selectedCt>-1) and (selectedKey>-1)
        then begin
          ADSYSTEM_LevelControl_SetCurrent(selectedCt);
          ADSYSTEM_LevelControlKey_Move(selectedKey,xToPos(x));
          paint;
          moving:=true;
        end;
  delete:  if (selectedCt>-1) and (selectedKey>-1)
        then begin
          ADSYSTEM_LevelControl_SetCurrent(selectedCt);
          ADSYSTEM_LevelControlKey_Delete(selectedKey);
          paint;
        end;
  end;
 end
 else begin
        if (selectedCt>-1) and (selectedKey>-1)
        then begin
          ADSYSTEM_LevelControl_SetCurrent(selectedCt);
          //form1.hide;
          ADSYSTEM_LevelControlKey_Edit(selectedKey,form1.handle);
          form1.efxlist.Invalidate;
        end;
      end;
end;
end;

procedure TTrackv.MouseUp(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
var MousePos:TPoint;
begin
  moving:=false;
  if (ADSYSTEM_Level_Count>0) and (selectedCt>=0) then
  begin
    MousePos.x:=x;
    MousePos.y:=y;
    MousePos:=clienttoscreen(MousePos);
    if button=mbright then form1.CTPopup.Popup(MousePos.x,MousePos.y);
  end;
  if selectedCt=0 then form1.efxList.Invalidate;
end;

procedure TTrackv.PaintKey(canvas:Tcanvas; x,y:integer; selected:boolean);
begin
  with canvas do
  begin
    pen.Color:=clBlack;
    if selected then brush.Color:=clRed
    else brush.Color:=clWhite;
    canvas.Ellipse(x-4,y-6,x+4,y+6);
  end;
end;

procedure TTrackv.PaintTrack(canvas:Tcanvas; ctnum,linenum:integer);
var rect:Trect;
    name,desc:pchar;
    i:integer;
    sel:boolean;
begin
  with canvas do
  begin
    //disegno il rettangolo
    pen.Color:=clBlack;
    pen.Style:=psSolid;
    if ctnum=selectedCt then brush.Color:=clSilver
    else brush.Color:=clGray;
    brush.Style:=bsSolid;
    rect.Left:=NameWidth;
    rect.Top:=LineHeight*linenum+1;
    rect.Right:=LineWidth+NameWidth-1;
    rect.Bottom:=rect.Top+LineHeight-1;
    rectangle(rect);
    ADSYSTEM_LevelControl_SetCurrent(ctnum);


    for i:=0 to ADSYSTEM_LevelControlKey_Count-1
    do begin
         if (ctnum=selectedCt) and (i=selectedKey) then sel:=true
         else sel:=false;
         if mode=add then sel:=false;
         PaintKey(canvas,posToX(ADSYSTEM_LevelControlKey_GetPos(i)),numToY(ctNum)+10,sel);
       end;

    //scrivo il nome del controller

    ADSYSTEM_LevelControl_GetInfo(name,desc);
    brush.Color:=clBtnFace;
    rect.Left:=1;
    rect.Right:=NameWidth;
    rect.Top:=rect.Top+1;
    rect.Bottom:=rect.Bottom-1;
    textrect(rect,4,rect.Top,name);
    //textout(100,rect.top,inttostr(ADSYSTEM_ControlKey_Count)+' keys');
  end;
end;

procedure TTrackv.Paint;
var ctNum,i:integer;
begin
  if ADSYSTEM_Level_Count>0 then
  begin
    //form1.Panel3.Enabled:=true;
    ADSYSTEM_Level_SetCurrent(form1.efxlist.selected);
    form1.SBkeyADD.Enabled:=true;
    form1.SBkeyMOVE.Enabled:=true;
    form1.SBkeyDELETE.Enabled:=true;
    ctNum:=ADSYSTEM_LevelControl_Count;
    self.height:=(ctNum+1)*LineHeight;
    for i:=0 to ctNum-1 do
    PaintTrack(canvas,i,i);
  end
  else
  begin
    //form1.Panel3.Enabled:=false;
    form1.SBkeyADD.Enabled:=false;
    form1.SBkeyMOVE.Enabled:=false;
    form1.SBkeyDELETE.Enabled:=false;
  end;
end;

end.
