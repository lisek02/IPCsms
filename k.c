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
  char nick[10];
  int loop = 1;
  int result;
  msgbuf message, messageS;
  messageS.type = 1;
  messageS.pid = getpid();
  strcpy(messageS.nick, "lis");
    
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
    exit(1);
  }
  
  while(loop == 1) {
    
    printf("Podaj nick: ");
    scanf("%s", nick);
    if(nick == "0") break;
    
    strcpy(messageS.nick, nick);
    printf("test1\n");
    
    result = msgsnd(msgid, &messageS, sizeof(messageS), 0);
    if(result == -1) {
      perror("Wysyłanie elementu");
      exit(1);
    }
    printf("test2\n");
    
    result = msgrcv(msgid, &message, sizeof(message), getpid(), 0);
    if(result == -1) {
      perror("Odbieranie elementu");
    } else {
      printf("%s\n", message.nick);
    }
    printf("Czy chcesz zakończyć? 0 - zakończ, 1 - kontynuuj: ");
    scanf("%d", &loop);
  }
    
  msgctl(msgid, IPC_RMID, 0);
}