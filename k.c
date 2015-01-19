//KLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct msgbuf{
  long type;
  int cmd;
  char nick[10];
  char text[256];
  char date[30];
  int pid;
  int status;
} msgbuf;

int main() {
  int msgid;
  msgbuf message, messageS;
  messageS.type = 1;
  messageS.pid = getpid();
  strcpy(messageS.nick, "lisek222");
    
  msgid = msgget(15071410, 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
  }
    
  if(msgsnd(msgid, &messageS, sizeof(messageS), 0) == -1) {
    perror("Wysyłanie elementu");
  }
  
  if(msgrcv(msgid, &message, sizeof(message), getpid(), 0) == -1) {
    perror("Odbieranie elementu");
  }
  printf("%s", message.nick);
  
  msgctl(msgid, IPC_RMID, 0);
}