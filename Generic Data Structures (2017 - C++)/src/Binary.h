#pragma once
#include <functional>
#include "stdafx.h"
#include "StructureFunctions.h"

template <typename T>
struct BinaryNode
{
	T data;
	BinaryNode<T> *left, *right;

	BinaryNode()
	{
		left = NULL;
		right = NULL;
	}
	BinaryNode(T data)
	{
		BinaryNode::data = data;
		left = NULL;
		right = NULL;	
	}
};

template <typename T>
class Binary
{
	private:
		StructureFunctions<T> *structureFunctions;
		BinaryNode<T> *head;
	private:
		BinaryNode<T>* insert(BinaryNode<T> *node, T data);
		void destroyNode(BinaryNode<T> *node);
		BinaryNode<T>* deleteNode(BinaryNode<T> *node, T *c, bool nofree);
		void inOrder(BinaryNode<T> *node, std::function<void(const T *data)> action);
	public:
		Binary(StructureFunctions<T> *structureFunctions);
		bool insert(T data);
		void destroyTree();
		BinaryNode<T>* search(T data);
		bool deleteNode(T data);
		void runActionOnEveryNode(std::function<void(const T *data)> action);
		bool editNode(T previous, T next);
};