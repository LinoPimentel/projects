#include "stdafx.h"

#pragma once
#include "StructureFunctions.h"
#include "Binary.h"


template <typename T> Binary<T>::Binary(StructureFunctions<T> *structureFunctions)
{
	Binary<T>::structureFunctions = structureFunctions;
	Binary<T>::head = NULL;
}

// O(n)
template <typename T> BinaryNode<T>* Binary<T>::insert(BinaryNode<T> *node, T data)
{
	if (node == NULL)
	{
		node = new BinaryNode<T>(data);
		return node;
	}
	int result = structureFunctions->comparator(&(node->data), &data);
	if (result < 0)
		node->left = insert(node->left, data);
	else if (result > 0)
		node->right = insert(node->right, data);
	else
		return NULL;
	return node;
}

// O(n)
template <typename T> bool Binary<T>::insert(T data)
{
	BinaryNode<T> *temp = Binary<T>::insert(Binary<T>::head, data);
	if (temp == NULL)
		return false;
	Binary<T>::head = temp;
	return true;
}

// O(n)
template<typename T> BinaryNode<T>* Binary<T>::search(T data)
{
	BinaryNode<T> *current = Binary<T>::head;
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

// O(n)
template<typename T> BinaryNode<T>* Binary<T>::deleteNode(BinaryNode<T> *node, T *data, bool nofree)
{
	if (node == NULL)
		return node;
	int result = structureFunctions->comparator(&(node->data), data);
	if (result < 0)
	{
		node->left = deleteNode(node->left, data, nofree);
	}
	else if (result > 0)
	{
		node->right = deleteNode(node->right, data, nofree);
	}
	else
	{
		BinaryNode<T> *temp = node;
		if (node->left == NULL && node->right == NULL) // No child
		{
			if (!nofree)
				structureFunctions->deleteT(&(node->data));
			free(node);
			node = NULL;
		}
		else if (node->left == NULL) // Has right child
		{
			node = node->right;
			if (!nofree)
				structureFunctions->deleteT(&(temp->data));
			free(temp);
		}
		else if (node->right == NULL) //Has left child
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
			node->left = deleteNode(node->left, &(temp->data), true);
		}
	}
	return node;
}

// O(n)
template<typename T> bool Binary<T>::deleteNode(T data)
{
	Binary<T>::deleteNode(Binary<T>::head, &data, false);
	return true;
}

// O(n)
template<typename T> bool Binary<T>::editNode(T previous, T next)
{
	Binary<T>::head = Binary<T>::deleteNode(Binary<T>::head, &previous, true);
	structureFunctions->edit(&previous, &next);
	insert(previous);
	return true;
}

template<typename T> void Binary<T>::destroyTree()
{
	if (Binary<T>::head != NULL)
		destroyNode(Binary<T>::head);
	Binary<T>::head = NULL;
}

template<typename T> void Binary<T>::destroyNode(BinaryNode<T> *node)
{
	if (node != NULL)
	{
		destroyNode(node->left);
		destroyNode(node->right);
		structureFunctions->deleteT(&(node->data));
		free(node);
	}
}

template<typename T> void Binary<T>::inOrder(BinaryNode<T> *node, std::function<void(const T *data)> action)
{
	if (node == NULL)
		return;
	inOrder(node->left, action);
	action(&(node->data));
	if (node == NULL)
		return;
	inOrder(node->right, action);
}

template<typename T> void Binary<T>::runActionOnEveryNode(std::function<void(const T *data)> action)
{
	//default is in-order
	inOrder(Binary<T>::head, action);
}