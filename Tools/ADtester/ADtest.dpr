program ADtest;

uses
  Forms,
  Unit1 in 'Unit1.pas' {Form1},
  ADsystem in 'ADsystem.pas',
  preThread in 'preThread.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'AD - EFXtester';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
