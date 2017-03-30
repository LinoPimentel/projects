#include "stdafx.h"

#include "StructureFunctions.h"
#include "List.h"

template <typename T> List<T>::List(StructureFunctions<T> *structureFunctions)
{
	List<T>::structureFunctions = structureFunctions;
	List<T>::head = NULL;
	List<T>::size = 0;
}
template <typename T> List<T>::List()
{
	List<T>::structureFunctions = NULL;
	List<T>::head = NULL;
	List<T>::size = 0;
}

// O(n)
template <typename T> ListNode<T>* List<T>::getNodeAt(int index)
{
	if (index < List<T>::size)
	{
		int current = 0;
		ListNode<T> *temp = List<T>::head;
		while (current != index)
		{
			temp = temp->next;
			current++;
		}
		return temp;
	}
	return NULL;
}

// O(1)
template <typename T> bool List<T>::insert(T data)
{
	List<T>::size++;
	if (List<T>::head == NULL)
	{ 
		List<T>::head = new ListNode<T>(data);
		List<T>::tail = List<T>::head;
		return true;
	}
	if (List<T>::head == List<T>::tail)
	{
		List<T>::head->next = new ListNode<T>(data);
		List<T>::tail = List<T>::head->next;
		return true;
	}
	List<T>::tail->next = new ListNode<T>(data);
	List<T>::tail = List<T>::tail->next;
	return true;
}

// O(n) --?
template <typename T> bool List<T>::deleteNode(T data)
{
	if (size == 0)
		return true;
	ListNode<T> *temp = List<T>::head;
	if (!structureFunctions->comparator(&(temp->data), &data))
	{
		temp = temp->next;
		structureFunctions->deleteT(&(List<T>::head->data));
		free(List<T>::head);
		List<T>::head = temp;
		if(size == 1) // means we deleted head as the only node
			List<T>::tail = NULL;
		size--;
		return true;
	}
	while (temp->next != NULL)
	{
		int res = structureFunctions->comparator(&(temp->next->data), &data);
		if (!res)
		{
			ListNode<T> *aux = temp->next->next;
			if (aux == NULL)
				List<T>::tail = temp;
			structureFunctions->deleteT(&(temp->next->data));
			free(temp->next);
			temp->next = aux;
			size--;
			return true;
		}
		temp = temp->next;
	}
	return false; // couldnt delete sicne it data didnt exist
}

// O(n)
template <typename T> ListNode<T>* List<T>::search(T data)
{
	ListNode<T> *temp = List<T>::head;
	for (int i = 0; i < List<T>::size && temp != NULL; i++, temp = temp->next)
	{
		if (!structureFunctions->comparator(&(temp->data), &data))
			return temp;
	}
	return NULL;
}

template <typename T> bool List<T>::edit(T prev, T next)
{
	ListNode<T> *temp = List<T>::head;
	for (int i = 0; i < List<T>::size && temp != NULL; i++, temp = temp->next)
	{
		if (List<T>::structureFunctions->comparator(&prev, &(temp->data)) == 0)
		{
			structureFunctions->edit(&(temp->data), &next);
			return true;
		}
	}
	return false;
}

template <typename T> bool List<T>::destroyList()
{
	ListNode<T> *temp = List<T>::head, *next;
	while (temp != NULL)
	{
		next = temp->next;
		if (structureFunctions != NULL)
			structureFunctions->deleteT(&(temp->data));
		free(temp);
		temp = next;
	}
	List<T>::size = 0;
	List<T>::head = NULL;
	List<T>::tail = NULL;
	return true;
}

template <typename T> void List<T>::runActionOnEveryNode(std::function<void(const T *data)> action)
{
	ListNode<T> *temp = List<T>::head;
	for (int i = 0; i < List<T>::size && temp != NULL; i++, temp = temp->next)
	{
		action(&(temp->data));
	}
}

template <typename T> int List<T>::getSize()
{
	return List<T>::size;
}