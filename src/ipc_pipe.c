#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipe_fd[2];  // File descriptors for pipe
    pid_t pid;
    char message[] = "Hello from parent!";
    char buffer[BUFFER_SIZE];

    // Create a pipe
    if (pipe(pipe_fd) == -1) {
        perror("Pipe failed");
        exit(1);
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {  // Child process
        close(pipe_fd[1]);  // Close unused write end
        read(pipe_fd[0], buffer, BUFFER_SIZE);
        printf("Child received: %s\n", buffer);
        close(pipe_fd[0]);  // Close read end
    } else {  // Parent process
        close(pipe_fd[0]);  // Close unused read end
        write(pipe_fd[1], message, strlen(message) + 1);
        close(pipe_fd[1]);  // Close write end
    }

    return 0;
}
