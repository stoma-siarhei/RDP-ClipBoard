//---------------------------------------------------------------------------

#ifndef aboutH
#define aboutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfmAbout : public TForm
{
__published:	// IDE-managed Components
	TImage *imgAbout;
	TLabel *lblAbout;
	TLabel *lblGPL;
	TButton *btnOK;
	TLabel *lblPage;
	TLabel *lblMe;
	TLabel *lblMail;
	TLabel *lblEmail;
	void __fastcall lblPageClick(TObject *Sender);
	void __fastcall lblPageMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall lblPageMouseLeave(TObject *Sender);
	void __fastcall lblEmailClick(TObject *Sender);
	void __fastcall lblEmailMouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall lblEmailMouseLeave(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfmAbout(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmAbout *fmAbout;
//---------------------------------------------------------------------------
#endif
