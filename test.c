#include "shell.h"
int main(void)
{
char str[20] = "a b - c - d e - f";
const char s[2] = "-";
char *t;

t = _strtok(str, s);

while( t != NULL)
{
printf("%s\n", t);
t = _strtok(NULL, s);
}
return (0);
}
