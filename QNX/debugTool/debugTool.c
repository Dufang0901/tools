#include<stdio.h>
#include<stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>


#define CMD_CLEAR      1      /*clear all log, including monitor log, sloginfo*/
#define CMD_EXPORT     2      /*create new dirctory in u disk and export all log into it*/

#define PARAM_LENGTH   64 
#define PARAM_NUM_MAX  5

#define DEFAULT_EXPORT_DIR     "/usb/logExport/"


int cmd = -1;
char *params[PARAM_NUM_MAX];
int   paramsNum = 0;
char dir[PARAM_LENGTH];

char clearList[][PARAM_LENGTH] = {
    ("/appfs/taskmonitor/*"),
    ("/appfs/slogs*"),
    ("/appfs/*.core")

};

char exportList[][PARAM_LENGTH] = {
    ("/appfs/taskmonitor/*"),
    ("/appfs/*.core")
};

#define CLEAR_CNT    sizeof(clearList)/sizeof(clearList[0])
#define EXPORT_CNT    sizeof(exportList)/sizeof(exportList[0])

void initParams()
{
    int i = 0;

    bzero(dir,sizeof(dir));
    for(i = 0;i < PARAM_NUM_MAX;i++)
    {
        params[i] = NULL;
    }
}
void freeParams()
{
    int i = 0;
    for(i = 0;i < PARAM_NUM_MAX;i++)
    {
        if(params[i])
        {
            free(params[i]);
        }
        params[i] = NULL;
    }
}

int mountUdisk()
{
    /*get U disk device node*/
    DIR *dir = NULL;
    struct dirent *ptr = NULL;
    char devNode[64] = {0};
    char devNodebak[64] = {0};
    char cmdLine[128] = {0};

    bzero(devNode,sizeof(devNode));
    bzero(devNodebak,sizeof(devNodebak));
    dir = opendir("/dev");
    if(NULL != dir)
    {
      /*search the directorys*/
      while ((ptr = readdir(dir)) != NULL)
      {
        /*skip the . and ..*/
        if((strcmp(ptr->d_name, ".") == 0) || (strcmp(ptr->d_name, "..") == 0))
          continue;
        /*get the device node*/
        if(strstr(ptr->d_name,"hd0t"))
        {
            snprintf(devNode,sizeof(devNode) - 1,"/dev/%s",ptr->d_name);
        }
        else if(strstr(ptr->d_name,"hd"))
        {
            snprintf(devNodebak,sizeof(devNodebak) - 1,"/dev/%s",ptr->d_name);
        }
     
      }
      /*close directory*/
      closedir(dir);
    }

    if(strlen(devNode))
    {
        snprintf(cmdLine,sizeof(cmdLine) - 1,"mount -t dos %s /usb",devNode);
    }
    else if(strlen(devNodebak))
    {
        snprintf(cmdLine,sizeof(cmdLine) - 1,"mount -t dos %s /usb",devNodebak);
    }
    else
    {
        printf("##### error: U disk device node not found\n");
        return -1;
    }
    
    printf("#### %s\n",cmdLine);
    system(cmdLine);

    return 0;
}
int umountUdisk()
{
    system("sync");
    system("sync");
    system("umount /usb");
}
int clearLog()
{
    char cmdLine[128];
    int i = 0;
   
    /*remove files in clearList*/
    for(i = 0;i < CLEAR_CNT;i++)
    {
        bzero(cmdLine,sizeof(cmdLine));
        snprintf(cmdLine,sizeof(cmdLine) - 1,"rm -rf %s",clearList[i]);
        printf("#### %s\n",cmdLine);
        system(cmdLine);
    }

    /*remove the specified files*/
    for(i = 0;i < PARAM_NUM_MAX;i++)
    {
        if(params[i])
        {
            bzero(cmdLine,sizeof(cmdLine));
            snprintf(cmdLine,sizeof(cmdLine) - 1,"rm -rf %s",params[i]);
            printf("#### %s\n",cmdLine);
            system(cmdLine);
        }
    }
    
}
int exportLog()
{
    char cmdLine[128];
    int i = 0;

    /*determin the exporting directory*/
    if(0 == strlen(dir))
    {
        bzero(dir,sizeof(dir));
        strncpy(dir,DEFAULT_EXPORT_DIR,sizeof(dir) - 1);
    }

    /*mount U disk*/
    if(mountUdisk())
        return -1;

    /*remove the original log export directory in U disk*/
    bzero(cmdLine,sizeof(cmdLine));
    snprintf(cmdLine,sizeof(cmdLine) - 1,"rm -rf %s",dir);
    printf("#### %s\n",cmdLine);
    system(cmdLine);
    
    /*create new directory*/
    bzero(cmdLine,sizeof(cmdLine));
    snprintf(cmdLine,sizeof(cmdLine) - 1,"mkdir -p %s",dir);
    printf("#### %s\n",cmdLine);
    system(cmdLine);

    /*copy log files into created usb directory*/
    /*copy slog files*/
    bzero(cmdLine,sizeof(cmdLine));
    snprintf(cmdLine,sizeof(cmdLine) - 1,"sloginfo /appfs/slogs1 > %s/1.txt",dir);
    printf("#### %s\n",cmdLine);
    system(cmdLine);

    bzero(cmdLine,sizeof(cmdLine));
    snprintf(cmdLine,sizeof(cmdLine) - 1,"sloginfo /appfs/slogs2 > %s/2.txt",dir);
    printf("#### %s\n",cmdLine);
    system(cmdLine);

    bzero(cmdLine,sizeof(cmdLine));
    snprintf(cmdLine,sizeof(cmdLine) - 1,"sloginfo > %s/sloginfo.txt",dir);
    printf("#### %s\n",cmdLine);
    system(cmdLine);

    /*copy files in export list*/
    for(i = 0;i < EXPORT_CNT;i++)
    {
        bzero(cmdLine,sizeof(cmdLine));
        snprintf(cmdLine,sizeof(cmdLine) - 1,"cp -rf %s %s",exportList[i],dir);
        printf("#### %s\n",cmdLine);
        system(cmdLine);
    }

    /*copy the specified files*/
    for(i = 0;i < PARAM_NUM_MAX;i++)
    {
        if(params[i])
        {
            bzero(cmdLine,sizeof(cmdLine));
            snprintf(cmdLine,sizeof(cmdLine) - 1,"cp -rf %s %s",params[i],dir);
            printf("#### %s\n",cmdLine);
            system(cmdLine);
        }
    }

    system("sync");

    umountUdisk();
    return 0;

}
int opt_parse(int argc,char *argv[])
{
    int i = 0;
    int paramCnt = 0;
    char *p = NULL;
    
    if(argc < 2)
    {
        return -1;
    }

    if(strstr(argv[1],"clear"))
    {
        cmd = CMD_CLEAR;
        paramsNum = argc - 2;
        if(paramsNum > 0 && paramsNum <= PARAM_NUM_MAX)
        {
            for(i = 0;i < paramsNum;i++)
            {
                params[i] = (char *)malloc(PARAM_LENGTH);
                memset(params[i],0,PARAM_LENGTH);
                strncpy(params[i],argv[i + 2],PARAM_LENGTH - 1);
            }
        }
        else if(paramsNum > PARAM_NUM_MAX)
        {
            printf("##### error: too many paramaters.\n");
            return -1;
        }
    }
    else if(strstr(argv[1],"export"))
    {
        cmd = CMD_EXPORT;
        paramCnt = 0;
        for(i = 2;i < argc;i++)
        {
            if(strstr(argv[i],"d="))
            {
                p = strchr(argv[i],'=');
                strncpy(dir,p + 1,PARAM_LENGTH - 1);
                printf("@@@@@@ dir is %s\n",dir);
            }
            else if(paramCnt < PARAM_NUM_MAX)
            {
                params[paramCnt] = (char *)malloc(PARAM_LENGTH);
                memset(params[paramCnt],0,PARAM_LENGTH);
                strncpy(params[paramCnt],argv[i],PARAM_LENGTH - 1);
                if(++paramCnt >= PARAM_NUM_MAX)
                {
                    printf("##### warning: more than maximum(%d) parameters will be dropped",PARAM_NUM_MAX);
                    
                }
            }
        }
        
    }
    else
    {
        return -1;
    }

    return 0;
}
int printHelpInfo()
{
printf("\n\n");
printf("invalid parameters\n");
printf("#####################################################\n");
printf("# debugtool [cmd] param1 param2 param3 param4 ...   #\n");
printf("#           [cmd] - mandatory    paramN - optinal   #\n\n");
printf("# usage: 1. debugtool clear file1 file2 file3 ...   #\n");
printf("#        2. debugtool export -d=dir file1 file2 ... #\n");
printf("####################################################\n\n");
}
int main(int argc,char *argv[])
{
    initParams();
    
    /*verify input optins*/
    if(opt_parse(argc,argv))
    {
        printHelpInfo();
        return -1;
    }


    /*process according to input command*/
    switch (cmd)
    {
        case CMD_CLEAR:
        {
            clearLog();
        }
        break;
        case CMD_EXPORT:
        {
            exportLog();
            
        }
        break;
        default:
        {
            printf("@@@@@@@@@@ command %s not recognized.\n",argv[1]);
            printHelpInfo();
        }
        break;
    }

    freeParams();

    return -1;
        
}
