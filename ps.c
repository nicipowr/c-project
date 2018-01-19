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

        puts("PID COMMAND RSS");

        while (ep = readdir(dp)) {
            //stat(ep->d_name,&statbuf);
            if (numbers_only(ep->d_name)) {
                char *filepathtostatus = malloc(30);
                strcat(filepathtostatus, "/proc/");
                strcat(filepathtostatus, ep->d_name);
                strcat(filepathtostatus, "/status");

                fp = fopen(filepathtostatus, "r");
                free(filepathtostatus);

                char str[250];
                int vmrss = 0;
                char name[256] = "";

                while (fgets(str,250,fp) !=NULL ){
                    if (strstr(str,"Name:")){
                        char *foo = trimwhitespace(str);
                        foo += 6;
                        //*str += 5;
                        strcat(name,foo);
                        //printf("%s ", ep->d_name);
                        //printf("%s \n",name);
                    }
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
                        printf("%s ", ep->d_name);
                        printf("%s\t",name);
                        printf("%d\n",vmrss);
                    }
                    //if (strstr(str,"Uid:")){

                      //  if (getuid() == )

                    //} 




                }


                //if (statbuf.st_uid == getuid()) {

            }
        }
        (void) closedir(dp);


    } else
        perror("Couldn't open the directory.");

    return 0;
}
