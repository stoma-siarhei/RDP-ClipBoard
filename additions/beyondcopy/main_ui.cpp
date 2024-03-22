//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

//---------------------------------------------------------------------------
int __fastcall TfmMain::InputPort(void)
{
    Application->MessageBox("Please make sure you use the same BeyondCopy port at all the hosts in the list.",
        "BeyondCopy", MB_OK + MB_ICONINFORMATION);

    bool inputAgain = true;
    int intPort;
    AnsiString errInfo = "";
    while(inputAgain)
    {
        AnsiString InputString = InputBox("Change Port",
        errInfo + "Please enter a interger between "
        + IntToStr(con_min_port)
        + " and "
        + IntToStr(con_max_port)
        +":",
        IntToStr(m_Port));
        inputAgain = false;
        try
        {
          	//TODO: check the port available
            intPort = StrToInt(InputString);
            if(intPort > con_max_port || intPort < con_min_port)
            {
                errInfo = "The input \""
                        + InputString
                        + "\" is too large or too small. ";
                inputAgain = true;
            }
        }
        catch(const EConvertError &err)
        {
            errInfo = "The input \""
                    + InputString
                    + "\" is not a valid interger. ";
            inputAgain = true;
        }

    }
    if(intPort != m_Port)
    {
        m_Port = intPort;
        //set the status panel
        statMain->Panels->Items[0]->Text = "Port: " + IntToStr(m_Port);
        //restart server
        RestartServer();
        //restart all client
        for(int i = 1; i < lvMain->Items->Count; i++)
        {
            TListItem *theItem = lvMain->Items->Item[i];
            UpdateConnection(theItem);//update the TClientSocket
        }
        WriteIniInfo();//save ini file
    }

    return 1;
}
//---------------------------------------------------------------------------
int __fastcall TfmMain::AddNewHost(void)
{
	AnsiString newHost = InputHostName("Add Host", "");
    if(newHost != "")
    {
      	AddHost(newHost);//add to ui
    }
    WriteIniInfo();//save ini file	    
    return 1;
}
//----------------------------------------------------------------------------
int __fastcall TfmMain::AddHost(const AnsiString &hostName)
{
    TListItem *newItem = lvMain->Items->Add();
    newItem->Caption = hostName;
    newItem->SubItems->Add(con_disconnected);//add running flag. It will be updated.
    newItem->Data = (void*)new TClientSocket(this);
    ((TClientSocket *)newItem->Data)->OnError = d_ClientSocketError;
    ((TClientSocket *)newItem->Data)->OnConnect = d_ClientSocketConnect;
    ((TClientSocket *)newItem->Data)->OnDisconnect = d_ClientSocketDisconnect;    



    UpdateConnection(newItem);//update the TClientSocket
    return 1;
}


//-----------------------------------------------------------------------------
int __fastcall TfmMain::ShowInfo(AnsiString info)//info record
{
    if(mmoMain->Lines->Count >= con_max_infolines)
    {
    	mmoMain->Lines->Clear();
    }
  	mmoMain->Lines->Add(m_time.CurrentTime().FormatString("[hh:nn:ss] ") + info);
    return 1;
}
//-----------------------------------------------------------------------------
int __fastcall TfmMain::UpdateButton(bool Selected, bool isLocalHost)
{
  	if(!isLocalHost)
    {
  		btnChange->Enabled = Selected;
  		btnDel->Enabled = Selected;
    }
    else
    {
  		btnChange->Enabled = false;
  		btnDel->Enabled = false;
    }    
      	
    return 1;
}
//-----------------------------------------------------------------------------
int __fastcall TfmMain::DelTheHost()
{
	DelHost(false);//not all
	WriteIniInfo();//save ini file
    return 1;
}
//-----------------------------------------------------------------------------
int __fastcall TfmMain::DelHost(bool isDelAll)//del host
{

    for(int i = 1; i < lvMain->Items->Count; i++)
    {
      	if(isDelAll || lvMain->Items->Item[i]->Selected)
        {
            TClientSocket *theClient = (TClientSocket *)lvMain->Items->Item[i]->Data;
            theClient->Close();
            theClient->Active = false;
            delete theClient;
            
            lvMain->Items->Delete(i);
		}

    }
    return 1;
}
//------------------------------------------------------------------------------
int __fastcall TfmMain::ChangeHost(void)
{
	AnsiString newHost = InputHostName("Change Host", lvMain->Selected->Caption);// the firest one can not be changed
    if(newHost != "")
    {
        for(int i = 1; i < lvMain->Items->Count; i++)
        {
            if(lvMain->Items->Item[i]->Selected)
            {
        		lvMain->Items->Item[i]->Caption = newHost;
				lvMain->Items->Item[i]->SubItems->Strings[0] = con_disconnected;
                UpdateConnection(lvMain->Items->Item[i]);//update the TClientSocket
                break;//for only one item
            }
        }
    }
    WriteIniInfo();//save ini file	    
    return 1;
}
//------------------------------------------------------------------------------
AnsiString __fastcall TfmMain::InputHostName(const AnsiString &dTitle, const AnsiString &defaultName)//input host name
{
    bool inputAgain = true;
    AnsiString newHost = "";
    AnsiString errInfo = "";
    while(inputAgain)
    {
        newHost = InputBox(dTitle,
          errInfo + "Please enter the new host name:",
          defaultName);
        inputAgain = false;

        //TODO: check the host name more
        //do not check the length for a strange case: name long
        //if(newHost.Length() > MAX_COMPUTERNAME_LENGTH)
        //{
        //    errInfo = "The input \""
        //            + newHost
        //            + "\" is not valid (too long). ";
        //    inputAgain = true;
        //}
        //else
        if(newHost != defaultName)
        {
          	//for debug the program you may want to not check the name
            //---------------------------------------------------
              //check if the name is in the list
              for(int i = 0; i < lvMain->Items->Count; i++)
              {
                  if(newHost.AnsiCompareIC(lvMain->Items->Item[i]->Caption) == 0)
                  {
                      errInfo = "The input host name \""
                              + newHost
                              + "\" is already in the list. ";
                      inputAgain = true;
                      break;
                  }
              }
            //----------------------------------------------------  
        }
    }
    return newHost;
}
