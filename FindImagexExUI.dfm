object Form2: TForm2
  Left = 0
  Top = 0
  Caption = #1055#1086#1080#1089#1082' '#1092#1072#1081#1083#1086#1074
  ClientHeight = 624
  ClientWidth = 772
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 337
    Top = 0
    Height = 605
    OnMoved = Splitter1Moved
    ExplicitLeft = 665
    ExplicitTop = 10
    ExplicitHeight = 450
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 337
    Height = 605
    Align = alLeft
    TabOrder = 0
    object Image1: TImage
      AlignWithMargins = True
      Left = 4
      Top = 276
      Width = 329
      Height = 325
      Align = alClient
      Proportional = True
      ExplicitLeft = 0
      ExplicitTop = 4
      ExplicitWidth = 448
      ExplicitHeight = 183
    end
    object Panel3: TPanel
      Left = 1
      Top = 1
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 0
      object Label1: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 80
        Height = 16
        Align = alLeft
        AutoSize = False
        Caption = #1043#1076#1077' '#1080#1089#1082#1072#1090#1100
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitHeight = 22
      end
      object ButtonIncludeAdd: TButton
        Left = 274
        Top = 1
        Width = 30
        Height = 22
        Align = alRight
        Caption = '+'
        TabOrder = 0
        OnClick = ButtonIncludeAddClick
      end
      object EditIncludeFolder: TEdit
        Left = 87
        Top = 1
        Width = 187
        Height = 22
        Align = alClient
        TabOrder = 1
        Text = 'EditIncludeFolder'
        ExplicitHeight = 21
      end
      object ButtonIncludeDel: TButton
        Left = 304
        Top = 1
        Width = 30
        Height = 22
        Align = alRight
        Caption = '-'
        TabOrder = 2
        OnClick = ButtonIncludeDelClick
      end
    end
    object StringGridExcludes: TStringGrid
      Left = 1
      Top = 113
      Width = 335
      Height = 64
      Align = alTop
      ColCount = 1
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      PopupMenu = PopupMenuExclude
      TabOrder = 1
      ColWidths = (
        190)
    end
    object Panel4: TPanel
      Left = 1
      Top = 89
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 2
      object Label2: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 80
        Height = 16
        Align = alLeft
        AutoSize = False
        Caption = #1043#1076#1077' '#1085#1077' '#1080#1089#1082#1072#1090#1100
        ExplicitLeft = 1
        ExplicitTop = -4
        ExplicitHeight = 22
      end
      object EditExcludeFolder: TEdit
        Left = 87
        Top = 1
        Width = 187
        Height = 22
        Align = alClient
        TabOrder = 0
        Text = 'EditExcludeFolder'
        ExplicitHeight = 21
      end
      object ButtonExcludeAdd: TButton
        Left = 274
        Top = 1
        Width = 30
        Height = 22
        Align = alRight
        Caption = '+'
        TabOrder = 1
        OnClick = ButtonExcludeAddClick
      end
      object ButtonExcludeDel: TButton
        Left = 304
        Top = 1
        Width = 30
        Height = 22
        Align = alRight
        Caption = '-'
        TabOrder = 2
        OnClick = ButtonExcludeDelClick
      end
    end
    object StringGridIncludes: TStringGrid
      Left = 1
      Top = 25
      Width = 335
      Height = 64
      Align = alTop
      ColCount = 1
      FixedCols = 0
      RowCount = 1
      FixedRows = 0
      PopupMenu = PopupMenuInclude
      TabOrder = 3
      ColWidths = (
        190)
    end
    object Panel5: TPanel
      Left = 1
      Top = 177
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 4
      object Label3: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 80
        Height = 16
        Align = alLeft
        AutoSize = False
        Caption = #1057#1090#1072#1088#1096#1077
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitHeight = 22
      end
      object DateTimePickerCreated: TDateTimePicker
        Left = 87
        Top = 1
        Width = 88
        Height = 22
        Align = alLeft
        Date = 41936.657892002320000000
        Time = 41936.657892002320000000
        TabOrder = 0
      end
    end
    object Panel6: TPanel
      Left = 1
      Top = 201
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 5
      object Label123: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 80
        Height = 16
        Align = alLeft
        AutoSize = False
        Caption = #1056#1072#1089#1096#1080#1088#1077#1085#1080#1103
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitHeight = 22
      end
      object EditExtensions: TEdit
        Left = 87
        Top = 1
        Width = 247
        Height = 22
        Align = alClient
        TabOrder = 0
        Text = 'EditExtensions'
        ExplicitHeight = 21
      end
    end
    object Panel8: TPanel
      Left = 1
      Top = 225
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 6
      object Label5: TLabel
        AlignWithMargins = True
        Left = 4
        Top = 4
        Width = 80
        Height = 16
        Align = alLeft
        AutoSize = False
        Caption = #1060#1072#1081#1083' '#1083#1086#1075#1086#1074
        ExplicitLeft = 1
        ExplicitTop = 1
        ExplicitHeight = 22
      end
      object EditLogFile: TEdit
        Left = 87
        Top = 1
        Width = 217
        Height = 22
        Align = alClient
        TabOrder = 0
        Text = 'EditLogFile'
        ExplicitHeight = 21
      end
      object ButtonSaveLogFile: TButton
        Left = 304
        Top = 1
        Width = 30
        Height = 22
        Align = alRight
        Caption = '...'
        TabOrder = 1
        OnClick = ButtonSaveLogFileClick
      end
    end
    object Panel9: TPanel
      Left = 1
      Top = 249
      Width = 335
      Height = 24
      Align = alTop
      TabOrder = 7
      object ButtonStartSearch: TButton
        Left = 1
        Top = 1
        Width = 160
        Height = 22
        Align = alLeft
        Caption = #1053#1072#1095#1072#1090#1100' '#1087#1086#1080#1089#1082
        TabOrder = 0
        OnClick = ButtonStartSearchClick
      end
      object ButtonStopSearch: TButton
        Left = 161
        Top = 1
        Width = 173
        Height = 22
        Align = alClient
        Caption = #1054#1089#1090#1072#1085#1086#1074#1080#1090#1100' '#1087#1086#1080#1089#1082
        TabOrder = 1
        OnClick = ButtonStopSearchClick
      end
    end
  end
  object Panel2: TPanel
    Left = 340
    Top = 0
    Width = 432
    Height = 605
    Align = alClient
    TabOrder = 1
    object Label6: TLabel
      AlignWithMargins = True
      Left = 4
      Top = 4
      Width = 424
      Height = 13
      Align = alTop
      Caption = #1056#1077#1079#1091#1083#1100#1090#1072#1090#1099' '#1087#1086#1080#1089#1082#1072
      ExplicitWidth = 99
    end
    object StringGridResults: TStringGrid
      Left = 1
      Top = 20
      Width = 430
      Height = 560
      Align = alClient
      ColCount = 2
      FixedCols = 0
      RowCount = 2
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSelect]
      PopupMenu = PopupMenuStringGrid
      TabOrder = 0
      OnSelectCell = StringGridResultsSelectCell
      ColWidths = (
        68
        64)
    end
    object Panel7: TPanel
      Left = 1
      Top = 580
      Width = 430
      Height = 24
      Align = alBottom
      TabOrder = 1
      object Button5: TButton
        Left = 1
        Top = 1
        Width = 133
        Height = 22
        Align = alLeft
        Caption = #1042#1099#1073#1088#1072#1090#1100' '#1074#1089#1077
        TabOrder = 0
        OnClick = Button5Click
      end
      object Button6: TButton
        Left = 134
        Top = 1
        Width = 133
        Height = 22
        Align = alLeft
        Caption = #1059#1076#1072#1083#1080#1090#1100' '#1074#1099#1073#1088#1072#1085#1085#1086#1077
        TabOrder = 1
        OnClick = Button6Click
      end
    end
  end
  object StatusBar: TStatusBar
    Left = 0
    Top = 605
    Width = 772
    Height = 19
    Panels = <>
    SimplePanel = True
  end
  object SaveDialog1: TSaveDialog
    Left = 256
    Top = 288
  end
  object PopupMenuInclude: TPopupMenu
    Left = 224
    Top = 40
    object DeleteInclude: TMenuItem
      Caption = 'Delete'
      OnClick = DeleteIncludeClick
    end
  end
  object PopupMenuExclude: TPopupMenu
    Left = 224
    Top = 128
    object DeleteExclude: TMenuItem
      Caption = 'Delete'
      OnClick = DeleteExcludeClick
    end
  end
  object TimerSyncFindBuffer: TTimer
    OnTimer = TimerSyncFindBufferTimer
    Left = 480
    Top = 80
  end
  object PopupMenuStringGrid: TPopupMenu
    Left = 488
    Top = 168
    object PMOpenFolder: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1092#1072#1081#1083' '#1074' '#1087#1072#1087#1082#1077
      OnClick = PMOpenFolderClick
    end
    object PMOpenFile: TMenuItem
      Caption = #1054#1090#1082#1088#1099#1090#1100' '#1092#1072#1081#1083
      OnClick = PMOpenFileClick
    end
  end
  object MainMenu1: TMainMenu
    Left = 376
    Top = 312
    object MMChart: TMenuItem
      Caption = #1043#1088#1072#1092#1080#1082
      OnClick = MMChartClick
    end
  end
end
