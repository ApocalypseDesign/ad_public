unit preThread;

interface

uses
  Classes;

type
  previewThread = class(TThread)
  private
    { Private declarations }
  protected
    procedure Invalidate;
    procedure Paint(pos:single);
    procedure TheEnd;
    procedure Execute; override;
  public
    dlength:integer;
  end;

implementation
uses windows, graphics, ADsystem, unit1;

{ Important: Methods and properties of objects in VCL can only be used in a
  method called using Synchronize, for example,

      Synchronize(UpdateCaption);

  and UpdateCaption could look like,

    procedure previewThread.UpdateCaption;
    begin
      Form1.Caption := 'Updated in a thread';
    end; }

procedure previewThread.Invalidate;
begin
  form1.image.Invalidate;
end;

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

procedure previewThread.Paint(pos:single);
var src,dest:pchar;
    y:integer;
    //col,col2:byte;
begin
  src:=ADSYSTEM_Paint(pos);
  with form1.Image.Picture.Bitmap do
  begin
   for y:=0 to Height-1 do
   begin
    dest:=ScanLine[y];
    memcpyd(dest,src,width);
    src:=src+(width shl 2);
   end;

           (*
  //mostro la barra di percentuale
  col:=$A0;col2:=$FF;
  dest:=ScanLine[Height-6];
  for x:=(Width-32)*4 to (Width-2)*4 do dest[x]:=col;
  dest:=ScanLine[Height-3];
  for x:=(Width-32)*4 to (Width-2)*4 do dest[x]:=col;

  dest:=ScanLine[Height-5];
  dest[(width-32)*4]:=col;
  dest[(width-32)*4+1]:=col;
  dest[(width-32)*4+2]:=col;
  dest[(width-32)*4+3]:=col;

  dest[(width-3)*4]:=col;
  dest[(width-3)*4+1]:=col;
  dest[(width-3)*4+2]:=col;
  dest[(width-3)*4+3]:=col;

  dest:=ScanLine[Height-4];
  dest[(width-32)*4]:=col;
  dest[(width-32)*4+1]:=col;
  dest[(width-32)*4+2]:=col;
  dest[(width-32)*4+3]:=col;

  dest[(width-3)*4]:=col;
  dest[(width-3)*4+1]:=col;
  dest[(width-3)*4+2]:=col;
  dest[(width-3)*4+3]:=col;

  dest:=ScanLine[Height-5];
  for x:=(Width-31)*4 to round(Width-31+28.0*pos)*4 do dest[x]:=col2;
  for x:=(round(Width-31+28.0*pos)+1)*4 to (Width-3)*4 do dest[x]:=0;
  dest:=ScanLine[Height-4];
  for x:=(Width-31)*4 to round(Width-31+28.0*pos)*4 do dest[x]:=col2;
  for x:=(round(Width-31+28.0*pos)+1)*4 to (Width-3)*4 do dest[x]:=0;
  //fine barra   *)
  end;

  Synchronize(form1.image.Repaint);
end;

procedure previewThread.TheEnd;
begin
  with form1.Image.Canvas do
  begin
    //brush.Color:=clBlack;
    //brush.Style:=bsSolid;
    brush.Style:=bsClear;
    font.Name:='Arial';
    font.Style:=[fsBold];
    font.size:=10;
    font.Color:=clBlack;
    textout(11,10,'Animazione terminata');
    textout(9,10,'Animazione terminata');
    textout(10,9,'Animazione terminata');
    textout(10,11,'Animazione terminata');
    font.Color:=clWhite;
    textout(10,10,'Animazione terminata');
  end;
end;

{ previewThread }

procedure previewThread.Execute;
var pos:single; ticks1,ticks2:integer;
begin
  { Place thread code here }
  ADSYSTEM_Update;

  ticks1:=gettickcount;
  repeat
    ticks2:=gettickcount;
    pos:=(ticks2-ticks1)/(dlength*1000); // secondi
    Paint(pos);
    if pos>1.0 then begin
                      ticks1:=ticks2;
                    end;
  until terminated; //or (pos>1.0);
  synchronize(TheEnd);
end;

end.
