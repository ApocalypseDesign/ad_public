unit AseUtils;

interface
uses classes;

function DivideString(s:string):TStringList;

implementation

function DivideString(s:string):TStringList;
var Strings:TStringList; st:string;
    i,l:integer;
begin
  Strings:=TStringList.Create;
  st:='';
  l:=length(s);
  i:=1;
  repeat
    while ((s[i]=' ') or (ord(s[i])=9)) and (i<=l) do inc(i);
    if i<=l then begin
                   while ((s[i]<>' ') and (ord(s[i])<>9)) and (i<=l) do begin
                                                     st:=st+s[i];
                                                     inc(i);
                                                   end;
                   Strings.Add(st);
                   st:='';
                 end;
  until i>l;
  Result:=Strings;
end;

end.
