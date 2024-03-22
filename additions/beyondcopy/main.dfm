object fmMain: TfmMain
  Left = 360
  Top = 263
  Width = 547
  Height = 401
  Caption = 'BeyondCopy'
  Color = clBtnFace
  Constraints.MinHeight = 400
  Constraints.MinWidth = 500
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = mmMain
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object statMain: TStatusBar
    Left = 0
    Top = 336
    Width = 539
    Height = 19
    Panels = <
      item
        Width = 50
      end>
    SimplePanel = False
  end
  object pnlUp: TPanel
    Left = 0
    Top = 0
    Width = 539
    Height = 153
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object pnlUL: TPanel
      Left = 0
      Top = 0
      Width = 437
      Height = 153
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object lvMain: TListView
        Left = 0
        Top = 0
        Width = 437
        Height = 153
        Align = alClient
        BevelInner = bvNone
        BevelOuter = bvNone
        Columns = <
          item
            AutoSize = True
            Caption = 'Host'
          end
          item
            AutoSize = True
            Caption = 'State'
          end>
        ColumnClick = False
        GridLines = True
        HideSelection = False
        ReadOnly = True
        RowSelect = True
        TabOrder = 0
        ViewStyle = vsReport
        OnSelectItem = lvMainSelectItem
      end
    end
    object pnlUR: TPanel
      Left = 437
      Top = 0
      Width = 102
      Height = 153
      Align = alRight
      BevelOuter = bvLowered
      TabOrder = 1
      object btnAdd: TButton
        Left = 14
        Top = 16
        Width = 75
        Height = 25
        Caption = '&Add...'
        TabOrder = 0
        OnClick = btnAddClick
      end
      object btnChange: TButton
        Left = 14
        Top = 56
        Width = 75
        Height = 25
        Caption = '&Change...'
        Enabled = False
        TabOrder = 1
        OnClick = btnChangeClick
      end
      object btnDel: TButton
        Left = 14
        Top = 96
        Width = 75
        Height = 25
        Caption = '&Delete'
        Enabled = False
        TabOrder = 2
        OnClick = btnDelClick
      end
    end
  end
  object pnlBottom: TPanel
    Left = 0
    Top = 153
    Width = 539
    Height = 183
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object pnlBo3: TPanel
      Left = 0
      Top = 25
      Width = 539
      Height = 158
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 0
      object mmoMain: TMemo
        Left = 0
        Top = 0
        Width = 539
        Height = 158
        Align = alClient
        BevelInner = bvNone
        ReadOnly = True
        ScrollBars = ssVertical
        TabOrder = 0
      end
    end
    object pnlBo2: TPanel
      Left = 0
      Top = 0
      Width = 539
      Height = 25
      Align = alTop
      BevelOuter = bvLowered
      TabOrder = 1
      object lblInfo: TLabel
        Left = 8
        Top = 6
        Width = 115
        Height = 13
        Caption = 'BeyondCopy Infomation:'
      end
    end
  end
  object mmMain: TMainMenu
    Left = 384
    object Port1: TMenuItem
      Caption = 'Options'
      object ChangePort1: TMenuItem
        Caption = 'Change Port...'
        OnClick = ChangePort1Click
      end
      object Autorunwithsystem1: TMenuItem
        Caption = 'Auto Run With System'
        Checked = True
        OnClick = Autorunwithsystem1Click
      end
    end
    object Help1: TMenuItem
      Caption = 'Help'
      object FAQ1: TMenuItem
        Caption = 'FAQ...'
        OnClick = FAQ1Click
      end
      object WhatsNew1: TMenuItem
        Caption = 'What'#39's New...'
        OnClick = WhatsNew1Click
      end
      object Licence1: TMenuItem
        Caption = 'Licence...'
        OnClick = Licence1Click
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object About1: TMenuItem
        Caption = 'About...'
        OnClick = About1Click
      end
    end
  end
  object serverSocketMain: TServerSocket
    Active = False
    Port = 0
    ServerType = stNonBlocking
    OnAccept = serverSocketMainAccept
    OnClientDisconnect = serverSocketMainClientDisconnect
    OnClientRead = serverSocketMainClientRead
    OnClientError = serverSocketMainClientError
    Left = 352
  end
  object tmrClientSendToAll: TTimer
    Enabled = False
    OnTimer = tmrClientSendToAllTimer
    Left = 320
  end
  object trycnMain: TTrayIcon
    Visible = True
    Hint = 'BeyondCopy'
    PopupMenu = pmMain
    Hide = True
    RestoreOn = imLeftClickUp
    PopupMenuOn = imRightClickUp
    OnMouseDown = trycnMainMouseDown
    Left = 288
  end
  object tmrConnect: TTimer
    Enabled = False
    OnTimer = tmrConnectTimer
    Left = 256
  end
  object pmMain: TPopupMenu
    Left = 224
    object OpenBeyondCopy1: TMenuItem
      Caption = 'Open BeyondCopy'
      Default = True
      OnClick = OpenBeyondCopy1Click
    end
    object About2: TMenuItem
      Caption = 'About'
      OnClick = About2Click
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object Exit1: TMenuItem
      Caption = 'Exit'
      OnClick = Exit1Click
    end
  end
end
