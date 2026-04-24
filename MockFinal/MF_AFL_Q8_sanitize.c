#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define MAXTOK 24

int sanitize_tag(char *s)
{
    char buf[MAXTOK + 1];
    char *bp = buf;
    char c;
    int bracket = 0;

    /* strip leading whitespace */
    while (*s != '\0' && isascii((int)*s) && isspace((int)*s))
        s++;

    while ((c = *s++) != '\0')
    {
        if (isalnum((unsigned char)c) || c == '_' || c == '-')
            *bp++ = c;

        if (c == '[')
        {
            bracket++;
            *bp++ = c;
        }
        if (c == ']')
        {
            bracket--;
            *bp++ = c;
        }
    }

    while (bracket-- > 0)
        *bp++ = ']';

    *bp++ = '\0';
    return (int)(bp - buf);
}

int main()
{
    char in[80];
    char *ptr = fgets(in, sizeof in, stdin);
    assert(ptr != NULL);

    sanitize_tag(in);
    return 0;
}
