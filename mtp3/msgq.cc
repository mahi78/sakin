/*
 * msgq.cc
 * A problem arises when trying to receive messages over different
 * channles, currently its one way only.
 * So I will migrate this to use UNIX domain sockets
 * The idea here is to create a class that allows a socket
 * server to be settup, you can send on the socket by calling
 * the send method, but there will be a static method that you call
 * to receive a message from any of the established sockets
 */
 
#include <iostream>
#include <unistd.h>
#include <sys/msg.h>
#include "error.h"
#include "msgq.h"



void
MessageSend(int id, Mesg *mesgptr) {
   if (id == 0) {
      // direct output to stdout
      // It would be nice to be able to direct it to
      // a specific file but that would mean more complex
      // management of streams versus file descriptors, so for
      // now I'll rely on UNIX redirection 
      cout << mesgptr->mesg_data << endl;
   }
   else
   {
      if (msgsnd(id, (char *) &(mesgptr->mesg_type),mesgptr->mesg_len,0) != 0) 
         cout << "Message send error" << endl;
   }
   return;
}


int 
MessageReceive(int id, Mesg *mesgptr)
{
   int n;
   return (msgrcv(id,(char *) &(mesgptr->mesg_type),MAXMESGDATA,0,0));
}

