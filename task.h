#ifndef TASK_H
#define TASK_H

typedef struct task_struct task;

task *add_task(char *desc, task *task_list);
task *remove_task(int ID, task *task_list);
void free_task_list(task *task_list);
int  print_task_list(task *task_list, char *buf);
int  peek_ID(task *task_list);
int  get_description(task *task_list, int ID, char *buf);
int  start_task(task *task_list, int ID);
int  pause_task(task *task_list, int ID);
int  to_XML(task *task_list, char *out);
int  from_XML(char *in, task **task_list);
#endif
