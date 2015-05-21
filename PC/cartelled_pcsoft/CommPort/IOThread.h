//---------------------------------------------------------------------------
// TReaderThread
//   - reads data and status from com port
//---------------------------------------------------------------------------
#ifndef IOThreadH
#define IOThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "DataDefinitions.h"
//---------------------------------------------------------------------------
#define AMOUNT_TO_READ 512
//---------------------------------------------------------------------------
class TIOThread : public TThread
{
private:

  int MaxQueueSize;
  TMemoryStream * InStream;
  CRITICAL_SECTION CriticalSection;
  HANDLE hComm;
  HANDLE hDataArrivedEvent;
  TEventsContainer * Events;
  BYTE lpBuf[AMOUNT_TO_READ];
  DWORD dwRead;
  DWORD dwWritten;

  void __fastcall GetQueuedData(TMemoryStream * OutStream);
  void __fastcall HandleStatusChange(DWORD EventFlag);
  void __fastcall HandleDataReceivedEvent();
  void __fastcall HandleSendBrokenEvent();
  void __fastcall HandleQueueEmptyEvent();
  void __fastcall HandleBreakOnInputEvent();
  void __fastcall HandleCTSChangedEvent();
  void __fastcall HandleDSRChangedEvent();
  void __fastcall HandleLineStatusErrorEvent();
  void __fastcall HandleRingDetectedEvent();
  void __fastcall HandleRLSDChangedEvent();
  void __fastcall HandleRxCharEvent();
  void __fastcall HandleRxEventCharEvent();
  void __fastcall HandleTxEmptyEvent();

protected:

  void __fastcall Execute();

public:

  __fastcall TIOThread(HANDLE CommDev, int MaxSendQueueSize,
    TEventsContainer * EventsContainer);
    // constructor

  __fastcall ~TIOThread();
    // destructor

  bool __fastcall Write(const void * Buffer, int Count);
    // return false if write failed - not enough memory available
    // or beyond MaxSendQueueSize

  void __fastcall SetMaxSendQueueSize(int MaxSendQueueSize);
    // sets maximum send queue size

  int __fastcall GetSendQueueUsed();
    // returns the current size of send-queue in use

  void __fastcall FlushQueue();
    // flushes the queue
};
//---------------------------------------------------------------------------
#endif
