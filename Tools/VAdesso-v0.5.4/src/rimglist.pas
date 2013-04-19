unit rimglist;

interface
uses extctrls,ADsystem,Graphics,sysutils,controls,classes,dialogs;
type
   TrImgList=class(TPaintBox)
   procedure Paint; override;
   procedure MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer); override;
   procedure DblClick; override;
   private
   protected
   public
     selected:integer;
   end;

implementation
uses unit1,shared;

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
(*
procedure previewThread.Paint(pos:single);
var src,dest:pchar;
    y:integer;
    //col,col2:byte;
begin
  src:=ADSYSTEM_Paint(pos);
  with prewin.Image.Picture.Bitmap do
  begin
   for y:=0 to Height-1 do
   begin
    dest:=ScanLine[y];
    memcpyd(dest,src,width);
    src:=src+(width shl 2);
   end;
  *)

procedure TrImgList.Paint;
var num,i,lastY,y:integer;
    name,desc:pchar;
    imgBuffer:PChar;
    imgWidth,imgHeight:integer;
    bmp:TBitmap;
    dest,src:PChar;
begin
  num:=ADSYSTEM_Image_Count;
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
    Textout(10,10,'Nessuna immagine disponibile');
  end
  else
  with canvas do
  begin

    Pen.Color:=clBlack;
    Font.name:='Arial';
    Font.size:=9;
    lastY:=0;
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
      ADSYSTEM_Image_SetCurrent(i);
      ADSYSTEM_Image_GetData(imgBuffer,imgWidth,imgHeight);
      if imgBuffer=nil then showmessage('imgBuffer=nil');
      Rectangle(0,lastY,width-1,lastY+imgHeight+6);
      bmp:=TBitmap.Create;
      bmp.Width:=imgWidth;
      bmp.Height:=imgHeight;
      bmp.PixelFormat:=pf32bit;

      src:=imgBuffer;
      for y:=0 to imgHeight-1 do //imgHeight-1 do
      begin
        dest:=bmp.ScanLine[y];
        memcpyd(dest,src,imgWidth);
        src:=src+(imgWidth shl 2);
      end;

      Draw(3,lastY+3,bmp);
      bmp.Free;
      ADSYSTEM_Image_GetInfo(name,desc);
      textout(imgWidth+10,lasty+5,name+' '+desc);

      inc(lastY,imgHeight+6);

      //Rectangle(1,i*37+1,34,i*37+36-1);
      //Draw(1,i*37+2,Form1.SpeedButton1.Glyph);

      //ADSYSTEM_Channel_GetInfo(name,desc);
      //TextOut(38,i*37+2,name+' ('+desc+')');
    end;
    height:=lasty+50;
    Brush.Color:=clAppWorkspace;
    Font.Color:=clBlack;
    Font.Size:=8;
    if num=1 then  Textout(0,lastY,inttostr(num)+' immagine')
    else Textout(0,lastY,inttostr(num)+' immagini');
  end;
  ADSYSTEM_Image_SetCurrent(selected);
end;

procedure TrImgList.MouseDown(Button: TMouseButton; Shift: TShiftState;
      X, Y: Integer);
begin
  selected:=y div 37;
  if selected<0 then selected:=0;
  if selected>ADSYSTEM_Channel_Count-1
  then selected:=ADSYSTEM_Channel_Count-1;
  ADSYSTEM_Image_SetCurrent(selected);
  Paint;
end;

procedure TrImgList.DblClick;
begin
  inherited DblClick;
end;

end.
