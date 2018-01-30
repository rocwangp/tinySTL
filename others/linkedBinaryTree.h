/************************************************************************/
/* Author:			Roc_King-Seven                                      */
/* Description:     Two Fork Chain to Achieve the Two Fork Tree         */
/* Date:            2016/12/02                                          */		
/************************************************************************/


#pragma once

#include <ostream>
#include <algorithm>
#include <string>
#include "binaryTreeNode.h"
#include "BinaryTree.h"
#include "arrayQueue.h"
template<class T>
class linkedBinaryTree : public binaryTree<binaryTreeNode<T>>
{
public:
	linkedBinaryTree() :root(NULL), treeSize(0) {}
	linkedBinaryTree(T* theElements, int n);
	linkedBinaryTree(const linkedBinaryTree<T> &);
	~linkedBinaryTree() { erase(); }
	const linkedBinaryTree<T>& operator=(const linkedBinaryTree<T>& );

	bool empty() const { return treeSize == 0; }
	int size() const { return treeSize; }
	void preOrder(void(*theVisit)(binaryTreeNode<T>*))
	{
		visit = theVisit;
		preOrder(root);
	}
	void inOrder(void(*theVisit) (binaryTreeNode<T>*))
	{
		visit = theVisit;
		inOrder(root);
	}
	void postOrder(void(*theVisit)(binaryTreeNode<T>*))
	{
		visit = theVisit;
		postOrder(root);
	}
	void levelOrder(void(*theVisit)(binaryTreeNode<T>*))
	{
		visit = theVisit;
		levelOrder(root);
	}
	void preOrderOutput()
	{
		visit = output;
		preOrder(root);
		cout << endl;
	}
	void levelOrderOutput() 
	{
		visit = output;
		levelOrder(root);
		cout << endl;
	}
	void inOrderOutput() 
	{
		visit = output;
		inOrder(root);
		cout << endl;
	}
	void postOrderOutput() 
	{
		visit = output;
		postOrder(root);
		cout << endl;
	}

	void erase()
	{
		postOrder(dispose);
		root = NULL;
		treeSize = 0;
	}
	int height() const { return height(root); }
	void swapLRTree() { preOrder(swapLRTree); }
	int heightDifference(binaryTreeNode<T>*);
	binaryTreeNode<T>* getRoot() { return root; } //need to be changed

protected:
	binaryTreeNode<T>* root;
	int treeSize;
	static void(*visit)(binaryTreeNode<T>*);
	static void preOrder(binaryTreeNode<T>* t);
	static void inOrder(binaryTreeNode<T>* t);
	static void postOrder(binaryTreeNode<T>* t);
	static void levelOrder(binaryTreeNode<T>* t);
	static void output(binaryTreeNode<T>* t) { cout << t->element << " "; }
	static int height(binaryTreeNode<T>* t);
	static void dispose(binaryTreeNode<T>* t) { delete t; }
	static void copyTree(binaryTreeNode<T>*, binaryTreeNode<T>*);
	static void swapLRTree(binaryTreeNode<T>*);
};
template<typename T>
void(*linkedBinaryTree<T>::visit)(binaryTreeNode<T>*);


template<class T>
linkedBinaryTree<T>::linkedBinaryTree(T* theElements, int n)
{
	treeSize = n;
	root = new binaryTreeNode<T>(theElements[0]);
	binaryTreeNode<T>* targetNode = root;
	arrayQueue<binaryTreeNode<T>*> *q = new arrayQueue<binaryTreeNode<T>*>;
	for (int i = 1; i != n; i++)
	{
		//judge if the node needs to create children
		if (i <= (n - (n + 1) / 2))
		{
			targetNode->leftChild = new binaryTreeNode<T>;
			q->push(targetNode->leftChild);
			//judge if the node needs to create left child
			if (!(i == (n - (n + 1) / 2) && (n + 1) % 2 != 0))
			{
				targetNode->rightChild = new binaryTreeNode<T>;
				q->push(targetNode->rightChild);
			}
		}
		targetNode = q->front();
		q->pop();
		targetNode->element = theElements[i];
	}
	delete q;
}


//copy constructor
template<class T>
linkedBinaryTree<T>::linkedBinaryTree(const linkedBinaryTree<T>& theTree)
{
	root = new binaryTreeNode<T>;
	treeSize = theTree.size();
	copyTree(root, theTree.root);
}

//copy assignment
template<class T>
const linkedBinaryTree<T>&
linkedBinaryTree<T>::operator=(const linkedBinaryTree<T>& theTree)
{
	if(this != &theTree)
	{
		erase();
		//call copy constructor
		this->linkedBinaryTree<T>::linkedBinaryTree(theTree);
	}
	return *this;
}


template<class T>
void linkedBinaryTree<T>::copyTree(binaryTreeNode<T>* targetNode, binaryTreeNode<T>* resourceNode)
{
	if (resourceNode != NULL)
	{
		targetNode->element = resourceNode->element;
		//judge if needing create left and right children
		if (resourceNode->leftChild != NULL)
			targetNode->leftChild = new binaryTreeNode<T>;
		if (resourceNode->rightChild != NULL)
			targetNode->rightChild = new binaryTreeNode<T>;
		copyTree(targetNode->leftChild, resourceNode->leftChild);
		copyTree(targetNode->rightChild, resourceNode->rightChild);
	}

}

template<class T>
void linkedBinaryTree<T>::preOrder( binaryTreeNode<T>* t)
{
	if (t != NULL)
	{
		visit(t);
		preOrder(t->leftChild);
		preOrder(t->rightChild);
	}
}

template<class T>
void linkedBinaryTree<T>::inOrder( binaryTreeNode<T>* t)
{
	if (t != NULL)
	{
		inOrder(t->leftChild);
		visit(t);
		inOrder(t->rightChild);
	}
}

template<class T>
void linkedBinaryTree<T>::postOrder( binaryTreeNode<T>* t)
{
	if (t != NULL)
	{
		postOrder(t->leftChild);
		postOrder(t->rightChild);
		visit(t);
	}
}


template<class T>
void linkedBinaryTree<T>::levelOrder( binaryTreeNode<T>* t)
{
	arrayQueue<binaryTreeNode<T>*> q;
	while (t != NULL)
	{
		visit(t);

		if (t->leftChild != NULL)
			q.push(t->leftChild);
		if (t->rightChild != NULL)
			q.push(t->rightChild);

		if (q.empty())
			return;
		t = q.front();
		q.pop();
	}
}


template<class T>
int linkedBinaryTree<T>::height(binaryTreeNode<T>* t)
{
	if (t == NULL)
		return 0;
	int hl = height(t->leftChild);
	int hr = height(t->rightChild);
	if (hl > hr)
		return ++hl;
	else
		return ++hr;
}




template<class T>
void linkedBinaryTree<T>::swapLRTree(binaryTreeNode<T>* t)
{
	binaryTreeNode<T>* tempNode = t->leftChild;
	t->leftChild = t->rightChild;
	t->rightChild = tempNode;
}

template<class T>
int linkedBinaryTree<T>::heightDifference(binaryTreeNode<T>* t)
{
	int leftHeight = height(t->leftChild);
	int rightHeight = height(t->rightChild);
	/*if (leftHeight > rightHeight)
		return leftHeight - rightHeight;
	else
		return rightHeight - leftHeight;*/
	return leftHeight - rightHeight;
}

//==
template<typename U>
bool operator==(linkedBinaryTree<U>& lhsTree, linkedBinaryTree<U>& rhsTree)
{
	if (lhsTree.size() != rhsTree.size())
		return false;

	arrayQueue<binaryTreeNode<U>*>
		*lq = new arrayQueue<binaryTreeNode<U>*>,
		*rq = new arrayQueue<binaryTreeNode<U>*>;
	binaryTreeNode<U>
		*lhs = lhsTree.getRoot(),
		*rhs = rhsTree.getRoot();
	lq->push(lhs);
	rq->push(rhs);
	binaryTreeNode<U>* ltarget, *rtarget;
	while ((!lq->empty()) && (!rq->empty()))
	{
		ltarget = lq->front();
		rtarget = rq->front();
		lq->pop();
		rq->pop();
		if ((ltarget->element != rtarget->element)
			||
		   ((ltarget->leftChild == NULL || rtarget->leftChild == NULL)&& 
			(ltarget->leftChild != NULL || rtarget->leftChild != NULL))
			||
		   ((ltarget->rightChild == NULL || rtarget->rightChild == NULL)&& 
			(ltarget->rightChild != NULL || rtarget->rightChild != NULL)))
			return false;

		if (ltarget->leftChild != NULL && rtarget->leftChild != NULL)
		{
			lq->push(ltarget->leftChild);
			rq->push(rtarget->leftChild);
		}
		if (ltarget->rightChild != NULL && rtarget->rightChild != NULL)
		{
			lq->push(ltarget->rightChild);
			rq->push(rtarget->rightChild);
		}
	}
	bool lempty = lq->empty(),
		rempty = rq->empty();
	delete lq;
	delete rq;
	if (lempty && rempty)
		return true;
	else
		return false;
}

//!=
template<typename U>
bool operator!=(linkedBinaryTree<U>& lhs, linkedBinaryTree<U>& rhs)
{
	return !(lhs==rhs);
}