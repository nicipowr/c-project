#include <dirent.h>
#include <stdio.h>
int main() {
  DIR *dir;
  struct dirent *de;

  dir = opendir("/proc");
  while (NULL != (de = readdir(dir)))
    printf ("%s\n", de -> d_name);
  closedir(dir);
}
