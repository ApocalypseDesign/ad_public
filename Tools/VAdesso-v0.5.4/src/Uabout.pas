unit Uabout;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, StdCtrls, ExtCtrls;

type
  TAbout = class(TForm)
    Label12: TLabel;
    Label11: TLabel;
    Label9: TLabel;
    Image1: TImage;
    Label1: TLabel;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Label5: TLabel;
    Bevel1: TBevel;
    Button1: TButton;
    procedure SpeedButton1Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  About: TAbout;

implementation

{$R *.DFM}

procedure TAbout.SpeedButton1Click(Sender: TObject);
begin
  ModalResult:=mrOk;
end;

procedure TAbout.Button1Click(Sender: TObject);
begin
  ModalResult:=mrOk;
end;

end.
