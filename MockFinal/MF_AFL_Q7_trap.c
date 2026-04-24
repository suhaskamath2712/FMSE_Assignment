#include<stdio.h>
#include<assert.h>

void main(int arc, char **argv){
  char inp[8];
  char *ptr =fgets(inp, sizeof inp, stdin);

  if(inp[0] == 'm'){
    if(inp[1] == 'o'){
      if(inp[2] == 'c'){
        if(inp[3] == 'k'){
          assert(0);
        }
      }
    }
  }

  return;
}
