#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/errno.h>

#include "error.h"
#include "mtp3.h"
#include "msgq.h"

char *program_name;
char version[] = "0.1";


extern int optind;
extern int opterr;
extern char *optarg;

void usage(void); 
mtp3 mtp31;


int mtp3ConnectionIndication()
{
   log(DEBUG,"MTP3 Connection Indication");
   return 0;
};

int mtp3ConnectionConfirm()
{
   log(DEBUG,"MTP3 Connection Confirm");
   return 0;
};

int mtp3DataIndication()
{
   log(DEBUG,"MTP3 Data Indication");
   return 0;
};

int mtp3ExpeditedDataIndication()
{
   log(DEBUG,"MTP3 Expedited Data Indication");
   return 0;
};

int mtp3ResetIndication()
{
   log(DEBUG,"MTP3 Reset Indication");
   return 0;
};

int mtp3ResetConfirm()
{
   log(DEBUG,"MTP3 Reset Confirm");
   return 0;
};

int mtp3DisconnectIndication()
{
   log(DEBUG,"MTP3 disconnect Indication");
   return 0;
};

int mtp3InformIndication()
{
   log(DEBUG,"MTP3 Inform Indication");
   return 0;
};

int mtp3UnitdataIndication()
{
   log(DEBUG,"MTP3 Unitdata Indication");
   return 0;
};

int mtp3NoticeIndication()
{
   log(DEBUG,"MTP3 Notice Indication");
   return 0;
};

Mesg mesg;

int main(int argc, char *argv[]) {

/* scua Wrapper.
   ------------
   Fire up, open an INPUT message Queue to  which other people can write to
   and we read from. This is where all input to protocol layer will enter.
   Handle all command line options which basically determine where the output
   will go (upper layer, Lower layer, Layer Mgmt)
   Command line options also for version, debug, timer handling, verbose, etc
   Then  declare an scua object which invokes the constructor.
   Initialize scua,
   Then have it process events.
*/
    
   char *cp;
   int op;
   int verb = 0;
   int debug = 0;
   int localPort, remotePort;
   struct in_addr localAddr, remoteAddr;
   int testMsgKey;
   int testfd = 0;
   int n;

   // PROCESS Optioons

   if ((cp = strrchr(argv[0], '/')) != NULL)
                program_name = cp + 1;
   else
                program_name = argv[0];

   while ( (op = getopt(argc, argv, "vdVA:P:a:p:t:")) != EOF)
   {
                switch (op) {

				case 'P':
                        localPort = atoi(optarg);
                        break;

                case 'A':
                        inet_pton(AF_INET,optarg, &localAddr);
                        break;
                
				case 'p':
                        remotePort = atoi(optarg);
                        break;

                case 'a':
                        inet_pton(AF_INET,optarg, &remoteAddr);
                        break;
                
				case 't':
                        testMsgKey = atoi(optarg);
                        break;

				case 'v':
                        // version
                        (void)fprintf(stdout, "%s version %s ", program_name, version);
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
                }
   }

   
   // END of option preocessing
   if ((testfd = msgget(testMsgKey,PERMS|IPC_CREAT)) < 0) {
           printf("Unable to open test message queue\n");
           return 1;
   }

   
   mtp31.bindConnectIndication(&mtp3ConnectionIndication);
   mtp31.bindConnectConfirm(&mtp3ConnectionConfirm);
   mtp31.bindDataIndication(&mtp3DataIndication);
   mtp31.bindExpeditedDataIndication(&mtp3ExpeditedDataIndication);
   mtp31.bindResetIndication(&mtp3ResetIndication);
   mtp31.bindResetConfirm(&mtp3ResetConfirm);
   mtp31.bindDisconnectIndication(&mtp3DisconnectIndication);
   mtp31.bindInformIndication(&mtp3InformIndication);
   mtp31.bindUnitdataIndication(&mtp3UnitdataIndication);
   mtp31.bindNoticeIndication(&mtp3NoticeIndication);
   
   // Note that messages to Layer Mgmt go to STDOUT
   mtp31.initialize(localPort,localAddr,remotePort,remoteAddr);

   mtp31.iterate();
	  
   log(DEBUG, "Entering Main Loop");
   for(;;)
   {
	  if ( (n=MessageReceive(testfd, &mesg)) >= 0) 
	  {
	     log(DEBUG, "MessageReceived");
         mesg.mesg_data[n] = '\0';
         log(DEBUG,mesg.mesg_data);  // Are we getting good data in 
         cout << mesg.mesg_type << endl;
	 /*  if (mesg.mesg_type > MAX_EVENTS) 
		 {
            cout << "exiting" << endl;
            break;
         } 
         */
	     switch(mesg.mesg_type)
	     {
                case 1:
                       mtp31.messageTest();
                       break;  
		       // Define SCCP Upper Layer interface  
               
		default:
		    exit(0);

         }
	  }
   }

   if(msgctl(testfd,IPC_RMID, (struct msqid_ds *)0) <0) {
          printf("Unable to RMID test message queue");
          return 1;
   }

}

void
usage(void)
{
    (void)fprintf(stderr, "%s version %s ", program_name, version);
    (void)fprintf(stderr, "Usage: %s [-v(version)] [-V(verbose)] [-d(debug)] ", program_name);
    exit(-1);
}

