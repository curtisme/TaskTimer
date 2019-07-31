#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "task.h"
#include "parse_XML.h"

#define TASK_FORMAT "----------------------\nID: %d\nDescription: %s\nTime spent: %ld mins %ld secs\n----------------------\n"
#define TASK_XML "<task>\n    <name>%s</name>\n    <time>%d</time>\n</task>\n"

struct task_struct {
	int ID;
	char   *desc;
	time_t total_time;
	time_t time_started;
	struct task_struct *next;
};

static task *new_task(char *desc);
static void free_task(task *T);
static int  print_task(task *T, char *buf);
static int  task_to_XML(task *T, char *buf);

task
*new_task(char *desc) {
	task *new;
	new = malloc(sizeof(*new));
	if (!new) {
		fprintf(stderr, "Malloc Error!\n");
		exit(EXIT_FAILURE);
	}
	new->ID = 0;
	new->desc = malloc(strlen(desc)+1);
	strcpy(new->desc, desc);
	new->total_time = 0;
	new->time_started = 0;
	new->next = NULL;
	return new;
}

void
free_task(task *T) {
	free(T->desc);
	free(T);
}

int
print_task(task *T, char *buf) {
	return sprintf(buf,TASK_FORMAT, T->ID, T->desc, T->total_time/60, T->total_time%60);
}

int
task_to_XML(task *T, char *buf) {
	return sprintf(buf, TASK_XML, T->desc, T->total_time);
}

task*
add_task(char *desc, task *task_list) {
	task *new;
	new = new_task(desc);
	new->next = task_list;
	if (task_list)
		new->ID = task_list->ID + 1;
	return new;
}

task
*remove_task(int ID, task *tasks) {

}
void
free_task_list(task *task_list) {
	if (task_list) {
		free_task_list(task_list->next);
		free_task(task_list);
	}
}

int
print_task_list(task *tasks, char *buf) {
	int n;
	if (!tasks)
		return 0;
	n = print_task_list(tasks->next, buf);
	return n + print_task(tasks, buf + n);
}

int
peek_ID(task *tasks) {
	if (tasks)
		return tasks->ID;
	return -1;
}

int
get_description(task *tasks, int ID, char *buf) {
	if (tasks) {
		if (tasks->ID == ID) {
			strcpy(buf, tasks->desc);
			return 1;
		}
		return get_description(tasks->next, ID, buf);
	}
	return 0;
}

int
start_task(task *tasks, int ID) {
	if (!tasks || ID > tasks->ID || ID < 0)
		return 0;
	if (tasks->ID == ID) {
		if (tasks->time_started != 0)
			return 0;
		tasks->time_started = time(NULL);
		return 1;
	}
	return start_task(tasks->next, ID);
}

int
pause_task(task *tasks, int ID) {
	if (!tasks || ID > tasks->ID || ID < 0)
		return 0;
	if (tasks->ID == ID) {
		if (tasks->time_started == 0)
			return 0;
		tasks->total_time += time(NULL) - tasks->time_started;
		tasks->time_started = 0;
		return 1;
	}
	return pause_task(tasks->next, ID);
}


int
to_XML(task *tasks, char *buf) {
	int n;
	if (!tasks)
		return 0;
	n = to_XML(tasks->next, buf);
	return n + task_to_XML(tasks, buf + n);
}

int
from_XML(char *in, task **tasks) {
	char tag[64];
	char content[128];
	char content2[128];
	char *tmp,*tmp2;
	time_t t;
	while (get_tag(in, tag, content, &tmp)) {
		if (strcmp(tag, "task"))
			return 0;
		in = tmp;
		if (!get_tag(content, tag, content2, &tmp2)||strcmp(tag,"name"))
			return 0;
		*tasks = add_task(content2, *tasks);
		if (!get_tag(tmp2, tag, content2, &tmp)||strcmp(tag,"time"))
			return 0;
		sscanf(content2, "%ld", &t);
		(*tasks)->total_time = t;
	}
	return 1;
}
