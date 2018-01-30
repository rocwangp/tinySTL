#pragma once


template<class T>
struct binaryTreeNode
{
	T element;
	binaryTreeNode* leftChild;
	binaryTreeNode* rightChild;

	binaryTreeNode() : leftChild(NULL), rightChild(NULL) {} 
	binaryTreeNode(const T& theElement) :
		element(theElement), leftChild(NULL), rightChild(NULL) {}
	binaryTreeNode(const T& theElement, binaryTreeNode<T>* lhs, binaryTreeNode<T>* rhs) :
		element(theElement), leftChild(lhs), rightChild(rhs)
	{}
};
