//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CommPort"
#pragma link "CommPort"
#pragma link "VrControls"
#pragma link "VrDigit"
#pragma link "CommPort"
#pragma link "VrControls"
#pragma link "VrDigit"
#pragma link "VrAudio"
#pragma resource "*.dfm"
TForm1 *Form1;
bool bContPrimState=false, bContSecState=false;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner)
{
 CommandType=CT_NONE;
 Values = new TStringList;

 f_Sound_5s=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button1Click(TObject *Sender)
{

 if(CommPort1->Connected)
 { CommPort1->Write((Edit1->Text+"\r").c_str(), Edit1->GetTextLen()+1);
   Memo1->Lines->Add(Edit1->Text+"\r");
 }

 else
    ShowMessage("Actualmente no está conectado.");

 Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::CommPort1DataReceived(TObject *Sender,
      const char *Buffer, unsigned Length)
{
  AnsiString tmpStr = AnsiString(Buffer,Length);
  RxData+=tmpStr;

  if(!RxData.AnsiPos("\r"))
        {
         return;
        }

  Form1->Memo1->Lines->Add(DateTimeToStr(Now())+ ": Rx: "+RxData);
 
  switch(CommandType)
    {case CT_NONE:  Values->CommaText = RxData;
                    if( Values->Count >= 4 )
                        {VrDigitGroup_Pri_Min->Value = atoi(Values->Strings[0].c_str())*10; //mm
                         VrDigitGroup_Pri_Seg->Value = atoi(Values->Strings[1].c_str())*10; //ss
                         VrDigitGroup_Sec->Value = atoi(Values->Strings[2].c_str())*10; //cccc
                         VrDigitGroup_Reg->Value = atoi(Values->Strings[3].c_str())*10; //s

                         if(atoi(Values->Strings[3].c_str())!=0)
                                f_Sound_5s=true;
                         if((atoi(Values->Strings[3].c_str())==0) && f_Sound_5s==true)
                                {f_Sound_5s=false;
                                 VrWave_Corneta->Play();
                                }
                        }

                    Values->Clear();
                    break;
     case CT_CLEAR: if(RxData.AnsiPos("ok\r"))
                        {//envia el comando update
                         CommPort1->Write(((AnsiString)"u\r").c_str(),((AnsiString)"u\r").Length());
                         CommandType=CT_UPDATE;
                         RetryCount=3;
                         TimerRXWait->Enabled=false;
                         TimerRXWait->Enabled=true;
                        }
                    else
                        {if(RetryCount--!=0)
                            {//reenvia el comando borrar
                             CommPort1->Write(((AnsiString)"b\r").c_str(),((AnsiString)"b\r").Length());
                             TimerRXWait->Enabled=false;
                             TimerRXWait->Enabled=true;
                            }
                         else
                            {//vencieron los intentos de envio.  Se anula la acción.
                             CommandType=CT_NONE;
                             TimerRXWait->Enabled=false;
                             Memo1->Lines->Add(DateTimeToStr(Now())+ ": (ERR) No se recibió la confirmación de borrar");
                            }
                        }
                    break;
     case CT_UPDATE: if(RxData.AnsiPos("ok\r"))
                        {//borrado de pantalla confirmado
                         CommandType=CT_NONE;
                         TimerRXWait->Enabled=false;
                         Memo1->Lines->Add(DateTimeToStr(Now())+ ": pantalla borrada");
                        }
                     else
                        {if(RetryCount--!=0)
                            {//reenvia el comando update
                             CommPort1->Write(((AnsiString)"u\r").c_str(),((AnsiString)"u\r").Length());
                             TimerRXWait->Enabled=false;
                             TimerRXWait->Enabled=true;
                            }
                         else
                            {//vencieron los intentos de envio.  Se anula la acción.
                             CommandType=CT_NONE;
                             TimerRXWait->Enabled=false;
                             Memo1->Lines->Add(DateTimeToStr(Now())+ ": (ERR) No se recibió la confirmación de actualizar");
                            }
                        }
                    break;
     case CT_CONT_P: if(RxData.AnsiPos("ok\r") || RxData.AnsiPos("k\r"))
                        {//comando confirmado
                         CommandType=CT_NONE;
                         if(bContPrimState)
                            {bContPrimState=false;
                             Graphics::TBitmap* Bitmap = SpeedButton1->Glyph;
                             ImageList1->GetBitmap(1, Bitmap);
                             SpeedButton1->Glyph = Bitmap;
                             //grafica en boton de contador secundario
                             Bitmap = SpeedButton2->Glyph;
                             ImageList1->GetBitmap(1, Bitmap);
                             SpeedButton2->Glyph = Bitmap;
                             bContSecState=false;

                             VrWaveContReg->Stop();
                             VrWave_Corneta->Stop();

                             Memo1->Lines->Add(DateTimeToStr(Now())+": Botón STOP Principal pulsado");
                            }
                         else
                            {bContPrimState=true;
                             Graphics::TBitmap* Bitmap = SpeedButton1->Glyph;
                             ImageList1->GetBitmap(2, Bitmap);
                             SpeedButton1->Glyph = Bitmap;
                             VrWaveContReg->Play();
                             Memo1->Lines->Add(DateTimeToStr(Now())+": Botón PLAY Principal pulsado");
                            }
                            
                          TimerRXWait->Enabled=false;
                         }
                     else
                        {if(RetryCount--!=0)
                            {//reenvia el comando cont principal
                             CommPort1->Write(((AnsiString)"c\r").c_str(),((AnsiString)"c\r").Length());
                             TimerRXWait->Enabled=false;
                             TimerRXWait->Enabled=true;
                            }
                         else
                            {//vencieron los intentos de envio.  Se anula la acción.
                             CommandType=CT_NONE;
                             TimerRXWait->Enabled=false;
                             Memo1->Lines->Add(DateTimeToStr(Now())+ ": (ERR) No se recibió la confirmación cont. prinsipal");
                            }
                        }
                     break;
     case CT_CONT_S: if(RxData.AnsiPos("ok\r"))
                        {//comando confirmado
                         CommandType=CT_NONE;
                         if(bContSecState)
                            {bContSecState=false;
                             Graphics::TBitmap* Bitmap = SpeedButton2->Glyph;
                             ImageList1->GetBitmap(1, Bitmap);
                             SpeedButton2->Glyph = Bitmap;
                             Memo1->Lines->Add(DateTimeToStr(Now())+": Botón STOP Secundario pulsado");
                            }
                         else
                            {bContSecState=true;
                             Graphics::TBitmap* Bitmap = SpeedButton2->Glyph;
                             ImageList1->GetBitmap(2, Bitmap);
                             SpeedButton2->Glyph = Bitmap;
                             Memo1->Lines->Add(DateTimeToStr(Now())+": Botón PLAY Secundario pulsado");
                            }

                          TimerRXWait->Enabled=false;
                         }
                     else
                        {if(RetryCount--!=0)
                            {//reenvia el comando cont secundario
                             CommPort1->Write(((AnsiString)"s\r").c_str(),((AnsiString)"s\r").Length());
                             TimerRXWait->Enabled=false;
                             TimerRXWait->Enabled=true;
                            }
                         else
                            {//vencieron los intentos de envio.  Se anula la acción.
                             CommandType=CT_NONE;
                             TimerRXWait->Enabled=false;
                             Memo1->Lines->Add(DateTimeToStr(Now())+ ": (ERR) No se recibió la confirmación cont. secundario");
                            }
                        }
                     break;
     case CT_PAUSE:  if(RxData.AnsiPos("ok\r"))
                        {//comando de pausa confirmado
                         CommandType=CT_NONE;
                         TimerRXWait->Enabled=false;
                         Memo1->Lines->Add(DateTimeToStr(Now())+": Botón PAUSA pulsado");
                        }
                     else
                        {if(RetryCount--!=0)
                            {//reenvia el comando pausa
                             CommPort1->Write(((AnsiString)"e\r").c_str(),((AnsiString)"e\r").Length());
                             TimerRXWait->Enabled=false;
                             TimerRXWait->Enabled=true;
                            }
                         else
                            {//vencieron los intentos de envio.  Se anula la acción.
                             CommandType=CT_NONE;
                             TimerRXWait->Enabled=false;
                             Memo1->Lines->Add(DateTimeToStr(Now())+ ": (ERR) No se recibió la confirmación de pausa");
                            }
                        }
                    break;
     default:   break;
    }

  RxData="";
  Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1Click(TObject *Sender)
{
Edit1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RadioButton1Click(TObject *Sender)
{
CommPort1->SerialPort=spCOM1;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::RadioButton2Click(TObject *Sender)
{
CommPort1->SerialPort=spCOM2;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Salir1Click(TObject *Sender)
{
Application->Terminate();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{

if(CommPort1->Connected)
  { CommPort1->Close();
    RadioButton1->Enabled=true;
    RadioButton2->Enabled=true;
    RadioButtonCOM3->Enabled=true;
    ComboBox1->Enabled=true;
    Salir1->Enabled=true;   // permito cerrar el programa si el puerto esta cerrado.
    Label3->Enabled=true;
    Label2->Enabled=false;
    Edit1->Enabled=false;
    Button1->Enabled=false;
    Memo1->Lines->Add(DateTimeToStr(Now())+": Desconexión del puerto.");

  }

else
  { CommPort1->Open();
    RadioButton1->Enabled=false;
    RadioButton2->Enabled=false;
    RadioButtonCOM3->Enabled=false;
    ComboBox1->Enabled=false;
    Salir1->Enabled=false;   // permito cerrar el programa hasta que se cierre el puerto.
    Label3->Enabled=false;
    Label2->Enabled=true;
    Edit1->Enabled=true;
    Button1->Enabled=true;
    Memo1->Lines->Add(DateTimeToStr(Now())+": Puerto abierto.");
  }

Button2->Caption=(CommPort1->Connected?"Desconectar":"Conectar");
Conectar1->Caption=(CommPort1->Connected?"&Desconectar":"&Conectar");
Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");    
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ComboBox1Change(TObject *Sender)
{
int Velocidad = atoi(ComboBox1->Text.c_str());
switch (Velocidad)
 { case 110: CommPort1->BaudRate=br110; break;
   case 300: CommPort1->BaudRate=br300; break;
   case 600: CommPort1->BaudRate=br600; break;
   case 1200: CommPort1->BaudRate=br1200; break;
   case 2400: CommPort1->BaudRate=br2400; break;
   case 4800: CommPort1->BaudRate=br4800; break;
   case 9600: CommPort1->BaudRate=br9600; break;
   case 14400: CommPort1->BaudRate=br14400; break;
   case 19200: CommPort1->BaudRate=br19200; break;
   case 38400: CommPort1->BaudRate=br38400; break;
   case 56000: CommPort1->BaudRate=br56000; break;
   case 57600: CommPort1->BaudRate=br57600; break;
   case 115200: CommPort1->BaudRate=br115200; break;
   case 128000: CommPort1->BaudRate=br128000; break;
   case 256000: CommPort1->BaudRate=br256000; break;
  }

}
//---------------------------------------------------------------------------
void __fastcall TForm1::GuardarDatos1Click(TObject *Sender)
{
 if (SaveDialog1->Execute())
   Memo1->Lines->SaveToFile(SaveDialog1->FileName);        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton2Click(TObject *Sender)
{
 if(!bContPrimState)
    {Memo1->Lines->Add(DateTimeToStr(Now())+": No está corriendo el contador primario.");
     return;
    }
 if(Form1->CommPort1->Connected)
    {CommPort1->Write(((AnsiString)"s\r").c_str(),((AnsiString)"s\r").Length());
     CommandType=CT_CONT_S;
     RetryCount=3;
     TimerRXWait->Enabled=true;
    }
 else
    ShowMessage("Actualmente no está conectado.");

 Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton1Click(TObject *Sender)
{
 if(Form1->CommPort1->Connected)
    {CommPort1->Write(((AnsiString)"c\r").c_str(),((AnsiString)"c\r").Length());
     CommandType=CT_CONT_P;
     RetryCount=3;
     TimerRXWait->Enabled=true;
    }
 else
    ShowMessage("Actualmente no está conectado.");

  Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::SpeedButton3Click(TObject *Sender)
{
//BOTON PAUSA
 if(CommPort1->Connected)
    {CommPort1->Write(((AnsiString)"e\r").c_str(),((AnsiString)"e\r").Length());
     CommandType=CT_PAUSE;
     RetryCount=3;
     TimerRXWait->Enabled=true;
    }
 else
    ShowMessage("Actualmente no está conectado.");

  Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
if (FileExists("ROBOT.TXT"))
   Form2->ListBox1->Items->LoadFromFile("ROBOT.TXT");
Form2->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
 AnsiString Aux;
 if(CommPort1->Connected)
 {  Aux = "o"+ComboBox2->Text+","+ ComboBox3->Text + char(0x0D);
    CommPort1->Write(Aux.c_str(), Aux.Length());
    Memo1->Lines->Add(DateTimeToStr(Now())+": NOMBRES DE CONTRINCANTES ENVIADOS.");    
    Memo1->Lines->Add(DateTimeToStr(Now())+": Comando Tx: "+Aux);
    Aux="";
 }

 else
    ShowMessage("Actualmente no está conectado.");

 Form1->Memo1->Lines->SaveToFile("REGISTRO.TXT");    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ValoresdeInicio1Click(TObject *Sender)
{
Form3->ShowModal();
}
//---------------------------------------------------------------------------



void __fastcall TForm1::TimerCONFIGURACIONTimer(TObject *Sender)
{
TimerCONFIGURACION->Enabled=false;

if (FileExists("ROBOT.TXT"))
   Form2->ListBox1->Items->LoadFromFile("ROBOT.TXT");

if (FileExists("REGISTRO.TXT"))
   Form1->Memo1->Lines->LoadFromFile("REGISTRO.TXT");

Form2->Button1Click(NULL);

}
//---------------------------------------------------------------------------

void __fastcall TForm1::N1Click(TObject *Sender)
{
Form5->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button5Click(TObject *Sender)
{
Form3->ShowModal();        
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button6Click(TObject *Sender)
{
 if(CommPort1->Connected)
    {CommPort1->Write(((AnsiString)"b\r").c_str(),((AnsiString)"b\r").Length());
     CommandType=CT_CLEAR;
     RetryCount=3;
     TimerRXWait->Enabled=true;
    }
 else
    ShowMessage("Actualmente no está conectado.");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
 delete Values;    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Edit1KeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
 if(Edit1->Focused() && Key==VK_RETURN)
    {Button1Click(this);
     Edit1->Clear();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Panel1CanResize(TObject *Sender, int &NewWidth,
      int &NewHeight, bool &Resize)
{//anchos
 VrDigitGroup_Reg->Width = Panel1->Width/10;
 VrDigitGroup_Pri_Min->Width = 2*VrDigitGroup_Reg->Width;
 VrDigitGroup_Pri_Seg->Width = 2*VrDigitGroup_Reg->Width;
 VrDigitGroup_Sec->Width = 3*VrDigitGroup_Reg->Width;
 Shape1->Width=VrDigitGroup_Reg->Width/4.6;
 Shape2->Width=VrDigitGroup_Reg->Width/4.6;
 //altos
 VrDigitGroup_Reg->Height = VrDigitGroup_Reg->Width * 1.6;
 VrDigitGroup_Pri_Min->Height = VrDigitGroup_Reg->Height;
 VrDigitGroup_Pri_Seg->Height = VrDigitGroup_Reg->Height;
 VrDigitGroup_Sec->Height = VrDigitGroup_Reg->Height;
 Shape1->Height=VrDigitGroup_Reg->Width/4.6;
 Shape2->Height=VrDigitGroup_Reg->Width/4.6;
 //posicion vertical
 VrDigitGroup_Sec->Top = (Panel1->Height/2)-(VrDigitGroup_Sec->Height/2);
 VrDigitGroup_Pri_Min->Top = (Panel1->Height/2)-(VrDigitGroup_Pri_Min->Height/2);
 VrDigitGroup_Pri_Seg->Top = (Panel1->Height/2)-(VrDigitGroup_Pri_Seg->Height/2);
 VrDigitGroup_Reg->Top = (Panel1->Height/2)-(VrDigitGroup_Reg->Height/2);
 Shape1->Top = (Panel1->Height/2)-(Shape1->Height/2)-(VrDigitGroup_Pri_Min->Height/4);
 Shape2->Top = (Panel1->Height/2)-(Shape2->Height/2)+(VrDigitGroup_Pri_Min->Height/4);
 //posicion horizontal
 VrDigitGroup_Pri_Min->Left = ((1.5/10.0)*Panel1->Width)-(VrDigitGroup_Pri_Min->Width/2);
 VrDigitGroup_Pri_Seg->Left = ((3.6/10.0)*Panel1->Width)-(VrDigitGroup_Pri_Seg->Width/2);
 VrDigitGroup_Sec->Left = ((6.5/10.0)*Panel1->Width)-(VrDigitGroup_Sec->Width/2);
 VrDigitGroup_Reg->Left = ((8.8/10.0)*Panel1->Width)-(VrDigitGroup_Reg->Width/2);
 Shape1->Left = ((2.47/10.0)*Panel1->Width)-(Shape1->Width/2);
 Shape2->Left = ((2.47/10.0)*Panel1->Width)-(Shape2->Width/2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimerRXWaitTimer(TObject *Sender)
{
 TimerRXWait->Enabled=false;
 CommandType=CT_NONE;
 Memo1->Lines->Add(DateTimeToStr(Now())+": No se recibe respuesta del cartel.");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormShow(TObject *Sender)
{
 Panel1->Perform(WM_SIZE,0,0);    
}
//---------------------------------------------------------------------------

void __fastcall TForm1::TimerParpadeoTimer(TObject *Sender)
{//parpadeo contador secundario
 if(VrDigitGroup_Sec->Value==0 && bContSecState==true)
        if(VrDigitGroup_Sec->Palette->High==clYellow)
                VrDigitGroup_Sec->Palette->High=clOlive;
        else
                VrDigitGroup_Sec->Palette->High=clYellow;
 else
        VrDigitGroup_Sec->Palette->High=clYellow;

 //parpadeo contador primario
 if(VrDigitGroup_Pri_Min->Value==0 && VrDigitGroup_Pri_Seg->Value==0 && VrDigitGroup_Reg->Value==0 && bContPrimState==true)
        if(VrDigitGroup_Pri_Min->Palette->High==clFuchsia)
                {VrDigitGroup_Pri_Min->Palette->High=clOlive;
                 VrDigitGroup_Pri_Seg->Palette->High=clOlive;
                 Shape1->Brush->Color=clOlive;
                 Shape2->Brush->Color=clOlive;
                }
        else
                {VrDigitGroup_Pri_Min->Palette->High=clFuchsia;
                 VrDigitGroup_Pri_Seg->Palette->High=clFuchsia;
                 Shape1->Brush->Color=clFuchsia;
                 Shape2->Brush->Color=clFuchsia;
                }
 else
        {VrDigitGroup_Pri_Min->Palette->High=clFuchsia;
         VrDigitGroup_Pri_Seg->Palette->High=clFuchsia;
         Shape1->Brush->Color=clFuchsia;
         Shape2->Brush->Color=clFuchsia;
        }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::RadioButtonCOM3Click(TObject *Sender)
{
 CommPort1->SerialPort=spCOM3;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::ButtonPunchClick(TObject *Sender)
{
  VrWavePunch->Play();
}
//---------------------------------------------------------------------------

