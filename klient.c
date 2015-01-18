#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>

struct msgbuf{
  long type;
  int cmd;
  char nick[10];
  char text[256];
  char date[30];
  int pid;
  int status;
};

int main() {
  char nick[10];
  char text[256];
  int choice, choice1, choice2, groupChoice;
  const char *group[3] = {"pierwsza", "druga", "trzecia"};	
  
  do {
    choice = 1;
    system("clear");
    //logowanie
    printf("Witaj! Podaj swoją nazwę użytkownika: ");
    scanf("%s", nick);
    
    //menu
    do {
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
    } while(choice1 != 0 && choice != 0);
  } while(choice != 0);
	

      
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