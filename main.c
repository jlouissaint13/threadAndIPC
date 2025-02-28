#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

int main() {
    //0 is to read 1 is to write;
    int fd[2];
    bool flag = 0;


    if (pipe(fd) == -1) {
        perror("Pipe creation failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    }

    while (flag == 0) {
        if (pid == 0) {
            close(fd[0]);

            char word[100];
            printf("Send a word to the parent process: ");
            scanf("%s", word);


            if (write(fd[1], word, strlen(word) + 1) == -1) {
                perror("Error occurred when writing");
                continue;
            }
            close(fd[1]);
            flag = 1;
        }
        else {
            close(fd[1]);

            char word[100];


            if (read(fd[0], word, sizeof(word)) == -1) {
                perror("Error occurred when reading");
                exit(1);
            }
            close(fd[0]);

            printf("Word received from the child process: %s\n", word);
            flag = 1;
        }
    }

    return 0;
}
