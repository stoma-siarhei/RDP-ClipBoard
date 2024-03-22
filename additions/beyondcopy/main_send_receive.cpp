//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include <Clipbrd.hpp>

//---------------------------------------------------------------------------
/*
The text send format is : tABC  which send ABC
The file send format is : f\\10.20.30.40\C$\1.txt>\\10.20.30.40\C$\2.txt>  which send
                          1.txt and 2.txt file 
*/
//---------------------------------------------------------------------------
int __fastcall TfmMain::SendInfoToAll(void)
{
  	if(lvMain->Items->Count > 1)//has other host
    {
		//test clipboard firstly
        try
        {
            if(!OpenClipboard(Application->Handle))
            {
                //can not open
                ShowInfo("Can't send info. Can't open clipboard. Maybe it is being used by another program. ");
                return 0;
            }
            else
            {
                //close it
                CloseClipboard();
            }
        }
        catch(...)
        {
          	ShowInfo("Can't send info. Can't open clipboard. Maybe it is being used by another program. ");
            return 0;
        }        

        int len = -1;;//length of the send
        if (Clipboard()->HasFormat(CF_TEXT))
        {
            AnsiString tCp= "t" + Clipboard()->AsText;
            if(tCp.Length() <= con_max_send && strcmp(tCp.c_str(), m_buffer) != 0 )
            {
                strcpy (m_buffer,tCp.c_str());//update buffer
                len = tCp.Length();
            }
        }
        else if (Clipboard()->HasFormat(CF_HDROP))
        {
            if(OpenClipboard(Application->Handle))
            {
                HDROP hd = (HDROP)GetClipboardData(CF_HDROP);
                CloseClipboard();

                AnsiString allfile = "";

                TCHAR filename[_MAX_PATH + 1];                
                int fileCount;
                fileCount = DragQueryFile(hd,0xFFFFFFFF,filename,_MAX_PATH + 1); //msdn
                for(int i=0;i < fileCount ;i++)
                {
                    DragQueryFile(hd,i,filename,_MAX_PATH + 1);

                    AnsiString temStr(filename);
                    if(temStr != "")
                    {
                        //if the filename is like C:\i.txt change it to \\hostname\c$\i.txt
                        //if like "\\..." , do not change it
                        if(temStr.SubString(1,2) != "\\\\")
                        {
                            temStr[temStr.AnsiPos(":")] = '$';
                            temStr = "\\\\"
                                   + lvMain->Items->Item[0]->Caption //local hostname
                                   + "\\"
                                   + temStr;
                        }
                        allfile += temStr;
                        allfile += ">";//split names
                    }
                    else
                    {
                        break;
                    }
                }


                AnsiString tCp= "f" + allfile;
                if(tCp.Length() <= con_max_send && strcmp(tCp.c_str(), m_buffer) != 0 )
                {
                    strcpy (m_buffer,tCp.c_str());//update buffer
                    len = tCp.Length();
                }
            }
        }
        //send m_buffer to all
        if(len != -1)
        {
            for(int i = 1; i < lvMain->Items->Count; i++)
            {
                if(lvMain->Items->Item[i]->SubItems->Strings[0] == con_connected)
                {
                    TClientSocket *theClient = (TClientSocket *)lvMain->Items->Item[i]->Data;
                    try
                    {
                        theClient->Socket->SendBuf(m_buffer,len);//error will in disconnect event
                        ShowInfo("Send info to host " + ShortMName(theClient->Socket->RemoteHost) + " in the list.");
                    }
                    catch (const ESocketError& err)
                    {
                        lvMain->Items->Item[i]->SubItems->Strings[0] = con_disconnected;
                        theClient->Socket->Close();
                    }
                }
            }
        }
    }
    return 1;
}
//----------------------------------------------------------------------------
int __fastcall TfmMain::ReceivedInfo(TCustomWinSocket *Socket)
{
    int len;//received text length
    //clean the buffer because there is no '\0' in the received string.
    for(int i=0;i < con_max_send + 1;i++)
    {
    	m_buffer[i] = '\0';
    }
    len = Socket->ReceiveBuf(m_buffer,con_max_send);//m_buffer is updated

    if(len > 1)//has 1 flag
    {

          //test clipboard firstly
          try
          {
              if(!OpenClipboard(Application->Handle))
              {
                  //can not open
                  ShowInfo("Can't receive info. Can't open clipboard. Maybe it is being used by another program. ");
                  return 0;
              }
              else
              {
                  //close it
                  CloseClipboard();
              }
          }
          catch(...)
          {
              ShowInfo("Can't receive info. Can't open clipboard. Maybe it is being used by another program. ");
              return 0;
          }




          char flag = m_buffer[0];
          if(flag == 't')
          {
              Clipboard()->SetTextBuf(m_buffer+1);
          }
          else//== 'f'
          {
              char szFiles[con_max_send];
              for(int j=0;j< con_max_send;j++)
              {
                  szFiles[j] = '\0';
              }
              int num;
              for(num=0;num<(len-1); num++)
              {
                  if(m_buffer[num+1] != '>')
                  {
                      szFiles[num] = m_buffer[num+1];
                  }
                  else
                  {
                      szFiles[num] = '\0';
                  }
              }
              //change it in later version
              if (::OpenClipboard (Application->Handle))
              {
                  ::EmptyClipboard ();
                  int nSize = sizeof (DROPFILES) + sizeof (szFiles);
                  HANDLE hData = ::GlobalAlloc (GHND, nSize);
                  LPDROPFILES pDropFiles = (LPDROPFILES) ::GlobalLock (hData);
                  pDropFiles->pFiles = sizeof (DROPFILES);

              #ifdef UNICODE
                  pDropFiles->fWide = TRUE;
              #else
                  pDropFiles->fWide = FALSE;
              #endif

                  LPBYTE pData = (LPBYTE) pDropFiles + sizeof (DROPFILES);
                  ::CopyMemory (pData, szFiles, sizeof (szFiles));
                  ::GlobalUnlock (hData);
                  ::SetClipboardData (CF_HDROP, hData);
                  ::CloseClipboard ();
                  //book:Programming Windows with MFC
              }
          }
          ShowInfo("Received info from host " + ShortMName(Socket->RemoteHost) + " .");
    }
    return 1;
}
