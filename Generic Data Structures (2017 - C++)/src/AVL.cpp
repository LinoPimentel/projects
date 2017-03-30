#include "stdafx.h"
#include "StructureFunctions.h"
#include "AVL.h"
#include <algorithm>
#include <iostream>

/* based on http://code.runnable.com/VUTjJDME6nRv_HOe/delete-node-from-bst-for-c%2B%2B and
// http://www.sanfoundry.com/cpp-program-implement-avl-trees/ */

//O construtor
template <typename T> AVL<T>::AVL(StructureFunctions<T> *structureFunctions)
{
	//AVL<T>::structureFunctions  is the same as doing     this.structureFunctions   in java
	AVL<T>::structureFunctions = structureFunctions;
	AVL<T>::head = NULL;
}

// O(log n)
template <typename T> AVL_Node<T>* AVL<T>::insert(AVL_Node<T> *node, T data)
{
	if (node == NULL)
	{
		node = new AVL_Node<T>(data);
		return node;
	}
	int result = structureFunctions->comparator(&(node->data), &data);
	if (result < 0)
		node->left = insert(node->left, data);
	else if (result > 0)
		node->right = insert(node->right, data);
	else
		return NULL;
	node = balanceTree(node);
	return node;
}

// O(log n)
template <typename T> bool AVL<T>::insert(T data)
{
	AVL_Node<T> *temp =  AVL<T>::insert(AVL<T>::head, data);
	if (temp == NULL)
		return false;
	AVL<T>::head = temp;
	return true;
}

// O(log n)
template<typename T> AVL_Node<T>* AVL<T>::search(T data)
{
	AVL_Node<T> *current = AVL<T>::head;
	while (current != NULL)
	{
		int result = structureFunctions->comparator(&(current->data), &data);
		if (result < 0)
		{
			current = current->left;
			continue;
		}
		else if (result > 0)
		{
			current = current->right;
			continue;
		}
		else
			return current;
	}
	return NULL;
}

// rotations
template<typename T> AVL_Node<T>* AVL<T>::rrRotation(AVL_Node<T> *parent)
{
	AVL_Node<T> *node = parent->right;
	parent->right = node->left;
	node->left = parent;
	return node;
}
template<typename T> AVL_Node<T>* AVL<T>::llRotation(AVL_Node<T> *parent)
{
	AVL_Node<T> *node = parent->left;
	parent->left = node->right;
	node->right = parent;
	return node;
}
template<typename T> AVL_Node<T>* AVL<T>::rlRotation(AVL_Node<T> *parent)
{
	AVL_Node<T> *node = parent->right;
	parent->right = AVL<T>::llRotation(node);
	return AVL<T>::rrRotation(parent);
}
template<typename T> AVL_Node<T>* AVL<T>::lrRotation(AVL_Node<T> *parent)
{
	AVL_Node<T> *node = parent->left;
	parent->left = AVL<T>::rrRotation(node);
	return AVL<T>::llRotation(parent);
}

template<typename T> int AVL<T>::height(AVL_Node<T> *node)
{
	if (node != NULL)
	{
		int lHeight = AVL<T>::height(node->left);
		int rHeight = AVL<T>::height(node->right);
		return 1 + std::max(lHeight, rHeight);
	}
	return 0;
}

template<typename T> int AVL<T>::getDiff(AVL_Node<T> *node)
{
	int lHeight = height(node->left), rHeight = height(node->right);
	return lHeight - rHeight;	 
}

//O(n)
template<typename T> AVL_Node<T>* AVL<T>::balanceTree(AVL_Node<T> *node)
{
	int diff = AVL::getDiff(node);
	if (diff > 1)
	{
		if (AVL<T>::getDiff(node->left) > 0)
			node = AVL<T>::llRotation(node);
		else
			node = AVL<T>::lrRotation(node);
	}
	else if (diff < -1)
	{
		if (AVL<T>::getDiff(node->right) > 0)
			node = AVL<T>::rlRotation(node);
		else
			node = AVL<T>::rrRotation(node);
	}
	return node;
}

// O(log n + n)
template<typename T> AVL_Node<T>* AVL<T>::deleteNode(AVL_Node<T> *node, T *data, bool nofree)
{
	if (node == NULL)
		return node;
	int result = structureFunctions->comparator(&(node->data), data);
	if (result < 0)
	{ 
		node->left = deleteNode(node->left, data, nofree);
		node = balanceTree(node);
	}
	else if(result > 0)
	{
		node->right = deleteNode(node->right, data, nofree);
		node = balanceTree(node);
	}
	else 
	{
		AVL_Node<T> *temp = node;
		if (node->left == NULL && node->right == NULL) // No child
		{
			if(!nofree)
				structureFunctions->deleteT(&(node->data));
			free(node);
			node = NULL;
		}
		else if(node->left == NULL) // Has right child
		{
			node = node->right;
			if (!nofree)
				structureFunctions->deleteT(&(temp->data));
			free(temp);
		}
		else if(node->right == NULL) //Has left child
		{
			node = node->left;
			if (!nofree)
				structureFunctions->deleteT(&(temp->data));
			free(temp);
		}
		else
		{
			temp = node->left;
			while (temp->right != NULL)
				temp = temp->right;
			node->data = temp->data;
			node->left = deleteNode(node->left, &(temp->data),true);
		}
	}
	return node;
}

// O(log n + n)
template<typename T> bool AVL<T>::deleteNode(T data)
{
	AVL<T>::head = AVL<T>::deleteNode(AVL<T>::head, &data, false);
	if(AVL<T>::head != NULL)
		AVL<T>::head = AVL<T>::balanceTree(AVL<T>::head);
	return true;
}

// O(log n + n)
template<typename T> bool AVL<T>::editNode(T previous, T next)
{
	AVL<T>::head = AVL<T>::deleteNode(AVL<T>::head, &previous, true);
	if (AVL<T>::head != NULL)
		AVL<T>::head = AVL<T>::balanceTree(AVL<T>::head);
	structureFunctions->edit(&previous, &next);
	insert(previous);
	return true;
}

template<typename T> void AVL<T>::destroyTree()
{
	if (AVL<T>::head != NULL)
		destroyNode(AVL<T>::head);
	AVL<T>::head = NULL;
}

template<typename T> void AVL<T>::destroyNode(AVL_Node<T> *node)
{
	if (node != NULL)
	{
		destroyNode(node->left);
		destroyNode(node->right);
		structureFunctions->deleteT(&(node->data));
		free(node);
	}
}

template<typename T> void AVL<T>::inOrder(AVL_Node<T> *node, std::function<void(const T *data)> action)
{

	if (node == NULL)
		return;
	inOrder(node->left, action);
	action(&(node->data));
	if (node == NULL)
		return;
	inOrder(node->right, action);
}

template<typename T> void AVL<T>::runActionOnEveryNode(std::function<void(const T *data)> action)
{
	//default is in-order
	inOrder(AVL<T>::head, action);
}