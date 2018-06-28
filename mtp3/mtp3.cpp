#include "mtp3.h"

extern mtp3 mtp31;


void global_sig_io(int signo)
{
    mtp31.sig_io(signo);
}

void
mtp3::decodeHeader(u8 data[], mtp3_msg &msg)
{
   mtp3_header *hdr = (mtp3_header *)&data[0];   
   msg.setbsn(hdr->bsn);
   msg.setfsn(hdr->fsn);
   msg.setsio(hdr->sio);
   msg.setDPCmember(hdr->DPCmember);   
   msg.setDPCcluster(hdr->DPCcluster);   
   msg.setDPCnetwork(hdr->DPCnetwork);   
   msg.setOPCmember(hdr->OPCmember);   
   msg.setOPCcluster(hdr->OPCcluster);   
   msg.setOPCnetwork(hdr->OPCnetwork);   
   msg.setsls(hdr->sls);   
   return;
};

void
mtp3::encodeHeader(u8 data[], mtp3_msg &msg)
{
  mtp3_header *hdr = (mtp3_header *)&data[0];
  hdr->bsn = msg.getbsn();
  hdr->fsn = msg.getfsn();
  hdr->sio = msg.getsio();
  hdr->DPCmember =  msg.getDPCmember();   
  hdr->DPCcluster = msg.getDPCcluster();   
  hdr->DPCnetwork = msg.getDPCnetwork();   
  hdr->OPCmember =  msg.getOPCmember();  
  hdr->OPCcluster = msg.getOPCcluster();   
  hdr->OPCnetwork = msg.getOPCnetwork();   
  hdr->sls = msg.getsls();   
  return;
};

void
mtp3_msg::displayHeader()
{
   log(DEBUG,"BSN %d",_bsn);
   log(DEBUG,"FSB %d",_fsn);

   switch(_si)
   {
      case SI_SNM:
	    log(DEBUG,"SI_SNM");
	    break; 
      case SI_SNM_TEST_REGULAR:
	    log(DEBUG,"SI_SNM_TEST_REGULAR");
	    break;  
      case SI_SNM_TEST_SPECIAL:
	    log(DEBUG,"SI_SNM_TEST_SPECIAL");
	    break;  
      case SI_SCCP:
	    log(DEBUG,"SI_SCCP");
	    break;  
      case SI_TUP:
	    log(DEBUG,"SI_TUP");
	    break; 
      case SI_ISUP:
	    log(DEBUG,"SI_ISUP");
	    break;  
      case SI_DUP_CALL:
	    log(DEBUG,"SI_DUP_CALL");
	    break;  
      case SI_DUP_FACILITY:
	    log(DEBUG,"SI_DUP_FACILITY");
	    break;  
      case SI_MTP_TEST_UP:
	    log(DEBUG,"SI_MTP_TEST_UP");
	    break;  
      case SI_BISDN:
	    log(DEBUG,"SI_BISDN");
	    break;
      default:
	    log(INFO,"ERROR - Unknown SIO %d",_si);
            break;
   };

   switch(_ni)
   {
      case NI_INTL:
	    log(DEBUG,"NI_INTL");
	    break; 
      case NI_INTL_SPARE:
	    log(DEBUG,"NI_INTL_SPARE");
	    break;  
      case NI_NATL:
	    log(DEBUG,"NI_NATL");
	    break;  
      case NI_NATL_SPARE:
	    log(DEBUG,"NI_NATL_SPARE");
	    break;     
      default:
	    log(INFO,"ERROR - Unknown NI %d",_ni);
            break;
   };

   log(DEBUG,"Priority %d",_priority);
   log(DEBUG,"DPC member %d",_DPCmember);
   log(DEBUG,"DPC cluster %d",_DPCcluster);
   log(DEBUG,"DPC network %d",_DPCnetwork);
   log(DEBUG,"OPC member %d",_OPCmember);
   log(DEBUG,"OPC cluster %d",_OPCcluster);
   log(DEBUG,"OPC network %d",_OPCnetwork);
   log(DEBUG,"SLS %d",_sls);
  
   return;
};


    
void mtp3::sig_io(int signo)
{
   int length;
   int size;
   int position = 0;
   mtp3_msg msg;

   u8  data[MAXSCUAMSGSIZE];

   log(DEBUG,"io signal");
   length = Recvfrom(_sockfd, data, MAXSCUAMSGSIZE, 0, NULL, NULL);
   // Decode the MTP3 message

   printf("\n");
   for(int i=0;i< length; i++)  printf("%d ",data[i]);
   printf("\n");

   decodeHeader(data, msg);
   msg.displayHeader();

   return;
};

void mtp3::initialize(int localPort,struct in_addr localIP, int remotePort, struct in_addr remoteIP)  
{
   int on = 1;
   log(DEBUG,"Initialize the state machine matrix");
   _localPort = localPort;
   _localIP = localIP;
   _remotePort = remotePort;
   _remoteIP = remoteIP;

   // Initialize the trasnport channel
   bzero(&_servaddr, sizeof(_servaddr));
   _servaddr.sin_family = AF_INET;
   _servaddr.sin_port = htons(_localPort);
   _servaddr.sin_addr = _localIP;
   _sockfd = Socket(AF_INET, SOCK_DGRAM, 0);
   Bind(_sockfd, (struct sockaddr *)&_servaddr, sizeof(_servaddr));
   // I'm a server now, I need to listen for incoming data
   
   bzero(&_remoteaddr, sizeof(_remoteaddr));
   _remoteaddr.sin_family = AF_INET;
   _remoteaddr.sin_port = htons(_remotePort);
   _remoteaddr.sin_addr = _remoteIP;

   // This code binds in the sig_io handler which will be called
   // when there is data on the _sockfd
   signal(SIGIO, global_sig_io);
   Fcntl(_sockfd, F_SETOWN, getpid());
   Ioctl(_sockfd, FIOASYNC, &on);
   Ioctl(_sockfd, FIONBIO, &on);

   process_event[ST_ASPDOWN][EVT_ESTABLISH] = &mtp3::St0Evt0;
   process_event[ST_ASPUP][EVT_ESTABLISH] = &mtp3::St1Evt0;
   process_event[ST_ASPACTIVE][EVT_ESTABLISH] = &mtp3::St2Evt0;
   process_event[ST_ASPDOWN][EVT_SHUTDOWN] = &mtp3::St0Evt1;
   process_event[ST_ASPUP][EVT_SHUTDOWN] = &mtp3::St1Evt1;
   process_event[ST_ASPACTIVE][EVT_SHUTDOWN] = &mtp3::St2Evt1;
   process_event[ST_ASPDOWN][EVT_SENDMSG] = &mtp3::St0Evt2;
   process_event[ST_ASPUP][EVT_SENDMSG] = &mtp3::St1Evt2;
   process_event[ST_ASPACTIVE][EVT_SENDMSG] = &mtp3::St2Evt2;
   process_event[ST_ASPDOWN][EVT_RECEIVEMSG] = &mtp3::St0Evt3;
   process_event[ST_ASPUP][EVT_RECEIVEMSG] = &mtp3::St1Evt3;
   process_event[ST_ASPACTIVE][EVT_RECEIVEMSG] = &mtp3::St2Evt3;
};

void mtp3::iterate(void) {
   Mesg dummy;
   dummy.mesg_len = 0;
   dummy.mesg_data[0] = '\0';

   for (int s=0; s < MAX_STATES; s++) {
      for (int en=0; en < MAX_EVENTS; en++) {
         _state = s;
         cout << "State: " << _state << "  Event: " << en << endl;
         dummy.mesg_type=en;
         (this->*process_event[_state][en])(dummy);
      }
   }
   // Reset the state
   _state = ST_ASPDOWN;
};


void
mtp3::sendTUP()
{
  mtp3_msg msg;
  u8 data[100];
  
  mtp31.encodeHeader(data, msg);
  log(DEBUG,"mtp3:sendTUP");
  Sendto(_sockfd, &data, 10, 0,  (struct sockaddr *)&_remoteaddr, sizeof(_remoteaddr));
  return;
};


void
mtp3::messageTest()
{
    sendTUP();sleep(2);
};

// Event Handling procedures for each state
bool mtp3::St0Evt0(Mesg &mesg) {
  struct testablishMsg *establishMsg = (struct testablishMsg *)mesg.mesg_data;
  log(DEBUG,"Process state ASPDOWN Event ESTABLISH");
  log(DEBUG,mesg.mesg_data); 
 
  ExtSt0();
  St0St1();
  EntSt1(); 
  return true;
}

bool mtp3::St1Evt0(Mesg &mesg) {
  log(DEBUG,"Process State ASPUP Event ESTABLISH");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"ASP already ASPUP"); 
  return true;
}

bool mtp3::St2Evt0(Mesg &mesg) {
  log(DEBUG,"Process State ASPACTIVE Event ESTABLISH");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"ASP already ASPACTIVE"); 
  return true;
}

bool mtp3::St0Evt1(Mesg &mesg) {
  log(DEBUG,"Process State ASPDOWN Event SHUTDOWN");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"ASP already ASPDOWN"); 
  return true;
}

bool mtp3::St1Evt1(Mesg &mesg) {
  log(DEBUG,"Process State ASPUP Event SHUTDOWN");
  log(DEBUG,mesg.mesg_data); 
  
  ExtSt1();
  St1St0();
  EntSt0(); 
  return true;
}

bool mtp3::St2Evt1(Mesg &mesg) {
  log(DEBUG,"Process State ASPACTIVE Event SHUTDOWN");
  log(DEBUG,mesg.mesg_data); 
  
  ExtSt2();
  St2St0();
  EntSt0(); 
  return true;
}

bool mtp3::St0Evt2(Mesg &mesg) {
  log(DEBUG,"Process State ASPDOWN Event SENDMSG");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"SENDMSG request with ASPDOWN - discarded"); 
  return true;
}

bool mtp3::St1Evt2(Mesg &mesg) {
  struct sockaddr_in serv;
  log(DEBUG,"Process State ASPUP Event SENDMSG");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"SENDMSG request with ASPUP - sent"); 

   return true;
}

bool mtp3::St2Evt2(Mesg &mesg) {
  struct sockaddr_in serv;
  log(DEBUG,"Process State ASPACTIVE Event SENDMSG");
  log(DEBUG,mesg.mesg_data); 
  log(DEBUG,"SENDMSG request with ASPACTIVE - sent"); 

  return true;
}

bool mtp3::St0Evt3(Mesg &mesg) {
  log(DEBUG,"Process State ASPDOWN Event 3");
  log(DEBUG,mesg.mesg_data); 
  return true;
}

bool mtp3::St1Evt3(Mesg &mesg) {
  log(DEBUG,"Process State ASPUP Event 3");
  log(DEBUG,mesg.mesg_data); 
  return true;
}

bool mtp3::St2Evt3(Mesg &mesg) {
  log(DEBUG,"Process State ASPACTIVE Event 3");
  log(DEBUG,mesg.mesg_data); 
  return true;
}


// Generic state transition, enter, and exit procedures

void mtp3::EntSt0(void) {
  log(DEBUG,"Entering St0");
  _state = ST_ASPDOWN;
}
void mtp3::EntSt1(void) {
  log(DEBUG,"Entering St1");
  _state = ST_ASPUP;
}
void mtp3::EntSt2(void) {
  log(DEBUG,"Entering St2");
  _state = ST_ASPACTIVE;
}
void mtp3::ExtSt0(void) {
  log(DEBUG,"Exiting St0");
}
void mtp3::ExtSt1(void) {
  log(DEBUG,"Exiting St1");
}
void mtp3::ExtSt2(void) {
  log(DEBUG,"Exiting St2");
}

void mtp3::St0St1(void) {
  log(DEBUG,"Transition from St0 to St1");
}
void mtp3::St1St0(void) {
  log(DEBUG,"Transition from St1 to St0");
}
void mtp3::St0St2(void) {
  log(DEBUG,"Transition from St0 to St2");
}
void mtp3::St2St0(void) {
  log(DEBUG,"Transition from St2 to St0");
}
void mtp3::St1St2(void) {
  log(DEBUG,"Transition from St1 to St2");
}
void mtp3::St2St1(void) {
  log(DEBUG,"Transition from St2 to St1");
}

