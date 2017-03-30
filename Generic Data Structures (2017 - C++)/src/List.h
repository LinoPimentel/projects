#pragma once
#include <functional>
#include "stdafx.h"
#include "StructureFunctions.h"

template <typename T>
struct ListNode
{
	T data;
	ListNode<T> *next;

	ListNode(T data)
	{
		ListNode<T>::data = data;
		ListNode<T>::next = NULL;
	}
};

// Look at List.cpp for implementation
template <typename T>
class List
{
	private:
		StructureFunctions<T> *structureFunctions;
		ListNode<T> *head, *tail;
		int size;
	public:
		List(StructureFunctions<T> *structureFunctions);
		List();
		ListNode<T>* getNodeAt(int index);
		bool insert(T data);
		bool destroyList();
		ListNode<T>* search(T data);
		int getSize();
		bool deleteNode(T data);
		void runActionOnEveryNode(std::function<void(const T *data)> action);
		bool edit(T prev, T next);
};