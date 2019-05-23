#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string.h>

int sock;    

void* read_msgsock();

void* signalhandler(int signalno){
    close(sock);
    write(STDOUT_FILENO, "\n", 1);
    write(STDOUT_FILENO, "Disconnected From Sever And Exited\n", 35);
    exit(1);
}

int main(int argc, char *argv[]){
    struct sockaddr_in server;
    struct hostent *hp;
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("opening stream socket");
	exit(1);
    }
    /* Connect socket using name specified by command line. */
    server.sin_family = AF_INET;
    hp = gethostbyname(argv[1]);
    if (hp == 0) {
        fprintf(stderr, "%s: unknown host\n", argv[1]);
	    exit(2);
    }
    bcopy(hp->h_addr, &server.sin_addr, hp->h_length);
    server.sin_port = htons(atoi(argv[2]));

    if (connect(sock,(struct sockaddr *) &server,sizeof(server)) < 0) {
        perror("connecting stream socket");
	    exit(1);
    }
    else{
        write(STDOUT_FILENO, "Connection Established\n", 23);
    }
    pthread_t c_thread;
    int ret = pthread_create(&c_thread, NULL, read_msgsock, (void *)sock);

    if(ret != 0){
        perror("thread");
        exit(1);
    }

    char *buff = (char*)malloc(50 * sizeof(char));

    if(signal(SIGINT, signalhandler) == SIG_ERR){
        exit;
    }

    while(1) {
        int readData = read(STDIN_FILENO, buff, 65536);
        
        if (strncmp(buff, "exit\n", 5) == 0){
            close(sock);
            write(STDOUT_FILENO, "Disconnected From Server\n", 26);
            exit(1);
        }
        int output = write(sock, buff, readData);
        
        if(output == -1){
            write(STDOUT_FILENO, "Unable to Get Data\n", 19);
        }
    }
    close(sock);
}

void * read_msgsock(void* sock){
    while(1){
        char *temp = (char*)malloc(50 * sizeof(char));
        int readTemp = read(sock, temp, 65536);
        if(readTemp == 0){
            write(STDOUT_FILENO, "Disconnected From Server\n", 25);
            exit(1);    
        }
        else if(strncmp(temp, "Dafa Ho", 7) == 0){
            write(STDOUT_FILENO, "Kicked From Server\n", 20);
            close(sock);
            exit(1);    
        }
        else{
            write(STDOUT_FILENO, temp, readTemp);    
        }
        strcpy(temp,"");
        write(STDOUT_FILENO, "\n", 1);    
    }
}
