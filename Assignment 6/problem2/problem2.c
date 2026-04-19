#include<stdio.h>
#include<assert.h>

void main(int arc, char **argv){
char inp[5];
char *ptr =fgets(inp, sizeof inp, stdin);
 if(inp[0] == 'b'){
   if(inp[1] == 'a'){
     if(inp[2] == 'd'){
       if(inp[3] == '!'){
         assert(0);
       }
     }
   }
 }
return;
}