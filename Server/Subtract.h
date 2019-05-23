#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

char* subtract(char sub[]) {
    char *finalsum;
    finalsum = (char*)malloc(50 * sizeof(char));

    if(strlen(sub) == 0){
        int sumlen = sprintf(finalsum,"No Inputs\n");
        return finalsum;
    }
    else{   
        int spacecount = 0;
        
        for(int i = 0; i < strlen(sub); i++){
            if(sub[i] == ' '){
                spacecount++;
            }
        }
        if(spacecount == strlen(sub)){
            int sumlen = sprintf(finalsum,"No Inputs\n");
            return finalsum;
        }
        
        double sum = 0;

        char *tokenword = strtok (sub, " ");
        int counttoken=0;
        
        sum = 2 * atoi(tokenword);
        
        while (tokenword != NULL){
            sum -= atoi(tokenword);
            tokenword = strtok(NULL," ");
        }
        int sumlen = sprintf(finalsum,"Subtraction: %.2f \n",sum);
        return finalsum;
    }
}

