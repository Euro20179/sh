#include <stdio.h>
#include <termios.h>
#include <unistd.h>

#include "string.h"
#include "line-editor.h"

struct termios line_editor_init(int fd){
    struct termios tios;
    struct termios initial;

    tcgetattr(fd, &tios);
    tcgetattr(fd, &initial);

    tios.c_lflag &= ~(ECHO|ICANON);
    tios.c_cc[VMIN] = 1;
    tios.c_cc[VTIME] = 0;

    tcsetattr(fd, TCSANOW, &tios);

    return initial;
}

void line_editor_editline(int fd, String *out){
    char temp[1];
    long bytesRead;
    while((bytesRead = read(fd, temp, 1)) > 0){
        switch(*temp){
            //4 = EOF aka EOT
            case 4:
            case '\n': return;
            case 8:
            //DEL (backspace key)
            case 0x7f:
                //chop off the last item
                string_substr(out, 0, out->len - 1);
                break;
            default:
                string_append_str(out, temp, bytesRead);
        }
        fprintf(stderr, "\x1b[2K\r");
        for(size_t i = 0; i < out->len; i++){
            fprintf(stderr, "%c", (out->buf[i]));
        }
    }
}

void line_editor_reset(int fd, struct termios *tios){
    tcsetattr(fd, TCSANOW, tios);
}
