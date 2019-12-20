#include <stdio.h>
#include <stdlib.h>
//#include <linux/list.h>
#include "libubox/list.h"

#define MAX_TASKS 5

struct task {
	int id;
	int done;
	struct list_head list;
};

LIST_HEAD(task_list);

int main(int argc, char *argv[])
{
	struct list_head *pos = NULL;
	struct list_head *next = NULL;

	struct task *pos_task = NULL;
	struct task *next_task = NULL;

	struct task *tmp = NULL;
	struct task *tsk = NULL;

	int i;
	int type; //0: stack; 1: queue
	int empty = 0;

	if (argc == 2) {
		type = atoi(argv[1]);
		printf("type: %d\n", type);
	}

	for (i = 0; i < MAX_TASKS; i++) {
		tsk = calloc(1, sizeof(struct task));
		if (!tsk) {
			printf("calloc failed\n");
			break;
		}
		INIT_LIST_HEAD(&tsk->list);
		tsk->id = i;
		tsk->done = (i % 2) ? 0 : 1;
		if (type == 0)
			//add tsk to the head of task_list
			list_add(&tsk->list, &task_list);
		else
			//add tsk to the tail of task_list
			list_add_tail(&tsk->list, &task_list);
	}


	list_for_each(pos, &task_list) {
		tmp = list_entry(pos, struct task, list);
		printf("task id: %d, task done: %d\n", tmp->id, tmp->done);
	}
	printf("\n");


	printf("delete the tasks whose done is 1\n");
	list_for_each_safe(pos, next, &task_list) {
		tmp = list_entry(pos, struct task, list);
		if (tmp->done == 1) {
			printf("delete done task %d...\n", tmp->id);
			//list_del_init(pos); //both are ok
			list_del(pos);
			free(tmp);
			tmp = NULL;
		}
	}
	printf("\n");

	list_for_each(pos, &task_list) {
		tmp = list_entry(pos, struct task, list);
		printf("id:%d, done: %d\n", tmp->id, tmp->done);
	}
	printf("\n");

	empty = list_empty(&task_list);
	printf("task list is %sempty\n", (empty == 1) ? "" : "not ");
	printf("\n");


	list_for_each_entry_safe(pos_task, next_task, &task_list, list) {
		if (pos_task->done == 0) {
			printf("delete undone task %d...\n", pos_task->id);
			list_del(&pos_task->list);
			free(pos_task);
			pos_task = NULL;
		}
	}
	printf("\n");

	empty = list_empty(&task_list);
	printf("task list is %sempty\n", (empty == 1) ? "" : "not ");
	printf("\n");


	list_for_each_safe(pos, next, &task_list) {
		printf("the remaining task...\n");
		tmp = list_entry(pos, struct task, list);
		if (tmp) {
			printf("id:%d, done: %d\n", tmp->id, tmp->done);
			list_del_init(pos);
			free(tmp);
			tmp = NULL;
		} else
			printf("tmp is null\n");
	}

	return 0;
}
