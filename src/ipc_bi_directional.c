#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
    int pipe1[2], pipe2[2];  // Two pipes for bi-directional communication
    pid_t pid;
    char parent_msg[] = "Hello from Parent!";
    char child_msg[] = "Message received!";
    char buffer[BUFFER_SIZE];

    // Create two pipes
    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    // Fork a child process
    pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {  // Child process
        close(pipe1[1]);  // Close unused write end of pipe1
        close(pipe2[0]);  // Close unused read end of pipe2

        read(pipe1[0], buffer, BUFFER_SIZE);
        printf("Child received: %s\n", buffer);

        // Send response back to parent
        write(pipe2[1], child_msg, strlen(child_msg) + 1);

        // Close remaining pipes
        close(pipe1[0]);
        close(pipe2[1]);
    } else {  // Parent process
        close(pipe1[0]);  // Close unused read end of pipe1
        close(pipe2[1]);  // Close unused write end of pipe2

        // Send message to child
        write(pipe1[1], parent_msg, strlen(parent_msg) + 1);

        // Read response from child
        read(pipe2[0], buffer, BUFFER_SIZE);
        printf("Parent received: %s\n", buffer);

        // Close remaining pipes
        close(pipe1[1]);
        close(pipe2[0]);
    }

    return 0;
}
