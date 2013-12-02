#include <sys/sysinfo.h>    // sysinfo
#include <stdio.h>
#include <unistd.h>     // sysconf
#include "map_lib.h"

//http://stackoverflow.com/questions/3585846/color-text-in-terminal-aplications-in-unix

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

#define BIOSMD5 "BIOS md5sum"
#define BIOSINFO "BIOS info"
#define BIOSVER "BIOS version"
#define BOARDPRD "Board prod"
#define OSVER "OS"
#define HOST "Host name"
#define KERNEL "Kernel"
#define CMDLINE "cmdline"
#define RUNLEVEL "Runlevel"
#define CPU "CPU"
#define CPUCORES "Cores"
#define CORESPEED "Core speed"
#define ROOTSIZE "Root size"
#define TOTALMEM "Total mem"
#define FREEMEM "Free mem"
#define UPTIME "Uptime"
#define PROCESS "Process"
#define PAGESIZE "Page size"
 
char buf[1024];
struct sysinfo info;

int strcmp(char * str1, char * str2)
{
  return (strlen(str1)==strlen(str2) && !strncasecmp(str1,str2))? 1 : 0;
}

char * sysCall(char * cmd)
{
	memset(buf,'\0',sizeof(buf));
	FILE *fp;
	char * rtn = buf;
	if((fp = popen(cmd,"r"))!=NULL)
	{
		fgets(buf,sizeof(buf),fp);
	}
	pclose(fp);
	return rtn;
}

char * procInfo(char * cmd)
{
    memset(buf,'\0',sizeof(buf));
    if (!sysinfo(&info))
    {
      if(strcmp(cmd,UPTIME)) sprintf(buf,"%ld:%ld:%ld",info.uptime/3600, info.uptime%3600/60, info.uptime%60); 
      if(strcmp(cmd,PROCESS)) sprintf(buf,"%d",info.procs);
      if(strcmp(cmd,PAGESIZE)) sprintf(buf,"%ld",sysconf(_SC_PAGESIZE));

      return buf;
   } 
   return "N/A";
}

char * memInfo(char * cmd)
{
    memset(buf,'\0',sizeof(buf));
    if (!sysinfo(&info))
    {
      if(strcmp(cmd,TOTALMEM)) sprintf(buf,"%ld MB",info.totalram/1024/1024); 
      if(strcmp(cmd,FREEMEM)) sprintf(buf,"%ld MB",(info.totalram-info.freeram)/1024/1024); 
      return buf;
   } 
   return "N/A";
}

long int toInt(char * str)
{
   return (long int) strtol(str, (char **)NULL, 10);
}

void printInfo(const char * color, char * section, char * value)
{
	printf("%s%-15s:%s%s",color,section, KNRM,value);
}

void printlnInfo(const char * color,char * section, char * value)
{
	printf("%s%-15s:%s%s\n",color,section,KNRM,value);
}

void display_both(Map *m,char *first) {
   //printf("%s %s\n",first,map_get(m,first));
	printf("%s%-15s: %s%s\n",KYEL,first,KNRM,map_get(m,first));
}

void gen_command_queue(CommandMap *cmd_map,char * name, char * cmd_alias, executor exe,char * cmd_string)
{
  Command *newcmd = command_create(cmd_alias,exe,cmd_string);
  cmdmap_set(cmd_map,name,newcmd);
}

void print_command(CommandMap *cmd_map,char * name)
{
  Command *thecmd = cmdmap_get(cmd_map,name);
  if(thecmd!=NULL) 
  {
	printf("%s%-15s: %s%s\n",KYEL,name,KNRM,thecmd->result);
  }
  else
  {
	printf("%s%-15s: %s%s\n",KYEL,name,KNRM,"N/A");
  }
}

// Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trim(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

// Stores the trimmed input string into the given output buffer, which must be
// large enough to store the result.  If it is too small, the output is
// truncated.
size_t trimwhitespace(char *out, size_t len, const char *str)
{
  if(len == 0)
    return 0;

  const char *end;
  size_t out_size;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
  {
    *out = 0;
    return 1;
  }

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;
  end++;

  // Set output size to minimum of trimmed string length and buffer size minus 1
  out_size = (end - str) < len-1 ? (end - str) : len-1;

  // Copy trimmed string and add null terminator
  memcpy(out, str, out_size);
  out[out_size] = 0;

  return out_size;
}


void load_config(Map *target,char * file)
{
  FILE * fp;
  char * line = NULL;
  size_t len = 0;
  ssize_t read;
  char * c = NULL;
  int index = -1;
char * p1=NULL;
char * p2=NULL;

  fp = fopen(file, "r");
  if (fp != NULL)
  {
       while ((read = getline(&line, &len, fp)) != -1) {
        //   printf("Retrieved line of length %zu:\n", read);
        //   printf("%s", line);
	//load to map
	c = strchr(line, ':');
	index = (int)(c - line);
	if( index>0 && index<(int)read-2) {
	p1=(char *)malloc(index);
	memset(p1,'\0',sizeof(p1));
	strncpy(p1,line,index);
	p2=&line[index+1];
	p2[strlen(p2)-1]='\0'; //trim newline
	map_set(target,trim(p1),trim(p2));
	free(p1);
	}


	}
   }
   else
   {
     printf("File doesn't exist!\n");
     exit(1);
   }
   
   if (line) free(line);
}
 
int main(int argc, char ** argv)
{
//TODO: check if run with root or not

//-c option would load a golden file to check against
 Map *expectedConfig = map_create();
 if(argc>2 && strcmp(argv[1],"-c"))
 {
	//load config file
	load_config(expectedConfig,argv[2]);
 }

 CommandMap *cmdtest = cmdmap_create();

 gen_command_queue(cmdtest,BIOSMD5,"BIOS",sysCall,"dmidecode | md5sum |awk '{print $1}'| tr -d '\n'");
 gen_command_queue(cmdtest,BIOSINFO,"BIOS",sysCall,"dmidecode |grep -A6 \"BIOS Info\" \
				| grep Vendor | awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'");
 gen_command_queue(cmdtest,BIOSVER,"BIOS",sysCall,"dmidecode |grep -A6 \"BIOS Info\"\
				| grep Version | awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'");
 gen_command_queue(cmdtest,BOARDPRD,"Board",sysCall,"dmidecode |grep -A4 \"Base Board Information\" | grep \"Product Name\"| awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'");
 gen_command_queue(cmdtest,OSVER,"OS",sysCall,"cat /etc/lsb-release | \
				grep DISTRIB_DESCRIPTION | awk -F= '{print $2 }' | tr -d '\"'| tr -d '\n'");
 gen_command_queue(cmdtest,HOST,"OS",sysCall,"uname -n |  tr -d '\n' ");
 gen_command_queue(cmdtest,KERNEL,"OS",sysCall,"uname -r |  tr -d '\n' ");
 gen_command_queue(cmdtest,CMDLINE,"OS",sysCall,"cat /proc/cmdline |  tr -d '\n' ");
gen_command_queue(cmdtest,RUNLEVEL,"OS",sysCall,"runlevel | awk '{print $2}' | tr -d '\n'");
 gen_command_queue(cmdtest,CPU,"CPU",sysCall,"cat /proc/cpuinfo |grep \"model name\" | head -1 | awk -F: '{print $2}' |  tr -d '\n' | sed 's/^ *//g'");
 gen_command_queue(cmdtest,CPUCORES,"CPU",sysCall,"cat /proc/cpuinfo | grep processor | wc -l |  tr -d '\n' ");
 gen_command_queue(cmdtest,CORESPEED,"CPU",sysCall,"cat /proc/cpuinfo | grep \"cpu MHz\" | head -1 | awk -F: '{print $2}' |  tr -d '\n' ");
 gen_command_queue(cmdtest,ROOTSIZE,"HDD",sysCall,"df -Ph | grep '/$' | awk '{print $1\",\"$2\",\"$4\"/\"$3}' | tr -d '\n'");
 gen_command_queue(cmdtest,TOTALMEM,"MEM",memInfo,TOTALMEM);
 gen_command_queue(cmdtest,FREEMEM,"MEM",memInfo,FREEMEM);
 gen_command_queue(cmdtest,UPTIME,"OS",procInfo,UPTIME);
 gen_command_queue(cmdtest,PROCESS,"PROCESS",procInfo,PROCESS);
 gen_command_queue(cmdtest,PAGESIZE,"PROCESS",procInfo,PAGESIZE);


 print_command(cmdtest,BIOSINFO);
 print_command(cmdtest,BIOSMD5);
 print_command(cmdtest,BIOSVER);
 print_command(cmdtest,BOARDPRD);
 print_command(cmdtest,OSVER);
 print_command(cmdtest,HOST);

 print_command(cmdtest,KERNEL);
 print_command(cmdtest,CMDLINE);
 print_command(cmdtest,RUNLEVEL);
 print_command(cmdtest,CPU);
 print_command(cmdtest,CPUCORES);
 print_command(cmdtest,CORESPEED);
 print_command(cmdtest,ROOTSIZE);
 print_command(cmdtest,TOTALMEM);
 print_command(cmdtest,FREEMEM);
 print_command(cmdtest,UPTIME);
 print_command(cmdtest,PROCESS);
 print_command(cmdtest,PAGESIZE);
printf("-------------------------\n");
display_both(expectedConfig,BIOSINFO);
display_both(expectedConfig,BIOSMD5);
display_both(expectedConfig,OSVER);
display_both(expectedConfig,CPU);

//	printInfo(KYEL,"BIOS",sysCall("dmidecode |grep -A6 \"BIOS Info\" | grep Vendor | awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'"));
//	printf(",%s\n",sysCall("dmidecode |grep -A6 \"BIOS Info\" | grep Version | awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'"));
//	printInfo(KYEL,"BOARD",sysCall("dmidecode |grep -A4 \"Base Board Information\" | grep Manufacturer | awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'"));
//	printf(",%s\n",sysCall("dmidecode |grep -A4 \"Base Board Information\" | grep \"Product Name\"| awk -F: '{print $2}' | tr -d '\n'| sed 's/^ *//g'"));

//	printlnInfo(KYEL,"OS",sysCall("cat /etc/lsb-release | grep DISTRIB_DESCRIPTION | awk -F= '{print $2 }' | tr -d '\"'| tr -d '\n'"));
//	printlnInfo(KYEL,"Hostname",sysCall("uname -n |  tr -d '\n' "));

//	printlnInfo(KYEL,"Kernel", sysCall("uname -r |  tr -d '\n' "));
//	printlnInfo(KYEL,"CMDLine", sysCall("cat /proc/cmdline |  tr -d '\n' "));
//	printInfo(KYEL,"CPU", sysCall("cat /proc/cpuinfo |grep \"model name\" | head -1 | awk -F: '{print $2}' |  tr -d '\n' | sed 's/^ *//g'"));
//	printf(",%s cores",sysCall("cat /proc/cpuinfo | grep processor | wc -l |  tr -d '\n' "));
//	printf(",%s MHz per core\n", sysCall("cat /proc/cpuinfo | grep \"cpu MHz\" | head -1 | awk -F: '{print $2}' |  tr -d '\n' "));

//	printlnInfo(KYEL,"HDD",sysCall("df -tph | grep '/$' | awk '{print $1\",\"$2\",\"$4\"/\"$3}' | tr -d '\n'"));
	//printlnInfo("Total RAM: %ld MB\n", toInt(sysCall("cat /proc/meminfo | grep MemTotal:| awk '{print $2}'"))/1024);
//	printf("%s%-15s:%s%ld MB\n",KYEL,"Total RAM",KNRM,info.totalram/1024/1024);
 //	printf("%s%-15s:%s%ld MB\n",KYEL,"Free RAM",KNRM,(info.totalram-info.freeram)/1024/1024);
//
//	printf("%s%-15s:%s%ld:%ld:%ld\n",KYEL,"Uptime",KNRM,info.uptime/3600, info.uptime%3600/60, info.uptime%60);
//	printf("%s%-15s:%s%d\n",KYEL,"Process counts",KNRM, info.procs);
//	printf("%s%-15s:%s%ld bytes\n",KYEL,"Page size",KNRM, sysconf(_SC_PAGESIZE));

//	printlnInfo(KYEL,"Network:",sysCall("lshw -class network | grep logical | awk -F: '{print $2}' | tr '\n' ','"));

    return 0;
}
