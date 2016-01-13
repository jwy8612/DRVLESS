#define CONF_FILE_PATH  "/mnt/sdcard/car/Config.ini"  
  
#include <string.h>  
  
#define  MAX_PATH 260  
  
#include <unistd.h>  
#include <fcntl.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <stdarg.h>  
  
char g_szConfigPath[MAX_PATH];  
  
//��ȡ��ǰ����Ŀ¼  
int GetCurrentPath(char buf[],char *pFileName)  
{  
#if 0
    char pidfile[64];  
    int bytes;  
    int fd;  
    sprintf(pidfile, "/proc/%d/cmdline", getpid());  
  
    fd = open(pidfile, O_RDONLY, 0);  
    bytes = read(fd, buf, 256);  
    close(fd);
#endif	
    buf[MAX_PATH] = '\0';  
  
    char *p = &buf[strlen(buf)];  
    do   
    {  
        *p = '\0';  
        p--;  
    } while( '/' != *p );  
    p++;  
  
    //�����ļ�Ŀ¼  
    memcpy(p,pFileName,strlen(pFileName));  
    return 0;  
}  
  
//��INI�ļ���ȡ�ַ�����������  
char *GetIniKeyString(char *title,char *key,char *filename)   
{   
    FILE *fp;   
    char szLine[1024];  
    static char tmpstr[1024];  
    int rtnval;  
    int i = 0;   
    int flag = 0;   
    char *tmp;  
  
    if((fp = fopen(filename, "r")) == NULL)   
    {   
        printf("have   no   such   file \n");  
        return "";   
    }  
    while(!feof(fp))   
    {   
        rtnval = fgetc(fp);   
        if(rtnval == EOF)   
        {   
            break;   
        }   
        else   
        {   
            szLine[i++] = rtnval;   
        }   
        if(rtnval == '\n')   
        {   
#ifndef WIN32  
            i--;  
#endif    
            szLine[--i] = '\0';  
            i = 0;   
            tmp = strchr(szLine, '=');   
  
            if(( tmp != NULL )&&(flag == 1))   
            {   
                if(strstr(szLine,key)!=NULL)   
                {   
                    //ע����  
                    if ('#' == szLine[0])  
                    {  
                    }  
                    else if ( '\/' == szLine[0] && '\/' == szLine[1] )  
                    {  
                          
                    }  
                    else  
                    {  
                        //�Ҵ�key��Ӧ����  
                        strcpy(tmpstr,tmp+1);   
                        fclose(fp);  
                        return tmpstr;   
                    }  
                }   
            }  
            else   
            {   
                strcpy(tmpstr,"[");   
                strcat(tmpstr,title);   
                strcat(tmpstr,"]");  
                if( strncmp(tmpstr,szLine,strlen(tmpstr)) == 0 )   
                {  
                    //�ҵ�title  
                    flag = 1;   
                }  
            }  
        }  
    }  
    fclose(fp);   
    return "";   
}  
  
//��INI�ļ���ȡ����������  
int GetIniKeyInt(char *title,char *key,char *filename)  
{  
    return atoi(GetIniKeyString(title,key,filename));  
}  
  
int config_read(param * param)  
{  
    char buf[MAX_PATH];  
    memset(buf,0,sizeof(buf));  
    GetCurrentPath(buf,CONF_FILE_PATH);  
    strcpy(g_szConfigPath,buf);  
  
    int iCatAge;  
    char szCatName[32];  
      
    iCatAge = GetIniKeyInt("CAT","age",g_szConfigPath);  
    strcpy(szCatName,GetIniKeyString("CAT","name",g_szConfigPath));  
  
    return 0;  
}  

