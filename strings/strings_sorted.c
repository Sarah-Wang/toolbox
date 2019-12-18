#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

struct node {
	char *cont;
	struct node *next;
	struct node *prev;
};

struct nlist {
	struct node *head;
	struct node *tail;
	int num;
};


static void insert_node(struct nlist *list, struct node *node)
{
	struct node *tmp = list->head;

	if (!tmp) {
		list->head = list->tail = node;
		list->num++;
		return;
	}
	while (tmp) {
		if (!(strcmp(node->cont, tmp->cont) > 0)) {
			if (!tmp->prev) {
				node->next = tmp;
				node->prev = NULL;
				tmp->prev = node;
				list->head = node;
			} else {
				node->prev = tmp->prev;
				tmp->prev->next = node;
				node->next = tmp;
				tmp->prev = node;
			}
			list->num++;
			break;
		}
		if (!tmp->next) {
			tmp->next = node;
			node->prev = tmp;
			list->tail = node;
			list->num++;
			break;
		}
		tmp = tmp->next;
	}
}

static void get_sorted_string(char *string)
{
	time_t now;
	char ts[128] = {0};
	char nonce[128] = {0};
	char secrt[128] = "f956098b9aeb3851d7c9376826d56538";
	char sn[128] = "RTS00000003";
	char cmd[128] = "register";

	time(&now);
	sprintf(ts, "%ld", now);

	srand(time(NULL));
	sprintf(nonce, "%d", rand());

	struct node n1 = {ts, NULL, NULL};
	struct node n2 = {nonce, NULL, NULL};
	struct node n3 = {secrt, NULL, NULL};
	struct node n4 = {sn, NULL, NULL};
	struct node n5 = {cmd, NULL, NULL};
	struct nlist list = {0};

	insert_node(&list, &n1);
	insert_node(&list, &n2);
	insert_node(&list, &n3);
	insert_node(&list, &n4);
	insert_node(&list, &n5);

	struct node *tmp = list.head;

	while (tmp) {
		strcat(string, tmp->cont);
		tmp = tmp->next;
	}
}

void main(void)
{
	char sorted[1024] = {0};

	get_sorted_string(sorted);
	printf("sorted string: %s\n", sorted);
}

