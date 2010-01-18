VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Begin VB.Form frmPrincipal 
   Caption         =   "Probador del protocolo"
   ClientHeight    =   7560
   ClientLeft      =   60
   ClientTop       =   450
   ClientWidth     =   8925
   LinkTopic       =   "Form1"
   ScaleHeight     =   7560
   ScaleWidth      =   8925
   StartUpPosition =   3  'Windows Default
   Begin VB.ListBox lstChannel 
      Height          =   4155
      Left            =   120
      TabIndex        =   22
      Top             =   3240
      Width           =   8655
   End
   Begin VB.TextBox txtCRC 
      Height          =   375
      Left            =   6480
      TabIndex        =   20
      Text            =   "00"
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtLargo 
      Height          =   375
      Left            =   5280
      TabIndex        =   18
      Text            =   "00"
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtComando 
      Height          =   375
      Left            =   3840
      TabIndex        =   16
      Text            =   "00"
      Top             =   1560
      Width           =   615
   End
   Begin VB.CommandButton txtSend 
      Caption         =   "Send"
      Height          =   375
      Left            =   7800
      TabIndex        =   15
      Top             =   1560
      Width           =   975
   End
   Begin VB.TextBox txtDATOS 
      Height          =   375
      Left            =   3840
      TabIndex        =   13
      Top             =   2040
      Width           =   4935
   End
   Begin VB.TextBox txtIDO 
      Height          =   375
      Left            =   2160
      TabIndex        =   11
      Text            =   "00"
      Top             =   2040
      Width           =   615
   End
   Begin VB.TextBox txtTIPOO 
      Height          =   375
      Left            =   840
      TabIndex        =   9
      Text            =   "00"
      Top             =   2040
      Width           =   615
   End
   Begin VB.TextBox txtIDD 
      Height          =   375
      Left            =   2160
      TabIndex        =   7
      Text            =   "00"
      Top             =   1560
      Width           =   615
   End
   Begin VB.TextBox txtTIPOD 
      Height          =   375
      Left            =   840
      TabIndex        =   5
      Text            =   "00"
      Top             =   1560
      Width           =   615
   End
   Begin VB.CommandButton cmdClose 
      Caption         =   "Cerrar Puerto"
      Height          =   375
      Left            =   7560
      TabIndex        =   4
      Top             =   360
      Width           =   1215
   End
   Begin VB.CommandButton cmdOpen 
      Caption         =   "Abrir Puerto"
      Height          =   375
      Left            =   6240
      TabIndex        =   3
      Top             =   360
      Width           =   1215
   End
   Begin VB.TextBox txtComm 
      Height          =   375
      Left            =   720
      TabIndex        =   1
      Top             =   360
      Width           =   4095
   End
   Begin VB.CommandButton cmdCommProperties 
      Caption         =   "Configurar..."
      Height          =   375
      Left            =   4920
      TabIndex        =   0
      Top             =   360
      Width           =   1215
   End
   Begin MSCommLib.MSComm comm 
      Left            =   8160
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      DTREnable       =   -1  'True
      BaudRate        =   115200
   End
   Begin VB.Label lblCRC 
      AutoSize        =   -1  'True
      Caption         =   "CRC:"
      Height          =   195
      Left            =   6000
      TabIndex        =   21
      Top             =   1560
      Width           =   375
   End
   Begin VB.Label lblLargo 
      AutoSize        =   -1  'True
      Caption         =   "LARGO:"
      Height          =   195
      Left            =   4560
      TabIndex        =   19
      Top             =   1560
      Width           =   600
   End
   Begin VB.Label lblComando 
      AutoSize        =   -1  'True
      Caption         =   "COMANDO:"
      Height          =   195
      Left            =   2880
      TabIndex        =   17
      Top             =   1560
      Width           =   870
   End
   Begin VB.Label lblDATOS 
      AutoSize        =   -1  'True
      Caption         =   "DATOS:"
      Height          =   195
      Left            =   2880
      TabIndex        =   14
      Top             =   2040
      Width           =   600
   End
   Begin VB.Label lblIDO 
      AutoSize        =   -1  'True
      Caption         =   "ID_O:"
      Height          =   195
      Left            =   1560
      TabIndex        =   12
      Top             =   2040
      Width           =   420
   End
   Begin VB.Label lblTIPOO 
      AutoSize        =   -1  'True
      Caption         =   "TIPO_O:"
      Height          =   195
      Left            =   120
      TabIndex        =   10
      Top             =   2040
      Width           =   630
   End
   Begin VB.Label lblIDD 
      AutoSize        =   -1  'True
      Caption         =   "ID_D:"
      Height          =   195
      Left            =   1560
      TabIndex        =   8
      Top             =   1560
      Width           =   420
   End
   Begin VB.Label lblTIPOD 
      AutoSize        =   -1  'True
      Caption         =   "TIPO_D:"
      Height          =   195
      Left            =   120
      TabIndex        =   6
      Top             =   1560
      Width           =   630
   End
   Begin VB.Label lblComm 
      AutoSize        =   -1  'True
      Caption         =   "Comm:"
      Height          =   375
      Left            =   120
      TabIndex        =   2
      Top             =   360
      Width           =   480
   End
End
Attribute VB_Name = "frmPrincipal"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Private Sub cmdClose_Click()
    If comm.PortOpen = True Then comm.PortOpen = False
End Sub

Private Sub cmdCommProperties_Click()
    frmProperties.Show vbModal, Me
    refreshComm
End Sub

Public Sub refreshComm()
    txtComm.Text = "Comm:" & comm.CommPort & "," & comm.Settings & ", Handshaking: " & comm.Handshaking
End Sub

Private Sub cmdOpen_Click()
    If comm.PortOpen = False Then comm.PortOpen = True
End Sub

Private Sub Form_Load()
    refreshComm
End Sub

Private Sub txtComando_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtComando) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtCRC_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtCRC) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtIDD_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtIDD) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtIDO_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtIDO) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtLargo_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtLargo) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtSend_Click()
    Dim texto As String, txt2 As String
    texto = Chr(hexStringToInt(txtTIPOD.Text)) & Chr(hexStringToInt(txtIDD.Text)) & Chr(hexStringToInt(txtTIPOO.Text)) & Chr(hexStringToInt(txtIDO.Text)) & Chr(hexStringToInt(txtComando.Text))
    txt2 = (txtTIPOD.Text) & " " & (txtIDD.Text) & " " & (txtTIPOO.Text) & " " & (txtIDO.Text) & " " & (txtComando.Text)
    If Len(txtDATOS.Text) > 1 And Len(txtDATOS.Text) < 256 Then
        texto = texto & Chr((Len(txtDATOS.Text) \ 2))
        txt2 = txt2 & " " & Format((Len(txtDATOS.Text) \ 2), "00")
        For i = 1 To Len(txtDATOS.Text) Step 2
            texto = texto & Chr(hexStringToInt(Mid(txtDATOS.Text, i, 2)))
            txt2 = txt2 & " " & ((Mid(txtDATOS.Text, i, 2)))
        Next
    End If
    texto = texto & Chr(hexStringToInt(txtCRC.Text))
    txt2 = txt2 & " " & ((txtCRC.Text))
    If (comm.PortOpen = True) Then
        comm.Output = texto
    End If
    lstChannel.AddItem txt2
End Sub

Private Sub txtTIPOD_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtTIPOD) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Private Sub txtTIPOO_KeyPress(KeyAscii As Integer)
    If (KeyAscii < Asc(0) Or KeyAscii > Asc("9")) And (KeyAscii < Asc("a") Or KeyAscii > Asc("f")) And (KeyAscii < Asc("A") Or KeyAscii > Asc("F")) And KeyAscii <> 8 Then
        KeyAscii = 0
    End If
    
    If (Len(txtTIPOO) >= 2 And KeyAscii <> 8) Then
        KeyAscii = 0
    End If
End Sub

Public Function hexStringToInt(hexStr As String) As Integer
    If (Len(hexStr) <> 2) Then
        hexStringToInt = 0
    Else
        l = Asc(Mid(hexStr, 1, 1))
        If (l >= Asc(0) And l <= Asc("9")) Then
            hexStringToInt = l - Asc("0")
        ElseIf (l >= Asc("a") And l <= Asc("f")) Then
            hexStringToInt = l - Asc("a") + 10
        ElseIf (l >= Asc("A") And l <= Asc("F")) Then
            hexStringToInt = l - Asc("A") + 10
        Else
            hexStringToInt = 0
        End If
        hexStringToInt = hexStringToInt * 16
        l = Asc(Mid(hexStr, 2, 1))
        If (l >= Asc(0) And l <= Asc("9")) Then
            hexStringToInt = hexStringToInt + l - Asc("0")
        ElseIf (l >= Asc("a") And l <= Asc("f")) Then
            hexStringToInt = hexStringToInt + l - Asc("a") + 10
        ElseIf (l >= Asc("A") And l <= Asc("F")) Then
            hexStringToInt = hexStringToInt + l - Asc("A") + 10
        Else
            hexStringToInt = 0
        End If
    End If
        
End Function


