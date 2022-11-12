//Print with sprintf and write
//0 is stdin, 1 is stdout, 2 is stderr

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

void printArgs(char* writeBuf){
   write(2, writeBuf, strlen(writeBuf));
}

int main(int argc, char** argv){
   char writeBuf[1024];
   int perms = 0600;
   int c2p1[2], c2p2[2], p2c0[2];

   int directory, file0, file1, file2;
   ssize_t readOut;

   //Checking general arg count
   if(argc < 3){
      sprintf(writeBuf, "Incorrect arguments! Usage: hscript <program name> <arguments> <directory>\n");
      write(2, writeBuf, strlen(writeBuf));
      exit(1);
   }

   //Create/open the directory
   directory = mkdir(argv[argc-1], 0700);

   //Create/open 0, recreate if exists already
   //Fail if folder exists as a file already
   sprintf(writeBuf, "./%s/0", argv[argc-1]);
   file0 = open(writeBuf, O_WRONLY|O_CREAT|O_TRUNC, perms);
   if(file0 == -1){
         sprintf(writeBuf, "An error occured when opening/creating file0.\n");
         write(2, writeBuf, strlen(writeBuf));
         exit(2);
   }

   //Create/open 1, recreate if exists already
   sprintf(writeBuf, "./%s/1", argv[argc-1]);
   file1 = open(writeBuf, O_WRONLY|O_CREAT|O_TRUNC, perms);
   if(file1 == -1){
         sprintf(writeBuf, "An error occured when opening/creating file1.\n");
         write(2, writeBuf, strlen(writeBuf));
         exit(3);
   }

   //Create/open 2, recreate if exists already
   sprintf(writeBuf, "./%s/2", argv[argc-1]);
   file2 = open(writeBuf, O_WRONLY|O_CREAT|O_TRUNC, perms);
   if(file2 == -1){
         sprintf(writeBuf, "An error occured when opening/creating file2.\n");
         write(2, writeBuf, strlen(writeBuf));
         exit(4);
   }

   //Prints all the arguments for the internal just for verification
   //Preps the list for execvp
   /*argList = malloc(argc-2);
   for(int i = 1; i < argc - 1; i++){
      argList[i-1] = argv[i];
   }
   for(int i = 0; i < argc - 2; i++){
      sprintf(writeBuf, "%s\n", argList[i]);
      printArgs(writeBuf);
   }*/

   pipe(p2c0);
   pipe(c2p1);
   pipe(c2p2);

   if(fork()==0){
      //Close the read side of stderr and stdout for child
      close(c2p1[0]);
      close(c2p2[0]);

      //Close the write side of stdin for child
      close(p2c0[1]);

      //Overwrite stdin, stdout, stderr with pipes
      dup2(p2c0[0],0);
      dup2(c2p1[1],1);
      dup2(c2p2[1],2);

      //Close the rest of the pipes now
      close(p2c0[0]);
      close(c2p1[1]);
      close(c2p2[1]);

      argv[argc - 1] = 0;
      return execv(argv[1], &argv[1]);
   }

   //Close the write side of stderr and stdout for parent
   close(c2p1[1]);
   close(c2p2[1]);

   //Close the read side of the child's stdin pipe
   close(p2c0[0]);

   fd_set write_fds;
   fd_set read_fds;

   readOut = 1;
   while (readOut > 0) {

      // clear write fd_set
      FD_ZERO(&write_fds);
      // child input
      FD_SET(p2c0[1], &write_fds);

      // clear read fd_set
      FD_ZERO(&read_fds);
      // process stdin
      FD_SET(0, &read_fds);
      // child stdout
      FD_SET(c2p1[0], &read_fds);
      // child stderr
      FD_SET(c2p2[0], &read_fds);

      if (select(c2p2[0] + 1, &read_fds, &write_fds, NULL, NULL) > 0) {
            if (FD_ISSET(c2p1[0], &read_fds)){
            readOut = read(c2p1[0], writeBuf, sizeof(writeBuf));
            write(1, writeBuf, readOut);
            write(file1, writeBuf, readOut);
         }

         if (FD_ISSET(c2p2[0], &read_fds)) {
            readOut = read(c2p2[0], writeBuf, sizeof(writeBuf));
            write(2, writeBuf, readOut);
            write(file2, writeBuf, readOut);
         }

         if(FD_ISSET(p2c0[1], &write_fds) && FD_ISSET(0, &read_fds)){
            readOut = read(0, writeBuf, sizeof(writeBuf));
            write(file0, writeBuf, readOut);
            write(p2c0[1], writeBuf, readOut);
            write(1, writeBuf, strlen(writeBuf));
         }
      }
   }

   wait(NULL);

   return 0;
}