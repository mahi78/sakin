#ifndef _MTP3_H
#define _MTP3_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include   <unistd.h>
#include   <fcntl.h>
#include   <sys/types.h>
#include   <sys/socket.h>
#include   <sys/stat.h>
#include   <sys/errno.h>
#include   <iostream>
#include   <string>
#include   <netinet/in.h>
#include   <arpa/inet.h>
#include   <signal.h>
#include   <sys/ioctl.h>
#include   <stdio.h>


#include "basic.h"
#include "error.h"
#include "msgq.h"
#include "wrapper.h"



using namespace std;

class smEvent {
public:
  smEvent (const int e): _eventNum(e) {}
  ~smEvent () {}
  int eventNum() { return _eventNum; }

private:
  int _eventNum;
  int debug;
};

// These definitions need to be made system specific
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int  u32;  

#define MAXSCUAMSGSIZE 128

// Events 
#define MAX_EVENTS     4
#define EVT_ESTABLISH  0
#define EVT_SHUTDOWN   1
#define EVT_SENDMSG    2
#define EVT_RECEIVEMSG 3

// States
#define MAX_STATES 3
#define ST_ASPDOWN   0
#define ST_ASPUP     1
#define ST_ASPACTIVE 2


#define SIO_SIZE 1
#define SI_MASK 0x0F
#define SI_POSITION 0
#define NI_MASK  0xC0
#define NI_POSITION 6
#define PRIORITY_MASK 0x30
#define PRIORITY_POSITION 4

#define SI_SNM              0
#define SI_SNM_TEST_REGULAR 1
#define SI_SNM_TEST_SPECIAL 2
#define SI_SCCP             3
#define SI_TUP              4
#define SI_ISUP             5
#define SI_DUP_CALL         6
#define SI_DUP_FACILITY     7
#define SI_MTP_TEST_UP      8
#define SI_BISDN            9

#define NI_INTL  0
#define NI_INTL_SPARE 1
#define NI_NATL  2
#define NI_NATL_SPARE 3

#define PRIORITY_0 0
#define PRIORITY_1 1
#define PRIORITY_2 2
#define PRIORITY_3 3


struct mtp3_header
{
  u8 bsn;
  u8 fsn;
  u8 sio;
  u8 DPCmember;
  u8 DPCcluster;
  u8 DPCnetwork;
  u8 OPCmember;
  u8 OPCcluster;
  u8 OPCnetwork;
  u8 sls;
};


class mtp3_msg 
{
 public:
  mtp3_msg(): _bsn(1), 
          _fsn(1),
          _si(SI_TUP),
          _ni(NI_NATL),
          _priority(PRIORITY_1),
          _DPCmember(1),
          _DPCcluster(1),
          _DPCnetwork(1),
          _OPCmember(2),
          _OPCcluster(2),
          _OPCnetwork(2),
    _sls(100){};
  ~mtp3_msg(){};

  void setbsn(u8 bsn){ _bsn = bsn; };
  void setfsn(u8 fsn){ _fsn = fsn; };
  void setsio(u8 sio){ _si = sio & SI_MASK >> SI_POSITION;
           _ni = sio & NI_MASK >> NI_POSITION;
           _priority & PRIORITY_MASK >> PRIORITY_POSITION; };
  void setDPCmember(u8 DPCmember){ _DPCmember = DPCmember; };
  void setDPCcluster(u8 DPCcluster){ _DPCcluster = DPCcluster; };
  void setDPCnetwork(u8 DPCnetwork){ _DPCnetwork = DPCnetwork; };
  void setOPCmember(u8 OPCmember){ _OPCmember = OPCmember; };
  void setOPCcluster(u8 OPCcluster){ _OPCcluster = OPCcluster; };
  void setOPCnetwork(u8 OPCnetwork){ _OPCnetwork = OPCnetwork; };
  void setsls(u8 sls) { _sls = sls; };

  u8 getbsn(){return _bsn; };
  u8 getfsn(){return _fsn; };
  u8 getsio(){return ((_si << SI_POSITION) |
                     (_ni << NI_POSITION) |
		     (_priority << PRIORITY_POSITION));};
  u8 getsi(){return _si; };
  u8 getni() {return _ni; };
  u8 getpriority() {return _priority; };
  u8 getDPCmember(){ return _DPCmember; };
  u8 getDPCcluster(){ return _DPCcluster; };
  u8 getDPCnetwork(){ return _DPCnetwork; };
  u8 getOPCmember(){ return _OPCmember; };
  u8 getOPCcluster(){ return _OPCcluster; };
  u8 getOPCnetwork(){ return _OPCnetwork; };
  u8 getsls() { return _sls; };

  void displayHeader();

 private:
  u8 _bsn;
  u8 _fsn;
  u8 _si;
  u8 _ni;
  u8 _priority;
  u8 _DPCmember;
  u8 _DPCcluster;
  u8 _DPCnetwork;
  u8 _OPCmember;
  u8 _OPCcluster;
  u8 _OPCnetwork;
  u8 _sls;

/*

ANSI MTP3 Message Header

Transmission order is top to bottom left to right
least significant bit first

Flag-8
BSN-7, BIB-1
FSN-7, FIB-1
LI-6, spare-2
SIO-8 (si-4, priority-2, ni-2)
DPC member-8
DPC cluster-8
DPC network-8
OPC member-8
OPC cluster-8
OPC network-8
SLS-8


*/


};


class mtp3 {
public:
  mtp3(): debug(true), _state(ST_ASPDOWN),
     _connectIndication(&nullCallback),
     _connectConfirm(&nullCallback),
     _dataIndication(&nullCallback),
     _expeditedDataIndication(&nullCallback),
     _resetIndication(&nullCallback),
     _resetConfirm(&nullCallback),
     _disconnectIndication(&nullCallback),
     _informIndication(nullCallback),
     _unitdataIndication(nullCallback),
     _noticeIndication(nullCallback) {}

  ~mtp3() {}

  // MTP3 User Interface
  int connectRequest();
  int bindConnectIndication(int (*p)()) { _connectIndication = p; return 0; };
  int connectResponse();
  int bindConnectConfirm(int (*p)()) { _connectConfirm = p;return 0; };
  int dataRequest();
  int bindDataIndication(int (*p)()) {_dataIndication = p;return 0; };
  int expeditedDataRequest();
  int bindExpeditedDataIndication(int (*p)()) {_expeditedDataIndication = p;return 0;};
  int resetRequest();
  int bindResetIndication(int (*p)()) {_resetIndication = p;return 0;};
  int resetResponse();
  int bindResetConfirm(int (*p)()) {_resetConfirm = p;return 0;};
  int disconnectRequest();
  int bindDisconnectIndication(int (*p)()) {_disconnectIndication = p;return 0;};
  int informRequest();
  int bindInformIndication(int (*p)()) {_informIndication = p;return 0;};
  int unitDataRequest();
  int bindUnitdataIndication(int (*p)()) {_unitdataIndication = p;return 0;};
  int bindNoticeIndication(int (*p)()) {_noticeIndication = p;return 0;};
  
  // MGMT Interface
  void initialize(int, struct in_addr, int, struct in_addr);

  // Debug procedure to iterate through all the states and events
  // Could be used to output a state diagram
  void iterate(void);
  
  // this method will build and send a bunch of m3ua messages
  // of all sorts of shapes and sizes including messages with
  // errors.
  void messageTest(void);

  //Other useful public funtions would be to query state variables
  //A debug interface to set state variables

  void sig_io(int);
  void encodeHeader(u8*, mtp3_msg&);
  void decodeHeader(u8*, mtp3_msg&);

private:


struct tASP
{
	int ASP_state;
};

struct tAS
{
	//should contain a linked list of ASPs
	struct tASP asp[2];
	int AS_state;
};

// AS States
#define  ASDOWN    1
#define  ASUP      2
#define  ASACTIVE  3
#define  ASPENDING 4

  const bool debug;
  typedef bool (mtp3::*pf) (Mesg&);
  pf process_event[MAX_STATES][MAX_EVENTS];
  int _state;
  int _sockfd;
  int _localPort, _remotePort;
  struct in_addr _localIP, _remoteIP;
  struct sockaddr_in _servaddr, _remoteaddr;

  static int  nullCallback() { log(DEBUG,"no callback bound");return 0;};

  int  (*_connectIndication)(); 
  int  (*_connectConfirm)();
  int  (*_dataIndication)();
  int  (*_expeditedDataIndication)();
  int  (*_resetIndication)();
  int  (*_resetConfirm)();
  int  (*_disconnectIndication)();
  int  (*_informIndication)();
  int  (*_unitdataIndication)();
  int  (*_noticeIndication)();

 
  // State Event handlers
  bool St0Evt0(Mesg&);
  bool St1Evt0(Mesg&);
  bool St2Evt0(Mesg&);
  bool St0Evt1(Mesg&);
  bool St1Evt1(Mesg&);
  bool St2Evt1(Mesg&);
  bool St0Evt2(Mesg&);
  bool St1Evt2(Mesg&);
  bool St2Evt2(Mesg&);
  bool St0Evt3(Mesg&);
  bool St1Evt3(Mesg&);
  bool St2Evt3(Mesg&);

  // Generic state transition, enter, and exit procedures
  void EntSt0(void);
  void ExtSt0(void);
  void EntSt1(void);
  void ExtSt1(void);
  void EntSt2(void);
  void ExtSt2(void);
  
  void St0St1(void);
  void St1St0(void);
  void St0St2(void);
  void St2St0(void);
  void St1St2(void);
  void St2St1(void);

  void sendTUP();


};

#endif



