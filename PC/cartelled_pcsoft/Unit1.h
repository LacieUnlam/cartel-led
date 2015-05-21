//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <jpeg.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include "CommPort.h"
#include "VrControls.hpp"
#include "VrDigit.hpp"
#include "VrAudio.hpp"
//---------------------------------------------------------------------------

#define CT_NONE         0
#define CT_CLEAR        1
#define CT_UPDATE       2
#define CT_CONFIG       3
#define CT_CONT_P       4
#define CT_CONT_S       5
#define CT_PAUSE        6

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TImage *Image1;
        TMainMenu *MainMenu1;
        TMenuItem *Archivo1;
        TMenuItem *Salir1;
        TCommPort *CommPort1;
        TGroupBox *GroupBox2;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TButton *Button2;
        TComboBox *ComboBox1;
        TLabel *Label3;
        TSaveDialog *SaveDialog1;
        TMenuItem *GuardarDatos1;
        TStatusBar *StatusBar1;
        TGroupBox *GroupBox3;
        TMemo *Memo1;
        TLabel *Label1;
        TGroupBox *GroupBox4;
        TMenuItem *Conectar1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TImageList *ImageList1;
        TGroupBox *GroupBox5;
        TComboBox *ComboBox2;
        TComboBox *ComboBox3;
        TButton *Button3;
        TButton *Button4;
        TLabel *LabelAux;
        TPanel *Panel1;
        TVrDigitGroup *VrDigitGroup_Pri_Min;
        TVrDigitGroup *VrDigitGroup_Pri_Seg;
        TVrDigitGroup *VrDigitGroup_Sec;
        TMenuItem *ValoresdeInicio1;
        TMenuItem *Borrado1;
        TMenuItem *CargarRobots1;
        TShape *Shape1;
        TShape *Shape2;
        TVrDigitGroup *VrDigitGroup_Reg;
        TGroupBox *GroupBox1;
        TLabel *Label2;
        TButton *Button1;
        TEdit *Edit1;
        TLabel *Label4;
        TLabel *Label5;
        TTimer *TimerCONFIGURACION;
        TMenuItem *N1;
        TButton *Button5;
        TButton *Button6;
        TBevel *Bevel1;
        TBevel *Bevel2;
        TBevel *Bevel3;
    TTimer *TimerRXWait;
    TSplitter *Splitter1;
    TPanel *Panel2;
        TTimer *TimerParpadeo;
        TRadioButton *RadioButtonCOM3;
        TVrWave *VrWave_Corneta;
        TVrWave *VrWaveContReg;
        TVrWave *VrWavePunch;
        TButton *ButtonPunch;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall CommPort1DataReceived(TObject *Sender,
          const char *Buffer, unsigned Length);
        void __fastcall Edit1Click(TObject *Sender);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
        void __fastcall Salir1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall ComboBox1Change(TObject *Sender);
        void __fastcall GuardarDatos1Click(TObject *Sender);
        void __fastcall SpeedButton2Click(TObject *Sender);
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall SpeedButton3Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall ValoresdeInicio1Click(TObject *Sender);
        void __fastcall TimerCONFIGURACIONTimer(TObject *Sender);
        void __fastcall N1Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Edit1KeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
        void __fastcall Panel1CanResize(TObject *Sender, int &NewWidth,
          int &NewHeight, bool &Resize);
        void __fastcall TimerRXWaitTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
        void __fastcall TimerParpadeoTimer(TObject *Sender);
        void __fastcall RadioButtonCOM3Click(TObject *Sender);
        void __fastcall ButtonPunchClick(TObject *Sender);
private:	// User declarations
                AnsiString RxData;
                int CommandType;
                int RetryCount;
                bool f_Sound_5s;
                TStringList *Values;
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
