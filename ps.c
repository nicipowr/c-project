#include <dirent.h>
#include <stdio.h>

int
main (void)
{
  DIR *dp;
  struct dirent *ep;
  
  dp = opendir ("/proc");
  if (dp != NULL)
    {
      while (ep = readdir (dp))
        puts (ep->d_name);
      (void) closedir (dp);
    }
  else
    perror ("Couldn't open the directory.");

  return 0;
}
