//---------------------------------------------------------------------------
#ifndef CommPortH
#define CommPortH
//---------------------------------------------------------------------------
#include <SysUtils.hpp>
#include <Controls.hpp>
#include <Classes.hpp>
#include <Forms.hpp>
#include "IOThread.h"
#include "DataDefinitions.h"
//---------------------------------------------------------------------------
class TCommStatus : public Classes::TPersistent
{
  friend class TCommPort;

  typedef Classes::TPersistent inherited;

private:

  bool FCTSOn;
  bool FDSROn;
  bool FRINGOn;
  bool FRLSDOn;

protected:

public:

__published:

  __property bool csCTSOn = { read = FCTSOn, nodefault };
  __property bool csDSROn = { read = FDSROn, nodefault };
  __property bool csRINGOn = { read = FRINGOn, nodefault };
  __property bool csRLSDOn = { read = FRLSDOn, nodefault };

};
//---------------------------------------------------------------------------
class TTimeOuts : public Classes::TPersistent
{
  typedef Classes::TPersistent inherited;

private:

  int FReadInterval;
  int FReadTotalMultiplier;
  int FReadTotalConstant;
  int FWriteTotalMultiplier;
  int FWriteTotalConstant;

protected:

public:

  __fastcall TTimeOuts();

__published:

  __property int toReadInterval =
    { read = FReadInterval, write = FReadInterval, default = DEFAULT_READINTERVAL };

  __property int toReadTotalMultiplier =
    { read = FReadTotalMultiplier, write = FReadTotalMultiplier, default = DEFAULT_READTOTALMULTIPLIER };

  __property int toReadTotalConstant =
    { read = FReadTotalConstant, write = FReadTotalConstant, default = DEFAULT_READTOTALCONSTANT };

  __property int toWriteTotalMultiplier =
    { read = FWriteTotalMultiplier, write = FWriteTotalMultiplier, default = DEFAULT_WRITETOTALMULTIPLIER };

  __property int toWriteTotalConstant =
    { read = FWriteTotalConstant, write = FWriteTotalConstant, default = DEFAULT_WRITETOTALCONSTANT };

};
//---------------------------------------------------------------------------
class PACKAGE TCommPort : public TComponent
{
private:

  HANDLE hComm;

  AnsiString FDummyStr;
  AnsiString FCopyrightNotice;
  AnsiString FVersion;
  TSerialPort FSerialPort;
  TBaudRate FBaudRate;
  TParity FParity;
  TStopBits FStopBits;
  int FByteSize;
  int FMaxSendQueueSize;
  bool FConnected;
  DWORD FStartTime;
  TTimeOuts * FTimeOuts;
  TCommStatus * FCommStatus;
  AnsiString FCustomPortName;
  char FEventChar;
  TFlowControl FFlowControl;
  TOnDataReceivedEvent FOnDataReceived;
  TNotifyEvent FOnSendBroken;
  TNotifyEvent FOnQueueEmpty;
  TNotifyEvent FOnBreakOnInput;
  TNotifyEvent FOnCTSChanged;
  TNotifyEvent FOnDSRChanged;
  TNotifyEvent FOnLineStatusError;
  TNotifyEvent FOnRingDetected;
  TNotifyEvent FOnRLSDChanged;
  TNotifyEvent FOnRxChar;
  TNotifyEvent FOnRxEventChar;
  TNotifyEvent FOnTxEmpty;
  TEventsContainer FEventsContainer;
  TIOThread * FIOThread;
  HANDLE FThreadExitEvent;
  COMMTIMEOUTS FOriginalTimeouts;

  void __fastcall SetMaxSendQueueSize(int queuesize);
  void __fastcall SetByteSize(int bytesize);
  TCommStatus * __fastcall GetCommStatus();
  int __fastcall GetSendQueueUsed();

  bool __fastcall UpdateConnection();

protected:

public:

  __property HANDLE Handle =
    { read = hComm, nodefault };

  __property bool Connected =
    { read = FConnected, default = false };

  __property TCommStatus * CommStatus =
    { read = GetCommStatus, nodefault };

  __property int SendQueueUsed =
    { read = GetSendQueueUsed, nodefault };

  __fastcall TCommPort(TComponent* Owner);
  __fastcall ~TCommPort();

  void __fastcall Open();
    // throws an Exception if Open fails

  void __fastcall Close();
    // throws an Exception if Close fails

  void __fastcall Write(const void * Buffer, int Count);
    // throws EWriteError if write failed - not enough memory available
    // or beyond MaxSendQueueSize, or Comm Port hasn't been opened

  void __fastcall FlushQueue();
    // flushes the send queue (useful for real time systems which
    // would rather have latest data sent than queued data)
    // throws EWriteError if FlushQueue failed (comm port hasn't been opened)

  DWORD __fastcall GetConnectedTime();

__published:

  __property AnsiString Copyright =
    { read = FCopyrightNotice, write = FDummyStr, nodefault, stored = false };

  __property AnsiString Version =
    { read = FVersion, write = FDummyStr, nodefault, stored = false };

  __property TSerialPort SerialPort =
    { read = FSerialPort, write = FSerialPort, default = DEFAULT_SERIALPORT };

  __property AnsiString CustomPortName =
    { read = FCustomPortName, write = FCustomPortName, nodefault };

  __property char EventChar =
    { read = FEventChar, write = FEventChar, default = DEFAULT_EVENTCHAR };

  __property TFlowControl FlowControl =
    { read = FFlowControl, write = FFlowControl, default = DEFAULT_FLOWCONTROL };

  __property TBaudRate BaudRate =
    { read = FBaudRate, write = FBaudRate, default = DEFAULT_BAUDRATE };

  __property TParity Parity =
    { read = FParity, write = FParity, default = DEFAULT_PARITY };

  __property TStopBits StopBits =
    { read = FStopBits, write = FStopBits, default = DEFAULT_STOPBITS };

  __property int ByteSize =
    { read = FByteSize, write = SetByteSize, default = DEFAULT_BYTESIZE };

  __property TTimeOuts * TimeOuts =
    { read = FTimeOuts, write = FTimeOuts, nodefault };

  __property int MaxSendQueueSize =
    { read = FMaxSendQueueSize, write = SetMaxSendQueueSize, default = DEFAULT_MAXQUEUESIZE };

  // --- EVENTS ---

  __property TOnDataReceivedEvent OnDataReceived =
    { read = FOnDataReceived, write = FOnDataReceived, nodefault };

  __property TNotifyEvent OnSendBroken =
    { read = FOnSendBroken, write = FOnSendBroken, nodefault };

  __property TNotifyEvent OnQueueEmpty =
    { read = FOnQueueEmpty, write = FOnQueueEmpty, nodefault };

  __property TNotifyEvent OnBreakOnInput =
    { read = FOnBreakOnInput, write = FOnBreakOnInput, nodefault };

  __property TNotifyEvent OnCTSChanged =
    { read = FOnCTSChanged, write = FOnCTSChanged, nodefault };

  __property TNotifyEvent OnDSRChanged =
    { read = FOnDSRChanged, write = FOnDSRChanged, nodefault };

  __property TNotifyEvent OnLineStatusError =
    { read = FOnLineStatusError, write = FOnLineStatusError, nodefault };

  __property TNotifyEvent OnRingDetected =
    { read = FOnRingDetected, write = FOnRingDetected, nodefault };

  __property TNotifyEvent OnRLSDChanged =
    { read = FOnRLSDChanged, write = FOnRLSDChanged, nodefault };

  __property TNotifyEvent OnRxChar =
    { read = FOnRxChar, write = FOnRxChar, nodefault };

  __property TNotifyEvent OnRxEventChar =
    { read = FOnRxEventChar, write = FOnRxEventChar, nodefault };

  __property TNotifyEvent OnTxEmpty =
    { read = FOnTxEmpty, write = FOnTxEmpty, nodefault };

};
//---------------------------------------------------------------------------
#endif
