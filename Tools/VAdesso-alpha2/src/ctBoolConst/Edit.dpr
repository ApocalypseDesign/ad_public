library EditADESSO;

uses
  SysUtils,
  Classes,
  Controls,
  Unit1 in 'Unit1.pas' {Form1};

function Edit(var data,size:pointer):boolean;
var datasize:^integer;
    datadata:^integer;
begin
  form1:=TForm1.Create(nil);

    datadata:=data;
    if datadata^<=0 then form1.ComboBox1.ItemIndex:=0
    else form1.ComboBox1.ItemIndex:=1;

  form1.ShowModal;
  if form1.ModalResult=mrOK then
  begin
    result:=true;
    new(datasize);
    datasize^:=4;
    size:=datasize;
    new(datadata);
    if form1.ComboBox1.ItemIndex=0 then datadata^:=0
    else datadata^:=1;
    data:=datadata;
  end
  else result:=false;
  form1.Free;
end;

exports
  Edit index 1;
begin
end.
 