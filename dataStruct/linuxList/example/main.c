

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "list.h"


typedef struct _dList
{
	int data;
	struct list_head node;
}stdList_t;


static stdList_t gDList;


static void addList(int data)
{
	/* TODO  */
}

static stdList* searchList(int data)
{
	/* TODO */
}

static void listDel(stdList *nodePtr)
{
	/* TODO */
}


int main(void)
{
	stdList_t *nodePtr = NULL;

	INIT_LIST_HEAD(&gDList.node);

	int idx;
	for (idx = 0;idx < 10;idx++){
		/* 添加节点 */
		nodePtr = malloc(sizeof(stdList_t));
		nodePtr->data = idx;
		list_add_tail(&nodePtr->node, &gDList.node);
	}

	stdList_t *posPtr = NULL;

	/* 遍历节点  */
	list_for_each_entry(posPtr, &gDList.node, node){
		printf("%d \n", posPtr->data);
	}

	/* 删除节点 */
	stdList_t *tmpPtr = NULL;
	list_for_each_entry_safe(posPtr, tmpPtr, &gDList.node, node){
		if (9 == posPtr->data){
			list_del_init(&posPtr->node);
			free(posPtr);
		}
	}
	printf("==================================\r\n");
	/* 遍历节点  */
	list_for_each_entry(posPtr, &gDList.node, node){
		printf("%d \n", posPtr->data);
	}


	/* 销毁节点 */
	list_for_each_entry_safe(posPtr, tmpPtr, &gDList.node, node){
			list_del_init(&posPtr->node);
			free(posPtr);
	}

	return 0;
}
