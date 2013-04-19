program Vadesso;

{%ToDo 'Vadesso.todo'}

uses
  Forms,
  unit1 in 'unit1.pas' {Form1},
  ADsystem in 'ADsystem.pas',
  efxlist in 'efxlist.pas',
  chnlist in 'chnlist.pas',
  rimglist in 'rimglist.pas',
  shared in 'shared.pas',
  preview in 'preview.pas' {prewin},
  preThread in 'preThread.pas',
  Uabout in 'Uabout.pas' {About},
  Trackv in 'Trackv.pas',
  efxchn in 'efxchn.pas',
  efxinfo in 'efxinfo.pas',
  efximg in 'efximg.pas',
  UnitCTset in 'UnitCTset.pas' {CTselForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Visual Adesso';
  Application.CreateForm(TForm1, Form1);
  Application.CreateForm(Tprewin, prewin);
  Application.CreateForm(TAbout, About);
  Application.CreateForm(TCTselForm, CTselForm);
  Application.Run;
end.
