unit player;

interface

uses
  Classes, unit1;

type
  Tplayer = class(TThread)
  private
    { Private declarations }
  protected
    procedure Execute; override;
  public
    procedure SetPos(p:real);
  end;

implementation


{ Important: Methods and properties of objects in VCL can only be used in a
  method called using Synchronize, for example,

      Synchronize(UpdateCaption);

  and UpdateCaption could look like,

    procedure player.UpdateCaption;
    begin
      Form1.Caption := 'Updated in a thread';
    end; }

{ player }

procedure Tplayer.SetPos(p:real);
begin
  form1.TrackBar1.Position:=round(form1.TrackBar1.Max*p)
end;

procedure Tplayer.Execute;
begin
  { Place thread code here }
  Synchronize(SetPos(0.0));
end;

end.
