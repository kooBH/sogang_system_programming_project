#ifndef _H_MEMORY_
#define _H_MEMORY_

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "command.h"

#define MAX_MEM 1024*1024 // 1MB

void dump(struct command*cmd);
void edit(struct command*cmd);
void fill(struct command*cmd);
void reset(struct command*cmd);

#endif
