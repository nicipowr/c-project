#include <dirent.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

int
main (void) {
  DIR *dp;
  struct dirent *ep;
  dp = opendir ("/proc");
  if (dp != NULL) {
    bool
    numbers_only (char charac[])
    {
      int i = 0;
      while (charac[i]) {
	if (isdigit (charac[i++]) == 0) { 
	  return false;
	}
      }

      return true;
    }

    puts ("PID");
    while (ep = readdir (dp))
      if (numbers_only (ep->d_name)) puts (ep->d_name);
    (void) closedir (dp);
  }
  else
    perror ("Couldn't open the directory.");

  return 0;
}
