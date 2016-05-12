#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) 
{
  struct stat buf;
  struct passwd *passwd;
  char *file, *home;
  uid_t uid; 
  gid_t gid;
  
  passwd = getpwuid(getuid());
  uid = getuid();
  gid = getgid();

  int k;
  for (k = 1; k < argc; k++) {
    // File is absolute
    if(argv[k][0] == '/') {
      file = malloc(sizeof(argv[k]));
      file = strdup(argv[k]);
    }
    // File is relative 
    else {
      home = passwd->pw_dir;
      //concat the homedirectory to argv[k]
      file = malloc(sizeof(home) + 1 + sizeof(argv[k]));
      strcat(file, home);
      strcat(file, "/");
      strcat(file, argv[k]);      
    }
    /* fetch inode information */
    if (stat(file, &buf) == (-1)) {
      fprintf(stderr, "%s: cannot access %s\n",
                       argv[0], argv[k]);
      exit(1);
    }

    printf("%s\n", file);

    /* print file permissions */
    if(uid == buf.st_uid) {
      printf("File owner:\n"); 
      if(buf.st_mode & 0400) {
        printf("\tRead permission.\n");
      }
      if(buf.st_mode & 0200) {
        printf("\tWrite permission.\n");
      }
      if(buf.st_mode & 0100) {
        printf("\tExecute permission.\n");
      }
    }
    else if(gid == buf.st_gid) {
      printf("Group:\n");
      if(buf.st_mode & 0040) {
        printf("\tRead permission.\n");
      }
      if(buf.st_mode & 0020) {
        printf("\tWrite permission.\n");
      }
      if(buf.st_mode & 0010) {
        printf("\tExecute permission.\n");
      }

    }
    else {
      printf("Other\n");
      if(buf.st_mode & 0004) {
        printf("\tRead permission.\n");
      }
      if(buf.st_mode & 0002) {
        printf("\tWrite permission.\n");
      }
      if(buf.st_mode & 0001) {
        printf("\tExecute permission.\n");
      }
    }    
  }
  free(file);
  exit(0);
}
