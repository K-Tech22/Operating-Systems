#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <wait.h>
#include <assert.h>
#include <fcntl.h>
#include <iostream>

#define MAX_LINE 80

using namespace std;

// This function executes commands with ';'
void exec_semicol(int cmdsize, char *args[], int specialcharIndex) {
    char *before[MAX_LINE / 2 + 1];
    char *after[MAX_LINE / 2 + 1];

    // Initializing all elements to NULL
    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        before[i] = NULL;
        after[i] = NULL;
    }
    // Iterating args array for cmd tokens
    for (int i = 0; i < cmdsize; i++) {
        if (i < specialcharIndex) {
            before[i] = args[i];
        } 
        else if (i > specialcharIndex) {
            after[i - (specialcharIndex + 1)] = args[i];
        }
    }
    // Forking new process
    int pid = fork();

    // Child process
    if (pid == 0) {
        int writecmd = execvp(before[0], before);
        return;
    }
    // Parent process
    else {
        wait(NULL);
    }
    // Forking another process
    pid = fork();
    if (pid == 0) {
        int writecmd = execvp(after[0], after);
        return;
    } 
    else {
        wait(NULL);
    }
}

// This function executes the commands with '&'
void exec_amp(int cmdsize, char *args[], int specialcharIndex) {

    char *before[MAX_LINE / 2 + 1];
    char *after[MAX_LINE / 2 + 1];

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        before[i] = NULL;
        after[i] = NULL;
    }
    for (int i = 0; i < cmdsize; i++) {

        if (i < specialcharIndex) {
            before[i] = args[i];
        } 

        else if (i > specialcharIndex) {
            after[i - (specialcharIndex + 1)] = args[i];
        }
    }

    int pid = fork();

    if (pid == 0) {
        int writecmd = execvp(before[0], before);
        return;
    }
    else {
        int pid2 = fork();
        if(pid2 == 0) {
            int writecmd_two = execvp(after[0], after);
            return;
        } 
        else {
            wait(NULL);
        }
        wait(NULL);
    }
}
// This function executes the commands with '|'
void exec_pipe(int cmdsize, char *args[], int specialcharIndex) {
    
    char *before[MAX_LINE / 2 + 1];
    char *after[MAX_LINE / 2 + 1];

    for (int i = 0; i < MAX_LINE / 2 + 1; i++)
    {
        before[i] = NULL;
        after[i] = NULL;
    }

    for (int i = 0; i < cmdsize; i++)
    {
        if (i < specialcharIndex) {
            before[i] = args[i];
        } 
        
        else if (i > specialcharIndex) {
            after[i - (specialcharIndex + 1)] = args[i];
        }
    }
    // File descriptors
    int pipe_fd[2];

    // Creating pipe
    pipe(pipe_fd);

    int pid = fork();
    // Child process
    if (pid == 0)
    {
        // To write
        dup2(pipe_fd[1], 1);
        // Closing read
        close(pipe_fd[0]);
        // Closing write
        close(pipe_fd[1]);

        int writecmd = execvp(before[0], before);
        return;
    }
    // Parent
    else {
        int pid2 = fork();
        // Child
        if(pid2 == 0){
            dup2(pipe_fd[0], 0);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            int writecmd_two = execvp(after[0], after);
            return;
        }
        // Parent
        else {
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            wait(NULL);
        }
        wait(NULL);
    }
}

// This function executes the command with '<'
void exec_inp_redr(int cmdsize, char *args[], int specialcharIndex) {
    char *shell_cmd[MAX_LINE / 2 + 1];
            char *file_source[MAX_LINE / 2 + 1];

            for (int i = 0; i < MAX_LINE / 2 + 1; i++)
            {
                shell_cmd[i] = NULL;
                file_source[i] = NULL;
            }

            for (int i = 0; i < cmdsize; i++) {

                if (i < specialcharIndex) {
                    shell_cmd[i] = args[i];
                } 
                
                else if (i > specialcharIndex) {
                    file_source[i - (specialcharIndex + 1)] = args[i];
                    shell_cmd[i - 1] = args[i];
                }
            }

            int pid = fork();

            if(pid == 0){

                int fd = open(file_source[0], O_RDONLY);

                dup2(0, fd);
                close(fd);

                int writecmd = execvp(shell_cmd[0], shell_cmd);
                return;
            } 
            else {
                wait(NULL);
            }
}
// This function executes the command with '>'
void exec_out_redr(int cmdsize, char *args[], int specialcharIndex) {
    char *before[MAX_LINE / 2 + 1];
            char *after[MAX_LINE / 2 + 1];

            for (int i = 0; i < MAX_LINE / 2 + 1; i++)
            {
                before[i] = NULL;
                after[i] = NULL;
            }

            for (int i = 0; i < cmdsize; i++) {

                if (i < specialcharIndex) {
                    before[i] = args[i];
                } 
                
                else if (i > specialcharIndex) {
                    after[i - (specialcharIndex + 1)] = args[i];
                }
            }
            int pid = fork();

            if(pid == 0) {

                int fd = open(after[0], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

                dup2(fd, 1);
                dup2(fd, 2);
                close(fd);

                int writecmd = execvp(before[0], before);
                return;
            } 
            
            else {
                wait(NULL);
            }
        
}

int main(void) {
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;

    char cmd[MAX_LINE / 2 + 1];
    char history[MAX_LINE / 2 + 1];

    while (should_run) {
        printf("osh> ");
        cin.getline(cmd, MAX_LINE);
        fflush(stdout);

        for (int i = 0; i < MAX_LINE / 2 + 1; i++)
        {
            args[i] = NULL;
        }
        if (strcmp(cmd, "exit") == 0) {
            should_run = 0;
        }
        // Shows history
        if (strcmp(cmd, "!!") == 0) {

            if (strcmp(history, "") == 0) {
                printf("History is empty\n");
            }

            strcpy(cmd, history);
            printf("osh>%s\n", cmd);
        }

        strcpy(history, cmd);

        int cmdsize = 1;
        args[0] = strtok(cmd, " ");

        while (args[cmdsize] = strtok(NULL, " "))
        {
            cmdsize++;
        }
        int specialchar = 0, specialcharIndex = 0;

        // Checks the command for a special character
        for (int i = 0; i < cmdsize; i++) {
            switch(args[i][0]) {
                case ';':
                    specialchar = 1;
                    specialcharIndex = i;
                    break;
                case '&':
                    specialchar = 2;
                    specialcharIndex = i;
                    break;
                case '|':
                    specialchar = 3;
                    specialcharIndex = i;
                    break;
                case '<':
                    specialchar = 4;
                    specialcharIndex = i;
                    break;
                case '>':
                    specialchar = 5;
                    specialcharIndex = i;
                    break;
            }

            if (specialchar != 0) {
                break;
            }
        }
        // Calls a specific function according to the special character 
        switch(specialchar) {
            case 1:
                exec_semicol(cmdsize, args, specialcharIndex);
                break;
            case 2:
                exec_amp(cmdsize, args, specialcharIndex);
                break;
            case 3:
                exec_pipe(cmdsize, args, specialcharIndex);
                break;
            case 4:
                exec_inp_redr(cmdsize, args, specialcharIndex);
                break;
            case 5:
                exec_out_redr(cmdsize, args, specialcharIndex);
                break;
            default:
                int pid = fork();
            
                if (pid == 0) {
                    int writecmd = execvp(args[0], args);
                    return 0;
                } 
                else {
                    wait(NULL);
                }
        }
    }
    return 0;
}