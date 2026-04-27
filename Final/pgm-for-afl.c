#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

int main() {
  char inp[500];
  char *ptr =fgets(inp, sizeof inp, stdin);
  int i=0; 
  int ac = 0;
  do {
     if (ptr[i] == 'a')
       ac = ac + 1;
     i = i + 1;
  } while(ptr[i] != 0);
  if (ac > 10)
    printf("More than 10 a's seen");
  return 0; 
}
