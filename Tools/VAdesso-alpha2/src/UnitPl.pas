unit UnitPl;

interface

uses
  Classes,windows;

type
  Tplayer = class(TThread)
  private
    { Private declarations }
  protected
    pos,startpos:real;
    ticks1,ticks2:integer;
    procedure SetPos;
    procedure TheEnd;
    procedure Execute; override;
  public
    procedure riprendi;
    procedure inizia;
  end;

implementation

uses unit1;

{ Important: Methods and properties of objects in VCL can only be used in a
  method called using Synchronize, for example,

      Synchronize(UpdateCaption);

  and UpdateCaption could look like,

    procedure Tplayer.UpdateCaption;
    begin
      Form1.Caption := 'Updated in a thread';
    end; }

{ Tplayer }


procedure Tplayer.SetPos;
begin
  form1.TrackBar1.Position:=round(form1.TrackBar1.Max*pos);
  form1.TrackBar1.OnChange(self);
end;

procedure Tplayer.theend;
begin
  form1.btplay.Enabled:=true;
  form1.btpause.Enabled:=false;
  form1.btstop.enabled:=false;
end;

procedure Tplayer.Riprendi;
begin
  startpos:=pos;
  ticks1:=gettickcount;
end;

procedure Tplayer.Inizia;
begin
  startpos:=0;
end;

procedure Tplayer.Execute;
begin
  { Place thread code here }
  ticks1:=gettickcount;
  repeat
    ticks2:=gettickcount;
    pos:=((ticks2-ticks1)/20000)+startpos;
    Synchronize(Setpos);
  until (terminated or (pos>1.0));
  synchronize(theend);
end;

end.
