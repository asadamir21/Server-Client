#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <wait.h>
#include <pthread.h>
#include "Add.h"
#include "Subtract.h"
#include "LinkedList.h"
#include "ClientList.h"
#include "Multiply.h"
#include "Divide.h"
#define TRUE 1

int connfd[2];

void * server_active();
void * pipe_read();
void * pipe_write();

void* signalhandler(int signalno){
    pid_t pid;
    pid = wait(NULL);
    endProcess(pid);
}

int main(){
    int sock, length;
    struct sockaddr_in server;
    int msgsock;
    char *buf = (char*)malloc(50 * sizeof(char));;
    int rval;
    int i;
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
	exit(1);
    }
    /* Name socket using wildcards */
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = 0;
    
    if (bind(sock, (struct sockaddr *) &server, sizeof(server))) {
        perror("binding stream socket");
	exit(1);
    }
    /* Find out assigned port number and print it out */
    length = sizeof(server);
    if (getsockname(sock, (struct sockaddr *) &server, (socklen_t*) &length)) {
        perror("getting socket name");
	exit(1);
    }
    char Kit[100];
    int count = sprintf(Kit, "Socket has port #%d\n", ntohs(server.sin_port));
    write(STDOUT_FILENO, Kit, count);
    fflush(stdout);

    /* Start accepting connections */
    listen(sock, 5);
    
    int fd[2];
    
    
    while(1){
        pthread_t s_thread;
        int ret = pthread_create(&s_thread, NULL, server_active, NULL);

        if(ret != 0){
            perror("thread");
            exit(1);
        }
        
        struct sockaddr_in clientaddr;
        socklen_t clientaddr_size = sizeof(clientaddr);
        msgsock = accept(sock, (struct sockaddr *)&clientaddr, &clientaddr_size);
        
        if (msgsock == -1){
            perror("accept");
            exit(1);
        }
        
        char clientip[15];
        //= inet_ntoa(clientaddr.sin_addr);
        strcpy(clientip, inet_ntoa(clientaddr.sin_addr));
        int clientport = ntohs(clientaddr.sin_port);
       
        char CI[100];
        int cip = sprintf(CI, "Connection Accepted From %s:%d\n", clientip, clientport);
        write(STDOUT_FILENO, CI, cip);
        
        int conpip = pipe(connfd);
        
        if(conpip == -1){
            perror("Pipe Error");
        }
        
        pid_t pid = fork();

        if (pid == -1) {
            perror("Cannot create client(fork)");
            exit(1);
        }
        
        if(pid > 0){
            insertClient(clientip, clientport, connfd[1], connfd[0]);
            
          //  pthread_t r_thread;
         //   int theret = pthread_create(&r_thread, NULL, pipe_read, NULL);
            
        }
        else if (pid == 0) {
            //close(sock);
            
            pthread_t pipe_thread;
            int cth = pthread_create(&pipe_thread, NULL, pipe_write, (void *)msgsock);
    
            if(cth == -1){
                perror("thread error");
                exit(1);
            }
            
            while(1) {
                bzero(buf, sizeof(buf));
                int rval = read(msgsock, buf, 65536);
                
                if (rval < 0){
                    perror("reading stream message");
                }
                i = 0;
                if (rval == 0){
                    write(STDOUT_FILENO, "Ending connection\n", 18);
                    TerminateAll();
                    DeleteAll();
                    exit(1);
                }
                else{
                    char *ret = (char *) malloc(sizeof(char));
                    
                    if(strncmp("list\n", buf, 5) == 0){
                        char temp[65536];
                        ListProcess(temp);
                        write(msgsock, temp, strlen(temp));
                        strcpy(temp,"");
                    }
                    else if(strncmp("listactive\n", buf, 11) == 0){
                        char temp[65536];
                        ListActiveProcess(temp);
                        write(msgsock, temp, strlen(temp));
                        strcpy(temp,"");
                    }
                    else if(strncmp("add ", buf, 4) == 0){
                        char sub[rval - 5];
                        
                        int c = 0;
                        while (c < rval - 5) {
                            sub[c] = buf[4 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        ret = add(sub);
                        
                        write(msgsock, ret, strlen(ret));
                        
                        strcpy(sub, "");
                        strcpy(buf, "");
                        strcpy(ret, "");
                    }
                    else if(strncmp("sub ", buf, 4) == 0){
                        char sub[rval - 5];
                        
                        int c = 0;
                        while (c < rval - 5) {
                            sub[c] = buf[4 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        ret = subtract(sub);
                        
                        write(msgsock, ret, strlen(ret));
                        
                        strcpy(sub, "");
                        strcpy(buf, "");
                        strcpy(ret, "");
                    }
                    else if(strncmp("mul ", buf, 4) == 0){
                        char sub[rval - 5];
                        
                        int c = 0;
                        while (c < rval - 5) {
                            sub[c] = buf[4 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        ret = mul(sub);
                        
                        write(msgsock, ret, strlen(ret));
                        
                        strcpy(sub, "");
                        strcpy(buf, "");
                        strcpy(ret, "");
                    }
                    else if(strncmp("div ", buf, 4) == 0){
                    char sub[rval - 5];
                        
                        int c = 0;
                        while (c < rval - 5) {
                            sub[c] = buf[4 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        ret = taqseem(sub);
                        
                        write(msgsock, ret, strlen(ret));
                        
                        strcpy(sub, "");
                        strcpy(buf, "");
                        strcpy(ret, "");
                    }
                    else if(strncmp("killp ", buf, 6) == 0){
                        char sub[rval - 7];
                        
                        int c = 0;
                        while (c < rval - 7) {
                            sub[c] = buf[6 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        pid_t Proc = (pid_t)atoi(sub);
                        
                        int k = kill(Proc, SIGTERM);
                           
                        if(k == 0){
                            endProcess(Proc);
                            write(msgsock, "Program Terminated\n", 19);
                        }
                        else{
                            write(msgsock, "No Currently Running Program with such Pid\n", 44);
                        }
                        strcpy(sub, "");
                        strcpy(buf, "");
                    }
                    else if(strncmp("killn ", buf, 6) == 0){
                        char *sub = malloc(rval);
                        
                        int c = 0;
                        while (c < rval - 7) {
                            sub[c] = buf[6 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        int x = GetPid(sub);

                        if(x != -1){
                            kill(x, SIGTERM);
                            endProcess(x);
                            write(msgsock, "Program Terminated\n", 19);
                        }
                        else{
                            write(msgsock, "No Currently Running Program with such Name\n", 45);
                        }
                        strcpy(sub, "");
                        strcpy(buf, "");                        
                    }
                    else if(strncmp("run ", buf, 4) == 0){
                        char *sub = malloc(rval);                  
                        int c = 0;
                        
                        while (c < rval - 5) {
                            sub[c] = buf[4 + c];
                            c++;
                        }
                        sub[c] = '\0';
                        
                        if(signal(SIGCHLD, signalhandler) == SIG_ERR){
                            exit(1);
                        }
                        
                        int fd[2];
                        int Ppip = pipe2(fd,O_CLOEXEC);

                        if(Ppip == -1){
                            perror("Pipe Error");
                        }
                        
                        pid_t Ppid = fork();

                        if(Ppid == 0){
                            close(fd[0]);
                            int exe = execlp(sub, sub, NULL);
                            char ret[10];
                            int s = sprintf(ret, "%d", errno);
                            write(fd[1], ret, s);
                            exit(0);                      
                        }
                        else if(Ppid > 0){
                            insertProcess(sub, Ppid);
                            
                            close(fd[1]);
                            char checkExec[100];
                            int y = read(fd[0], checkExec, 100);
                            
                            if(y == 0){
                                write(msgsock, "Exec Successful\n", 16);
                                strcpy(buf, "");
                            }
                            else if(y > 0) {
                                deleteProcess(Ppid);
                                if(strncmp(checkExec, "2", y) == 0){
                                    write(msgsock, "No such file or directory\n", 26); 
                                }
                                else if(strncmp(checkExec, "13", y) == 0){
                                    write(msgsock, "Permission denied\n", 18); 
                                }
                                else if(strncmp(checkExec, "8", y) == 0){
                                    write(msgsock, "Exec format error\n", 18); 
                                }
                                else if(strncmp(checkExec, "7", y) == 0){
                                    write(msgsock, "Argument list too long\n", 23); 
                                }
                                else if(strncmp(checkExec, "12", y) == 0){
                                    write(msgsock, "Out of memory\n", 13); 
                                }
                                strcpy(buf, "");
                            }
                        }
                    }
                    else{
                        write(STDOUT_FILENO, buf, strlen(buf));
                        write(msgsock, "Invalid Syntax\n", 15);
                    }
                }   
            } 
        }
        close(msgsock);
    }
}

void * server_active(){
    char *cmd = (char*)malloc(50 * sizeof(char));
    
    while(1){
        int readcmd = read(STDIN_FILENO, cmd, 65536);
        
        if(strncmp(cmd, "clients\n", readcmd) == 0){
            ListClient();
        }
        else if(strncmp(cmd, "kick ", 5) == 0){
            char sub[readcmd - 6];
            strncpy(sub, cmd + 5, readcmd);
            
            int pipe = GetPipe(sub, sizeof(sub));
            
            if(pipe == -1){
                write(STDOUT_FILENO, "Could not Kick\n", 15);
            }
            else{
                deleteClient(sub);
                write(pipe, "Dafa Ho\n", 8);
            }
        }
        else if(strncmp(cmd, "printall ", 9) == 0){
            char sub[readcmd - 10];
            strncpy(sub, cmd + 9, readcmd);
            
            struct Client *cl = headClient;
            
            while(cl != NULL) {
                write(cl->pipe, sub, readcmd - 6);
                cl = cl->next;
            }
        }
        else if(strncmp(cmd, "print ", 6) == 0){
            char sub[readcmd - 7];
            strncpy(sub, cmd + 6, readcmd - 6);
            sub[readcmd-7] = '\0';
            
            bool flag = false;
            int index = 0;
            
            for(int i = 0; i < strlen(sub); i++){
                if(sub[i] == ' '){
                    index = i;
                    flag = true;
                    break;
                }
            }
            if(flag){
                char ipp[index];
                int c = 0;
                while (c < sizeof(ipp)) {
                    ipp[c] = sub[c];
                    c++;
                }

                char message[strlen(sub) - index];
                int d = 0;
                while (d < strlen(sub) - index) {
                    message[d] = sub[d + index + 1];
                    d++;
                }
                
                int pipe = GetPipe(ipp, index);
                    
                if(pipe != -1){
                    write(pipe, message, strlen(message));
                }
                else{
                    write(STDOUT_FILENO, "Message Failed\n", 16);
                }
                strcpy(ipp,"");
                strcpy(message,"");
            }
            else{
                write(STDOUT_FILENO, "IP or Message not properly given\n", 33);
            }
            strcpy(sub,"");
        }
        else if(strncmp(cmd, "list\n", readcmd) == 0){
            write(STDOUT_FILENO, "\n", 1);
            struct Client *cl = headClient;
            write(STDOUT_FILENO, "All Processes\n", 14);
            while(cl != NULL) {
                write(cl->pipe, "list dekhao", 11);
                cl = cl->next;
            }
        }
        else if(strncmp(cmd, "listactive\n", readcmd) == 0){
            write(STDOUT_FILENO, "\n", 1);
            struct Client *cl = headClient;
            write(STDOUT_FILENO, "All Active Processes\n", 19);
            while(cl != NULL) {
                write(cl->pipe, "active", 5);
                cl = cl->next;
            }
        }
        else if(strncmp(cmd, "client count\n", 13) == 0){
            char sub[100];
            int n = sprintf(sub, "No. of Clients = %d", CLen());
            write(STDOUT_FILENO, sub, n);
        }
        else{
            write(STDOUT_FILENO, "Invalid Syntax\n", 15);
        }
        write(STDOUT_FILENO, "\n", 1);
        strcpy(cmd,"");

    }
}

void * pipe_write(void* sock){
    while(1){
        char *buff= (char*)malloc(50 * sizeof(char));
        int readbuff = read(connfd[0], buff, 65536);
        
        if(strncmp(buff, "list dekhao", 11) == 0){
            char temp[65536];
            ListProcess(temp);
            write(STDOUT_FILENO, temp, strlen(temp));
            strcpy(temp, "");
        }
        else if(strncmp(buff, "active", 5) == 0){
            char temp[65536];
            ListActiveProcess(temp);
            write(STDOUT_FILENO, temp, strlen(temp));
            strcpy(temp, "");
        }
        else{
            int wr = write(sock, buff, readbuff);

            if(wr == -1){
                perror("write");
            }
        }
        strcpy(buff, "");
    }
}
/*
void * pipe_read(){
    char ip[15];
    int rd = read(connfd[0], ip, 15);
    
    deleteClient(ip);
    char stat[100];
    int p = sprintf(stat, "Client with IP: %s successfully removed from client list\n", ip);
    write(STDOUT_FILENO, stat, p);
}
 */ 