//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ScktComp.hpp>
#include "trayicon.h"
#include "systdate.h"
//---------------------------------------------------------------------------
#include "defaultPara.h"
#include <AppEvnts.hpp>
//---------------------------------------------------------------------------
typedef struct _DROPFILES {
   DWORD pFiles;                       // offset of file list
   POINT pt;                           // drop point (client coords)
   BOOL fNC;                           // is it on NonClient area
                                       // and pt is in screen coords
   BOOL fWide;                         // WIDE character switch
} DROPFILES, *LPDROPFILES;//for clipborad
class TfmMain : public TForm
{
__published:	// IDE-managed Components
    TStatusBar *statMain;
    TMainMenu *mmMain;
    TMenuItem *Port1;
    TMenuItem *Help1;
    TMenuItem *ChangePort1;
    TMenuItem *About1;
    TPanel *pnlUp;
    TPanel *pnlBottom;
    TPanel *pnlUL;
    TListView *lvMain;
    TPanel *pnlUR;
    TButton *btnAdd;
    TButton *btnChange;
    TButton *btnDel;
    TMenuItem *Autorunwithsystem1;
    TPanel *pnlBo3;
    TMemo *mmoMain;
    TPanel *pnlBo2;
    TLabel *lblInfo;
	TServerSocket *serverSocketMain;
	TTimer *tmrClientSendToAll;
	TTrayIcon *trycnMain;
	TTimer *tmrConnect;
	TMenuItem *N1;
	TMenuItem *FAQ1;
	TMenuItem *Licence1;
	TMenuItem *WhatsNew1;
	TPopupMenu *pmMain;
	TMenuItem *OpenBeyondCopy1;
	TMenuItem *About2;
	TMenuItem *N2;
	TMenuItem *Exit1;
    void __fastcall ChangePort1Click(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall serverSocketMainClientRead(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall tmrClientSendToAllTimer(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall serverSocketMainAccept(TObject *Sender,
          TCustomWinSocket *Socket);
	void __fastcall tmrConnectTimer(TObject *Sender);
	void __fastcall btnChangeClick(TObject *Sender);
	void __fastcall btnDelClick(TObject *Sender);
	void __fastcall lvMainSelectItem(TObject *Sender, TListItem *Item,
          bool Selected);
	void __fastcall Autorunwithsystem1Click(TObject *Sender);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall trycnMainMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FAQ1Click(TObject *Sender);
	void __fastcall Licence1Click(TObject *Sender);
	void __fastcall WhatsNew1Click(TObject *Sender);
	void __fastcall serverSocketMainClientError(TObject *Sender,
          TCustomWinSocket *Socket, TErrorEvent ErrorEvent,
          int &ErrorCode);
	void __fastcall OpenBeyondCopy1Click(TObject *Sender);
	void __fastcall About2Click(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall serverSocketMainClientDisconnect(TObject *Sender,
          TCustomWinSocket *Socket);
private:	// User declarations
    int m_Port;//the Port used.
//------help functions -------------------------------
    int __fastcall InputPort(void); //change port
    int __fastcall ShowLocalHost(void); //get Local Host name

    int __fastcall AddNewHost(void);//add new host
    int __fastcall AddHost(const AnsiString &hostName);//add host

    int __fastcall DelHost(bool isDelAll);//del host
    int __fastcall DelTheHost();//del host by hand

    int __fastcall ChangeHost(void);

    AnsiString __fastcall InputHostName(const AnsiString &dTitle,
    		const AnsiString &defaultName);//input host name    

	int __fastcall RestartServer(void);//restart the server socket

    int __fastcall UpdateConnection(TListItem *theItem); //update as client
    int __fastcall AutoConnectAll(void); //update as client    

    void __fastcall d_ClientSocketError(TObject *Sender,
      TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode);

    void __fastcall d_ClientSocketConnect(TObject *Sender,
          TCustomWinSocket *Socket);

    void __fastcall d_ClientSocketDisconnect(TObject *Sender,
      TCustomWinSocket *Socket);

    //ini file--------------------------------
    int __fastcall ReadIniInfo(void);
    int __fastcall WriteIniInfo(void);

    //----------------------------------------
    int __fastcall ShowInfo(AnsiString info);//info record

    int __fastcall SendInfoToAll(void);
    int __fastcall ReceivedInfo(TCustomWinSocket *Socket);


    int __fastcall ServerCheckAccept(TCustomWinSocket *Socket, bool isReceive);

    int __fastcall MarkClientDisConnected(TCustomWinSocket *Socket);

    int m_tempIndex;//The tempFlag which is used to change index state

    char *m_buffer;//the buffer is used to avoid send or write many times.
    //when send info, 1)only when the buffer and clipboard is different.
    //2) buffer must be write after send.
    //when receive info, 
    //1) buffer must be write after clipboard is write.

    //UI
    int __fastcall UpdateButton(bool Selected, bool isLocalHost);
	int __fastcall writeThekey(void);

    //change machine as.sd.sd.com to as
    static AnsiString __fastcall ShortMName(const AnsiString &host);

    TDateTime m_time;
    //-------system----
    bool m_SignalGenerator;
//---------------------------------------------------------------------------

public:		// User declarations
    __fastcall TfmMain(TComponent* Owner);

	void __fastcall OnWMQueryEndSession(TMessage &Message);    

BEGIN_MESSAGE_MAP  
	VCL_MESSAGE_HANDLER(WM_QUERYENDSESSION,TMessage,OnWMQueryEndSession)  
END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TfmMain *fmMain;
//---------------------------------------------------------------------------
#endif
