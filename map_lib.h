// map_lib
// A simple associative-array library for C
//
// License: MIT / X11
// Copyright (c) 2009 by James K. Lawless
// jimbo@radiks.net http://www.radiks.net/~jimbo
// http://www.mailsend-online.com
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.


#ifndef MAP_LIB_H
#define MAP_LIB_H

typedef char * (* executor)(char *);

typedef struct command_t {
  char *name;
  executor *exe;
  char *result;
} Command;

typedef struct map_t {
   struct map_t *nxt;
   char *name;
   char *value;
} Map;


typedef struct cmdmap_t {
   struct cmdmap_t *nxt;
   char *name;
   struct command_t *cmd;
} CommandMap;


Map *map_create();
void map_set(Map *m,char *name,char *value);
char *map_get(Map *m,char *name);

Command *command_create(const char *name, executor exec_p, char * cmd_str);
void command_update(Command *cmd, const char *result);
void command_run(Command *cmd, char * cmd_str);
CommandMap *cmdmap_create();
void cmdmap_set(CommandMap *m,char *name,Command *cmd);
Command *cmdmap_get(CommandMap *m,char *name);

#endif
 
