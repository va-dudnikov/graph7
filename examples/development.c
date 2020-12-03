#include <stdio.h>
#include <errno.h>
#include <graph7/errno.h>

int main()
{
    printf("%s\n", strerror(10));
}
