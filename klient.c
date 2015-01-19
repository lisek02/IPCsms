//KLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

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
  int msgid, result, condition;
  char nick[10];
  char text[256];
  
  int choice = 1, choice1 = 1, choice2 = 1, groupChoice;
  const char *group[3] = {"pierwsza", "druga", "trzecia"};

  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
    exit(1);
  }  
  
  msgbuf to_send, recieved;
  
  to_send.type = 1;
  to_send.pid = getpid();
  
  while(choice) {
    //log in
    do {
      printf("Witaj! Podaj swoją nazwę użytkownika: ");
      scanf("%s", nick);
      strcpy(to_send.nick, nick);
      
      to_send.cmd = 1;
      
      //sending username
      result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
      if(result == -1) {
	perror("Wysyłanie elementu");
	exit(1);
      }
      
      //response after sending username
      result = msgrcv(msgid, &recieved, sizeof(recieved), getpid(), 0);
      if(result == -1) {
	perror("Odbieranie elementu");
      } else {
	condition = recieved.status;
	switch(condition) {
	  case 0:
	    printf("Wszystko ok\n");
	    break;
	  case 1:
	    printf("Taki nick już istnieje\n");
	    break;
	  case 2:
	    printf("Jesteś już zalogowany\n");
	    condition = 0;
	    break;
	  case 3:
	    printf("Maksymalna liczba użytkowników. Wpisz 0 aby wyjść\n");
	    scanf("%d", &choice2);
	    if(choice2 == 0) choice = 0;	    
	    condition = 0;
	    choice1 = 0;
	    break;
	  default:
	    printf("Oops, coś poszło nie tak\n");
	    break;
	}
      }
    } while (condition != 0);
    
    //displaying menu
    while(choice1) {
      system("clear");
      printf("Witaj %s! Oto Twoje możliwości:\n1. wyświetl użytkowników\n2. wyświetl grupy\n3. zmień nazwę użytkownika\n4. zapisz się do grupy\n5. wypisz się z grupy\n6. wyświetl czat\n7. wyślij wiadomość do grupy\n8. wyślij wiadomość prywatną\n9. wyloguj się\n0. zakończ\nWybierz jedną: ", nick);
      scanf("%d", &choice1);
      printf("\n");
      
      switch(choice1) {
	
	case 1:
	  printf("Lista użytkowników: ");
	  do {
	    printf("Wybierz 0 aby wrócić do menu: ");
	    scanf("%d", &choice2);
	  }
	  while(choice2 != 0);
	  break;
	  
	case 2:
	  printf("Lista grup:\npierwsza\ndruga\ntrzecia\n");
	  do {
	    printf("Wybierz 0 aby wrócić do menu: ");
	    scanf("%d", &choice2);
	  }
	  while(choice2 != 0);
	  break;
	  
	case 3:
	  printf("Wprowadź nową nazwę użytkownika: ");
	  scanf("%s", nick);
	  break;
	  
	case 4:
	  printf("Wybierz grupę do które chcesz się zapisać:\n1. %s\n2. %s\n3. %s\n", group[0], group[1], group[2]);
	  do {
	    scanf("%d", &groupChoice);
	  } while (groupChoice != 1 && groupChoice != 2 && groupChoice != 3);
	  break;
	  
	case 5:
	  printf("Wybierz grupę z której chcesz się wypisać:\n1. %s\n2. %s\n3. %s\n", group[0], group[1], group[2]);
	  do {
	    scanf("%d", &groupChoice);
	  } while (groupChoice != 1 && groupChoice != 2 && groupChoice != 3);
	  break;
	  
	case 6:
	  printf("Czat:\ndata: 18.01.2014\tod: nieznajomy\t treść: elo elo\n");
	  do {
	    printf("Wybierz 0 aby wrócić do menu: ");
	    scanf("%d", &choice2);
	  }
	  while(choice2 != 0);
	  break;
	  
	case 7:
	  printf("Wybierz grupę do której chcesz wysłać wiadomość:\n1. %s\n2. %s\n3. %s\n", group[0], group[1], group[2]);
	  do {
	    scanf("%d", &groupChoice);
	  } while (groupChoice != 1 && groupChoice != 2 && groupChoice != 3);
	  printf("Podaj wiadomość: ");
	  scanf("%s", text);
	  printf("Wiadomość została wysłana!\n");
	  do {
	    printf("Wybierz 0 aby wrócić do menu: ");
	    scanf("%d", &choice2);
	  }
	  while(choice2 != 0);
	  break;
	  
	case 8:
	  printf("Wybierz użytkownika do którego chcesz wysłać wiadomość:\n1. %s\n2. %s\n3. %s\n", group[0], group[1], group[2]);
	  do {
	    scanf("%d", &groupChoice);
	  } while (groupChoice != 1 && groupChoice != 2 && groupChoice != 3);
	  printf("Podaj wiadomość: ");
	  scanf("%s", text);
	  printf("Wiadomość została wysłana!\n");
	  do {
	    printf("Wybierz 0 aby wrócić do menu: ");
	    scanf("%d", &choice2);
	  }
	  while(choice2 != 0);
	  break;
	  
	case 9:
	  choice1 = 0;
	  break;
	  
	case 0:
	  choice1 = 0;
	  choice = 0;
	default:
	  break;
      }
    }
  }
  
  msgctl(msgid, IPC_RMID, 0);	

      
/*  
  int msgid;
  struct msgbuf message;
  
  msgid = msgget(15071410, IPC_CREAT | 0644); 
  if(msgid == -1) {
    perror("Utworzenie kolejki komunikatów");
  }
  printf("%d", msgid);
  
  strcpy(message.text, "hello");
  message.type = 1;
  
  if(msgsnd(msgid, &message, sizeof(message.text), 0) == -1) {
    perror("Wysyłanie elementu");
  }
  msgctl(15071410, IPC_RMID, 0);
*/  
}