#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "task.h"

#define BUFFER_MAX 1024
#define TMP 128
#define PROMPT "?> "

int begin_work(task *task_list, int ID, char *prompt); 
int backup(char *filename, task *task_list, char *buf);
int file_to_string(FILE *in, char *out);

int
main(int argc, char **argv) {
	char buf[BUFFER_MAX];
	char prompt[TMP];
	char input[TMP];
	char tmp[TMP];
	FILE *f;
	task *tasks = NULL;
	int current_task = -1;
	if (argc < 2) {
		printf("Incorrect number of arguments!\n");
		exit(EXIT_FAILURE);
	}
	if (f = fopen(argv[1], "r")) {
		file_to_string(f, buf);
		if(!from_XML(buf, &tasks))
			tasks = NULL;
		fclose(f);

	}
	strcpy(prompt, PROMPT);
	do {
		printf("%s", prompt);
		scanf("%s", input);
		if (!strcmp(input, "n")) { 
			scanf(" %[^\n]s", tmp);
			pause_task(tasks, current_task);
			tasks = add_task(tmp, tasks);
			current_task = peek_ID(tasks);
			if (!begin_work(tasks, current_task, prompt)) {
				printf("Error starting new task\n");
				exit(EXIT_FAILURE);
			}
		}
		else if (!strcmp(input, "s")) {
			pause_task(tasks, current_task);
			scanf("%d%*[^\n]s", &current_task);
			if (!begin_work(tasks, current_task, prompt)) {
				printf("ERROR: Invalid Task ID!\n");
				continue;
			}
		}
		else {
			scanf("%*[^\n]s");
			if (!strcmp(input, "p")) {
				if (!tasks)
					printf("No tasks!\n");
				else {
					print_task_list(tasks, buf);
					printf("%s", buf);
				}
			}
			else if (!strcmp(input, "x")) {
				pause_task(tasks, current_task);
				strcpy(prompt, PROMPT);
				current_task = -1;
			}
			else if (!strcmp(input, "b")) {
				pause_task(tasks,current_task);
				backup(argv[1], tasks, buf);
				start_task(tasks, current_task);
			}
		}
	} while (strcmp(input, "end")); 
	pause_task(tasks, current_task);
	backup(argv[1], tasks, buf);
	free_task_list(tasks);
	return 0;
}

int
begin_work(task *tasks, int ID, char *prompt) {
	int success;
	success = get_description(tasks, ID, prompt);
	if (!success)
		return success;
	strcat(prompt, PROMPT);
	success = success && start_task(tasks, ID);
	return success;
}

int
backup(char *filename, task *tasks, char *buf) {
	FILE *f;
	f = fopen(filename, "w");
	if (!f)
		return 0;
	to_XML(tasks, buf);
	fprintf(f, "%s", buf);
	fclose(f);
	return 1;
}

int
file_to_string(FILE *in, char *out) {
	int i,c;
	i=0;
	while ((c = fgetc(in)) != EOF)
		out[i++] = c;
	out[i] = '\0';
	return 1;
}

