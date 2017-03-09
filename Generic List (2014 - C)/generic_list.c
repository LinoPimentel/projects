#include "generic_list.h"
#include <stdlib.h>

/*STRUCT DECLARATIONS*/
/*This struct holds the minimum ammount of variables necessary in each instance*/
struct Nodes /*This is the struct that holds the next and the pointer to the data, this data can be anything, the functions in this file dont need to know what it is*/
{
	struct Nodes *next;
	void* data;
};

/*Contains the pointer to the first and last element of the list, they're in this struct to avoid being duplicated in the "Nodes" struct*/
/*It also contains the size variable, the pointers to the compare and delete data passed by the user in the "new_list" function*/
/*By having the compare and free data function pointers here, we can have multiple lists using different sorting methods and also correctly freeing data if it was allocd dinamicly*/
/*the last_access variables are for an optimization. Whenever we want to delete, search or insert a node, we first check if the targeted node is in front or behind the last accessed node
/*this way if we want to search the whole list from 0 to size, it will only pass by each node once*/
struct Members
{
	struct Nodes *head; /* pointer to the head*/
	struct Nodes *tail; /* pointer to the tail*/
	int size;
	struct Nodes *last_access;
	int last_index_access;
	char (*free_data)(void *data); /* pointer to delete node function, given by the user since we dont know how to delete their data, returns 0 if failed, 1 if succeed*/
	char (*compare_data)(void *node1, void* node2); /* given by the user return 1 if node1 >= node2 and 0 if node1 <= node2*/
};

char Insert(void *data, struct List *myList);
void* Search(int index, struct List *myList);
char Free_Node(int index, struct List *myList);
int Get_Size(struct List *list);

/*Function called directly by the user, it iterates through the entire list passed and deletes every node. Making use of the free_data function passed by the user when creating the list*/
char Delete_List(struct List **list)
{
	struct List *mylist = *list;
	if(*list == NULL || mylist->__members == NULL)
		return 0;

	while(mylist->__members->head != NULL)
	{
		if(!mylist->remove(0,mylist))
			return 0;
	}
	free(mylist->__members);
	free(mylist);
	*list = NULL; /* makes list point to NULL to prevent crashes*/
	return 1;
}

/*This recieves the compare and delete data function pointers and assigns them to a list*/
/*It initializes all the necessary varibles*/
struct List* New_List(void *compare_data_ptr, void *delete_data_ptr)
{
    struct List *new_list; /* allocates a new list*/
	if(compare_data_ptr == NULL || delete_data_ptr == NULL)
		return NULL; /* returns null if any of the target functions were NULL to prevent crashes*/
	new_list = (struct List*)malloc(sizeof(struct List));
	new_list->__members = (struct Members*)malloc(sizeof(struct Members));
	new_list->insert = Insert;
	new_list->__members->compare_data = (char (__cdecl *)(void *,void *))compare_data_ptr; /* cast to function pointer*/
	new_list->search = Search;
	new_list->__members->free_data = (char (__cdecl *)(void *))delete_data_ptr;
	new_list->remove = Free_Node;
	new_list->get_size = Get_Size;
	new_list->__members->size = 0;
	new_list->__members->head = NULL;
	new_list->__members->tail = NULL;
	new_list->__members->last_index_access = -1;
	new_list->__members->last_access = new_list->__members->head;

	return new_list; /* returns the initialized list*/
}

/*Recieves a pointer to the data to insert as well as the list where to insert
it makes use of the compare function to determine where to insert the data
For performance, it first checks if head has been created or not, then checks if its to insert behind or after the head.
If its after, then checks if its to insert before or after the tail. If its before then it finally checks if its before or after the last access.
If its after, it starts checking where to insert starting from the last access, if its before it starts from the start of the list.
The last access gets overwriten by the last node to be accessed*/
char Insert(void *data, struct List *list)
{
	struct Nodes *current, *aux;

	if(list == NULL)
		return 0;

	if(list->__members->head == NULL) /* checks if the head has been created or not*/
	{
		list->__members->head = (struct Nodes*)malloc(sizeof(struct Nodes)); /* allocates the head if it hasnt been created*/
		list->__members->tail = list->__members->head; /* changes the tail pointer to the new head*/
		list->__members->tail->next = NULL;
		list->__members->tail->data = data;
		list->__members->last_index_access = 0;
		list->__members->last_access = list->__members->head;
	}
	else /* the head has already been created*/
	{
		if(list->__members->compare_data(data,list->__members->tail->data)) /*Check if data to append is greater than data in tail, if so, append at the end of tail*/
		{
			list->__members->tail->next = (struct Nodes*)malloc(sizeof(struct Nodes)); /* allocate a new node*/
			list->__members->last_access = list->__members->tail; /* makes last access into the previous tail*/
			list->__members->tail = list->__members->tail->next; /* changes the tail to the new allocated node*/
			list->__members->tail->next = NULL; /* makes NEXT into null to prevent crashes*/
			list->__members->tail->data = data; /* assigns data to new allocated node*/
			list->__members->last_index_access = list->__members->size-1;
		}
		else if(list->__members->compare_data(list->__members->head->data,data)) /* check if data in head is greater than data to append, if so, append before the head*/
		{
			aux = list->__members->head; /* saves the current head*/
			list->__members->head = (struct Nodes*)malloc(sizeof(struct Nodes)); /* allocates the new node as the new head*/
			list->__members->head->next = aux; /* next in the new head now points to the old head*/
			list->__members->head->data = data; /* assigns the desired data*/
			list->__members->last_index_access = 1; /* makes the last access into the second node, since we gain nothing from putting last access into head*/
			list->__members->last_access = list->__members->head->next;
		}
		else /* if it enters here, then the data to append is greater than the head and lesser than the tail, so we need to go through the whole list looking for the right spot*/
		{
			/*makes sure last access is not null and if the last accessed node data is the same or lesser than the new data. then start searching at that index*/
			if(list->__members->last_access != NULL && list->__members->compare_data(data,list->__members->last_access->data))
				current = list->__members->last_access;
			else /* else start searching at the start*/
			{
				current = list->__members->head;
				list->__members->last_index_access = 0;
			}
			while(list->__members->compare_data(data,current->next->data)) /* will always stop at most at current->next being the tail from the filters above.*/
			{
				current = current->next;
				list->__members->last_index_access++;
			}
			list->__members->last_access = current; /* overwrites the last access for future uses.*/
			/*once it reaches here, we have determined the 2 nodes where our new node should be in between, the current and the current->next*/
			aux = current->next; /* saves the pointer to next node so we dont lose it*/
			current->next = (struct Nodes*)malloc(sizeof(struct Nodes)); /* allocates new node*/
			current->next->data = data; /* assigns the data*/
			current->next->next = aux; /* assigns the old next to this one*/
		}
	}
	list->__members->size++;
	return 1;
}

/*Recieves a pointer to the index to search as well as which list to search in
For performance it first checks if the index is valid, if so it checks if the index to search is the last index or after, if so, it starts checking from there, if not it starts from the head
The last access gets overwriten by the last node to be accessed*/
void* Search(int index, struct List *list)
{
    int i;
    struct Nodes *current;
	if(index<0 || list == NULL || index >= list->__members->size)
       return NULL;

	/* makes sure the last index access is still valid and checks if the last searched index was lesser or the same as the desired to optimize searches*/
	if(list->__members->last_index_access<=index && list->__members->last_index_access < list->__members->size && list->__members->last_access != NULL)
	{ /* also makes sure that the last access is not null*/
		i = list->__members->last_index_access;
		current = list->__members->last_access;
	}
	else
	{
		if(list->__members->head == NULL)
			return NULL;
		current = list->__members->head; /* Starts searching in the Head*/
		i = 0;
	}
    for(i;i<index;i++)
    {
		if(current->next == NULL) /* makes sure there are no crashes*/
			return NULL;
        current = current->next; /* iterates 1 node in front*/
    }
	list->__members->last_access = current; /*overwrites last access member and index*/
	list->__members->last_index_access = index;
    return current->data;

}

/*Recieves a pointer to the index to delete as well as which list to delete in
For performance it first checks if the index is valid, if so it checks if the one to delete is the head or the tail.
If its neither it checks the last index (it cant be the last access itself because we need to have the pointer to the previous as well).
The node previous to the one deleted is now the new last access. Its the previous isntead of the next to have a wider range in the next operation
*/
char Free_Node(int index, struct List *list)
{
	 /* will always try to delete the data before reassiging the list*/
	int i;
	struct Nodes *current, *aux;

	if(list == NULL || index<0)
		return 0; /* returns 0 if the list doesnt exist or if index is negative*/

	current = list->__members->head; /* makes current into the first element, the head*/
	if(current == NULL)
		return 0;

	if(index == 0) /* to delete the head, enters only if head is different than NULL*/
	{
		if(current->next != NULL) /* checks if there are more nodes other than the head*/
		{
			if(!list->__members->free_data(current->data)) /* frees the data first*/
				return 0; /* data freeing failed, cant delete*/
			list->__members->head = current->next; /* head is now the second node, tail stays the same*/
			free(current); /* frees the node*/
			if(index == list->__members->last_index_access) /* assigns the last access to the head to prevent crashes*/
				list->__members->last_access = list->__members->head;
		}
		else /* the head is the only node, deletes and resets head and tail and last access*/
		{
			if(!list->__members->free_data(current->data))
				return 0;
			free(current);
			list->__members->head = NULL;
			list->__members->tail = NULL;
			list->__members->last_access = NULL;
			list->__members->last_index_access = 0;
		}
	}
	else /* didnt want to delete the head*/
	{
		/* checks if we should start at last access or at beggining*/
		if(list->__members->last_access != NULL && list->__members->last_index_access+1 < list->__members->size && index > list->__members->last_index_access)
		{
			i = list->__members->last_index_access;
			current = list->__members->last_access; /* if it was valid and it wasnt the tail, start at last access else start at head*/
		}
		else
		{
			i = 0;
			current = list->__members->head;
		}
		for(i;i<index-1;i++) /* iterates to the node previous to the one he wants to delete*/
		{
			if(current->next == NULL)
				return 0;
			current = current->next;
		}
		if(current->next != NULL) /* makes sure the node we want to delete exists*/
		{
			if(current->next == list->__members->tail) /* checks if that node is the tail*/
			{
				if(!list->__members->free_data(current->next->data)) /* tries to delete the data first*/
					return 0;
				list->__members->tail = current; /* tail is now the node previous to the one we're going to delete*/
				free(current->next); /* deletes the node*/
				current->next = NULL; /*puts next as null to prevent crashes*/
			}
			else /* node is not the tail, meaning there are more nodes in front*/
			{
				if(!list->__members->free_data(current->next->data))
					return 0;
				aux = current->next; /*saves the node we want to delete so we can free him later*/
				current->next = current->next->next; /* the previous one points to the one in front now*/
				free(aux);
			}
			list->__members->last_access = current;
			list->__members->last_index_access = i;
		}
	}
	list->__members->size--;
	return 1; /* returns success*/
}

/*Simply returns the size of given list*/
int Get_Size(struct List *list)
{
	return list->__members->size;
}
