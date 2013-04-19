unit Childwin;

interface

uses Windows, Classes, Graphics, Forms, Controls, StdCtrls, ComCtrls,
     Dialogs;

type
  TMDIChild = class(TForm)
    RE: TRichEdit;
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

implementation

uses Main;

{$R *.DFM}


procedure TMDIChild.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  Action := caFree;
end;

procedure TMDIChild.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
 if RE.Modified then	{ only prompt if there are unsaved changes }
 begin
   case MessageDlg('Salvare le modifiche in '+caption+' ?', mtConfirmation,
     [mbYes, mbNo, mbCancel], 0) of	{ produce the message dialog box }
     id_Yes: MainForm.FileSaveItemClick(Self);	{ yes means user wants to save }
     id_Cancel: CanClose := False;	{ if user Cancels, exit dialog but don't close form }
   end;
 end;
end;

end.
