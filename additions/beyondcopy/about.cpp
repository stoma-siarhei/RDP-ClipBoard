//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfmAbout *fmAbout;
//---------------------------------------------------------------------------
__fastcall TfmAbout::TfmAbout(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::lblPageClick(TObject *Sender)
{
	ShellExecute(Handle, "open", "http://beyondcopy.sourceforge.net",NULL,NULL,SW_SHOWDEFAULT);
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::lblPageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	//underline
    lblPage->Font->Style = TFontStyles() << fsUnderline;

}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::lblPageMouseLeave(TObject *Sender)
{
    lblPage->Font->Style = TFontStyles();
}
//---------------------------------------------------------------------------
void __fastcall TfmAbout::lblEmailClick(TObject *Sender)
{
	ShellExecute(Handle, "open", "mailto:liuxuhere@hotmail.com",NULL,NULL,SW_SHOWDEFAULT);  
}
//---------------------------------------------------------------------------

void __fastcall TfmAbout::lblEmailMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
	//underline
    lblEmail->Font->Style = TFontStyles() << fsUnderline;	
}
//---------------------------------------------------------------------------


void __fastcall TfmAbout::lblEmailMouseLeave(TObject *Sender)
{
    lblEmail->Font->Style = TFontStyles();
}
//---------------------------------------------------------------------------

