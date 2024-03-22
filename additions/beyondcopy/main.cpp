//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "about.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "trayicon"
#pragma resource "*.dfm"
TfmMain *fmMain;
//---------------------------------------------------------------------------
__fastcall TfmMain::TfmMain(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::ChangePort1Click(TObject *Sender)
{
    InputPort();//set the m_Port value
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::FormCreate(TObject *Sender)
{
  	m_SignalGenerator = false;//no close machine

    ShowLocalHost();//add hosts name to list
    ReadIniInfo();//read ini file from ini file(add client too)

	m_buffer = new char[con_max_send + 1];

    tmrClientSendToAll->Interval = con_send_timer;
    tmrClientSendToAll->Enabled = true;//enable sender

    tmrConnect->Interval = con_connect_timer;
    tmrConnect->Enabled = true;//enable sender

    RestartServer();//restart the socket server

	//if has ini file, minimaize it.
  	int iFileHandle;
    iFileHandle = FileOpen(ChangeFileExt(Application->ExeName, ".ini"), fmOpenRead);
    if( iFileHandle >= 0)
    {
    	ShowWindow(Application->Handle,SW_HIDE);
    	Application->ShowMainForm=false;
		fmMain->Visible = false;//show form

		FileClose(iFileHandle);
    }
    else
    {
      	Application->MessageBox("This is the first time you use BeyondCopy. Please notice BeyondCopy must be run on all of your computers.  ", 
            "BeyondCopy", MB_OK + MB_ICONINFORMATION);
    }    
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btnAddClick(TObject *Sender)
{
	AddNewHost();//it should check 1)if the host exist 2)is the host name valid?
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::serverSocketMainClientRead(TObject *Sender,
      TCustomWinSocket *Socket)
{
	ServerCheckAccept(Socket, true);
	ReceivedInfo(Socket);
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::tmrClientSendToAllTimer(TObject *Sender)
{
	SendInfoToAll();//send localhost Clipboard Info to all connected server 
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormDestroy(TObject *Sender)
{
	WriteIniInfo();

	serverSocketMain->Close();//close server
  	DelHost(true);//delete all host items (close client too)
	delete []m_buffer; //delete send buffer

    //save the auto start key
    writeThekey();

    	
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::serverSocketMainAccept(TObject *Sender,
      TCustomWinSocket *Socket)
{
	ServerCheckAccept(Socket, false);		
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::tmrConnectTimer(TObject *Sender)
{
	AutoConnectAll();//connect all un connected server
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btnChangeClick(TObject *Sender)
{
	ChangeHost();	
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::btnDelClick(TObject *Sender)
{
	DelTheHost();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::lvMainSelectItem(TObject *Sender, TListItem *Item,
      bool Selected)
{
  	bool isLocalHost = (Item->Index == 0);
	UpdateButton(Selected, isLocalHost);//isLocalHost
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Autorunwithsystem1Click(TObject *Sender)
{
	Autorunwithsystem1->Checked = !Autorunwithsystem1->Checked;
    WriteIniInfo();//save ini file	    
}
//---------------------------------------------------------------------------


void __fastcall TfmMain::About1Click(TObject *Sender)
{
	fmAbout->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
    ShowWindow(Application->Handle,SW_HIDE);
    Application->ShowMainForm=false;
    fmMain->Visible = false;//show form

    CanClose = m_SignalGenerator;
}
//---------------------------------------------------------------------------


void __fastcall TfmMain::trycnMainMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  	if(Button == mbLeft)
  	{
        ShowWindow(Application->Handle,SW_RESTORE);
        Application->ShowMainForm=true;
        fmMain->Visible = true;//show form
        
    }
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::FAQ1Click(TObject *Sender)
{
  	AnsiString faqName = ExtractFilePath(Application->ExeName) + "FAQ.txt";
	//check the file exist
  	int iFileHandle;
    iFileHandle = FileOpen(faqName, fmOpenRead);
    if( iFileHandle >= 0)
    {
		FileClose(iFileHandle);
		ShellExecute(Handle, "open", faqName.c_str() ,NULL,NULL,SW_SHOWDEFAULT);        
    }
    else
    {
      	Application->MessageBox("Can not open FAQ.txt. Please install BeyondCopy again.", 
            "BeyondCopy", MB_OK + MB_ICONINFORMATION);
    }    


}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Licence1Click(TObject *Sender)
{
  	AnsiString leName = ExtractFilePath(Application->ExeName) + "License.txt";
  	int iFileHandle;
    iFileHandle = FileOpen(leName, fmOpenRead);
    if( iFileHandle >= 0)
    {
		FileClose(iFileHandle);
		ShellExecute(Handle, "open", leName.c_str() ,NULL,NULL,SW_SHOWDEFAULT);
    }
    else
    {
      	Application->MessageBox("Can not open License.txt. Please install BeyondCopy again.",
            "BeyondCopy", MB_OK + MB_ICONINFORMATION);

    }
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::WhatsNew1Click(TObject *Sender)
{
  	AnsiString nwName = ExtractFilePath(Application->ExeName) + "Whatisnew.txt";
  	int iFileHandle;
    iFileHandle = FileOpen(nwName, fmOpenRead);
    if( iFileHandle >= 0)
    {
		FileClose(iFileHandle);
		ShellExecute(Handle, "open", nwName.c_str() ,NULL,NULL,SW_SHOWDEFAULT);
    }
    else
    {
      	Application->MessageBox("Can not open Whatisnew.txt. Please install BeyondCopy again.",
            "BeyondCopy", MB_OK + MB_ICONINFORMATION);

    }
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::serverSocketMainClientError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	ErrorCode = 0;//do not show error box. ignore it.(client show it)	
}
//---------------------------------------------------------------------------
//---------send "close machine" message--------------------------------------
void __fastcall TfmMain::OnWMQueryEndSession(TMessage &Message)
{
  	Message.Result = 1;
    m_SignalGenerator = true; //SignalGenerator-->OnCloseQuery(is it close machine?)
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TfmMain::OpenBeyondCopy1Click(TObject *Sender)
{
    ShowWindow(Application->Handle,SW_RESTORE);
    Application->ShowMainForm=true;
	fmMain->Visible = true;//show form
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::About2Click(TObject *Sender)
{
	fmAbout->ShowModal();	
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::Exit1Click(TObject *Sender)
{
    m_SignalGenerator = true;
	Close();	
}
//---------------------------------------------------------------------------

void __fastcall TfmMain::serverSocketMainClientDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
	MarkClientDisConnected(Socket);	
}
//---------------------------------------------------------------------------

