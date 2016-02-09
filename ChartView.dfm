object FormChart: TFormChart
  Left = 0
  Top = 0
  Caption = #1048#1089#1090#1086#1088#1080#1103' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103
  ClientHeight = 464
  ClientWidth = 684
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object PageControl1: TPageControl
    Left = 153
    Top = 0
    Width = 531
    Height = 464
    ActivePage = TabSheet1
    Align = alClient
    TabOrder = 0
    object TabSheet1: TTabSheet
      Caption = #1043#1088#1072#1092#1080#1082
      object Panel2: TPanel
        Left = 0
        Top = 0
        Width = 523
        Height = 436
        Align = alClient
        Caption = 'Panel2'
        TabOrder = 0
        object Chart1: TChart
          Left = 1
          Top = 1
          Width = 521
          Height = 434
          AllowPanning = pmHorizontal
          Legend.Visible = False
          MarginLeft = 30
          MarginUnits = muPixels
          Title.Text.Strings = (
            #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086' '#1085#1072#1081#1076#1077#1085#1085#1099#1093' '#1092#1072#1081#1083#1086#1074' '#1079#1072' '#1076#1072#1090#1091' '#1089#1082#1072#1085#1080#1088#1086#1074#1072#1085#1080#1103)
          OnUndoZoom = Chart1UndoZoom
          OnZoom = Chart1Zoom
          BottomAxis.Title.Caption = #1044#1072#1090#1072' '#1089#1086#1079#1076#1072#1085#1080#1103
          BottomAxis.Title.Pen.SmallSpace = 1
          LeftAxis.Automatic = False
          LeftAxis.AutomaticMinimum = False
          LeftAxis.Axis.SmallSpace = 1
          LeftAxis.AxisValuesFormat = '#'
          LeftAxis.Grid.DrawEvery = 2
          LeftAxis.LabelsFormat.Margins.Left = 48
          LeftAxis.LabelsFormat.Margins.Top = 37
          LeftAxis.LabelsFormat.Margins.Right = 52
          LeftAxis.LabelsFormat.Margins.Bottom = 25
          LeftAxis.LabelsFormat.Margins.Units = maPercentSize
          LeftAxis.LabelStyle = talValue
          LeftAxis.MaximumRound = True
          LeftAxis.TickLength = 5
          LeftAxis.Ticks.Fill.Gradient.Colors = <
            item
              Color = clWhite
            end
            item
              Color = clGreen
              Offset = 0.362275449101796400
            end
            item
              Color = clGreen
              Offset = 0.637724550898203600
              Transparency = 94
            end
            item
              Color = clGreen
              Offset = 0.910179640718562800
            end
            item
              Color = clYellow
              Offset = 1.000000000000000000
            end>
          LeftAxis.Ticks.Fill.Gradient.MidColor = clGreen
          LeftAxis.Ticks.SmallSpace = 1
          LeftAxis.Title.Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086
          LeftAxis.Title.Font.OutLine.SmallSpace = 1
          LeftAxis.Title.Font.Shadow.Smooth = False
          LeftAxis.Title.Font.Shadow.Visible = False
          LeftAxis.Title.Shadow.Visible = False
          LeftAxis.Title.ShapeCallout.Position = 43
          View3D = False
          Zoom.Animated = True
          Zoom.Direction = tzdHorizontal
          Zoom.Pen.Mode = pmNotXor
          Align = alClient
          TabOrder = 0
          DefaultCanvas = 'TGDIPlusCanvas'
          PrintMargins = (
            15
            11
            15
            11)
          ColorPaletteIndex = 9
          object Series1: TBarSeries
            Marks.Visible = False
            Marks.AutoPosition = False
            BarWidthPercent = 60
            Dark3D = False
            MarksLocation = mlCenter
            Shadow.Visible = False
            XValues.DateTime = True
            XValues.Name = 'X'
            XValues.Order = loAscending
            YValues.Name = 'Bar'
            YValues.Order = loNone
          end
        end
      end
    end
    object TabSheet2: TTabSheet
      Caption = #1058#1072#1073#1083#1080#1094#1072
      ImageIndex = 1
      object StringGridFiles: TStringGrid
        Left = 0
        Top = 0
        Width = 523
        Height = 412
        Align = alClient
        ColCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goThumbTracking]
        TabOrder = 0
      end
      object Panel1: TPanel
        Left = 0
        Top = 412
        Width = 523
        Height = 24
        Align = alBottom
        TabOrder = 1
        object LabelCount: TLabel
          AlignWithMargins = True
          Left = 4
          Top = 4
          Width = 515
          Height = 16
          Align = alClient
          Caption = #1050#1086#1083#1080#1095#1077#1089#1090#1074#1086':'
          ExplicitWidth = 64
          ExplicitHeight = 13
        end
      end
    end
  end
  object StringGridScanDate: TStringGrid
    Left = 0
    Top = 0
    Width = 153
    Height = 464
    Align = alLeft
    ColCount = 1
    FixedCols = 0
    TabOrder = 1
    OnSelectCell = StringGridScanDateSelectCell
    ColWidths = (
      102)
  end
end
