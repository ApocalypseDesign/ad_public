unit Unit3;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, Buttons, ADsystem;

type
  TForm3 = class(TForm)
    name: TEdit;
    desc: TEdit;
    BitBtn1: TBitBtn;
    BitBtn2: TBitBtn;
    Label1: TLabel;
    Label2: TLabel;
    cttype: TLabel;
    procedure BitBtn1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form3: TForm3;

implementation

{$R *.DFM}

procedure TForm3.BitBtn1Click(Sender: TObject);
begin
  ADSYSTEM_CreateObj(SYS_OBJECTS_CT,
  pchar(cttype.Caption),
  pchar(name.Text),
  pchar(desc.Text));
end;

end.
