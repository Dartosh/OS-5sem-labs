#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

pid_t createChild(int(*child_fn)()) {
    pid_t childPID = fork();
    switch(childPID) {
        case 0:
            exit(child_fn());
        case -1:
            abort();
        default:
            printf("%d create %d\n", getpid(), childPID);
            return childPID;
    }
}

void deleteChildP(){
    int status;
    pid_t childPID = wait(&status);
    if(childPID == -1)
        abort();
    else
        printf("Child process %d with parent %d was destroyed \n", childPID, getpid());
}

int p7(){return 0;}

int p6(){return 0;}

int p5(){
    createChild(p6);
    createChild(p7);
    deleteChildP();
    deleteChildP();
    return 0;
}

int p4(){return 0;}

int p3(){return 0;}

int p2(){
    createChild(p4);
    createChild(p5);
    deleteChildP();
    deleteChildP();
    return 0;
}

int p1() {
    printf("%d started\n", getpid());
    createChild(p2);
    createChild(p3);
    deleteChildP();
    deleteChildP();
    execl("/bin/date", "date", NULL);
    return 0;
}

int main()
{
    return p1();
}