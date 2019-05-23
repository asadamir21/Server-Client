#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct Client {
    char IP[15];
    int Port;
    bool isActive;
    char starttime[9];
    char endtime[9];
    int pipe;
    int pipe2;
    int pid;
    struct Client *next;
}node2;

struct timeC {
    int hour; 
    int minute;
    int second;
};

void ClientTimeDiff(struct timeC t1, struct timeC t2, struct timeC *t3) {
    //calculate difference
    //get time in total seconds
    
    int seconds1 = t1.hour*60*60 + t1.minute*60 + t1.second;
    int seconds2 = t2.hour*60*60 + t2.minute*60 + t2.second;
    int totalSeconds = seconds1-seconds2;
    
    //extract time in Hours, Minutes and Seconds
    t3->minute = totalSeconds/60;
    t3->hour = t3->minute/60;
    t3->minute = t3->minute%60;
    t3->second = totalSeconds%60;
    
}

struct Client *headClient = NULL;
struct Client *currentClient = NULL;

//display the list
void ListClient() {
    struct Client *ptr = headClient;
    //start from the beginning
    while(ptr != NULL) {
        char buff[1000];
        int n;
        
        if(strcmp(ptr->endtime,"") == 0){
           n = sprintf(buff, "     %s        %d       %s       %s  \n",ptr->IP, ptr->Port, ptr->isActive ? "true" : "false" , ptr->starttime);
        }
        else{
            struct timeC dtime;
            struct timeC stime, etime;
            
            char st_hr[2];
            memcpy(st_hr, &ptr->starttime[0], 2);
            st_hr[2]='\0';
            stime.hour=atoi(st_hr);
            
            char st_m[2];
            memcpy(st_m, &ptr->starttime[3], 2);
            st_m[2]='\0';
            stime.minute=atoi(st_m);
            
            char st_s[2];
            memcpy(st_s, &ptr->starttime[6], 2);
            st_s[2]='\0';
            stime.second=atoi(st_s);
            
            //populate t2 with end time values
            char et_hr[2];
            memcpy(et_hr, &ptr->endtime[0], 2);
            et_hr[2]='\0';
            etime.hour=atoi(et_hr);
            
            char et_m[2];
            memcpy(et_m, &ptr->endtime[3], 2);
            et_m[2]='\0';
            etime.minute=atoi(et_m);
            
            char et_s[2];
            memcpy(et_s, &ptr->endtime[6], 2);
            et_s[2]='\0';
            etime.second=atoi(et_s);

            ClientTimeDiff(etime, stime, &dtime);
	    char time_diff[50];
            int time_count=sprintf(time_diff, "%02d:%02d:%02d", dtime.hour, dtime.minute, dtime.second);
            n = sprintf(buff, "     %s        %d       %s       %s        %s     %s \n", ptr->IP, ptr->Port, ptr->isActive ? "true" : "false", ptr->starttime , ptr->endtime, time_diff);
        }
            write(STDOUT_FILENO, buff, n);
        ptr = ptr->next;
    }
}

//insert Client at the first location
void insertClient(char IP[], int Port, int pipe, int pipe2) {
    struct Client *link = (struct Client*) malloc(sizeof(struct Client));
    struct Client *temp = headClient;
            
    strcpy(link->IP,IP);        
            
    link->Port = Port;
    link->pipe = pipe;
    link->pipe2 = pipe2;
    link->isActive = "true";
  
    time_t curr_time;
						
    struct tm * time_det;
    
    time(&curr_time);
    time_det=localtime(&curr_time);
    strftime(link->starttime, sizeof(link->starttime), "%H:%M:%S", time_det);
    
    link->next = temp;
    headClient = link;
}

//delete by IP Client
char* deleteClient(char* IP) {
    struct Client *ptr = headClient;
    struct Client *ptr2 = NULL;
    bool flag = false;
    
    while(ptr != NULL) {
        if(*ptr->IP == *IP){
            if(memcmp(&ptr, &headClient, sizeof(ptr)) == 0){
                headClient = ptr->next;
                flag = true;
                return "Done";
            }
            else{
                ptr2->next = ptr->next;
                return "Done";
                break;
            }
        }
        else{
            ptr2 = ptr;
            ptr = ptr->next;
        }
    }
    if(!flag){
        return "Client Not Found\n";
    }
}

void endClient(char *IP) {
    struct Client *ptr = headClient;
   
    while(ptr != NULL) {
        if(*ptr->IP == *IP){
            ptr->isActive = false;
            time_t curr_time;
            struct tm * time_det;

            time(&curr_time);
            time_det=localtime(&curr_time);
            strftime(ptr->endtime, sizeof(ptr->endtime), "%H:%M:%S", time_det);
            break;
        }
        else{
            ptr = ptr->next;
        }
    }
}

int GetPipe(char IP[], int index) {
    struct Client *ptr = headClient;
    bool flag = false;
    
    while(ptr != NULL) {
        if(strncmp(ptr->IP,IP, index) == 0){
            return ptr->pipe;
        }
        else{
            ptr = ptr->next;
        }
    }
    return -1;
}

int CLen() {
    struct Client *ptr = headClient;
    struct Client *ptr2 = NULL;
    int length = 0;
    
    while(ptr != NULL) {
        length++;
        ptr = ptr->next;
    }
    return length;
}