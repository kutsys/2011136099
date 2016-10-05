#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>

void printdir(char* dir, int depth) {
    DIR* dp;
    struct dirent* entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr, "cannot open directory: %s\n", dir);
        return;
    }
    chdir(dir);
    while((entry = readdir(dp)) != NULL) {
        lstat(entry->d_name, &statbuf);
        if(S_ISDIR(statbuf.st_mode)) {
            /* 디렉터리를 찾았음. .와 ..는 무시한다. */
            if('.' == *(entry->d_name))
                continue;
            printf("%s\n",entry->d_name);
            if(strcmp(".",entry->d_name) == 0 ||
                        strcmp("..",entry->d_name) == 0)
                continue;
            printf("%*s%s/\n", depth, "", entry->d_name);
            /* 새로운 들여쓰기 수준을 지정해서 재귀 호출*/
            printdir(entry->d_name,depth+4);
        }
        else if('.' != *(entry->d_name))
            printf("%*s%s\n",depth,"",entry->d_name);
        else
            continue;
    }
    chdir("..");
    closedir(dp);
}

int main() {
    printf("Directory scan of /home:\n");
    printdir("/home", 0);
    printf("done.\n");

    exit(0);
}
