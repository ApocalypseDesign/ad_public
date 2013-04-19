library EditADESSO;

uses
  SysUtils,
  Classes,
  Controls,
  Unit1 in 'Unit1.pas' {Form1};

function Edit(var data,size:pointer):boolean;
var datadata,datasize:^integer;
begin
  form1:=TForm1.Create(nil);
  datasize:=size;
  if datasize^=4 then
  begin
    datadata:=data;
    form1.Edit1.Text:=inttostr(datadata^);
  end;
  form1.ShowModal;
  if form1.ModalResult=mrOK then
  begin
    result:=true;
    new(datasize);
    datasize^:=4;
    size:=datasize;
    new(datadata);
    datadata^:=strtoint(form1.edit1.text);
    data:=datadata;
  end
  else result:=false;
  form1.Free;
end;

exports
  Edit index 1;
begin
end.
 