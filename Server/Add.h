#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

char* add(char sub[]) {
    char *finalsum = (char*)malloc(50 * sizeof(char));

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

        while (tokenword != NULL){
            sum += atoi(tokenword);
            tokenword = strtok(NULL," ");
        }
        int sumlen = sprintf(finalsum,"Sum: %.2f \n",sum);
        return finalsum;
    }
}