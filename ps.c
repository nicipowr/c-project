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

char *getStatusFilePath(char *name)
{
    char *filePath = malloc(12);
    strcpy(filePath, "/proc/");
    strcat(filePath, name);
    strcat(filePath, "/status");
    return filePath;
}

int GetNumber(const char *str) {
    while (*str) {
        int number;
        if (sscanf(str, "%d", &number) == 1) {
            return number;
        }
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
        bool numbers_only(char charac[]) {
            int i = 0;
            while (charac[i]) {
                if (isdigit(charac[i++]) == 0) {
                    return false;
                }
            }
            return true;
        }

        //puts("PID\tCOMMAND\tRSS");

        while (ep = readdir(dp)) {
            //stat(ep->d_name,&statbuf);
            if (numbers_only(ep->d_name)) {
                FILE *fp;
                char *path = getStatusFilePath(ep->d_name);
                fp = fopen(path, "r");
                free(path);

                char str[250];
                int vmrss = 0;
                char name[256] = "";
                bool myfile = false;
                while (fgets(str, 250, (FILE *)fp) != NULL) {

                    if (strstr(str, "Name:")) {
                        char *foo = trimwhitespace(str);
                        foo += 6;
                        //*str += 5;
                        strcat(name, foo);
                        //printf("%s ", ep->d_name);
                        //printf("%s \n",name);
                    }
                    if (strstr(str, "VmRSS:")) {
                        /*char *p = str;
                        while (*p) { // While there are more characters to process...
                            if (isdigit(*p)) { // Upon finding a digit, ...
                                long val = strtol(p, &p, 10); // Read a number, ...
                                if (val != 0){vmrss = val;}
                            } else { // Otherwise, move on to the next character.
                                p++;
                            }
                        }*/
                        vmrss = GetNumber(str);
                        if (myfile ){
                            printf("%s\t", ep->d_name);
                            printf("%s\t", name);
                            printf("%d\n", vmrss);
                        }
                        //char *foo = trimwhitespace(str);
                        //foo += 7;
                        //strcat()

                    }

                    if (strstr(str, "Uid:")) {

                        char *n = trimwhitespace(str);

                        int tmpstor;
                        char tmpstruid[50];
                        char finalstruid[4];
                        char tmpuid[50];

                        /*while (*n) { // While there are more characters to process...
                            if (isdigit(*n)) { // Upon finding a digit, ...
                                long val2 = strtol(n, &n, 10); // Read a number, ...
                                tmpstor = val2;
                            } else { // Otherwise, move on to the next character.
                                n++;
                            }
                        }*/
                        tmpstor = GetNumber(str);

                        sprintf(tmpstruid, "%d", tmpstor);
                        strncpy(finalstruid,tmpstruid,4);
                        finalstruid[4]=0;
                        sprintf(tmpuid, "%d", getuid());
                        //printf("%s  %s\n",finalstruid,tmpuid);
                        if (strcmp(finalstruid,tmpuid)==0){
                            myfile = true;
                        }
                    }


                }


                //if (statbuf.st_uid == getuid()) {

            }
        }
        (void) closedir(dp);


    } else
        perror("Couldn't open the directory.");

    return 0;
}