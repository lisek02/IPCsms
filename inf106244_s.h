#ifndef inf106244_S_H
#define inf106244_S_H

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

int translateUP(logged loggedArray[18], char nick[10]);

int removeFromGroup(logged loggedArray[18], group uGroup[3], char nick[10], int pid);

int addToGroup(logged loggedArray[18], group uGroup[3], char nick[10], int pid);

int returnUserInArray(logged loggedArray[18], char nick[10], int pid);

int setNewLogin(logged loggedArray[18], int pid, char nick[10]);

int loggedIn(logged loggedArray[18], int pid);

#endif