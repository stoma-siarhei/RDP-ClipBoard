//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include <Registry.hpp>
//---------------------------------------------------------------------------
int __fastcall TfmMain::ShowLocalHost(void)
{
    AnsiString LocalHostName = "";
    char cComputerName[MAX_COMPUTERNAME_LENGTH + 1 + 1];
    //MSDN: Windows Me/98/95:
    //GetComputerName fails if the input size is less than MAX_COMPUTERNAME_LENGTH + 1.
    unsigned long ilen  =  MAX_COMPUTERNAME_LENGTH + 1 + 1;
    if (::GetComputerName(cComputerName,&ilen))
    {
        LocalHostName = AnsiString(cComputerName);
    }
    else
    {
        Application->MessageBox("Can't get local host name! \
          It usually means the computer is not in a network. BeyondCopy will exit.", 
            "BeyondCopy", MB_OK + MB_ICONWARNING);
        Application->Terminate();
    }

    lvMain->Items->Clear();
    TListItem *firstItem = lvMain->Items->Add();
    firstItem->Caption = LocalHostName;
    firstItem->SubItems->Add("(localhost)");//add running flag

    return 1;
}
//-------------------------------------------------------------------
int __fastcall TfmMain::RestartServer(void)
{
    AnsiString info = "Start localhost server at port "
    				+ IntToStr (m_Port)
                    + " .";
	ShowInfo(info);

    serverSocketMain->Close();
	serverSocketMain->Active = false;

    serverSocketMain->Port = m_Port;
	serverSocketMain->Active = true;
    serverSocketMain->Open();
    return 1;    
}
//----------------------------------------------------------------------
int __fastcall TfmMain::UpdateConnection(TListItem *theItem)
{
  	//for error message, set m_tempIndex;
    m_tempIndex = theItem->Index;

    AnsiString info = "Try to conncet as a client. Server name: "
                    + theItem->Caption
                    + ".";
	ShowInfo(info);

  	TClientSocket *theClientSocket = (TClientSocket *)(theItem->Data);
    theClientSocket->Close();
    theClientSocket->Active = false;

    theClientSocket->Host = theItem->Caption;
    theClientSocket->Port= m_Port;

    //theClientSocket->Active = true;  add the line then using host has error.(can use IP only)
    theClientSocket->Open();//open will set Active to true.
    return 1;


}
//----------------------------------------------------------------------
void __fastcall TfmMain::d_ClientSocketError(TObject *Sender,
  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
    AnsiString info = "Can not connect the host "
                    + ShortMName(Socket->RemoteHost)
                    + ". ErrorEvent: "
    				+ IntToStr (ErrorEvent)
                    + ".";
    if(ErrorEvent == eeConnect)
    {
      	info += " Refused by remote host or BeyondCopy is not running?";
    }
    else
    {
      	info += " The Host is not running?";
    }

	ShowInfo(info);

    AnsiString retry = "Retry after " + IntToStr(con_connect_timer/1000) + " second.";
    ShowInfo(retry);       
    //set the UI
    lvMain->Items->Item[m_tempIndex]->SubItems->Strings[0] = con_disconnected;
	ErrorCode = 0;//do not show error box
}
//----------------------------------------------------------------------
void __fastcall TfmMain::d_ClientSocketConnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString info = "Successful!";
	ShowInfo(info);
    //set the UI
    lvMain->Items->Item[m_tempIndex]->SubItems->Strings[0] = con_connected;
}
//----------------------------------------------------------------------
void __fastcall TfmMain::d_ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket)
{
    AnsiString info = "Disconnected. The remote host is closed or BeyondCopy in remote host is closed?";
	ShowInfo(info);

    //set the UI
    lvMain->Items->Item[m_tempIndex]->SubItems->Strings[0] = con_disconnected;
    
}
//----------------------------------------------------------------------
int __fastcall TfmMain::ServerCheckAccept(TCustomWinSocket *Socket, bool isReceive)
{
  	/******************TODO: why the check will fail???*******************
	AnsiString remoteHost = ShortMName(Socket->RemoteHost);
    //check if the client is in the list. If not, do not connect it.
    bool sAccept = false;
    for(int i = 1; i < lvMain->Items->Count; i++)
    {
        if(remoteHost.AnsiCompareIC(lvMain->Items->Item[i]->Caption) == 0)//in the list
        {
        	sAccept = true;
            break;
        }
    }
    if(!sAccept)
    {
      	Socket->Close();//do not accept
        if(isReceive)
        {
        	ShowInfo("Warning! Host " + remoteHost + " tries to send infomation to localhost. It is not in the hosts list. Disconnected.");
        }
        else
        {
        	ShowInfo("Warning! Host " + remoteHost + " tries to connect as a client. It is not in the hosts list. Disconnected.");
        }
    }
    ************************************************************************/
    return 1;
}
//----------------------------------------------------------------------
int __fastcall TfmMain::AutoConnectAll(void)
{
    for(int i = 1; i < lvMain->Items->Count; i++)
    {
        TListItem *newItem = lvMain->Items->Item[i];
        if( newItem->SubItems->Strings[0] == con_disconnected)
        {
        	UpdateConnection(newItem);//update the TClientSocket
        }
    }
  	return 1;
}
//-----------------------------------------------------------------------
int __fastcall TfmMain::writeThekey(void)
{
    if(Autorunwithsystem1->Checked)//write the key
    {
        AnsiString now = Application->ExeName;
        TRegistry *Registry = new TRegistry(KEY_ALL_ACCESS);
        try
        {
            Registry->RootKey = HKEY_CURRENT_USER;
            // true because we want to create it if it doesn't exist
            if (Registry->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Run",true))
            {
                Registry->WriteString("BeyondCopy",Application->ExeName);
                Registry->CloseKey();
            }
        }
        __finally
        {
            delete Registry;
        }
    }
    else//delete the key
    {
        TRegistry *Registry = new TRegistry(KEY_ALL_ACCESS);
        try
        {
            Registry->RootKey = HKEY_CURRENT_USER;
            // false because we do not want to create it if it doesn't exist
            if (Registry->OpenKey("Software\\Microsoft\\Windows\\CurrentVersion\\Run",false))
            {
                Registry->DeleteValue("BeyondCopy");
                Registry->CloseKey();
            }
        }
        __finally
        {
            delete Registry;
        }
    }
	return 1;
}
//-----------------------------------------------------------------------------
AnsiString __fastcall TfmMain::ShortMName(const AnsiString &host)
{
  	if(host.AnsiPos(".") <= 0)
    {
      	return host;
    }
    else
    {
  		return host.SubString(1, host.AnsiPos(".")-1);
    }
}
//-----------------------------------------------------------------------------
int __fastcall TfmMain::MarkClientDisConnected(TCustomWinSocket *Socket)
{
	AnsiString remoteHost = ShortMName(Socket->RemoteHost);
	//mark as disconnected
    for(int i = 1; i < lvMain->Items->Count; i++)
    {
        if(remoteHost.AnsiCompareIC(lvMain->Items->Item[i]->Caption) == 0)//in the list
        {
          	lvMain->Items->Item[i]->SubItems->Strings[0] = con_disconnected;
            TClientSocket *theClient = (TClientSocket *)lvMain->Items->Item[i]->Data;
            theClient->Socket->Close();

            ShowInfo("Client " + remoteHost + " disconnected.");
        }
    }
    return 1;
}
//-----------------------------------------------------------------------------




