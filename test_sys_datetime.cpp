#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
   time_t givemetime = time(NULL);
   printf("%s", ctime(&givemetime)); //ctime() returns given time
   return 0;
}
