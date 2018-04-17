#include<stdio.h>
#include<dirent.h>
#include<string.h>

int getFileList(char *path)
{
	if(path == NULL)
	{
		printf("path is NULL\n");
		return 0;
	}

	DIR *dir;
	struct dirent *file;

	if((dir = opendir(path)) == NULL)
	{
		perror("open dir failed:");
		return 0;
	}

	while((file = readdir(dir)) != NULL)
	{
		printf("%d %s/%s\n",file->d_type, path,file->d_name);
	}
	return 1;
	
}
int main(int argc, char *argv[])
{
	char path[1024];
	memset(path, 0, sizeof(path));
	if(getcwd(path,sizeof(path)) == NULL)
	{
		perror("get current path:");
		return 0;
	}
	printf("current path is: %s\n",path);
	
	getFileList(path);
}
