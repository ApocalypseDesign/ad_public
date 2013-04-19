program ASE2A3D;

uses
  Forms,
  Unit1 in '..\Unit1.pas' {Form1},
  AseUtils in 'AseUtils.pas';

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'A3D converter';
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
