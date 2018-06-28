#ifndef _MSGQ_H
#define _MSGQ_H

#define MAXMESGDATA     1024
#define MESGHDRSIZE     (sizeof(Mesg) - MAXMESGDATA)

typedef struct {
  int mesg_len;
  long mesg_type;
  char mesg_data[MAXMESGDATA];
} Mesg;


#define MKEYSM 1234L
// #define MKEY2 2345L
// #define MKEYSM 1000L
#define PERMS 0666

void MessageSend(int, Mesg*);
int  MessageReceive(int, Mesg*);


#endif
