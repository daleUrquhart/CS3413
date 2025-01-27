#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 100

void handle_onechild() {
    pid_t pid = fork()

    if (pid < 0) {
        perror("(Parent) Failed to fork child")
        exit(1)
    } else if (pid == 0) {
        printf("(Child) Parent PID: %d, My PID: %d\n", getppid(), getpid())
        exit(0) // Terminate child process
    } else {
        wait(NULL) // Wait for the child to terminate
        printf("(Parent) Child process terminated\n")
    }
}

void handle_addnumbers() {
    int pipe_fd[2]
    if (pipe(pipe_fd) < 0) {
        perror("(Parent) Failed to create pipe")
        exit(1)
    }

    pid_t pid = fork()
    if (pid < 0) {
        perror("(Parent) Failed to fork child")
        exit(1)
    } else if (pid == 0) { // Child process
        close(pipe_fd[1]) // Close write end
        int sum = 0, num
        while (1) {
            read(pipe_fd[0], &num, sizeof(num))
            if (num == 0) break
            sum += num
        }
        close(pipe_fd[0]) // Close read end
        printf("(Child) Sum calculated: %d\n", sum)
        exit(0)
    } else { // Parent process
        close(pipe_fd[0]) // Close read end
        int num
        printf("(Parent) Enter numbers (0 to terminate):\n")
        while (1) {
            scanf("%d", &num)
            write(pipe_fd[1], &num, sizeof(num))
            if (num == 0) break
        }
        close(pipe_fd[1]) // Close write end
        wait(NULL) // Wait for child
        printf("(Parent) Child process terminated\n")
    }
}

int main() {
    char input[BUFFER_SIZE]
    int run = 1

    while (run) {
        printf("Please input your shell command:\n")
        fgets(input, BUFFER_SIZE, stdin)
        input[strcspn(input, "\n")] = '\0' // Remove newline

        if (strcmp(input, "onechild") == 0) {
            handle_onechild()
        } else if (strcmp(input, "addnumbers") == 0) {
            handle_addnumbers()
        } else if (strcmp(input, "stop") == 0) {
            printf("Terminating shell...\n")
            run = 0
        } else {
            printf("Unknown command: %s\n", input)
        }
    }

    return 0
}
