#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>

char* mul(char sub[]) {
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
        
        double Product = 1;

        char *tokenword = strtok (sub, " ");
        int counttoken=0;

        while (tokenword != NULL){
            Product *= atof(tokenword);
            tokenword = strtok(NULL," ");
        }
        int sumlen = sprintf(finalsum,"Product: %.2f \n",Product);
        return finalsum;
    }
}