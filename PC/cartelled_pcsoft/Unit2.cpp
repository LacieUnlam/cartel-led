//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "VrControls"
#pragma link "VrLcd"
#pragma resource "*.dfm"
TForm2 *Form2;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm2::Button1Click(TObject *Sender)
{
int i=0;
Form1->ComboBox3->Clear();
Form1->ComboBox2->Clear();
while (i<ListBox1->Count)
 {  Form1->ComboBox3->Items->Add(ListBox1->Items->Strings[i]);
    Form1->ComboBox2->Items->Add(ListBox1->Items->Strings[i]);
    i++;
 }
Form2->ListBox1->Items->SaveToFile("ROBOT.TXT");
Form2->Close();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button2Click(TObject *Sender)
{
ListBox1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Button3Click(TObject *Sender)
{
 ListBox1->Items->Add(Edit1->Text);
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Edit1Click(TObject *Sender)
{
Edit1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm2::Edit1Change(TObject *Sender)
{
Button3->Enabled=true;
}
//---------------------------------------------------------------------------
