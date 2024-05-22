#include <errno.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <sys/wait.h>

#include "line-editor.h"
#include "string.h"

void runLine(String *line) {
    Array spaceLocations;
    array_init(&spaceLocations, sizeof(size_t), line->len / 2);
    string_needle_at(line, ' ', &spaceLocations);

    //+2 because argv MUST end with NULL, and also to account for the first word
    const char *args[spaceLocations.len + 2];

    char *buf = calloc(line->len, 1);

    Array offsets;
    array_init(&offsets, sizeof(size_t), spaceLocations.len);

    size_t offset = 0;

    // this section accounts for the first word{{{
    size_t zero = 0;
    array_append(&offsets, &zero);

    int64_t end;
    if (!array_at(&spaceLocations, 0, &end)) {
        end = line->len;
    }

    string_slice_to_cstr(line, buf, 0, end);

    if (end > 0) {
        offset += end + 1;
    }
    //}}}

    for (size_t i = 0; i < spaceLocations.len; i++) {
        array_append(&offsets, &offset);

        size_t start;
        array_at(&spaceLocations, i, &start);

        int64_t end;
        if (!array_at(&spaceLocations, i + 1, &end)) {
            end = line->len;
        }
        // offset points to the start of the next string
        string_slice_to_cstr(line, buf + offset, start + 1, end);
        if (end > 0) {
            offset += end - start;
        } else {
            offset += line->len - start;
        }
    }

    for (size_t i = 0; i < offsets.len; i++) {
        size_t offset;
        array_at(&offsets, i, &offset);
        args[i] = buf + offset;
    }

    args[offsets.len] = 0;

    int pid;
    int status = 0;

    if(strncmp(args[0], "exit", 4) == 0){
        exit(0);
    }

    if(!(pid = fork())){

        //the child needs to recieve ctrl-c (aka SIGINT)
        signal(SIGINT, SIG_DFL);

        //connect this fork to the
        int err = execvp(args[0], (char* const*)args);
        if(errno == ENOENT){
            status = 127;
            fprintf(stderr, "\nCould not find \"%s\" in path", args[0]);
        }
    }
    int childpid = wait(&status);

    array_delete(&spaceLocations);
    array_delete(&offsets);

    free(buf);
}

int main() {
    //parent shell should NOT recieve sigint aka (ctrl-c)
    signal(SIGINT, SIG_IGN);

    struct termios tios = line_editor_init(0);
    while (1) {
        String s;
        string_init(&s);
        line_editor_editline(0, &s);
        fprintf(stderr, "\n");
        runLine(&s);
        string_delete(&s);
    }

    line_editor_reset(0, &tios);
    return 0;
}
