#include <stdio.h>
#include <time.h>

int main () 
{
   time_t seconds;

   seconds = time(NULL);
   printf("Hours since January 1, 1970 = %ld\n", seconds/3600);
   printf("%s", ctime(&seconds));
  
   return(0);
}
