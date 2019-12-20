list_head is a simple doubly linked list implementation.


==========Where is it?=============
sarah@Ubuntu1404:/$ uname -a
Linux Ubuntu1404 4.4.0-97-generic #120~14.04.1-Ubuntu SMP Wed Sep 20 15:53:13 UTC 2017 x86_64 x86_64 x86_64 GNU/Linux

$sudo find / -name list.h
./usr/include/libxml2/libxml/list.h
...
./usr/src/linux-headers-4.4.0-97/include/linux/list.h
...
./usr/local/include/libubox/list.h


///////////in linux/types.h ///////////
struct list_head {
	struct list_head *next, *prev;
};

/////////////////////////////////


///////////in linux/list.h ///////////
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}



/////////////////////////////////




==========API=============
//insert new to the beginning of head, for stack
static inline void list_add(struct list_head *new, struct list_head *head)

//insert new to the end of head, for queues
static inline void list_add_tail(struct list_head *new, struct list_head *head)


#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_prev(pos, head) \
	for (pos = (head)->prev; pos != (head); pos = pos->prev)

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)


#define list_for_each_prev_safe(pos, n, head) \
	for (pos = (head)->prev, n = pos->prev; \
	     pos != (head); \
	     pos = n, n = pos->prev)


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))

/**
 * list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_head within the struct.
 */
#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_last_entry(ptr, type, member) \
	list_entry((ptr)->prev, type, member)



static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	WRITE_ONCE(prev->next, next);
}

static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
	entry->next = LIST_POISON1;
	entry->prev = LIST_POISON2;
}

static inline void __list_del_entry(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);
}

static inline void list_del_init(struct list_head *entry)
{
	__list_del_entry(entry);
	INIT_LIST_HEAD(entry);
}


#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
	     &pos->member != (head);					\
	     pos = list_next_entry(pos, member))


#define list_for_each_entry_safe(pos, n, head, member)			\
	for (pos = list_first_entry(head, typeof(*pos), member),	\
		n = list_next_entry(pos, member);			\
	     &pos->member != (head); 					\
	     pos = n, n = list_next_entry(n, member))


static inline int list_empty(const struct list_head *head)
{
	return head->next == head;
}


==========test=============
$ ./list_head_test
task id: 0, task done: 1
task id: 1, task done: 0
task id: 2, task done: 1
task id: 3, task done: 0
task id: 4, task done: 1

delete the tasks whose done is 1
delete done task 0...
delete done task 2...
delete done task 4...

id:1, done: 0
id:3, done: 0

task list is not empty

delete undone task 1...
delete undone task 3...

task list is empty



$ ./list_head_test 1
type: 1
task id: 0, task done: 1
task id: 1, task done: 0
task id: 2, task done: 1
task id: 3, task done: 0
task id: 4, task done: 1

delete the tasks whose done is 1
delete done task 0...
delete done task 2...
delete done task 4...

id:1, done: 0
id:3, done: 0

task list is not empty

delete undone task 1...
delete undone task 3...

task list is empty



