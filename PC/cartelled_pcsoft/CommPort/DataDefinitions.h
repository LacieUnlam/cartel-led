//---------------------------------------------------------------------------
#ifndef DataDefinitionsH
#define DataDefinitionsH
//---------------------------------------------------------------------------
#define DEFAULT_SERIALPORT              spCOM1
#define DEFAULT_BAUDRATE                br9600
#define DEFAULT_PARITY                  pNoParity
#define DEFAULT_BYTESIZE                8
#define DEFAULT_STOPBITS                sbOne
#define DEFAULT_READINTERVAL            1
#define DEFAULT_READTOTALMULTIPLIER     0
#define DEFAULT_READTOTALCONSTANT       0
#define DEFAULT_WRITETOTALMULTIPLIER    0
#define DEFAULT_WRITETOTALCONSTANT      0
#define DEFAULT_EVENTCHAR               '\n'
#define DEFAULT_FLOWCONTROL             fcNone
#define DEFAULT_MAXQUEUESIZE          0x100000
//---------------------------------------------------------------------------
#define ASCII_XON                       0x11
#define ASCII_XOFF                      0x13
#define MAX_STATUS_BUFFER               20000
#define MAX_WRITE_BUFFER                1024
#define MAX_READ_BUFFER                 2048
#define XON_LIMIT_PERCENTAGE            0.5
#define XOFF_LIMIT_PERCENTAGE           0.75
//---------------------------------------------------------------------------
#define THREADS_HUNG_TIMEOUT            20000
//---------------------------------------------------------------------------
const DWORD INITIAL_EVENTFLAGS =
  EV_BREAK | EV_CTS   | EV_DSR | EV_ERR | EV_RING |
  EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY;
const DWORD PURGE_FLAGS =
  PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR;
//---------------------------------------------------------------------------
typedef void __fastcall (__closure *TOnDataReceivedEvent)(TObject *Sender, const char * Buffer, unsigned Length);
//---------------------------------------------------------------------------
struct TEventsContainer
{
  TOnDataReceivedEvent *   DataReceivedEvent;
  TNotifyEvent *           SendBrokenEvent;
  TNotifyEvent *           QueueEmptyEvent;
  TNotifyEvent *           BreakOnInputEvent;
  TNotifyEvent *           CTSChangedEvent;
  TNotifyEvent *           DSRChangedEvent;
  TNotifyEvent *           LineStatusErrorEvent;
  TNotifyEvent *   RingDetectedEvent;
  TNotifyEvent *    RLSDChangedEvent;
  TNotifyEvent *         RxCharEvent;
  TNotifyEvent *    RxEventCharEvent;
  TNotifyEvent *        TxEmptyEvent;
};
//---------------------------------------------------------------------------
enum TFlowControl
{
  fcNone, fcSoftware, fcHardware
};
//---------------------------------------------------------------------------
enum TSerialPort
{
  spCOM1, spCOM2, spCOM3, spCOM4, spCustom
};
//---------------------------------------------------------------------------
enum TBaudRate
{
  br110,    br300,    br600,    br1200,   br2400,
  br4800,   br9600,   br14400,  br19200,  br38400,
  br56000,  br57600,  br115200, br128000, br256000
};
//---------------------------------------------------------------------------
enum TParity
{
  pNoParity, pEven, pOdd, pMark, pSpace
};
//---------------------------------------------------------------------------
enum TStopBits
{
  sbOne, sbOneHalf, sbTwo
};
//---------------------------------------------------------------------------
#endif
