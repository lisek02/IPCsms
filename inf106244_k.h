#ifndef inf106244_K_H
#define inf106244_K_H

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

#endif