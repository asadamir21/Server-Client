#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include <math.h>

char* taqseem(char sub[]) {
    char *finalDiv = (char*)malloc(50 * sizeof(char));
    if(strlen(sub) == 0){
        int sumlen = sprintf(finalDiv,"No Inputs\n");
        return finalDiv;
    }
    else{   
        int spacecount = 0;
        
        for(int i = 0; i < strlen(sub); i++){
            if(sub[i] == ' '){
                spacecount++;
            }
        }
        
        if(spacecount == strlen(sub)){
            int sumlen = sprintf(finalDiv,"No Inputs\n");
            return finalDiv;
        }
        
        double Div;
        
        char *tokenword = strtok (sub, " ");
        
        int check = atoi(tokenword);
        int count = 0;
        Div = atoi(tokenword);

        while (tokenword != NULL){
            if(atoi(tokenword) != check && count != 0){
                if(atoi(tokenword) == 0){
                    int sumlen = sprintf(finalDiv,"Zero Included Cannot Divide\n");
                    return finalDiv;
                }
                else{
                    Div = Div/atoi(tokenword);
                    tokenword = strtok(NULL," ");
                } 
            }
            else{
                count++;
                Div = atoi(tokenword);
                tokenword = strtok(NULL," ");
            }
        }
        int sumlen = sprintf(finalDiv,"Answer: %f \n",Div);
        return finalDiv;
    }
}