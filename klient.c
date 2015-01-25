//KLIENT
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdlib.h>
#include <time.h>

typedef struct msgbuf{
    long type;
    int cmd;
    char nick[10];
    char text[256];
    char date[30];
    int pid;
    int status;
} msgbuf;

typedef struct message {
    char nick[10];
    char date[30];
    char text[256];
} message;

void displayUserList(int *msgid, msgbuf *to_send, msgbuf *received, int *result);

int main() {
    int msgid, result, condition, i;
    char nick[10], new_nick[10];
    char text[256];
    
    int choice = 1, choice1 = 1, choice2 = 1;
    char groupChoice[10];

    char *group[3];

    FILE *config;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    i=0;
    
    config = fopen("groups.conf", "r");
    if (config == NULL)
	exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, config)) != -1) {
	if(line[read-1] == '\n') {
	    line[read-1] = '\0';
	}
	group[i] = (char*) malloc(10*sizeof(char));
	strcpy(group[i], line);
	i++;
    }
    
    int chatToWrite = 0;
    message chat[10];
    time_t currTime;
    
    msgid = msgget(15071410, IPC_CREAT | 0644); 
    if(msgid == -1) {
        perror("Utworzenie kolejki komunikatów");
        exit(1);
    }    
    
    msgbuf to_send, received;
    
    to_send.type = 1;
    to_send.pid = getpid();
    
    strcpy(nick, "nieznajomy");
        
    //displaying menu
    while(choice1) {
	system("clear");
	printf("Witaj %s! Oto Twoje możliwości:\n1. zaloguj się\n2. wyświetl użytkowników\n3. wyświetl grupy\n4. zmień nazwę użytkownika\n5. zapisz się do grupy\n6. wypisz się z grupy\n7. wyświetl czat\n8. wyślij wiadomość prywatną\n9. wyślij wiadomość do grupy\n10. wyloguj się\n0. zakończ\nWybierz jedną: ", nick);
	scanf("%d", &choice1);
	printf("\n");
	
	switch(choice1) {
	    
	    case 1:
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
		break;
		
	    case 2:    
		//sending request for user list
		to_send.cmd = 2;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 3:
		//sending request for group list
		to_send.cmd = 3;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 4:
		to_send.cmd = 4;
		printf("Wprowadź nową nazwę użytkownika: ");
		scanf("%s", new_nick);
		strcpy(to_send.nick, new_nick);
		
		//sending new nick
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 5:
		printf("Wybierz grupę do której chcesz się zapisać: %s; %s; %s\n", group[0], group[1], group[2]);
		scanf("%s", groupChoice);
		strcpy(to_send.nick, groupChoice);
		
		//sending request for adding to a group
		to_send.cmd = 5;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 6:
		printf("Wybierz grupę z której chcesz się wypisać: %s; %s; %s\n", group[0], group[1], group[2]);
		scanf("%s", groupChoice);
		strcpy(to_send.nick, groupChoice);
		
		//sending request for adding to a group
		to_send.cmd = 6;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 7:
		to_send.cmd = -1;
		
		//displaying chat
		printf("chat: \n\n");
		for(i=0; i<chatToWrite; i++) {
		    printf("data: %s\tod: %s\t treść: %s\n", chat[i].date, chat[i].nick, chat[i].text);
		    printf("\n");
		}

		do {
		    printf("Wybierz 0 aby wrócić do menu: ");
		    scanf("%d", &choice2);
		}
		while(choice2 != 0);
		break;
		
	    case 8:
		//sending request for user list
		to_send.cmd = 2;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		
		printf("Lista użytkowników: ");
		
		//receiving user list		    
		result = msgrcv(msgid, &received, sizeof(received), getpid(), 0);
		if(result == -1) {
		    perror("Odbieranie elementu");
		} else {
		    printf("%s", received.text);
		}
		
		printf("\nWybierz użytkownika do którego chcesz wysłać wiadomość: ");
		scanf("%s", to_send.nick);
				
		printf("Podaj wiadomość: ");
		scanf("%s", to_send.text);
		
		currTime = time(NULL);
		strcpy(to_send.date, ctime(&currTime));
		
		to_send.cmd = 8;
		//sending message to a user
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		} else {
		    
		}		
		break;

	    case 9:
		//sending request for group list
		to_send.cmd = 3;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		
		printf("Lista grup: ");
		
		//receiving group list		    
		result = msgrcv(msgid, &received, sizeof(received), getpid(), 0);
		if(result == -1) {
		    perror("Odbieranie elementu");
		} else {
		    printf("%s", received.text);
		}
		
		printf("\nWybierz grupę do której chcesz wysłać wiadomość: ");
		scanf("%s", to_send.nick);
				
		printf("Podaj wiadomość: ");
		scanf("%s", to_send.text);
		
		currTime = time(NULL);
		strcpy(to_send.date, ctime(&currTime));
		
		to_send.cmd = 9;
		//sending message to group
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		} else {
		    
		}		
		break;
	    
	    case 10:
		//sending log out request
		to_send.cmd = 10;
		result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
		if(result == -1) {
		    perror("Wysyłanie elementu");
		    exit(1);
		}
		break;
		
	    case 0:
		to_send.cmd = -1;
		choice1 = 0;
		break;
	    default:
		break;
	}
	
	//receive status
	if(to_send.cmd >= 0) {
	    do {
		result = msgrcv(msgid, &received, sizeof(received), getpid(), 0);
		if(result == -1) {
		    perror("Odbieranie elementu");
		} else {
		    if(((received.cmd == 8) || (received.cmd == 9)) && (strcmp(received.text, ""))) {
			strcpy(chat[chatToWrite].text, received.text);
			strcpy(chat[chatToWrite].date, received.date);
			strcpy(chat[chatToWrite].nick, received.nick);
			chatToWrite++;
			printf("ODEBRANO WIADOMOŚĆ!\n\n");
		    } else {
			switch(received.cmd) {
			    case 1:
				condition = received.status;
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
				
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				} while(choice2 != 0);
				
				break;
		    
			    case 2:    
				//receiving user list
				printf("Lista użytkowników: \n");
				printf("%s", received.text);
				
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    case 3:
				//receiving group list
				printf("Lista grup: ");
				printf("%s", received.text);

				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    case 4:
				switch(received.status) {
				    case  0:
					printf("Zmiana loginu zakończona powodzeniem");
					strcpy(nick, new_nick);
					break;
				    case 1:
					printf("Taki login już istnieje");
					break;
				    case 8:
					printf("Nie jesteś zalogowany");
					break;
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}
		
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    case 5:
				switch(received.status) {
				    case 0:
					printf("Pomyślnie zapisano go grupy\n");
					break;
					
				    case 3:
					printf("Nie ma miejsc w grupie\n");
					break;
					
				    case 4:
					printf("Jesteś już zapisany do tej grupy\n");
					break;
					
				    case 5:
					printf("Grupa nie istnieje\n");
					break;
					
				    case 8:
					printf("Nie jesteś zalogowany\n");
					break;
					
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}    
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    case 6:
				switch(received.status) {
				    case 0:
					printf("Pomyślnie usunięto z grupy\n");
					break;
					
				    case 5:
					printf("Grupa nie istnieje\n");
					break;
					
				    case 6:
					printf("Nie jesteś zapisany do tej grupy\n");
					break;
					
				    case 8:
					printf("Nie jesteś zalogowany\n");
					break;
					
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}    
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    case 8:
				switch(received.status) {
				    case 0:
					printf("Wiadomość została wysłana!\n");    
					break;
					
				    case 7:
					printf("Nie istnieje taki użytkownik\n");
					break;
					
				    case 8:
					printf("Nie jesteś zalogowany!\n");
					break;
				    
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}

				do {
				    printf("Wybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;

			    case 9:
				switch(received.status) {
				    case 0:
					printf("Wiadomość została wysłana!\n");    
					break;
					
				    case 5:
					printf("Nie istnieje taka grupa\n");
					break;
					
				    case 6:
					printf("Nie jesteś w grupie\n");
					break;
					
				    case 8:
					printf("Nie jesteś zalogowany!\n");
					break;
				    
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}

				do {
				    printf("Wybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;

			    case 10:
				switch(received.status) {
				    case 8:
					printf("Nie jesteś zalogowany!\n");
					break;
					
				    case 0:
					strcpy(nick, "nieznajomy");
					printf("Wylogowano poprawnie\n");
					break;
				    
				    default:
					printf("Oops, coś poszło nie tak\n");
					break;
				}
				
				do {
				    printf("\nWybierz 0 aby wrócić do menu: ");
				    scanf("%d", &choice2);
				}
				while(choice2 != 0);
				break;
				
			    default:
				break;
			}
		    }
		}
	    } while (((received.cmd == 8) || (received.cmd == 9)) && (strcmp(received.text, "")));
	}
    }
    msgctl(msgid, IPC_RMID, 0);	
}

void displayUserList(int *msgid, msgbuf *to_send, msgbuf *received, int *result) {
    to_send->cmd = 2;
    
    printf("msgid %d, result %d\n", *msgid, *result);
    
    *result = msgsnd(*msgid, &to_send, sizeof(to_send), 0);
    if(*result == -1) {
	perror("Wysyłanie elementu");
	exit(1);
    }
    
    printf("wysłano\n");
    
    *result = msgrcv(*msgid, &received, sizeof(received), getpid(), 0);
    if(*result == -1) {
	perror("Odbieranie elementu");
    } else {
	printf("%s", received->text);
    }
    printf("odebrano");
}