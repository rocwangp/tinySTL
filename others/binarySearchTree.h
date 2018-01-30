#pragma once
#include "linkedBinaryTree.h"
#include <utility>

template<class K, class E>
class binarySearchTree : public linkedBinaryTree<pair<const K, E>>
{
public:
	binarySearchTree() : linkedBinaryTree<pair<const K, E>>() {}
	~binarySearchTree() {}

	pair<const K, E>* find(const K&) const;
	void insert(const pair<const K, E>&);
	void erase(const K&);
	void ascend() { inOrder(output); }

	void outputInRange(K theLow, K theHigh) const 
		{ outputInRange(theLow, theHigh, root); }
	pair<const K, E>* split(const K&, binarySearchTree<K, E>&, binarySearchTree<K, E>&);
private:
	static void output(binaryTreeNode<pair<const K, E>>* t);
	static void outputInRange(K, K, binaryTreeNode<pair<const K, E>>*);
	static void split(const K&,
		binarySearchTree<K, E>&, 
		binarySearchTree<K, E>&, 
		binaryTreeNode<pair<const K, E>>*);
};


template<class K, class E>
pair<const K, E>* binarySearchTree<K, E>::find(const K&  theKey) const
{
	binaryTreeNode<pair<const K, E>>* p = root;
	while (p != NULL)
	{
		if (theKey < p->element.first)
			p = p->leftChild;
		else
		{
			if (theKey > p->element.first)
				p = p->rightChild;
			else
				return &p->element;
		}
	}
	return NULL;
}

template<class K, class E>
void binarySearchTree<K, E>::insert(const pair<const K, E>& thePair)
{
	binaryTreeNode<pair<const K, E>>* p = root,
									*pp = NULL;
	while (p != NULL)
	{
		pp = p;
		if (thePair.first < p->element.first)
			p = p->leftChild;
		else
		{
			if (thePair.first > p->element.first)
				p = p->rightChild;
			else
			{
				p->element.second = thePair.second;
				return;
			}
		}
	}
	binaryTreeNode<pair<const K, E>> *newNode
		= new  binaryTreeNode<pair<const K, E>>(thePair);
	if (root != NULL)
	{
		if (thePair.first < pp->element.first)
			pp->leftChild = newNode;
		else
			pp->rightChild = newNode;
	}
	else
		root = newNode;
	treeSize++;
}

template<class K, class E>
void binarySearchTree<K, E>::erase(const K& theKey)
{
	binaryTreeNode<pair<const K, E>>* p = root,
									*pp = NULL;
	while (p != NULL && p->element.first != theKey)
	{
		pp = p;
		if (p->element.first < theKey)
			p = p->rightChild;
		else
			p = p->leftChild;
	}
	if (p == NULL)
		return;

	if(p->leftChild != NULL && p->rightChild != NULL)
	{
		binaryTreeNode<pair<const K, E>>*s = p->leftChild,
			*ps = p;
		while (s->rightChild != NULL)
		{
			ps = s;
			s = s->rightChild;
		}
		binaryTreeNode<pair<const K, E>>* q
			= new (nothrow) binaryTreeNode<pair<const K, E>>
			(s->element, p->leftChild, p->rightChild);
		if (pp == NULL)
			root = q;
		else if (pp->leftChild == p)
			pp->leftChild = q;
		else
			pp->rightChild = q;

		if (ps == p)
			pp = q;
		else
			pp = ps;
		delete p;
		p = s;
	}

	binaryTreeNode<pair<const K, E>> *c;
	if (p->leftChild != NULL)
		c = p->leftChild;
	else
		c = p->rightChild;
	if (p == root)
		root = c;
	else
	{
		if (p == pp->leftChild)
			pp->leftChild = c;
		else
			pp->rightChild = c;
	}
	treeSize--;
	delete p;
}

template<class K, class E>
void binarySearchTree<K, E>::
outputInRange(K theLow, K theHigh, binaryTreeNode<pair<const K, E>>*t)
{
	if (t != NULL)
	{
		outputInRange(theLow, theHigh, t->leftChild);
		if (t->element.first >= theLow && t->element.first <= theHigh)
			output(t);
		outputInRange(theLow, theHigh, t->rightChild);
	}
}

template<class K, class E>
pair<const K, E>* binarySearchTree<K, E>::split
(const K& theKey,
	binarySearchTree<K, E>& lessThan, binarySearchTree<K, E>& greaterThan)
{
	pair<const K, E>* p = find(theKey);
	split(theKey, lessThan, greaterThan, root);
}

template<class K, class E>
void binarySearchTree<K, E>::split
(const K& theKey, 
	binarySearchTree<K, E>& lessThan, binarySearchTree<K, E>& greaterThan, 
	binaryTreeNode<pair<const K,E>>*t)
{
	if (t != NULL)
	{
		split(theKey, lessThan, greaterThan, t->leftChild);
		split(theKey, lessThan, greaterThan, t->rightChild);		
		if (t->element.first > theKey)
			greaterThan.insert(t->element);
		else
			lessThan.insert(t->element);
		delete t;
	}
}

template<class K, class E>
void binarySearchTree<K, E>::output(binaryTreeNode<pair<const K, E>>* pnode)
{
	if (pnode != NULL)
	{
		cout << "the Key: " << pnode->element.first
			<< " the Value: " << pnode->element.second
			<< endl;
	}
}

template<class K,class E>
ostream& operator<<(ostream& out, const pair<K, E>& thePair)
{
	out << "the Key: " << thePair.first
		<< " the Value: " << thePair.second << endl;
	return out;
}
