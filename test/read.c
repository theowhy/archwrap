#include <archwrap.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#define LINE_SIZE 16

void sighandler(int signo) {
}

int main(int argc, char **argv) {
	unsigned char buff[100] = {0};
	char line[LINE_SIZE+1] = {0};
	size_t size;
	size_t line_current;
	FILE * fstr;

	if(argc < 2) {
		return 1;
	}

	struct sigaction sa;
	sa.sa_handler =  &sighandler;
	sa.sa_flags = SA_NOCLDSTOP;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGALRM, &sa, NULL );

	fstr = archwrap_open(argv[1]);
	if(fstr == NULL) {
		return -2;
	}

	line_current = 0;
	do {
		size = fread(buff, 1, sizeof(buff), fstr);
		if(size >= 0) {
			for(size_t i=0; i<size; i++) {
				printf("%.2x ", buff[i]);

				if(isprint(buff[i])) {
					line[line_current] = buff[i];
				} else {
					line[line_current] = '.';
				}
				line_current++;

				if(line_current == LINE_SIZE) {
					printf("| %s\n", line);
					memset(line, 0, LINE_SIZE);
					line_current = 0;
				}

			}
		}
	} while(!feof(fstr));

	for(size_t j = 0;j<LINE_SIZE-(line_current%LINE_SIZE);j++) {
		printf("   ");
	}
	printf("| %s\n", line);

	fclose(fstr);

	return 0;
}
