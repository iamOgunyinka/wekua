#ifndef WEKUA_LINKED_LIST_H
#define WEKUA_LINKED_LIST_H

typedef struct _w_linked_list_node_t {
	struct _w_linked_list_node_t *prev;
	struct _w_linked_list_node_t *next;
	void *data;
} *wlinked_list_node;

typedef struct _w_linked_list_t {
	wlinked_list_node first; // First node
	wlinked_list_node last; // Last node
} *wlinked_list;

wlinked_list wekuaAllocLinkedList();

void wekuaLinkedListAppend(wlinked_list list, void *data);
void wekuaLinkedListAppendLeft(wlinked_list list, void *data);

void *wekuaLinkedListPop(wlinked_list list);
void *wekuaLinkedListPopLeft(wlinked_list list);

void wekuaFreeLinkedList(wlinked_list list);

#endif