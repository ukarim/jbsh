#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>

#define BUF_SIZE 16
#define MAX_PID_LEN 10

static bool is_pid(char *str) {
    size_t len = strlen(str);

    if (len > MAX_PID_LEN) {
        // too long
        return false;
    }

    bool is_num = false;
    for (size_t i = 0; i < len; i++) {
        is_num = (str[i] >= '0') && (str[i] <= '9'); // is ascii digit?
        if (!is_num) {
            break;
        }
    }
    return is_num;
}

static void do_kill(char *pid_str) {
    if (!is_pid(pid_str)) {
        printf("mission failed: argument must be process id\n");
        return;
    }

    int pid = 0;
    sscanf(pid_str, "%d", &pid);
    int sts = kill(pid, SIGTERM);
    if (sts != 0) {
        perror("mission failed");
    }
}

int main(void) {

    printf("Jason Bourne Shell 0.2.0\n\n");

    char cmd_buf[BUF_SIZE];
    char arg_buf[BUF_SIZE];
    int input_buf_size = 2 * BUF_SIZE;
    char input_buf[input_buf_size];

    while(true) {
        printf("bourne> ");

        memset(cmd_buf, 0, BUF_SIZE);
        memset(arg_buf, 0, BUF_SIZE);
        memset(input_buf, 0, input_buf_size);

        fgets(input_buf, input_buf_size, stdin);

        bool nl_found = false;
        for (int i = 0; i < input_buf_size; i++) {
            nl_found = input_buf[i] == '\n';
            if (nl_found) {
                break;
            }
        }

        if (!nl_found) {
            char ch;
            while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {
                // consume extra input
            }
            printf("jbsh: input is too long\n");
            continue;
        }

        if(input_buf[strlen(input_buf) - 1] == '\n') {
            input_buf[strlen(input_buf) - 1] = 0;
        }

        sscanf(input_buf, "%s%s", cmd_buf, arg_buf);

        if (strcmp("kill", cmd_buf) == 0) {
            do_kill(arg_buf);
        } else if (strcmp("history", cmd_buf) == 0) {
            printf("I don't remember\n");
        } else if (strcmp("clear", cmd_buf) == 0) {
            printf("\033[2J\033[H");
        } else if (strcmp("exit", cmd_buf) == 0) {
            break;
        } else {
            printf("jbsh: %s: command not found...\n", cmd_buf);
        }
    }
}

