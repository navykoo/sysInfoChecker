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

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "map_lib.h"

int getArrSize(char ** array) {
//can't use char * array[]
    return sizeof(array) / sizeof(char *);
}

Map *map_create() {
   Map *m;
   m=(Map *)malloc(sizeof(Map));
   m->name=NULL;
   m->value=NULL;
   m->nxt=NULL;
   return m;
}


void map_set(Map *m,char *name,char *value) {
   Map *map;

   if(m->name==NULL) {
      m->name=(char *)malloc(strlen(name)+1);
      strcpy(m->name,name);
      m->value=(char *)malloc(strlen(value)+1);
      strcpy(m->value,value);
      m->nxt=NULL;
      return;
   }
   for(map=m;;map=map->nxt) {
      if(strlen(name)==strlen(map->name) && !strncasecmp(name,map->name,strlen(name))) {
         if(map->value!=NULL) {
            free(map->value);
            map->value=(char *)malloc(strlen(value)+1);
            strcpy(map->value,value);
            return;
         }
      }
      if(map->nxt==NULL) {
         map->nxt=(Map *)malloc(sizeof(Map));
         map=map->nxt;
         map->name=(char *)malloc(strlen(name)+1);
         strcpy(map->name,name);
         map->value=(char *)malloc(strlen(value)+1);
         strcpy(map->value,value);
         map->nxt=NULL;
         return;
      }      
   }
}

char *map_get(Map *m,char *name) {
   Map *map;
   for(map=m;map!=NULL;map=map->nxt) {
      if(strlen(name)==strlen(map->name) && !strncasecmp(name,map->name,strlen(name))) {
         return map->value;
      }
   }
   return "";
}
 
Command *command_create(const char *name, executor exec_p, char * cmd_str) {
  Command *c;
  c=(Command *)malloc(sizeof(Command));
  c->name=(char *)malloc(strlen(name)+1);
  strcpy(c->name,name);
  c->exe=&exec_p;
  c->result=NULL;

  if(exec_p!=NULL) {
 	char * rtn = exec_p(cmd_str);
	c->result=(char *)malloc(strlen(rtn)+1);
	strcpy(c->result,rtn);
  }
  return c;
};

void command_update(Command *newcmd, const char *result) {
  if(newcmd->result!=NULL) {
	free(newcmd->result);
  }  
	newcmd->result=(char *)malloc(strlen(result)+1);
	strcpy(newcmd->result,result);

};

void command_run(Command *cmd, char * cmd_str) {
	//run the executor and update the result
	if(cmd->exe!=NULL){
		char * rtn = (*cmd->exe)(cmd_str);
		command_update(cmd,rtn);
	}
};


CommandMap *cmdmap_create() {
   CommandMap *m;
   m=(CommandMap *)malloc(sizeof(CommandMap));
   m->name=NULL;
   m->cmd=NULL;
   m->nxt=NULL;
   return m;
}


void cmdmap_set(CommandMap *m,char *name,Command *newcmd) {
   CommandMap *map;

   if(m->name==NULL) {
      m->name=(char *)malloc(strlen(name)+1);
      strcpy(m->name,name);
      //m->value=(char *)malloc(strlen(value)+1);
      //strcpy(m->value,value);
      m->cmd=(Command *)malloc(sizeof(Command));
      m->cmd->name=(char *)malloc(strlen(newcmd->name)+1);
      strcpy(m->cmd->name,newcmd->name);
      m->cmd->result=(char *)malloc(strlen(newcmd->result)+1);
      strcpy(m->cmd->result,newcmd->result);

      m->nxt=NULL;
      return;
   }
   for(map=m;;map=map->nxt) {
      if(strlen(name)==strlen(map->name) && !strncasecmp(name,map->name,strlen(name))) {
         if(map->cmd!=NULL) {
					if(map->cmd->name!=NULL) free(map->cmd->name);
					if(map->cmd->result!=NULL) free(map->cmd->result);
      		map->cmd->name=(char *)malloc(strlen(newcmd->name)+1);
		      strcpy(map->cmd->name,newcmd->name);
    		  map->cmd->result=(char *)malloc(strlen(newcmd->result)+1);
		      strcpy(map->cmd->result,newcmd->result);
     
          return;
         }
      }
      if(map->nxt==NULL) {
         map->nxt=(CommandMap *)malloc(sizeof(CommandMap));
         map=map->nxt;
       /*  map->name=(char *)malloc(strlen(name)+1);
         strcpy(map->name,name);
         map->value=(char *)malloc(strlen(value)+1);
         strcpy(map->value,value);*/


      map->name=(char *)malloc(strlen(name)+1);
      strcpy(map->name,name);
      //m->value=(char *)malloc(strlen(value)+1);
      //strcpy(m->value,value);
      map->cmd=(Command *)malloc(sizeof(Command));
      map->cmd->name=(char *)malloc(strlen(newcmd->name)+1);
      strcpy(map->cmd->name,newcmd->name);
      map->cmd->result=(char *)malloc(strlen(newcmd->result)+1);
      strcpy(map->cmd->result,newcmd->result);

         map->nxt=NULL;
         return;
      }      
   }
}

Command *cmdmap_get(CommandMap *m,char *name) {
   CommandMap *map;
   for(map=m;map!=NULL;map=map->nxt) {
      if(strlen(name)==strlen(map->name) && !strncasecmp(name,map->name,strlen(name))) {
         return map->cmd;
      }
   }
   return NULL;
}
 
