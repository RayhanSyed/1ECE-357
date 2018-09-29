#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdint.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
int errno = 0;
extern char *optarg;
extern int optind;
int DirecRead (char* D);
void FoundOutput(struct stat S, char* F);
int main(int argc, char *argv[]){
		char* InitDir;
		switch(argc){
		case 1 : InitDir= "./";
		break;
		case 2 : InitDir = argv[optind];
		break;
		default: perror("Invalid arguments error: Too many");
		exit(-1);
		break;
		}
		DirecRead(InitDir);
		return(0);
		}
int DirecRead (char* D){
		DIR* dirp;
		struct dirent* dp;
		 dirp = opendir(D);
		//fprintf(stderr, "dir %s err %s \n", D, strerror(errno));
		while (dirp) {
			char filename[257];
    	errno = 0;
    	dp = readdir(dirp);
    	if (dp != NULL) {
    		snprintf(filename, sizeof(filename), "%s/%s", D, dp->d_name);
    	//fprintf(stderr, "filename is %s \n", dp->d_name);
    	struct stat fileStat;
    	int statSuccess= lstat(filename,&fileStat);
    	if(!(statSuccess == -1))  {  
    		if ((strcmp(dp->d_name, ".")==0) ||(strcmp(dp->d_name, "..")==0)){
			FoundOutput(fileStat, filename);
    		}
    		else if(fileStat.st_mode & S_IFDIR){
    	FoundOutput(fileStat, filename);
    	DirecRead(filename);	
    	}
    		else if(fileStat.st_mode & S_IFREG){
    	FoundOutput(fileStat, filename);
    	}
       else if(fileStat.st_mode & S_IFREG){
       	//fprintf(stdout, "Block: %ld BLKSIZE: %ld \n", fileStat.st_blocks, fileStat.st_blksize);	
       //	printf("\n comp: %i \n", (S_ISLNK(fileStat.st_mode)));
    	FoundOutput(fileStat, filename);
    	}
    }
        }
    
    if (dp == NULL){
       // fprintf(stderr, "exiting \n");
        closedir(dirp);
	return(0);
        }	
}
}

void FoundOutput (struct stat S, char* F){
	struct passwd  *pwd;
	struct group   *grp;
	int Sym = 0;
	printf( "%ld \t", S.st_ino);	
	printf( "%ld \t", S.st_blocks/2);	//right number?
	if (S_ISLNK(S.st_mode)){
		printf("l");
		Sym = 1;
	}
	else {
	printf( (S_ISDIR(S.st_mode)) ? "d" : "-");
}
    printf( (S.st_mode & S_IRUSR) ? "r" : "-");
    printf( (S.st_mode & S_IWUSR) ? "w" : "-");
    printf( (S.st_mode & S_IXUSR) ? "x" : "-");
    printf( (S.st_mode & S_IRGRP) ? "r" : "-");
    printf( (S.st_mode & S_IWGRP) ? "w" : "-");
    printf( (S.st_mode & S_IXGRP) ? "x" : "-");
    printf( (S.st_mode & S_IROTH) ? "r" : "-");
    printf( (S.st_mode & S_IWOTH) ? "w" : "-");
    printf( (S.st_mode & S_IXOTH) ? "x" : "-");
	printf("\t %ld \t", S.st_nlink);	
	if((pwd = getpwuid(S.st_uid))!=NULL)
		printf("%s \t", pwd->pw_name);
	else
		printf("%d \t", S.st_uid);
	if((grp = getgrgid(S.st_uid))!=NULL)
		printf("%s \t", grp->gr_name);
	else
		printf("%d \t", S.st_gid);
	printf("%ld ",S.st_size); 	
	printf("%s ", ctime(&S.st_mtime));
	printf("%s", F);	
	if (Sym){
		char SymBuf[257];
		readlink(F,SymBuf, sizeof(SymBuf)-1);
		printf("-> %s \n", SymBuf);
	}
		else
			printf ("\n");
	}
