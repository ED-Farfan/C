#include <stdio.h>
#include <time.h>

int main ()
{
   time_t seconds;
   struct tm *timeinfo;
   struct tm start, end;
   double d;

   // Get the current time value.
   seconds = time (NULL);
   // Convert seconds into a struct tm.
   timeinfo = localtime (&seconds );

   // Copy the timeinfo elements to start and end.
   start = *timeinfo;
   end = *timeinfo;
   // Change the date for start.
   start.tm_mon = 1;
   start.tm_mday = 23;
   start.tm_hour = 20;
   start.tm_min = 34;
   start.tm_sec = 21;
   // Change the date for end.
   end.tm_mon = 4;
   end.tm_mday = 23;
   end.tm_hour = 20;
   end.tm_min = 34;
   end.tm_sec = 45;
   // Compute the difference in seconds from start and end.
   d = difftime(mktime(&start),mktime(&end));
   // Print the difference.
	printf("%.2f", d);

	return 0;
}
