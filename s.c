//SERWER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <ctype.h>

typedef struct msgbuf {
  long type;
  int cmd;
  char nick[10];
  char text[256];
  char date[30];
  int pid;
  int status;
} msgbuf;

typedef struct logged {
  char nick[10];
  int pid;
} logged;

int main() {
  int msgid;
  int loop = 1;
  int result;
  msgbuf message, messageS;
  
  
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
    exit(1);
  }
  
  while(loop == 1) {
    //memset(message.nick, 0, sizeof(message.nick));
    //memset(messageS.type, 0, sizeof(messageS.type));
    //memset(messageS.status, 0, sizeof(messageS.status));
    //memset(messageS.nick, 0, sizeof(messageS.nick));    
    
    result = msgrcv(msgid, &message, sizeof(message), 0, 0);
    if(result == -1) {
      perror("Odbieranie elementu");
      exit(1);
    } else {
      printf("%s\n", message.nick);
      char nick2[10];
      printf("test1\n");

      messageS.type = message.pid;
      messageS.cmd = 1;
      messageS.status = 0;
      strcpy(messageS.nick, message.nick);
      printf("test2\n");
      
      result = msgsnd(msgid, &messageS, sizeof(messageS), 0);
      if(result == -1) {
	perror("Wysyłanie elementu");
	exit(1);
      }
    }
    //msgctl(msgid, IPC_RMID, 0);
  }
  return 0;  
}