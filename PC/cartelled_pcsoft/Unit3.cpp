//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit3.h"
#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm3 *Form3;
//---------------------------------------------------------------------------
__fastcall TForm3::TForm3(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm3::Button1Click(TObject *Sender)
{
Form3->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm3::Button3Click(TObject *Sender)
{
 if(Form1->CommPort1->Connected)
  { AnsiString Aux="d";
    Aux = Aux + ((CSpinEdit1->Value<10)?"0":"") + IntToStr(CSpinEdit1->Value)
        + ","+ (CSpinEdit2->Value<10?"0":"")+ IntToStr(CSpinEdit2->Value)
        + "," +(CSpinEdit3->Value<100?"0":"")+ (CSpinEdit3->Value<10?"0":"") + IntToStr(CSpinEdit3->Value)
        + "," + IntToStr(CSpinEdit4->Value)+char(0x0D);

    Form1->CommPort1->Write(Aux.c_str(), Aux.Length());
    Form1->Memo1->Lines->Add(DateTimeToStr(Now())+": VALORES DE INICIO ENVIADOS.");
    Form1->Memo1->Lines->Add(DateTimeToStr(Now())+": Comando Tx: "+Aux);
    Aux="";

   }
 else
    ShowMessage("Actualmente no está conectado.");
}
//---------------------------------------------------------------------------
