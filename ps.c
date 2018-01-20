#include <ctype.h>
#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated. The return
// value must NOT be deallocated using free() etc.
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

char *getStatusFilePath(char *name) {
  char *filePath = malloc(12);
  strcpy(filePath, "/proc/");
  strcat(filePath, name);
  strcat(filePath, "/status");
  return filePath;
}

int getNumber(const char *str) {
  int number;
  
  while (*str) {
    if (sscanf(str, "%d", &number) == 1)
      return number;
    str++;
  }
  
  // No int found
  return -1;
}

int main(void) {
  DIR *dp;
  struct dirent *ep;
  struct stat statbuf;

  dp = opendir("/proc");
  if (dp != NULL) {
    FILE *fp;
    bool myfile;
    char finalstruid[4], name[256], str[250], tmpstruid[50], tmpuid[50];
    int tmpstor, vmrss = 0;

    bool numbers_only(char charac[]) {
      int i = 0;
      while (charac[i])
	if (isdigit(charac[i++]) == 0)
	  return false;
      return true;
    }
    
    while (ep = readdir(dp)) {
      if (numbers_only(ep->d_name)) {
	myfile = false;
	char *path = getStatusFilePath(ep->d_name);
	fp = fopen(path, "r");
	free(path);

	while (fgets(str, 250, fp) != NULL) {	  
	  if (strstr(str, "Name:")) {
	    char *foo = trimwhitespace(str);
	    foo += 6;
	    name[0] = 0;
	    strcat(name, foo);
	  }
	  if (strstr(str, "VmRSS:")) {
	    vmrss = getNumber(str);
	    if (myfile)
	      printf("%5s %-15s %5d\n", ep->d_name, name, vmrss);
	  }

	  if (strstr(str, "Uid:")) {
	    char *n = trimwhitespace(str);
	    
	    tmpstor = getNumber(str);
	    sprintf(tmpstruid, "%d", tmpstor);
	    strncpy(finalstruid, tmpstruid, 4);
	    finalstruid[4] = 0;
	    sprintf(tmpuid, "%d", getuid());
	    if (strcmp(finalstruid, tmpuid) == 0)
	      myfile = true;
	  }
	}
      }
    }
    closedir(dp);

  } else
    perror("Couldn't open the directory.");

  return 0;
}
