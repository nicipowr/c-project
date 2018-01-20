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

char *trimwhitespace(char *str) {
  char *end;

  while (isspace((unsigned char) *str)) str++;

  if (*str == 0)  // All spaces?
    return str;

  end = str + strlen(str) - 1;
  while (end > str && isspace((unsigned char) *end)) end--;

  *(end + 1) = 0;

  return str;
}

char *getPathToPID(char *name) {
  char *filePath = malloc(12);
  strcpy(filePath, "/proc/");
  strcat(filePath, name);
  return filePath;
}

int main(void) {
  DIR *dp;
  struct dirent *ep;
  struct stat statbuf;

  dp = opendir("/proc");
  if (dp != NULL) {
    bool numbers_only(char charac[]) {
      int i = 0;
      while (charac[i]) {
	if (isdigit(charac[i++]) == 0) {
	  return false;
	}
      }
      return true;
    }

    puts("PID\tCommand\tRSS");

    char *pathToStat;
    char *pathToStatus;
    FILE *fp;
    int ret;
    long int rss = 0;

    while (ep = readdir(dp)) {
      //stat(ep->d_name,&statbuf);
      if (numbers_only(ep->d_name)) {
	char *pathToPID = getPathToPID(ep->d_name);

	pathToStat = pathToPID;
	strcat(pathToStat, "/stat");
	fp = fopen(pathToStat, "r");
	ret = fscanf(fp, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %*u %*u %*d %*d %*d %*d %*d %*d %*u %*u %ld", &rss);
	fclose(fp);

	pathToStatus = pathToPID;
	strcat(pathToStatus, "/status");
	fp = fopen(pathToStatus, "r");
	free(path);

	char str[250];
	char name[256] = ""; //assignment needed?

	while (fgets(str, 250, (FILE *)fp) != NULL) {
	  if (strstr(str, "Name:")) {
	    char *foo = trimwhitespace(str);
	    foo += 6;
	    //*str += 5;
	    strcat(name, foo);
	    //printf("%s ", ep->d_name);
	    //printf("%s \n",name);
	  }

	  if (strstr(str, "Uid:")) {

	    char *n = trimwhitespace(str);

	    int tmpstor;
	    char tmpstruid[50];
	    char finalstruid[4];
	    char tmpuid[50];

	    while (*n) { // While there are more characters to process...
	      if (isdigit(*n)) { // Upon finding a digit, ...
		long val2 = strtol(n, &n, 10); // Read a number, ...
		tmpstor = val2;
	      } else { // Otherwise, move on to the next character.
		n++;
	      }
	    }
	    sprintf(tmpstruid, "%d", tmpstor);
	    strncpy(finalstruid,tmpstruid,4);
	    finalstruid[4]=0;
	    sprintf(tmpuid, "%d", getuid());
	    //printf("%s  %s\n",finalstruid,tmpuid);
	    if (strcmp(finalstruid,tmpuid)==0) {
	      printf("%s\t", ep->d_name);
	      printf("%s\t", name);

	      printf("%d\n", ret);
	      // name[sizeof(name) - 1] = "\0";
	      // strncpy(name, name, sizeof(name) - 1);
	      //printf("%ld\n", rss);
	    }
	  }


	}
	fclose(fp);

	//if (statbuf.st_uid == getuid()) {

      }
    }
    (void) closedir(dp);


  } else
    perror("Couldn't open the directory.");

  return 0;
}
