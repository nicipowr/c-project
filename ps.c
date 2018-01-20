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

char *trimwhitespace(char *str)
{
  char *end;

  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  *(end+1) = 0;

  return str;
}

int main(void) {
  DIR *dp;
  FILE *fp;
  struct dirent *ep;
  struct stat statbuf;
  char *filepathtostatus;
  char name[10];

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

    while (ep = readdir(dp)) {
      //stat(ep->d_name,&statbuf);
      if (numbers_only(ep->d_name)) {
	char *filepathtostatus = malloc(30);
	strcat(filepathtostatus, "/proc/");
	strcat(filepathtostatus, ep->d_name);
	strcat(filepathtostatus, "/status");

	if ((fp = fopen(filepathtostatus,"r")) != NULL) {
	  int vmrss = 0;

	  fscanf(fp, "Name:\t%s", name);
	  //fscanf(fp, "Name:\t%s", name);
	  //*str += 5;

	  printf("%s %s\n", ep->d_name, name);
	  //printf("%s ", ep->d_name);
	  //printf("%s \n",name);
	      
	  /*
	    if (strstr(str,"VmRSS:")) {
	    char *p = str;
	    while (*p) { // While there are more characters to process...
	    if (isdigit(*p)) { // Upon finding a digit, ...
	    long val = strtol(p, &p, 10); // Read a number, ...
	    vmrss = val;
	    } else { // Otherwise, move on to the next character.
	    p++;
	    }
	    }
	    //char *foo = trimwhitespace(str);
	    //foo += 7;
	    //strcat()
	    printf("%d\n",vmrss);
	    }
	  */
	  //if (strstr(str,"Uid:")){

	  //  if (getuid() == )

	  //}

	  //if (statbuf.st_uid == getuid()) {
	} else
	  printf("Couldn't open the file.");
	fclose(fp);	
	free(filepathtostatus);
      }
    }
    closedir(dp);


  } else
    printf("Couldn't open the directory.");

  return 0;
}
