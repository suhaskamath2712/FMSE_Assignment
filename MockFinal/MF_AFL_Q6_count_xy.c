#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int arc, char **argv){

  int i,cx=0,cy=0;
  char inp[500];
  char *ptr =fgets(inp, sizeof inp, stdin);
  if (ptr == NULL) return 0;

  i=0;
  while(inp[i] !='\0'){
    if(inp[i] == 'x')
      cx++;
    if(inp[i] == 'y')
      cy++;
    i++;
  }

  return cx+cy;
}
