/*
 ============================================================================
 Name        : Function_w.c
 Author      : Elena Gorskaya
 Version     :
 Copyright   : 
 Description : The 1st Lab with sthe 2nd function "w"
 ============================================================================
 */
 
#include <stdlib.h>
#include <unistd.h>
#include <err.h>
#include <stdio.h>
#include <utmp.h>
#include <time.h>

FILE *file(char *name) {
	FILE *ufp;

	if (!(ufp = fopen(name, "r"))) {
		err(1, "%s", name);
	}
	return (ufp);
}

void getTime() {
	time_t tmpTime;
	char buf[64];
	time(&tmpTime);
	strftime(buf, 64, "%X", localtime(&tmpTime));
	printf("%s", buf);

}
void getUp() {
	FILE *fTime;
	int sec;
	fTime = fopen("/proc/uptime", "r");
	fscanf(fTime, "%i", &sec);
	fclose(fTime);
	if (sec / 60 / 60 > 0)
		printf("%i:%02d", sec / 60 / 60, sec / 60 % 60);
	else
		printf("%i min,", sec / 60);
}
void getUsersNum() {
	FILE *utmp;
	utmp = file(_PATH_UTMP);
	int num = -1;
	struct utmp user;
	while (fread((char *) &user, sizeof(user), 1, utmp) ==1) {
		if (*user.ut_name && *user.ut_line && *user.ut_line != '~') {
			num++;
		}
	}
	printf("%i user", num);
}
void getAverage() {
	double lAvg[3];
	if (getloadavg(lAvg, 3) != -1) {
		printf("load average: %.2f, %.2f, %.2f", lAvg[0], lAvg[1], lAvg[2]);
	} else {
		printf("load average :?");
	}
}
void getUserInfo() {
	FILE *ufp;
	struct utmp usr;
	ufp = file(_PATH_UTMP);
	while (fread((char *) &usr, sizeof(usr), 1, ufp) ==1) {
		if (*usr.ut_name && *usr.ut_line && *usr.ut_line != '~' && usr.ut_line[3] != '1') {
				char buffer[80];
				struct tm *info;
				time_t timestamp = usr.ut_tv.tv_sec;
				info = localtime(&timestamp);
				strftime(buffer, 80, "%R", info);
				printf("%s\t%s\t%s\t\t%s\t?\t", usr.ut_name, usr.ut_line,
						usr.ut_host, buffer);
				getUp();
				printf("m\n");
		}
	}
}
int main() {
	getTime();
	printf(" up ");
	getUp();
	printf(", ");
	getUsersNum();
	printf(", ");
	getAverage();
	printf("\n");
	printf("USER\tTTY\tFROM\t\tLOGIN@\tIDLE\tJCPU\tPCPU WHAT\n");
	getUserInfo();
	return 0;
}
