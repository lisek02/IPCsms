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

typedef struct group {
    char name[10];
    int users[10];
} group;

void generateUserList(logged loggedArray[18], char *text);

int main() {
    int msgid, result, type, loop = 1, status;
    int i,j;
    group *uGroup = malloc(3 * sizeof(group));
    
    strcpy(uGroup[0].name, "heheszki");
    strcpy(uGroup[1].name, "kicioch");
    strcpy(uGroup[2].name, "humor");
    
    for(i=0; i<3; i++) {
	for(j=0; j<10; j++) {
	    uGroup[i].users[j] = 0;
	}
    }
    
//     for(i=0; i<3; i++) {
// 	for(j=0; j<10; j++) {
// 	    printf("%s", uGroup[i].users[j]);
// 	    //printf("%s", uGroup[i].users[j]);
// 	}
//     }
    
    
    
//     FILE *config;
//     if((config = fopen("groups.conf", "r")) == NULL) {
// 	perror("błąd otwarcia pliku");
//     } else {
// 	char *buffer;
// 	i=0;
// 	while(fgets(buffer, 20, config) != NULL && i<3) {
// 	    strcpy(group[i], buffer);
// 	    i++;
// 	}
//     }
    
    msgbuf to_send, received;
    logged loggedArray[18];
    
    for(i=0; i<18; i++) {
        loggedArray[i].pid = 0;
        strcpy(loggedArray[i].nick, "");
    }
//     //test - zapełnij prawie całą tablicę
//     for(i=0; i<15; i++) {
//             loggedArray[i].pid = i+1;
//     }
    
//     printf("test");

//     for(i=0; i<9; i++) {
// 	uGroup[1].users[i] = i+1;
//     }
    
    msgid = msgget(15071410, IPC_CREAT | 0644); 
    if(msgid == -1) {
        perror("Utworzenie kolejki komunikatów");
        exit(1);
    }
    
    printf("msgid: %d\n", msgid);
    
    while(loop == 1) {
        
        result = msgrcv(msgid, &received, sizeof(received), 1, 0);
        if(result == -1) {
            perror("Odbieranie elementu");
            exit(1);
        } 
        else {
//             printf("%s", received.nick);
            to_send.type = received.pid;
            
        switch(received.cmd) {
	    case 1:
		to_send.cmd = 1;
		int userInArray = returnUserInArray(loggedArray, received.nick, received.pid);
    // 	    printf("User in array: %d\n\n", userInArray);
		if(userInArray < 0) {
		    to_send.status = abs(userInArray);
		} else {
		    to_send.status = 0;
		    strcpy(loggedArray[userInArray].nick, received.nick);
		    loggedArray[userInArray].pid = received.pid;
		}
    // 	    for(i=0; i<18; i++) printf("pid value: %d, nick: %s\n ", loggedArray[i].pid, loggedArray[i].nick);
    // 	    printf("type: %ld, cmd: %d. status: %d\n", to_send.type, to_send.cmd, to_send.status);
		break;
		
	    case 2:
    // 	    printf("weszło w case 2\n");
		to_send.cmd = 2;
		strcpy(to_send.text, "");
		for(i=0; i<18; i++) {
		    if(strcmp(loggedArray[i].nick, "")) {
			strcat(to_send.text, loggedArray[i].nick);
			strcat(to_send.text, "; ");
		    }		
		}
		printf("%s\n", to_send.text);
		break;

	    case 3:
		to_send.cmd = 3;
		strcpy(to_send.text, "");
		for(i=0; i<3; i++) {
		    strcat(to_send.text, uGroup[i].name);
		    strcat(to_send.text, "; ");
		}
		printf("%s\n", to_send.text);
		break;
		
	    case 4:
		to_send.cmd = 4;
		status = setNewLogin(loggedArray, received.pid, received.nick);
		to_send.status = status;
		break;
		
	    case 5:
		to_send.cmd = 5;
		status = addToGroup(loggedArray, uGroup, received.nick, received.pid);
		to_send.status = status;
		break;
		
	    case 6:
		to_send.cmd = 6;

		for(i=0; i<3; i++) {
		    for(j=0; j<10; j++) {
			printf("%d\t", uGroup[i].users[j]);
		    }
		    printf("\n");
		}
		printf("\n");
		
		status = removeFromGroup(loggedArray, uGroup, received.nick, received.pid);
		
		for(i=0; i<3; i++) {
		    for(j=0; j<10; j++) {
			printf("%d\t", uGroup[i].users[j]);
		    }
		    printf("\n");
		}	 
		
		to_send.status = status;
		break;
		
	    case 10:
		to_send.cmd = 10;
		if(loggedIn(loggedArray, received.pid)) {
		    for(i=0; i<18; i++) {
			if(loggedArray[i].pid == received.pid) {		//delete user
			    loggedArray[i].pid = 0;
			    strcpy(loggedArray[i].nick, "");
			}
		    }
		    
		    for(i=0; i<3; i++) {					//delete from group
			for(j=0; j<10; j++) {
			    if(uGroup[i].users[j] == received.pid) {	
				uGroup[i].users[j] = 0;
			    }
			}
		    }
		    
		    to_send.status = 0;
		} else {
		    to_send.status = 8;
		}
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

int removeFromGroup(logged loggedArray[18], group uGroup[3], char nick[10], int pid) {
    int i,j;
    int logged = loggedIn(loggedArray, pid);
    if(logged == 1) {
	for(i=0; i<3; i++) {
	    if(!strcmp(uGroup[i].name, nick)) {
		for(j=0; j<10; j++) {
		    if(uGroup[i].users[j] == pid) {
			uGroup[i].users[j] = 0;
			return 0;
		    }
		}
		return 6;
	    }
	}
	return 5;
    } else {
	return 8;
    }
}

int addToGroup(logged loggedArray[18], group uGroup[3], char nick[10], int pid) {
    int i,j;
    int logged = loggedIn(loggedArray, pid);
    if(logged == 1) {
	for(i=0; i<3; i++) {
	    if(!strcmp(uGroup[i].name, nick)) {		//find proper group
		for(j=0; j<10; j++) {
		    if(uGroup[i].users[j] == pid) {	//check if user is already in this group
			return 4;
		    }
		}
		for(j=0; j<10; j++) {
		    if(uGroup[i].users[j] == 0) {			//find empty space in group
			uGroup[i].users[j] = pid;
			return 0;
		    }
		}
		return 3;
	    }
	}
	return 5;
    } else {
	return 8;
    }
}

int returnUserInArray(logged loggedArray[18], char nick[10], int pid) {
    int toReturn = -3;
    int i;
//     for(i=0; i<18; i++) printf("pid value: %d, nick: %s\n ", loggedArray[i].pid, loggedArray[i].nick);
    for(i=0; i<18; i++) {
//             printf("nick to insert %s\n", nick);
	if(!strcmp(loggedArray[i].nick,nick)) {
	    if(loggedArray[i].pid == pid) {
		return -2;
	    }
	return -1;
	}
    }
    for(i=17; i>=0; i--) {
//         printf("%lu", sizeof(loggedArray[i]));
        if(loggedArray[i].pid == 0) {
            return i;
        }
    }
    printf("\n");
    return -3;
}

int setNewLogin(logged loggedArray[18], int pid, char nick[10]) {
    int i;
    int logged = loggedIn(loggedArray, pid);
    if(logged == 0) {
	return 8;
    }
    
//     for(i=0; i<18; i++) {
// 	if(!strcmp(loggedArray[i].nick, nick)) {
// 	    if(loggedArray[i].pid == pid) {
// 		return 0;
// 	    } else {	    
// 		return 1;
// 	    }
// 	}
//     }
    
//     for(i=0; i<18; i++) {
// 	if(loggedArray[i].pid == pid) {
// 	    strcpy(loggedArray[i].nick, nick);
// 	    return 0;
// 	}
//     }
    
    
    for(i=0; i<18; i++) {
	if(!strcmp(loggedArray[i].nick, nick)) {
	    return 1;
	}
    }
    
    for(i=0; i<18; i++) {
	if(loggedArray[i].pid == pid) {
	    strcpy(loggedArray[i].nick, nick);
	    return 0;
	}
    }
    
    
//     printf("pid: %d\n", pid);
//     printf("nick: %s\n", nick);
//     for(i=0; i<18; i++) {
// 	printf("F\n");
// 	printf("logged pid: %d\n", loggedArray[i].pid);
// 
// 	printf("logged nick: %s\n", loggedArray[i].nick);
// 
// 	if(loggedArray[i].pid == pid) {
// 	    printf("test1\n");
// 	    strcpy(loggedArray[i].nick, nick);
// 	    return 0;
// 	} else if(!strcmp(loggedArray[i].nick, nick)) {
// 	    printf("test2\n");
// 	    return 1;
// 	}
//     }
//     return 8;
}


/*
 * returns 1 if user is logged in or 0 otherwise
 */
int loggedIn(logged loggedArray[18], int pid) {
    int i;
    int toReturn = 0;
    //printf("pid: %d\n", pid);
    for(i=0; i<18; i++) {
	printf("%d\n", loggedArray[i].pid);
	if(loggedArray[i].pid == pid) {
	    toReturn = 1;
	    break;	   
	}
    }
    printf("to return %d: ]n\n", toReturn);
    return toReturn;
}