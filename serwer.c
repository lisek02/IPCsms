//SERWER
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>
#include <unistd.h>

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
    char pidToUser[10];
    int userToPid;
    group *uGroup = malloc(3 * sizeof(group));

    for(i=0; i<3; i++) {
	for(j=0; j<10; j++) {
	    uGroup[i].users[j] = 0;
	}
    }  

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
	strcpy(uGroup[i].name, line);
	i++;
    }
    
    msgbuf to_send, received, toSendMessage;
    logged loggedArray[18];
    
    for(i=0; i<18; i++) {
        loggedArray[i].pid = 0;
        strcpy(loggedArray[i].nick, "");
    }
    
    msgid = msgget(15071410, IPC_CREAT | 0644); 
    if(msgid == -1) {
        perror("Utworzenie kolejki komunikatów");
        exit(1);
    }

    while(loop == 1) {
        
        result = msgrcv(msgid, &received, sizeof(received), 1, 0);
        if(result == -1) {
            perror("Odbieranie elementu");
            exit(1);
        } 
        else {
            to_send.type = received.pid;
            
	    switch(received.cmd) {
		case 1:
		    to_send.cmd = 1;
		    int userInArray = returnUserInArray(loggedArray, received.nick, received.pid);
		    if(userInArray < 0) {
			to_send.status = abs(userInArray);
		    } else {
			to_send.status = 0;
			strcpy(loggedArray[userInArray].nick, received.nick);
			loggedArray[userInArray].pid = received.pid;
		    }
		    break;
		    
		case 2:
		    to_send.cmd = 2;
		    strcpy(to_send.text, "");
		    for(i=0; i<18; i++) {
			if(strcmp(loggedArray[i].nick, "")) {
			    strcat(to_send.text, loggedArray[i].nick);
			    strcat(to_send.text, "; ");
			}		
		    }
		    break;

		case 3:
		    to_send.cmd = 3;
		    strcpy(to_send.text, "");
		    for(i=0; i<3; i++) {
			strcat(to_send.text, uGroup[i].name);
			strcat(to_send.text, "; ");
		    }
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
		    status = removeFromGroup(loggedArray, uGroup, received.nick, received.pid);
		    to_send.status = status;
		    break;
		    
		case 8:
		    to_send.cmd = 8;
		    
		    //translate user to pid
		    userToPid = translateUP(loggedArray, received.nick);
		    
		    //translate pid to user
		    for(i=0; i<18; i++) {
			if(loggedArray[i].pid == received.pid) {
			    strcpy(pidToUser, loggedArray[i].nick);
			    break;
			}
		    }
		    
		    strcpy(to_send.text, "");
    ;
		    toSendMessage.type = userToPid;
		    toSendMessage.cmd = 8;
		    strcpy(toSendMessage.text, received.text);
		    strcpy(toSendMessage.date, received.date);
		    strcpy(toSendMessage.nick, pidToUser);
		    
		    if(loggedIn(loggedArray, received.pid)) {
			to_send.status = 7;
			for(i=0; i<18; i++) {
			    if(loggedArray[i].pid == userToPid) {
				result = msgsnd(msgid, &toSendMessage, sizeof(toSendMessage), 0);
				if(result == -1) {
				    perror("Wysyłanie elementu");
				    exit(1);
				}
				
				to_send.status = 0;
				break;
			    }
			}
		    } else {
			to_send.status = 8;
		    }
		    break;		
		    
		case 9:
		    to_send.cmd = 9;
		    
		    int proceed = 0;
		    //translate pid to user
		    for(i=0; i<18; i++) {
			if(loggedArray[i].pid == received.pid) {
			    strcpy(pidToUser, loggedArray[i].nick);
			    break;
			}
		    }
		    
		    strcpy(to_send.text, "");
		    
		    toSendMessage.cmd = 9;
		    strcpy(toSendMessage.text, received.text);
		    strcpy(toSendMessage.date, received.date);
		    strcpy(toSendMessage.nick, pidToUser);		
		    
		    
		    
		    if(loggedIn(loggedArray, received.pid)) {
			for(i=0; i<3; i++) {
			    to_send.status = 5;
			    if(!strcmp(uGroup[i].name, received.nick)) {
				to_send.status = 6;
				for(j=0; j<10; j++) {
				    if(uGroup[i].users[j] == received.pid) {
					proceed = 1;
					to_send.status = 0;
					break;
				    }
				}
				if(proceed == 1) {
				    for(j=0; j<10; j++) {
					if(uGroup[i].users[j] != 0) {
					    toSendMessage.type = uGroup[i].users[j];
					    result = msgsnd(msgid, &toSendMessage, sizeof(toSendMessage), 0);
					    if(result == -1) {
						perror("Wysyłanie elementu");
						exit(1);
					    }
					}
				    }
				}
				break;
			    }
			}
		    } else {
			to_send.status = 8;
		    }
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
		    break;
	    }
	}
		
	result = msgsnd(msgid, &to_send, sizeof(to_send), 0);
	if(result == -1) {
	    perror("Wysyłanie elementu");
	    exit(1);
	}
    }
    
    if (raise(SIGINT) != 0) {
        fputs("Error raising the signal.\n", stderr);
	printf("end");
	sleep(10);
        return EXIT_FAILURE;
    }
    
    return 0;
}

int translateUP(logged loggedArray[18], char nick[10], int pid) {
    int i;
    for(i=0; i<18; i++) {
	if(!strcmp(loggedArray[i].nick, nick)) {
	    return loggedArray[i].pid;
	}
    }  
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
    for(i=0; i<18; i++) {
	if(!strcmp(loggedArray[i].nick,nick)) {
	    if(loggedArray[i].pid == pid) {
		return -2;
	    }
	return -1;
	}
    }
    for(i=17; i>=0; i--) {;
        if(loggedArray[i].pid == 0) {
            return i;
        }
    }
    return -3;
}

int setNewLogin(logged loggedArray[18], int pid, char nick[10]) {
    int i;
    int logged = loggedIn(loggedArray, pid);
    if(logged == 0) {
	return 8;
    }  
    
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
}


/*
 * returns 1 if user is logged in or 0 otherwise
 */
int loggedIn(logged loggedArray[18], int pid) {
    int i;
    int toReturn = 0;
    for(i=0; i<18; i++) {
	if(loggedArray[i].pid == pid) {
	    toReturn = 1;
	    break;	   
	}
    }
    return toReturn;
}