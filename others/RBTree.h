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

//���ؽڵ����ɫ
template<class T>
int RBTree<T>::color(binaryTreeNode<pair<const T, int>>* pnode)
{
	if (pnode == NULL)
		return BLACK;
	return pnode->element.second;
}


/************************************************************************/
/* ������ת��������������������������������������������						*/
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
/*                    ����������Ȩֵ���ڵ�					        	*/
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
/*                    ����������Ȩֵ��С�ڵ�					        	*/
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
/*            *                      ��������õ��ĸ�ԭƽ�����					    *          */
/*            ***********************************************************************          */
/***********************************************************************************************/



/************************************************************************/
/* ����ڵ�Ϊpnode->leftChild->leftChild��pnode->leftChild->rightChild	*/
/* color(pnode->leftChild) == RED && color(pnode->rightChild) == RED    */
/************************************************************************/
/*                              ����										*/
/************************************************************************/
/* ����ڵ�Ϊpnode->rightChild->leftChild��pnode->rightChild->rightChild	*/
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
/* ����ڵ�Ϊpnode->leftChild->leftChild��pnode->leftChild->rightChild	*/
/* color(pnode->leftChild) == RED && color(pnode->rightChild) == BLACK  */
/* �ڽ����������������������������󣬸ı���ɫ								*/
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
/* ����ڵ�Ϊpnode->rightChild->leftChild��pnode->rightChild->rightChild	*/
/* color(pnode->rightChild) == RED && color(pnode->leftChild) == BLACK  */
/* �ڽ����������������������������󣬸ı���ɫ								*/
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
/*            *                      ɾ�������õ��ĸ�ԭƽ�����					    *          */
/*            ***********************************************************************          */
/***********************************************************************************************/


/************************************************************************/
/* ��ɾ���ڵ��������ⲿ�ڵ�												*/
/* ����1��ֱ�ӰѸýڵ�ɾ�����ͷ��ڴ�										*/
/* ����2������ýڵ��Ǻ�ɫ������Ҫ�ڵݹ��н��С�˫�ڡ�����						*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::leafModify(binaryTreeNode<pair<const T, int>>* pnode)
{
	delete pnode;
	return NULL;
}


/************************************************************************/
/* ��ɾ���ڵ���һ���ⲿ�ڵ�												*/
/* ����1�����ڲ��ڵ��ƶ����ýڵ��λ�ã��ı��ڲ��ڵ���ɫ���������ɾ���ڵ���ɫ  */
/* ����2��ɾ����ɾ���ڵ㣬�ͷ��ڴ�											*/
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
/*  *                      ˫�ڴ���					    * */
/*  ****************************************************  */
/**********************************************************/
/*                         �������                        */
/**********************************************************/
/*		1.˫�ڽڵ���ֵܽڵ��Ǻ�ɫ�����ӽڵ��к�ɫ�ڵ�         */                                                    
/*		2.˫�ڽڵ���ֵܽڵ��Ǻ�ɫ������������ɫ�ڵ�		      */
/*		3.˫�ڽڵ���ֵܽڵ��Ǻ�ɫ							  */
/**********************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::rightDBModify(binaryTreeNode<pair<const T, int>>* &pnode, int pcolor)
{
	//����Һ���Ϊ����ɾ��֮ǰ�ýڵ��Ǻ�ɫ�������Һ����Ѿ��ж���˫�ڽڵ�
	//��Ҫ�������ӵ���ɫ�ж�����˫�ڴ������һ��
	if ((pnode->rightChild == NULL && pcolor == BLACK) ||
		(DBNode != NULL && DBNode == pnode->rightChild))
	{	
		//����Ϊ��ɫ��Ϊ˫�ڴ����ǰ����
		if (color(pnode->leftChild) == BLACK)
		{
			pnode = DB0Modify(pnode);
			pnode = DB1Modify(pnode);
		}
		else
		{
			//�����ִ�������Ϊ��ɫ
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
	//ͬrightDBModify
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
/* ˫�ڽڵ���ֵܽڵ��Ǻ�ɫ�����ӽڵ��к�ɫ�ڵ�											
/* ˫�ڽڵ�Զֶ�ӽڵ�Ϊ��ɫ
/*��˫�ڽڵ���Ϊ���ӣ���˫�ڽڵ���ֵܽڵ���Һ���ΪԶֶ�ӽڵ㣩  
/*��˫�ڽڵ���Ϊ�Һ��ӣ���˫�ڽڵ���ֵܽڵ������ΪԶֶ�ӽڵ㣩
/************************************************************************/
/* ��������																*/
/* ��˫�ڽڵ�Ϊ���ӣ�													*/		
/* ���ֵܽڵ�ȾΪ˫�ڽڵ�ĸ��ڵ����ɫ�����ֵܽڵ���Һ���ȾΪ��ɫ				*/
/* �ٰѸ��ڵ�ȾΪ��ɫ��Ȼ����Ը��ڵ����һ������ת							*/
/* ��˫�ڽڵ�Ϊ�Һ��ӣ�												    */
/* ���ֵܽڵ�ȾΪ˫�ڽڵ�ĸ��ڵ����ɫ�����ֵܽڵ������ȾΪ��ɫ				*/
/* �ٰѸ��ڵ�ȾΪ��ɫ��Ȼ����Ը��ڵ����һ������ת							*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB00Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//�ں�������֮ǰ�Ѿ��ж�������ΪNULL����������˫�ڽڵ�
	//������������һ�εݹ����Ѿ��ж�Ϊ˫�ڽڵ�
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
/* ˫�ڽڵ���ֵܽڵ��Ǻ�ɫ�����ӽڵ��к�ɫ�ڵ�
/* ˫�ڽڵ��ֶ�ӽڵ�Ϊ��ɫ
/*��˫�ڽڵ���Ϊ���ӣ���˫�ڽڵ���ֵܽڵ������Ϊ��ֶ�ӽڵ㣩
/*��˫�ڽڵ���Ϊ�Һ��ӣ���˫�ڽڵ���ֵܽڵ���Һ���Ϊ��ֶ�ӽڵ㣩
/************************************************************************/
/* ��������																*/
/* ��˫�ڽڵ�Ϊ���ӣ�													*/
/* ���˫�ڽڵ���ֵ���һ������ת�����ʹ˫�ڽڵ�Ľ�ֶ�ӳ�Ϊ˫�ڽڵ�����ֵ�   */
/* �����ֵܽڵ���Ϊ˫�ڽڵ㸸�ڵ����ɫ�����ڵ���Ϊ��ɫ������Ը��ڵ�������ת   */
/* ��˫�ڽڵ�Ϊ�Һ��ӣ�												    */
/* ���˫�ڽڵ���ֵ���һ������ת�����ʹ˫�ڽڵ�Ľ�ֶ�ӳ�Ϊ˫�ڽڵ�����ֵ�   */
/* �����ֵܽڵ���Ϊ˫�ڽڵ㸸�ڵ����ɫ�����ڵ���Ϊ��ɫ������Ը��ڵ�������ת   */
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB01Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//�ж�����ͬDB00Modify
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
/* ˫�ڽڵ���ֵܽڵ��Ǻ�ɫ�����ӽڵ��к�ɫ�ڵ�								*/
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
/* ˫�ڽڵ���ֵܽڵ��Ǻ�ɫ������������ɫ�ڵ�								*/
/* ��������																*/	
/* ��˫�ڽڵ���ֵܽڵ���Ϊ��ɫ��˫�ڽڵ�ĸ��ڵ���Ϊ��ɫ						*/
/* �����ڵ�ԭ��Ϊ��ɫ�����㷨��������ԭ��Ϊ��ɫ���򸸽ڵ���Ϊ˫�ڽڵ㣬��������	*/
/************************************************************************/
template<class T>
binaryTreeNode<pair<const T, int>>*
RBTree<T>::DB1Modify(binaryTreeNode<pair<const T, int>>* pnode)
{
	//����Ϊ˫�ڽڵ����ֵ���������ɫ�ڵ�
	if ((pnode->leftChild == NULL || (DBNode != NULL && DBNode == pnode->leftChild)) &&
		color(pnode->rightChild->leftChild) == BLACK &&
		color(pnode->rightChild->rightChild) == BLACK)
	{
		if (color(pnode) == BLACK)
			DBNode = pnode;
		pnode->rightChild->element.second = RED;
		pnode->element.second = BLACK;
	}
	//ͬ��
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
	if (pnode == NULL)	//�����½ڵ㣬����Ϊ��ɫ���ǿ���Ϊ��ɫ��
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
		{	//���keyС�������������в���
			pnode->leftChild = insert(pnode->leftChild, key);
			//�������в���������ӵĺ����Ǻ�ɫ������Ҫ��һ���жϡ�
			if ((pnode->leftChild->leftChild != NULL && color(pnode->leftChild->leftChild) == RED) ||
				(pnode->leftChild->rightChild != NULL && color(pnode->leftChild->rightChild) == RED))
			{
				//�������Ǻ�ɫ������Ҫ����
				if (color(pnode->leftChild) == RED)
				{
					if (color(pnode->rightChild) == RED)	//���Һ����Ǻ�ɫ��Ϊ��һ�����
						pnode = rModify(pnode);
					else               //����Ϊ�ڶ��ֻ��ߵ�����
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
		//�������Գ�
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
				//�ҵ��ýڵ��Ұ��������Ȩֵ��С�Ľڵ㣨��̽ڵ㣩������Ȩֵ���Ƶ���ɾ���ڵ㴦������������ɫ
				//�ݹ�ɾ�����Ȩֵ��С�Ľڵ㡣��Ϊ��̽ڵ�û�����ӣ�����תΪǰ�������
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
			//��¼��ɾ��ǰ����ɫ����Ϊɾ��֮���п��ܱ�ΪNULL
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
