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
  msgbuf message, messageS;
  
  
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
  }
  
  do {
    if(msgrcv(msgid, &message, sizeof(message), 1, 0) == -1) {
      //perror("Odbieranie elementu");
      exit(1);
    }
    printf("%s", message.nick);
    char nick2[10];

    messageS.type = message.pid;
    messageS.cmd = 1;
    messageS.status = 0;
    strcpy(messageS.nick, message.nick);
    
    if(msgsnd(msgid, &messageS, sizeof(messageS), 0) == -1) {
      perror("Wysyłanie elementu");
    }
    
  } while(1);
  
  
}