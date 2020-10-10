#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include<fstream>
#include "common_api.hpp"
#include <stdarg.h>
#include <signal.h>
#include<execinfo.h>
#include <sys/stat.h>
#include <pthread.h>
#include<iostream>
using namespace std;

pthread_mutex_t log_file_mutex = PTHREAD_MUTEX_INITIALIZER;

std::string dragon_before = "";
std::string dragon = dragon_before +
"----------Dragon be here!----------" + "\n" +
"　　　┏┓　　　┏┓" + "\n" +
"　　┏┛┻━━━┛┻┓" + "\n" +
"　　┃　　　　　　　┃" + "\n" +
"　　┃　　　━　　　┃" + "\n" +
"　　┃　┳┛　┗┳　┃" + "\n" +
"　　┃　　　　　　　┃" + "\n" +
"　　┃　　　┻　　　┃" + "\n" +
"　　┃　　　　　　　┃" + "\n" +
"　　┗━┓　　　┏━┛" + "\n" +
"　　　　┃　　　┃神兽保佑" + "\n" +
"　　　　┃　　　┃代码无BUG！" + "\n" +
"　　　　┃　　　┗━━━┓" + "\n" +
"　　　　┃　　　　　　　┣┓" + "\n" +
"　　　　┃　　　　　　　┏┛" + "\n" +
"　　　　┗┓┓┏━┳┓┏┛" + "\n" +
"　　　　　┃┫┫　┃┫┫" + "\n" +
"　　　　　┗┻┛　┗┻┛" + "\n" +
"━━━━━━神兽出没━━━━━━" + "\n";



//去除字符串中的空白字符
void string_trim(char *s)
{
	char *target;
	int i = 0;
	int j = 0;
	int length = strlen(s) + 1;
	target = (char*)malloc(length);
	memset(target, 0, length);
	for (i = 0; s[i] != '\0'; i++)
	{
		if (s[i] == ' ' || s[i] == '\r' || s[i] == '\n' || s[i] == '\t')
		{
			continue;
		}
		else
		{
			target[j] = s[i];
			j++;
		}
		//由于前面有memset,最后的一个'\0'可以不用加
	}
	memset(s, 0, length);
	strcat(s, target);
	free(target);
}

bool parse_property_from_config_file(const char* key, char *value)
{
	char line[400];
	char* index;
	int lenkey, lenvalue;
	int i;
	ifstream file("../config/config.ini", ios::in);
	if (!file)
	{
		return false;
	}
	while (file.good() && !file.eof())
	{
		memset(line, 0, sizeof(line));
		file.getline(line, sizeof(line));
		//在windows中编辑过的文件会在结尾添加"\r\n"，先把这两个字符消掉,并且也把空格去掉
		string_trim(line);
		if (line[0] == '#' || line[0] == '\0')//#是注释
		{
			continue;
		}
		index = strstr(line, "=");//找到=出现的位置
		if (index == NULL)//这一行没有找到，或者这一行写的有问题，没有等于号，直接读下一行
		{
			continue;
		}
		lenkey = index - line;//key的长度
		lenvalue = strlen(index) - 1;//value的长度
		if (lenvalue == 0)//防止出现类似key=的情况,这一行也作废
		{
			continue;
		}
		if (strncmp(line, key, strlen(key)) == 0 && strlen(key) == lenkey)
		{
			strcat(value, index + 1);
			file.close();
			return true;
		}

	}
	file.close();
	return false;
}

//用来记录系统的log，默认程序启动后就永远不关闭它，一直保持开启状态

void LOG(const char* fmt, ...)
{

	struct tm * ptm;
	time_t seconds;
	static bool first_time = true;
	static FILE *f;
	time(&seconds);
	ptm = localtime(&seconds);
	pthread_mutex_lock(&log_file_mutex);
	if (first_time)
	{
		f = fopen("../data_center.log", "ab");
		first_time = false;
		fprintf(f, "%s\n", dragon.c_str());
		fflush(f);
	}

	char buff[1024];//默认不会超过1024个字节
	int cnt;
	va_list argptr;
	va_start(argptr, fmt);
	cnt = vsnprintf(buff, 1024, fmt, argptr);
	buff[cnt] = 0;//可以省略,vsnprintf会自动加入\0
	va_end(argptr);

	fprintf(f, "%d-%d-%d %d:%d:%d ", ptm->tm_year + 1900, ptm->tm_mon + 1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
	fprintf(f, "%s\n", buff);
	fflush(f);
	pthread_mutex_unlock(&log_file_mutex);

}

void exception_entry(int signum)
{
	int n;
	int i;
	char **s;
	void* buff[10];
	printf("here comes a bug,see the log file\n");
	LOG("here comes a signal %d", signum);
	n = backtrace((void **)&buff, sizeof(buff) / sizeof(void*));
	s = backtrace_symbols(buff, n);
	if (s == NULL)
	{
		return;
	}
	for (i = 0; i < n; i++)
	{
		printf("%s\n", s[i]);
		LOG("%s", s[i]);
	}
	free(s);
	_exit(1);
}


void handle_exception()
{
	signal(SIGPIPE, SIG_IGN);
	signal(SIGSEGV, exception_entry);
	signal(SIGABRT, exception_entry);
	signal(SIGILL, exception_entry);
	signal(SIGFPE, exception_entry);
}

bool file_exists(const char * filename)
{
	struct stat sbuf;
	int ret = stat(filename, &sbuf);
	if (ret < 0)
	{
		return false;
	}
	return true;
}

int get_file_size(const char * file)
{
	struct stat sbuf;
	int ret = stat(file, &sbuf);
	if (ret < 0)
	{
		return -1;
	}
	return sbuf.st_size;
}


bool parse_property_from_config_file(const char* filename, const char* key, char *value)
{
	char line[400];
	char* index;
	int lenkey, lenvalue;
	int i;
	ifstream file(filename, ios::in);
	if (!file)
	{
		return false;
	}
	while (file.good() && !file.eof())
	{
		memset(line, 0, sizeof(line));
		file.getline(line, sizeof(line));
		//在windows中编辑过的文件会在结尾添加"\r\n"，先把这两个字符消掉,并且也把空格去掉
		string_trim(line);
		if (line[0] == '#' || line[0] == '\0')//#是注释
		{
			continue;
		}
		index = strstr(line, "=");//找到=出现的位置
		if (index == NULL)//这一行没有找到，或者这一行写的有问题，没有等于号，直接读下一行
		{
			continue;
		}
		lenkey = index - line;//key的长度
		lenvalue = strlen(index) - 1;//value的长度
		if (lenvalue == 0)//防止出现类似key=的情况,这一行也作废
		{
			continue;
		}
		if (strncmp(line, key, strlen(key)) == 0 && strlen(key) == lenkey)
		{
			strcat(value, index + 1);
			file.close();
			return true;
		}

	}
	file.close();
	return false;
}