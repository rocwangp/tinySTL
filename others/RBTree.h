#pragma once
#include "binarySearchTree.h"

#define RED 0
#define BLACK 1

template<class T>
class RBTree : public binarySearchTree<T, int>
{
public:
	RBTree() : binarySearchTree<T, int>() { DBNode = NULL; }
	~RBTree() {}

	void insert(const T& key) { insert(root, key); }
	void remove(const T& key) { remove(root, key); }
	
private:
	binaryTreeNode<pair<const T, int>>* 
		insert(binaryTreeNode<pair<const T, int>>*& ,const T& key);
	binaryTreeNode<pair<const T, int>>* 
		remove(binaryTreeNode<pair<const T, int>>*&, const T& key);

	binaryTreeNode<pair<const T, int>>*
		leftRotation(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		rightRotation(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		leftRightRotation(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		rightLeftRotation(binaryTreeNode<pair<const T, int>>*);

	binaryTreeNode<pair<const T, int>>*
		rModify(binaryTreeNode<pair<const T, int>>*);

	binaryTreeNode<pair<const T, int>>*
		LbModify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		RbModify(binaryTreeNode<pair<const T, int>>*);
	
	binaryTreeNode<pair<const T, int>>*
		rightDBModify(binaryTreeNode<pair<const T, int>>* &, int );
	binaryTreeNode<pair<const T, int>>*
		leftDBModify(binaryTreeNode<pair<const T, int>>* &, int);
	binaryTreeNode<pair<const T, int>>*
		leafModify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		hasOneLeafModify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		DB0Modify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		DB00Modify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		DB01Modify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		DB1Modify(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		DB2Modify(binaryTreeNode<pair<const T, int>>*);


	binaryTreeNode<pair<const T, int>>*
		maximun(binaryTreeNode<pair<const T, int>>*);
	binaryTreeNode<pair<const T, int>>*
		minimun(binaryTreeNode<pair<const T, int>>*);

	int color(binaryTreeNode<pair<const T, int>>*);
	
private:
	binaryTreeNode<pair<const T, int>>* DBNode;
};

//返回节点的颜色
template<class T>
int RBTree<T>::color(binaryTreeNode<pair<const T, int>>* pnode)
{
	if (pnode == NULL)
		return BLACK;
	return pnode->element.second;
}


/************************************************************************/
/* 四种旋转操作：左旋，右旋，先左旋后右旋，先右旋后左旋						*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::leftRotation(binaryTreeNode<pair<const T, int>>* pnode)
{
	binaryTreeNode<pair<const T, int>>* prnode = pnode->rightChild;
	pnode->rightChild = prnode->leftChild;
	prnode->leftChild = pnode;
	return prnode;
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::rightRotation(binaryTreeNode<pair<const T, int>>* pnode)
{
	binaryTreeNode<pair<const T, int>>* plnode = pnode->leftChild;
	pnode->leftChild = plnode->rightChild;
	plnode->rightChild = pnode;
	return plnode;
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::leftRightRotation(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->leftChild = leftRotation(pnode->leftChild);
	return rightRotation(pnode);
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::rightLeftRotation(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->rightChild = rightRotation(pnode->rightChild);
	return leftRotation(pnode);
}


/************************************************************************/
/*                    返回子树中权值最大节点					        	*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::maximun(binaryTreeNode<pair<const T, int>>* pnode)
{
	if (pnode != NULL)
	{
		while (pnode - rightChild != NULL)
			pnode = pnode->rightChild;
		return pnode;
	}
	return NULL;
}

/************************************************************************/
/*                    返回子树中权值最小节点					        	*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::minimun(binaryTreeNode<pair<const T, int>>* pnode)
{
	if (pnode != NULL)
	{
		while (pnode->leftChild != NULL)
			pnode = pnode->leftChild;
		return pnode;
	}
	return NULL;
}



/***********************************************************************************************/
/*            ***********************************************************************          */
/*            *                      插入操作用到的复原平衡操作					    *          */
/*            ***********************************************************************          */
/***********************************************************************************************/



/************************************************************************/
/* 插入节点为pnode->leftChild->leftChild或pnode->leftChild->rightChild	*/
/* color(pnode->leftChild) == RED && color(pnode->rightChild) == RED    */
/************************************************************************/
/*                              或者										*/
/************************************************************************/
/* 插入节点为pnode->rightChild->leftChild或pnode->rightChild->rightChild	*/
/* color(pnode->rightChild) == RED && color(pnode->leftChild) == RED    */
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::rModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->leftChild->element.second = BLACK;
	pnode->rightChild->element.second = BLACK;
	if (pnode != root)
		pnode->element.second = RED;
	return pnode;
}


/************************************************************************/
/* 插入节点为pnode->leftChild->leftChild或pnode->leftChild->rightChild	*/
/* color(pnode->leftChild) == RED && color(pnode->rightChild) == BLACK  */
/* 在进行右旋或者先左旋后右旋操作后，改变颜色								*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::LbModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->element.second = BLACK;
	pnode->rightChild->element.second = RED;
	return pnode;
}

/************************************************************************/
/* 插入节点为pnode->rightChild->leftChild或pnode->rightChild->rightChild	*/
/* color(pnode->rightChild) == RED && color(pnode->leftChild) == BLACK  */
/* 在进行左旋或者先右旋后左旋操作后，改变颜色								*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::RbModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->element.second = BLACK;
	pnode->leftChild->element.second = RED;
	return pnode;
}



/***********************************************************************************************/
/*            ***********************************************************************          */       
/*            *                      删除操作用到的复原平衡操作					    *          */
/*            ***********************************************************************          */
/***********************************************************************************************/


/************************************************************************/
/* 待删除节点有两个外部节点												*/
/* 步骤1：直接把该节点删除，释放内存										*/
/* 步骤2：如果该节点是黑色，则需要在递归中进行“双黑”操作						*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::leafModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	delete pnode;
	return NULL;
}


/************************************************************************/
/* 待删除节点有一个外部节点												*/
/* 步骤1：将内部节点移动到该节点的位置，改变内部节点颜色，令其等于删除节点颜色  */
/* 步骤2：删除待删除节点，释放内存											*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::hasOneLeafModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	binaryTreeNode<pair<const T, int>>* pchild;
	if (pnode->leftChild != NULL)
		pchild = pnode->leftChild;
	else
		pchild = pnode->rightChild;
	pchild->element.second = color(pnode);
	delete pnode;
	return pchild;
}


/**********************************************************/
/*  ****************************************************  */
/*  *                      双黑处理					    * */
/*  ****************************************************  */
/**********************************************************/
/*                         三种情况                        */
/**********************************************************/
/*		1.双黑节点的兄弟节点是黑色，且子节点有红色节点         */                                                    
/*		2.双黑节点的兄弟节点是黑色，且有两个黑色节点		      */
/*		3.双黑节点的兄弟节点是红色							  */
/**********************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::rightDBModify(binaryTreeNode<pair<const T, int>>* &pnode, int pcolor)
{
	//如果右孩子为空且删除之前该节点是黑色，或者右孩子已经判断是双黑节点
	//需要根据左孩子的颜色判断属于双黑处理的哪一种
	if ((pnode->rightChild == NULL && pcolor == BLACK) ||
		(DBNode != NULL && DBNode == pnode->rightChild))
	{	
		//左孩子为黑色，为双黑处理的前两种
		if (color(pnode->leftChild) == BLACK)
		{
			pnode = DB0Modify(pnode);
			pnode = DB1Modify(pnode);
		}
		else
		{
			//第三种处理，左孩子为红色
			pnode = rightRotation(pnode);
			pnode->element.second = BLACK;
			pnode->rightChild->leftChild->element.second = RED;
		}
	}
	return pnode;
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::leftDBModify(binaryTreeNode<pair<const T, int>>* &pnode, int pcolor)
{
	//同rightDBModify
	if ((pnode->leftChild == NULL && pcolor == BLACK) ||
		(DBNode != NULL && DBNode == pnode->leftChild))
	{
		if (color(pnode->rightChild) == BLACK)
		{
			pnode = DB0Modify(pnode);
			pnode = DB1Modify(pnode);
		}
		else
		{
			pnode = leftRotation(pnode);
			pnode->element.second = BLACK;
			pnode->leftChild->rightChild->element.second = RED;
		}
	}
	
	return pnode;
}


/************************************************************************/
/* 双黑节点的兄弟节点是黑色，且子节点有红色节点											
/* 双黑节点远侄子节点为红色
/*（双黑节点若为左孩子，则双黑节点的兄弟节点的右孩子为远侄子节点）  
/*（双黑节点若为右孩子，则双黑节点的兄弟节点的左孩子为远侄子节点）
/************************************************************************/
/* 处理方法：																*/
/* 若双黑节点为左孩子：													*/		
/* 把兄弟节点染为双黑节点的父节点的颜色，把兄弟节点的右孩子染为黑色				*/
/* 再把父节点染为黑色，然后针对父节点进行一次左旋转							*/
/* 若双黑节点为右孩子：												    */
/* 把兄弟节点染为双黑节点的父节点的颜色，把兄弟节点的左孩子染为黑色				*/
/* 再把父节点染为黑色，然后针对父节点进行一次右旋转							*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB00Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//在函数调用之前已经判断若左孩子为NULL，则左孩子是双黑节点
	//或者左孩子在上一次递归中已经判断为双黑节点
	if ((pnode->leftChild == NULL || (DBNode != NULL && DBNode == pnode->leftChild)) && 
		pnode->rightChild->rightChild != NULL)
	{
		if (color(pnode->rightChild->rightChild) == RED)
		{
			pnode->rightChild->element.second = color(pnode);
			pnode->rightChild->rightChild->element.second = BLACK;
			pnode->element.second = BLACK;
			pnode = leftRotation(pnode);
		}
	}
	else if ((pnode->rightChild == NULL || (DBNode != NULL && DBNode == pnode->rightChild)) && 
			  pnode->leftChild->leftChild != NULL)
	{
		if (color(pnode->leftChild->leftChild) == RED)
		{
			pnode->leftChild->element.second = color(pnode);
			pnode->element.second = BLACK;
			pnode->leftChild->leftChild->element.second = BLACK;
			pnode = rightRotation(pnode);
		}
	}
	return pnode;
}


/************************************************************************/
/* 双黑节点的兄弟节点是黑色，且子节点有红色节点
/* 双黑节点近侄子节点为红色
/*（双黑节点若为左孩子，则双黑节点的兄弟节点的左孩子为近侄子节点）
/*（双黑节点若为右孩子，则双黑节点的兄弟节点的右孩子为近侄子节点）
/************************************************************************/
/* 处理方法：																*/
/* 若双黑节点为左孩子：													*/
/* 针对双黑节点的兄弟做一次右旋转，结果使双黑节点的近侄子成为双黑节点的新兄弟   */
/* 将新兄弟节点着为双黑节点父节点的颜色，父节点着为黑色，再针对父节点做左旋转   */
/* 若双黑节点为右孩子：												    */
/* 针对双黑节点的兄弟做一次左旋转，结果使双黑节点的近侄子成为双黑节点的新兄弟   */
/* 将新兄弟节点着为双黑节点父节点的颜色，父节点着为黑色，再针对父节点做右旋转   */
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB01Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//判断条件同DB00Modify
	if ((pnode->leftChild == NULL || (DBNode != NULL && DBNode == pnode->leftChild)) && 
		 pnode->rightChild->leftChild != NULL)
	{
		if (color(pnode->rightChild->leftChild) == RED)
		{
			pnode->rightChild = rightRotation(pnode->rightChild);
			pnode->rightChild->element.second = color(pnode);
			pnode->element.second = BLACK;
			pnode = leftRotation(pnode);
		}
	}
	else if ((pnode->rightChild == NULL || (DBNode != NULL && DBNode == pnode->rightChild)) && 
			pnode->leftChild->rightChild != NULL)
	{
		if (color(pnode->leftChild->rightChild) == RED)
		{
			pnode->leftChild = leftRotation(pnode->leftChild);
			pnode->leftChild->element.second = color(pnode);
			pnode->element.second = BLACK;
			pnode = rightRotation(pnode);
		}
	}
	return pnode;
}


/************************************************************************/
/* 双黑节点的兄弟节点是黑色，且子节点有红色节点								*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB0Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode = DB00Modify(pnode);
	pnode = DB01Modify(pnode);
	return pnode;
}


/************************************************************************/
/* 双黑节点的兄弟节点是黑色，且有两个黑色节点								*/
/* 处理方法：																*/	
/* 把双黑节点的兄弟节点着为红色，双黑节点的父节点着为黑色						*/
/* 若父节点原先为红色，则算法结束；若原先为黑色，则父节点作为双黑节点，继续调整	*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB1Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//左孩子为双黑节点且兄弟有两个黑色节点
	if ((pnode->leftChild == NULL || (DBNode != NULL && DBNode == pnode->leftChild)) &&
		color(pnode->rightChild->leftChild) == BLACK &&
		color(pnode->rightChild->rightChild) == BLACK)
	{
		if (color(pnode) == BLACK)
			DBNode = pnode;
		pnode->rightChild->element.second = RED;
		pnode->element.second = BLACK;
	}
	//同上
	else if ((pnode->rightChild == NULL || (DBNode != NULL && DBNode == pnode->rightChild)) && 
			 color(pnode->leftChild->leftChild) == BLACK &&
			 color(pnode->leftChild->rightChild) == BLACK)
	{
		if (color(pnode) == BLACK)
			DBNode = pnode;
		pnode->leftChild->element.second = RED;
		pnode->element.second = BLACK;
	}
	return pnode;
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB2Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	pnode->element.second = BLACK;
	pnode->leftChild->element.second = RED;
}



template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::insert(binaryTreeNode<pair<const T, int>>* &pnode, const T& key)
{
	if (pnode == NULL)	//申请新节点，空则为黑色，非空则为红色。
	{
		if(root == NULL)
			pnode = new binaryTreeNode<pair<const T, int>>
			(pair<const T, int> (key, BLACK));
		else
			pnode = new binaryTreeNode<pair<const T, int>>
			(pair<const T, int>(key, RED));
	}
	else
	{
		if (key < pnode->element.first)	
		{	//如果key小，则在左子树中插入
			pnode->leftChild = insert(pnode->leftChild, key);
			//左子树中插入后，若左孩子的孩子是红色，则需要进一步判断。
			if ((pnode->leftChild->leftChild != NULL && color(pnode->leftChild->leftChild) == RED) ||
				(pnode->leftChild->rightChild != NULL && color(pnode->leftChild->rightChild) == RED))
			{
				//若左孩子是红色，则需要调整
				if (color(pnode->leftChild) == RED)
				{
					if (color(pnode->rightChild) == RED)	//若右孩子是红色，为第一种情况
						pnode = rModify(pnode);
					else               //否则，为第二种或者第三种
					{
						if (key > pnode->leftChild->element.first)
							pnode = leftRightRotation(pnode);
						else
							pnode = rightRotation(pnode);
						pnode = LbModify(pnode);
					}
				}
			}			
		}
		//与上述对称
		else if (key > pnode->element.first)
		{
			pnode->rightChild = insert(pnode->rightChild, key);
			if ((pnode->rightChild->leftChild != NULL && color(pnode->rightChild->leftChild) == RED) ||
				(pnode->rightChild->rightChild != NULL && color(pnode->rightChild->rightChild) == RED))
			{
				if (color(pnode->rightChild) == RED)
				{
					if (color(pnode->leftChild) == RED)
						pnode = rModify(pnode);
					else
					{
						if (key > pnode->rightChild->element.first)
							pnode = leftRotation(pnode);
						else
							pnode = rightLeftRotation(pnode);
						pnode = RbModify(pnode);
					}
				}
			}	
		}
	}
	return pnode;
}

template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::remove(binaryTreeNode<pair<const T, int>>* &pnode, const T& key)
{
	if (pnode != NULL)
	{
		if (pnode->element.first == key)
		{
			if (pnode->leftChild == NULL && pnode->rightChild == NULL)
			{
				pnode = leafModify(pnode);
			}
			else if ((pnode->leftChild == NULL && pnode->rightChild != NULL) ||
				(pnode->leftChild != NULL && pnode->rightChild == NULL))
			{
				pnode = hasOneLeafModify(pnode);
			}
			else
			{
				//找到该节点右半个子树中权值最小的节点（后继节点），将其权值复制到待删除节点处，但不复制颜色
				//递归删除这个权值最小的节点。因为后继节点没有左孩子，所以转为前两种情况
				binaryTreeNode<pair<const T, int>>* ppre = minimun(pnode->rightChild);
				binaryTreeNode<pair<const T, int>>* temp = pnode;
				pnode = new binaryTreeNode<pair<const T, int>>
					(pair<const T, int>(ppre->element.first, color(pnode)), 
					 pnode->leftChild, pnode->rightChild);
				delete temp;
				int pcolor = color(pnode->rightChild);
				pnode->rightChild = remove(pnode->rightChild, ppre->element.first);
				pnode = rightDBModify(pnode, pcolor);
			}
		}
		else if (key > pnode->element.first)
		{
			//记录下删除前的颜色，因为删除之后有可能变为NULL
			int pcolor = color(pnode->rightChild);
			pnode->rightChild = remove(pnode->rightChild, key);
			pnode = rightDBModify(pnode, pcolor);
		}
		else
		{
			int pcolor = color(pnode->leftChild);
			pnode->leftChild = remove(pnode->leftChild, key);
			pnode = leftDBModify(pnode, pcolor);
		}
	}
	return pnode;
}
