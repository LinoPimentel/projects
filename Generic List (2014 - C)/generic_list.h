struct Members; /*Only need to know it exists, its declared in generic_list.c*/

/*Call the functions in this struct from a struct variable. Eg: lists->insert(data,lists);*/
struct List /* struct given to the user, only has the functions he can use along with a pointer to the structure with all the intern data that the user wont be able to see*/
{
	/*We call this function, it'll return 1 on success and 0 on failure. Its pointer to append function, the append function will insert accounting the data from lesser to greater using the compare function given by the user*/
	char (*insert)(void* data, struct List *list);
	/* We call this function. Its a pointer to search function, returns NULL if it failed. Otherwise returns the data in index*/
	void* (*search)(int index, struct List *list); 
	/*We call this function. Its pointer to delete node function it makes use of the , returns 0 if failed, 1 if succeed, makes use of the delete data function given by the user*/
	char (*remove)(int index, struct List *list); 
	/*We call this function. Simply returns the size of the list*/
	int (*get_size)(struct List *list);
	struct Members *__members; /* pointer to the member struct that holds the data for the list to work with internally, we dont use this*/
};

/*Call this function directly*/
/*initializes function pointers in the list. recieves the methods to delete data and compare data. Puts head and tail on NULL. Returns 1 if success, 0 if failure*/
/*The compare data function needs to return a char and recieve 2 pointers to anything. Return 1 if the data in pointer1 is greater than the date in pointer2*/
/*The delete data function needs to return a char, return 1 if the data was successfully eliminated and 0 if it fails.*/
struct List* New_List(void *compare_data_ptr, void *delete_data_ptr);

/*Call this function directly*/
char Delete_List(struct List **list); /* deletes the whole list and makes the pointer argument NULL*/
