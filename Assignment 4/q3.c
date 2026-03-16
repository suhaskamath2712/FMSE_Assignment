//Suhas Kamath
//25945

#include <vcc.h>
#include <limits.h>

unsigned int unum;

_(ghost \integer intval)

#define INV (unum <= 255 && intval >= -128 && intval <= 127)
#define GLUE (unum <= 127 ? (intval == (int)unum) : (intval == (int)unum - 256))

void init()
    _(writes &unum, &intval)
{
  unum = 0;
  _(ghost intval = 0;)
}

void inc()
    _(writes &unum, &intval)
    _(requires INV && GLUE)
    _(ensures INV && GLUE)
{
    if (unum == 255)
        unum = 0;
    else
        unum++;

    _(ghost intval = (intval == 127) ? -128 : intval + 1;)
}

void dec()
    _(writes &unum, &intval)
    _(requires INV && GLUE)
    _(ensures INV && GLUE)
{
    if (unum == 0)
        unum = 255;
    else
        unum--;

    _(ghost intval = (intval == -128) ? 127 : intval - 1;)
}