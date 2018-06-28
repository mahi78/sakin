#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/msg.h>

#include "msgq.h"

char *program_name;
char version[] = "1.0";

extern int errno;
extern int optind;
extern int opterr;
extern char *optarg;

void usage(void);

Mesg mesg;
// Add an option to this program to read messages from
// a file and send them to the protocol port.
// I need to figure out how to deliniate the messages.


int main(int argc, char **argv) 
{
   int  fd,writeid;
   int  n;
   int  msgKey;
   char *cp;
   int  op;
   int verb = 0;
   int debug = 0;
   char filename[20];

   cout << "driver" << endl;

// PROCESS Optioons

   if ((cp = strrchr(argv[0], '/')) != NULL)
                program_name = cp + 1;
   else
                program_name = argv[0];

   while (
            (op = getopt(argc, argv, "vdVw:")) != EOF)
                switch (op) {

                case 'w':
                        msgKey = atoi(optarg);
                        break;

                case 'v':
                        // version
                        (void)fprintf(stdout, "%s version %s
", program_name, version);
                        return(0);

                case 'V':
                        // verbose
                        verb = 1;
                        break;


                case 'd':
                        // Debug
                        debug = 1;
                        break;


                default:
                        usage();
                        /* NOTREACHED */
                }

   // END of option preocessing

   printf("Message Key %d\n",msgKey);

   // Get the message Queue ID, must be created by Server 
   if ((writeid = msgget(msgKey,0)) < 0) {
      cout << "Error: cant get message queue" << endl;
      return 1;
   }
  
   // Get input filename 
   // cout << "Enter Filename: ";
   // cin >> filename;
  
   int cont = 1;

   // This is gross cuz I got to read in the line
   // and then parse it 
   // maybe a better idea would be a Perl driver
   // The format of the message should be
   // TYPE data<cr>

   while(cont) {
      char mesgType[3];
      cin >> mesgType;
      cout << mesgType << endl;
      mesg.mesg_type = atoi(mesgType);
      cout << mesg.mesg_type << endl;
      cin.getline(mesg.mesg_data, MAXMESGDATA, '\n');
      cout << mesg.mesg_data << endl;
      if (mesg.mesg_type == 0) 
         cont = 0;
      else { 
        n = strlen(mesg.mesg_data);
        if (mesg.mesg_data[n-1] == '\n') n--;
        mesg.mesg_len = n;
        // mesg.mesg_type = 1L;
        MessageSend(writeid, &mesg);
      }
   }

   return 0;
}

void
usage(void)
{
    (void)fprintf(stderr, "%s version %s
", program_name, version);
    (void)fprintf(stderr, "Usage: %s [-v(version)] [-V(ve1Grbose)] [-d(debug)]
", program_name);
    exit(-1);
}

