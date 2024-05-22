#pragma once
#include <termios.h>
#include "string.h"

struct termios line_editor_init(int fd);

void line_editor_editline(int fd, String *out);

void line_editor_reset(int fd, struct termios* tios);
