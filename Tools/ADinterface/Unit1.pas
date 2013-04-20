unit Unit1;

interface

uses
  Windows, Messages, SysUtils, Classes, Controls, Forms, Dialogs,
  StdCtrls, ExtCtrls, Graphics;

type mode=record
            width,height,bpp:integer;
          end;
type device=record
              guid:TGUID;
              name:string;
              modes:array of mode;
            end;
type driver=record
              guid:TGUID;
              name:string;
              devices:array of device;
            end;
            
var zeroguid:tguid='{00000000-0000-0000-C000-000000000000}';
var gofullscreen:boolean;
var drivers:array of driver;

type
  TForm1 = class(TForm)
    Button1: TButton;
    Button2: TButton;
    BoxDevice: TComboBox;
    BoxRes: TComboBox;
    Image1: TImage;
    BoxMode: TComboBox;
    Label2: TLabel;
    Label3: TLabel;
    Label4: TLabel;
    Bevel1: TBevel;
    Label1: TLabel;
    BoxDriver: TComboBox;
    CheckBox1: TCheckBox;
    procedure Button2Click(Sender: TObject);
    procedure Button1Click(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure BoxDriverChange(Sender: TObject);
    procedure BoxDeviceChange(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  Form1: TForm1;

implementation

{$R *.DFM}

procedure TForm1.Button2Click(Sender: TObject);
begin
  ModalResult:=MrCancel;
end;

procedure TForm1.Button1Click(Sender: TObject);
begin
  ModalResult:=MrOk;
end;

procedure TForm1.FormCreate(Sender: TObject);
begin
  boxmode.ItemIndex:=0;
  boxres.ItemIndex:=0;
  boxdevice.ItemIndex:=0;
end;

procedure TForm1.BoxDriverChange(Sender: TObject);
var i,j:integer;
begin
  j:=-1;
  for i:=low(drivers) to high( drivers) do
  if drivers[i].name=boxdriver.items[boxdriver.ItemIndex]
  then j:=i;
  if j>=0 then
  with drivers[j] do
  begin
    boxdevice.items.clear;
    for i:=low(devices) to high(devices)
    do boxdevice.Items.add(devices[i].name);
    boxdevice.itemindex:=0;
  end;
end;

procedure TForm1.BoxDeviceChange(Sender: TObject);
var i,j,k:integer;
begin
  j:=-1;
  for i:=low(drivers) to high( drivers) do
  if drivers[i].name=boxdriver.items[boxdriver.ItemIndex]
  then j:=i;
  if j>=0 then
  with drivers[j] do
  begin
    k:=-1;
    for i:=low(devices) to high( devices) do
    if devices[i].name=boxdevice.items[boxdevice.ItemIndex]
    then k:=i;
    if k>=0 then
    with devices[j] do
    begin
     boxres.items.clear;
     for i:=low(modes) to high(modes)
     do boxres.Items.add(inttostr(modes[i].width)
     +' x '
     +inttostr(modes[i].height)
     +' x '
     +inttostr(modes[i].bpp));
     boxres.itemindex:=0;
    end;
  end;
end;

end.
