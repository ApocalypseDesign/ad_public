library EditADESSO;

uses
  SysUtils,
  Classes,
  Controls,
  Unit1 in 'Unit1.pas' {Form1};

function Edit(var data,size:pointer):boolean;
var datasize:^integer;
    datadata:pchar;
begin
  form1:=TForm1.Create(nil);
  datasize:=size;

    datadata:=data;
    form1.Edit1.Text:=string(datadata);

  form1.ShowModal;
  if form1.ModalResult=mrOK then
  begin
    result:=true;
    new(datasize);
    datasize^:=length(form1.edit1.text)+1;
    size:=datasize;
    new(datadata);
    datadata:=pchar(form1.edit1.text);
    data:=datadata;
  end
  else result:=false;
  form1.Free;
end;

exports
  Edit index 1;
begin
end.
 