#pragma once
#include "stdafx.h"
#include <functional>
#include "StructureFunctions.h"

// Look at AVL.cpp for implementation
template <typename T>
struct AVL_Node
{
	T data;
	AVL_Node<T> *left, *right;

	AVL_Node()
	{
		c = NULL;
		left = NULL;
		right = NULL;
	}
	AVL_Node(T data)
	{
		AVL_Node::data = data;
		left = NULL;
		right = NULL;
	}
};

template <typename T>
class AVL
{
	private:StructureFunctions<T> *structureFunctions;
			AVL_Node<T> *head;
	private:
		AVL_Node<T>* balanceTree(AVL_Node<T> *node);
		AVL_Node<T>* insert(AVL_Node<T> *node, T data);
		void destroyNode(AVL_Node<T> *node);
		int getDiff(AVL_Node<T> *node);
		int height(AVL_Node<T> *node);
		AVL_Node<T>* rrRotation(AVL_Node<T> *parent);
		AVL_Node<T>* llRotation(AVL_Node<T> *parent);
		AVL_Node<T>* rlRotation(AVL_Node<T> *parent);
		AVL_Node<T>* lrRotation(AVL_Node<T> *parent);
		AVL_Node<T>* deleteNode(AVL_Node<T> *node, T *c, bool nofree);
		void inOrder(AVL_Node<T> *node, std::function<void(const T *data)> action);
	public: //every method or variable declared bellow this will be public until we type "private:"
		AVL(StructureFunctions<T> *structureFunctions);
		bool insert(T data);
		AVL_Node<T>* search(T c);
		bool deleteNode(T c);
		bool editNode(T previous, T next);
		void destroyTree();
		void runActionOnEveryNode(std::function<void(const T *data)> action);
};