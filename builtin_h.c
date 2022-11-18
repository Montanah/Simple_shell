#include "shell.h"

struct buitin b[] = {
	{"cd", change_dir},
};
int shell_num_builtins()
{
    return sizeof(b) / sizeof(struct buitin);
}
void sh_exec(char **args) {
    for (int i = 0; i < shell_num_builtins(); i++) {
        if (strcmp(args[0], b[i].name) == 0) {
            b[i].func(args);
            return;
        }
    }

    pid_t child_pid = fork();

    if (child_pid == 0) {
        execvp(args[0], args);
        perror("kash");
        exit(1);
    } else if (child_pid > 0) {
        int status;
        do {
            waitpid(child_pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    } else {
        perror("kash");
    }