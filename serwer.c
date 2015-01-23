//SERWER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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
  int msgid, result, type, loop = 1;
  int i;
  
  msgbuf to_send, recieved;
  logged loggedArray[18];
  
  for(i=0; i<18; i++) {
    loggedArray[i].pid = 0;
    strcpy(loggedArray[i].nick, "");
  }
//   //test - zapełnij prawie całą tablicę
//   for(i=0; i<15; i++) {
//       loggedArray[i].pid = i+1;
//   }
  
//   printf("test");
  
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
    exit(1);
  }
  
  printf("msgid: %d\n", msgid);
  
  while(loop == 1) {
    
    result = msgrcv(msgid, &recieved, sizeof(recieved), 1, 0);
    if(result == -1) {
      perror("Odbieranie elementu");
      exit(1);
    } 
    else {
      printf("%s", recieved.nick);
      to_send.type = recieved.pid;
      
      switch(recieved.cmd) {
	case 1:
	  to_send.cmd = 1;
	  int userInArray = returnUserInArray(loggedArray, recieved.nick, recieved.pid);
// 	  printf("User in array: %d\n\n", userInArray);
	  if(userInArray < 0) {
	    to_send.status = abs(userInArray);
	  } else {
	    to_send.status = 0;
	    strcpy(loggedArray[userInArray].nick, recieved.nick);
	    loggedArray[userInArray].pid = recieved.pid;
	  }
// 	  for(i=0; i<18; i++) printf("pid value: %d, nick: %s\n ", loggedArray[i].pid, loggedArray[i].nick);
// 	  printf("type: %ld, cmd: %d. status: %d\n", to_send.type, to_send.cmd, to_send.status);
	  break;
	  
	default:
	  printf("Something went wrong");
	  break;
      }
    }
    
    result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
    if(result == -1) {
      perror("Wysyłanie elementu");
      exit(1);
    } else (printf("wysłano odpowiedź"));
  }
  
  return 0;
}

int returnUserInArray(logged loggedArray[18], char nick[10], int pid) {
  int toReturn = -3;
  int i;
//   for(i=0; i<18; i++) printf("pid value: %d, nick: %s\n ", loggedArray[i].pid, loggedArray[i].nick);
  for(i=0; i<18; i++) {
//       printf("nick to insert %s\n", nick);
      if(!strcmp(loggedArray[i].nick,nick)) {
	if(loggedArray[i].pid == pid) {
	  return -2;
	}
	return -1;
      }
  }
  for(i=17; i>=0; i--) {
//     printf("%lu", sizeof(loggedArray[i]));
    if(loggedArray[i].pid == 0) {
      return i;
    }
  }
  printf("\n");
  return -3;
}