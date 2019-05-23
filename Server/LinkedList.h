#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct process {
    char *ProcName;
    pid_t Proc_ID;
    bool isActive;
    char starttime[9];
    char endtime[9];
    struct process *next;
} node;

struct time {
    int hour; 
    int minute;
    int second;
};

void calcTimeDiff(struct time t1, struct time t2, struct time *t3) {
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

struct process *head = NULL;
struct process *current = NULL;

//display the list
void ListProcess(char temp[]) {
    struct process *ptr = head;
    //start from the beginning
    
    while(ptr != NULL) {
        char buff[1000];
        int n;
        
        if(strcmp(ptr->endtime,"") == 0){
           n = sprintf(buff, "     %s       %d         %s       %s\n", ptr->ProcName, ptr->Proc_ID, ptr->isActive ? "true" : "false" , ptr->starttime);
        }
        else{
            struct time dtime;
            struct time stime, etime;
            
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

            calcTimeDiff(etime, stime, &dtime);
	    char time_diff[50];
            int time_count = sprintf(time_diff, "%02d:%02d:%02d", dtime.hour, dtime.minute, dtime.second);
            n = sprintf(buff, "     %s       %d         %s       %s        %s     %s \n", ptr->ProcName, ptr->Proc_ID, ptr->isActive ? "true" : "false", ptr->starttime , ptr->endtime, time_diff);
        }
        strcat(temp,buff);
        ptr = ptr->next;
    }    
}

//insert link at the first location

void insertProcess(char *PName, int PID) {
    struct process *link = (struct process*) malloc(sizeof(struct process));
    struct process *temp = head;
    
    link->ProcName = PName;
    link->Proc_ID = PID;
    link->isActive = "true";
    
    time_t curr_time;
						
    struct tm * time_det;
    
    time(&curr_time);
    time_det=localtime(&curr_time);
    strftime(link->starttime, sizeof(link->starttime), "%H:%M:%S", time_det);
    
    link->next = temp;
    head = link;
}

//delete by PID item
void deleteProcess(pid_t PID) {
    struct process *ptr = head;
    struct process *ptr2 = NULL;
    bool flag = false;

    while(ptr != NULL) {
        if(ptr->Proc_ID == PID){
            if(memcmp(&ptr, &head, sizeof(ptr)) == 0){
                head = ptr->next;
                flag = true;
                break;
            }
            else{
                ptr2->next = ptr->next;
                flag = true;
                break;
            }
        }
       else{
            ptr2 = ptr;
            ptr = ptr->next;
       }
   }
   if(!flag){ 
    write(STDOUT_FILENO, "Program Not Found", 18);
   }
}

void TerminateAll() {
    struct process *ptr = head;
   
    while(ptr != NULL) {
        if(ptr->isActive == true){
            time_t curr_time;
            struct tm * time_det;

            time(&curr_time);
            time_det=localtime(&curr_time);
            strftime(ptr->endtime, sizeof(ptr->endtime), "%H:%M:%S", time_det);
            kill(ptr->Proc_ID, SIGTERM);
        }
        ptr = ptr->next;
    }
}


void DeleteAll() {
    struct process *ptr = head;
   
    while(ptr != NULL) {
        deleteProcess(ptr->Proc_ID);
        ptr = ptr->next;
    }
}

void endProcess(pid_t PID) {
    struct process *ptr = head;
   
    while(ptr != NULL) {
        if(ptr->Proc_ID == PID){
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


bool isPresent(pid_t Pid){
    struct process *ptr = head;
    
    while(ptr != NULL) {
        if(ptr->Proc_ID == Pid && ptr->isActive){
            return true;
        }
    }
    return false;
}

int GetPid(char* ProcName) {
    struct process *ptr = head;
    
    while(ptr != NULL) {
        if(strcmp(ptr->ProcName, ProcName) == 0 && ptr->isActive == true){
            return ptr->Proc_ID;
        }
        else{
            ptr = ptr->next;
        }
    }
    return -1;
}

char* GetProcName(pid_t PID) {
    struct process *ptr = head;
   
    while(ptr != NULL) {
        if(ptr->Proc_ID == PID){
            return ptr->ProcName;
        }
        else{
            ptr = ptr->next;
        }
    }
    return "Not Found";
}

void ListActiveProcess(char temp[]) {
    struct process *ptr = head;
    //start from the beginning
    
    while(ptr != NULL) {
        char buff[1000];
        int n;
        
        if(strcmp(ptr->endtime,"") == 0){
            n = sprintf(buff, "     %s       %d         %s       %s\n", ptr->ProcName, ptr->Proc_ID, ptr->isActive ? "true" : "false" , ptr->starttime);
            strcat(temp,buff);
        }
        ptr = ptr->next;
    }    
}