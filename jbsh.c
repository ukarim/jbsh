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
        is_num = (str[i] > 47) && (str[i] < 58); // is ascii digit?
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

static void listen_for_input(char *cmd_buf, char *arg_buf) {
    char input_buf[2 * BUF_SIZE] = {0};
    printf("bourne> ");
    fgets(input_buf, 2 * BUF_SIZE, stdin);

    if(input_buf[strlen(input_buf) - 1] == '\n') {
        input_buf[strlen(input_buf) - 1] = 0;
    }

    sscanf(input_buf, "%s%s", cmd_buf, arg_buf);
}

int main(void) {

    printf("Jason Bourne Shell 0.1.0\n\n");

    char cmd_buf[BUF_SIZE] = {0};
    char arg_buf[BUF_SIZE] = {0};

    while(true) {
        memset(cmd_buf, 0, BUF_SIZE);
        memset(arg_buf, 0, BUF_SIZE);

        listen_for_input(cmd_buf, arg_buf);

        if (strcmp("kill", cmd_buf) == 0) {
            do_kill(arg_buf);
        } else if (strcmp("exit", cmd_buf) == 0) {
            break;
        } else {
            printf("jbsh: %s: command not found...\n", cmd_buf);
        }
    }
}

