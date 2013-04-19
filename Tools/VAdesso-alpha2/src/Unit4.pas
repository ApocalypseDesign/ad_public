unit Unit4;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ADsystem;

type
  TForm4 = class(TForm)
    name: TEdit;
    desc: TEdit;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    Label2: TLabel;
    cttype: TLabel;
    Label1: TLabel;
    procedure BitBtn1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form4: TForm4;

implementation

{$R *.DFM}

procedure TForm4.BitBtn1Click(Sender: TObject);
begin
  ADSYSTEM_CreateObj(SYS_OBJECTS_EFX,
  pchar(cttype.Caption),
  pchar(name.Text),
  pchar(desc.Text));
end;

end.
