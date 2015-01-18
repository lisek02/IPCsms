#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct msgbuf{
  long type;
  int cmd;
  char nick[10];
  char text[256];
  char date[30];
  int pid;
  int status;
}

int main() {
  int msgid;
  struct msgbuf message;
  
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
  }

  printf("%d", msgid);
  
  if(msgrcv(msgid, &message, sizeof(message), 1, 0) == -1) {
    perror("Odbieranie elementu");
  } 
  
  printf("%s\n", message.text);
  msgctl(15071410, IPC_RMID, 0);
}