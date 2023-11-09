#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>



#define LOOP_COUNT 25
void ChildProcess(int []);
void ParentProcess(int []);



int main(int argc, char *argv[]) {
    int ShmID;
    int *ShmPTR;
    pid_t pid;
    int status;

    // Initialize variables with default values of 0
    int initialBankAccountValue = 0;
    int initialTurnValue = 0;
    // Update variables based on provided arguments
    if (argc > 1) {
        initialBankAccountValue = atoi(argv[1]);
    }
    if (argc > 2) {
        initialTurnValue = atoi(argv[2]);
    }
    // Print the initial values
    printf("Initial bank account value: %d\n", initialBankAccountValue);
    printf("Initial turn value: %d\n", initialTurnValue);
    printf("\n");
    


    // Create shared memory segment for two integers
    ShmID = shmget(IPC_PRIVATE, 2 * sizeof(int), IPC_CREAT | 0666);
    if (ShmID < 0) {
        printf("*** shmget error (server) ***\n");
        exit(1);
    }
    printf("Process has received a shared memory of two integers...\n");



    // Attach to the shared memory segment
    ShmPTR = (int *)shmat(ShmID, NULL, 0);
    if (*ShmPTR == -1) {
        printf("*** shmat error (server) ***\n");
        exit(1);
    }
    printf("Process has attached the shared memory...\n");



    // Initialize BankAccount and Turn using command line arguments
    ShmPTR[0] = initialBankAccountValue; // BankAccount
    ShmPTR[1] = initialTurnValue; // Turn
    printf("Bank initialized to: %d\n", ShmPTR[0]);


    // Fork a child process
    printf("Bank is about to fork a child process...\n");
    pid = fork();
    if (pid < 0) {
        printf("*** fork error (server) ***\n");
        exit(1);
    } else if (pid == 0) {
        ChildProcess(ShmPTR);
        exit(0);
    }
    // Parent Process (Dear Old Dad)
    ParentProcess(ShmPTR);



    // Wait for the child process to complete
    wait(&status);
    printf("Server has detected the completion of its child...\n");



    // Detach and remove shared memory
    shmdt((void *) ShmPTR);
    printf("Process has detached its shared memory...\n");
    shmctl(ShmID, IPC_RMID, NULL);
    printf("Process has removed its shared memory...\n");
    printf("Process exits...\n");
    exit(0);
}




// Child Process (Poor Student)
void ChildProcess(int SharedMem[]) {
    printf("   Child process started\n");
    srand(time(NULL) + 1); // Seed the random number generator with a different value

    for(int i = 0; i < LOOP_COUNT; i++) {
        sleep(rand() % 6); // Sleep for a random time between 0-5 seconds
        int account = SharedMem[0];
        while(SharedMem[1] != 1); // Wait for turn to be 1

        int balance = rand() % 51; // Generate a random amount needed between 0-50
        printf("Poor Student needs $%d\n", balance);
        
        if (balance <= account) {
            account -= balance; // Withdraw money
            printf("Poor Student: Withdraws $%d / Balance = $%d\n", balance, account);
        } else {
            printf("Poor Student: Not Enough Cash ($%d)\n", account);
        }

        SharedMem[0] = account; // Update BankAccount
        SharedMem[1] = 0; // Set Turn to 0
    }

    printf("   Child process is about to exit\n");
}



// Parent Process (Dear Old Dad)
void ParentProcess(int SharedMem[]) {
    srand(time(NULL)); // Seed the random number generator

    for(int i = 0; i < LOOP_COUNT; i++) {
        sleep(rand() % 6); // Sleep for a random time between 0-5 seconds
        int account = SharedMem[0];
        while(SharedMem[1] != 0); // Wait for turn to be 0
        if (account <= 100) {
            int balance = rand() % 101; // Generate a random amount between 0-100
            if (balance % 2 == 0) {
                account += balance; // Deposit money
                printf("Dear old Dad: Deposits $%d / Balance = $%d\n", balance, account);
            } else {
                printf("Dear old Dad: Doesn't have any money to give\n");
            }
        } else {
            printf("Dear old Dad: Thinks Student has enough Cash ($%d)\n", account);
        }
        SharedMem[0] = account; // Update BankAccount
        SharedMem[1] = 1; // Set Turn to 1
    }
}

