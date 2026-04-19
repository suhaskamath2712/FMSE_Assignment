#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#define MAXNAME 30
int crackaddr(char *addr)
{
	register char *p;
    register char *bp;
	register char c;
	char buf[MAXNAME + 1];
	int realanglelev = 0;
	/* strip leading spaces */
	assert(addr != NULL);
	while (*addr != '\0' && isascii((int)*addr) && isspace((int)*addr))
		addr++;
    bp = buf;
	p =  addr;

	while ((c = *p++) != '\0')
	{
		if (isalnum(c) || c == '@' || c == '.')  {
			/* ensure we have room for another char plus terminating NUL */
			assert(bp < buf + MAXNAME);
			*bp++ = c;
		}
		if (c == '<') {
			realanglelev++;
			assert(bp < buf + MAXNAME);
			*bp++ = c;
		}
		if (c == '>') {
			realanglelev--;
			assert(bp < buf + MAXNAME);
			*bp++ = c;
		}
	}
	while (realanglelev-- > 0){
	  assert(bp < buf + MAXNAME);
	  *bp++ = '>';
	}
	/* allow writing terminating NUL at buf[MAXNAME] */
	assert(bp <= buf + MAXNAME);
	*bp++ = '\0';
	return bp - buf;
}

int main(){

  char address[40];
  int  addrl;
  /* Get email address */
  char *ptr = fgets(address,sizeof address,stdin);
  assert(ptr != NULL);
  addrl = crackaddr(address);
  return 0;
}