object CTselForm: TCTselForm
  Left = 121
  Top = 401
  BorderStyle = bsToolWindow
  Caption = 'Nuovo Controllo'
  ClientHeight = 266
  ClientWidth = 283
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnActivate = FormActivate
  PixelsPerInch = 120
  TextHeight = 16
  object CTList: TListBox
    Left = 8
    Top = 8
    Width = 177
    Height = 249
    ItemHeight = 16
    TabOrder = 0
  end
  object BTok: TButton
    Left = 200
    Top = 200
    Width = 75
    Height = 25
    Caption = '&OK'
    TabOrder = 1
    OnClick = BTokClick
  end
  object BTcancel: TButton
    Left = 200
    Top = 232
    Width = 75
    Height = 25
    Caption = '&Annulla'
    TabOrder = 2
    OnClick = BTcancelClick
  end
end
