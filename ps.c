#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <string.h>
#include <ctype.h>

int main(void) {
  DIR *dp;
  FILE *fp;
  struct dirent *ep;
  struct stat statbuf;
  char *filepathtostat;

  if ((dp = opendir("/proc")) != NULL) {
    bool numbers_only(char charac[]) {
      int i = 0;
      while (charac[i]) {
	if (isdigit(charac[i++]) == 0)
	  return false;
      }
    return true;
    }

    puts("PID Name");
    
    char name[10];
    long int rss;
    
    while (ep = readdir(dp)) {
      //stat(ep->d_name,&statbuf);
      if (numbers_only(ep->d_name)) {
	char *filepathtostat = malloc(30);
	strcat(filepathtostat, "/proc/");
	strcat(filepathtostat, ep->d_name);
	strcat(filepathtostat, "/stat");

	if ((fp = fopen(filepathtostat,"r")) != NULL) {
	  fscanf(fp, "%*d %s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %*u %ld", name, &rss);
	  // name[sizeof(name) - 1] = "\0";
	  // strncpy(name, name, sizeof(name) - 1);
	  printf("%s %s %ld\n", ep->d_name, name, rss);
	} else
	  printf("Couldn't open the file.");
	fclose(fp);	
	free(filepathtostat);
      }
    }
    closedir(dp);


  } else
    printf("Couldn't open the directory.");

  return 0;
}
