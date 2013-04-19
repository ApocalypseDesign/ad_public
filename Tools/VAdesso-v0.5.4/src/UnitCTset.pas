unit UnitCTset;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  StdCtrls, ADsystem;

type
  TCTselForm = class(TForm)
    CTList: TListBox;
    BTok: TButton;
    BTcancel: TButton;
    procedure BTokClick(Sender: TObject);
    procedure BTcancelClick(Sender: TObject);
    procedure FormActivate(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
    BaseCtName:string;
  end;

var
  CTselForm: TCTselForm;

implementation

{$R *.DFM}

procedure TCTselForm.BTokClick(Sender: TObject);
begin
  BaseCTName:=CTList.Items[CTList.ItemIndex];
  ModalResult:=mrOk;
end;

procedure TCTselForm.BTcancelClick(Sender: TObject);
begin
  ModalResult:=mrCancel;
end;

procedure TCTselForm.FormActivate(Sender: TObject);
var i :integer;
    name,desc:pchar;
begin
  CTlist.Clear;
  for i:=0 to ADSYSTEM_CControl_Count-1 do
  begin
    ADSYSTEM_CControl_SetCurrent(i);
    ADSYSTEM_CControl_GetBase(name);
    if name=BaseCTName then
    begin
      ADSYSTEM_CControl_GetInfo(name,desc);
      CTlist.Items.Add(name);
    end;
  end;
  CTlist.ItemIndex:=CTlist.Items.Count-1;
end;

end.
