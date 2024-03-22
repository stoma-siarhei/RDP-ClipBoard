//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"

#include <inifiles.hpp>
//---------------------------------------------------------------------------
int __fastcall TfmMain::ReadIniInfo(void)
{
    TIniFile *ini;
    ini = new TIniFile(ChangeFileExt(Application->ExeName, ".ini" ) );
    AnsiString hosts = ini->ReadString( "INFOR", "HOSTS", "");
    m_Port = ini->ReadInteger( "INFOR", "PORT", con_port ); //start client here and server in FormCreate
    Autorunwithsystem1->Checked = ini->ReadBool ( "INFOR", "AUTO", true );//set auto
    delete ini;
    
    //show port
    statMain->Panels->Items[0]->Text = "Port: " + IntToStr(m_Port);

    //show and load these host
    //ab>cd>ed>
    AnsiString theHost;
    for(;hosts.Length() != 0;)
    {
      	theHost = hosts.SubString(1, hosts.AnsiPos(">")-1);
        AddHost(theHost);
        hosts = hosts.SubString(hosts.AnsiPos(">")+1, hosts.Length()-theHost.Length()-1);
    }
    return 1;
}
//--------------------------------------------------------------------------
int __fastcall TfmMain::WriteIniInfo(void)
{
  	AnsiString hosts = "";
    for(int i = 1; i < lvMain->Items->Count; i++)
    {
        hosts += lvMain->Items->Item[i]->Caption;
        hosts += ">";
    }

	TIniFile *ini;
    ini = new TIniFile(ChangeFileExt( Application->ExeName, ".ini" ) );
    ini->WriteString( "INFOR", "HOSTS", hosts);
    ini->WriteString( "INFOR", "PORT", m_Port );
    ini->WriteBool( "INFOR", "AUTO", Autorunwithsystem1->Checked );
    delete ini;

    return 1;
}
//--------------------------------------------------------------------------
