#pragma once

#include "iterator.h"
#include "alloc.h"
#include "queue.h"
#include "vector.h"
#include "string.h"

#include <cstdint>
#include <utility>
#include <iostream>

namespace tinystl
{

template <class T>
struct TreeNode
{
    typedef TreeNode<T>*   node_pointer;

    enum class Color { BLACK, RED };
    TreeNode(const T& v, 
             Color c,
             node_pointer p = nullptr,
             node_pointer l = nullptr,
             node_pointer r = nullptr)
        : value(v), color(c), parent(p), left(l), right(r)
    {  }

    T value;
    Color color;
    node_pointer parent;
    node_pointer left;
    node_pointer right;

};

namespace 
{
    template <class T>
    TreeNode<T>* __minKey(TreeNode<T>* root) 
    {
        while(root && root->left)
            root = root->left;
        return root;
    }
    template <class T>
    TreeNode<T>* __maxKey(TreeNode<T>* root) 
    {
        while(root && root->right)
            root = root->right;
        return root;
    }

}

template <class T>
class TreeIterator 
{
public:
    typedef bidirectional_iterator_tag      iterator_category;

    typedef TreeNode<T>                                    node_type;
    typedef typename TreeNode<T>::node_pointer             node_pointer;
    /* typedef typename TreeNode<T>::value_type               value_type; */
    typedef T value_type;
    
    typedef TreeIterator<T>    Self;
public:
    node_pointer node_;
    node_pointer root_; 

public:
   TreeIterator(node_pointer node, node_pointer root)
       : node_(node), root_(root)
   {  }

   value_type& operator*()
   {
       return node_->value;
   }
   const value_type& operator*() const
   {
       return node_->value;
   }
   value_type* operator->()
   {
       return &node_->value;
   }
   const value_type* operator->() const
   {
       return &node_->value;
   }

   Self& operator++()
   {
       increment();
       return *this;
   }
   Self operator++(int)
   {
       Self tmp(*this);
       increment();
       return tmp;
   }
   Self& operator--()
   {
       decrement();
       return *this;
   }
   Self operator--(int)
   {
       Self tmp(*this);
       decrement();
       return tmp;
   }

   bool operator==(const Self& rhs) const
   {
       return node_ == rhs.node_;
   }
   bool operator!=(const Self& rhs) const
   {
       return !(operator==(rhs));
   }

private:
   void increment()
   {
       if(!node_)   return;
       if(node_->right)
       {
           node_ = __minKey(node_->right);
           return;
       }
       else
       {
           node_pointer parent = node_->parent;
           while(parent && parent->right == node_)
           {
               node_ = parent;
               parent = parent->parent;
           }
           node_ = parent;
       }
   }
   void decrement()
   {
       if(!node_)   
       {
           node_ = __maxKey(root_);
           return;
       }
       if(node_->left)
       {
           node_pointer left = node_->left;
           while(left->right)
               left = left->right;
           node_ = left;
       }
       else
       {
           node_pointer parent = node_->parent;
           while(parent && parent->left == node_)
           {
               node_ = parent;
               parent = parent->parent;
           }
           node_ = parent;
       }
   }
};

template <class Key, class Value, class KeyOfValue = std::_Identity<Value>, class Compare = std::less<Key>, class Alloc = alloc>
class RBTree 
{
public:
    typedef Key                                 key_type;
    typedef Value                               value_type;
    typedef Compare                             key_compare;
    typedef Compare                             value_compare;
    typedef value_type&                         reference;
    typedef const value_type&                   const_reference;
    typedef TreeIterator<Value>                 iterator;
    typedef const iterator                      const_iterator;
    typedef tinystl::reverse_iterator<iterator> reverse_iterator;
    typedef const reverse_iterator              const_reverse_iterator;

    typedef std::size_t                         size_type;
    typedef std::ptrdiff_t                      difference_type;

    typedef TreeNode<Value>                     node_type;
    typedef typename node_type::node_pointer    node_pointer;
    typedef typename node_type::Color           Color;
    typedef RBTree<Key, Value, KeyOfValue, Compare, Alloc>  Self;

    typedef SimpleAlloc<node_type, Alloc>       nodeAllocator;

    enum class InsertPos { LEFT, RIGHT };
    enum class InsertPattern { UNIQUE, MULTI };
protected:
    node_pointer root_;
    key_compare comp_;
    size_type nodeSize_;
    node_pointer dbnode_;
public:
    RBTree() : root_(nullptr), nodeSize_(0) {}
    RBTree(key_compare comp) : root_(nullptr), nodeSize_(0), comp_(comp) {}
    /* RBTree(const RBTree& other); */
    /* RBTree(RBTree&& other); */
    /* ~RBTree(); */

    Self& operator=(const Self& other);
    Self& operator=(Self&& other);

    bool isBalance();
    void isBalance(node_pointer node, int num, tinystl::string cur, tinystl::vector<int>& blackNums, tinystl::vector<tinystl::string>& paths);
    iterator begin() { return iterator(minKey(root_), root_); }
    iterator end() { return iterator(nullptr, root_); }

    const_iterator begin() const { return const_iterator(minKey(root_), root_); }
    const_iterator end() const { return const_iterator(nullptr, root_); }

    const_iterator cbegin() const { return const_iterator(minKey(root_), root_); }
    const_iterator cend() const { return const_iterator(nullptr, root_); }

    reverse_iterator rbegin() { return reverse_iterator(iterator(nullptr, root_)); }
    reverse_iterator rend() { return reverse_iterator(iterator(minKey(root_), root_)); }

    bool empty() const noexcept { return nodeSize_ == 0; }
    size_type size() const noexcept { return nodeSize_; }
    size_type max_size() const noexcept { return static_cast<size_type>(-1); }

    iterator insert_unique(const value_type& value) { return iterator(insertAux(value, InsertPattern::UNIQUE), root_); }
    iterator insert_multi(const value_type& value) { return iterator(insertAux(value, InsertPattern::MULTI), root_); }
    /* iterator insert(const_iterator hint, const value_type& value); */

    size_type erase(const value_type& value); 
    iterator erase(const_iterator pos); 
    void erase(iterator first, iterator last);

    void print_levelOrder()
    {
        tinystl::queue<node_pointer> q;
        if(root_)
        {
            q.push(root_);
            q.push(nullptr);
        }
        while(!q.empty())
        {
            node_pointer topNode = q.front();
            q.pop();
            if(topNode == nullptr)
            {
                if(!q.empty())
                    q.push(nullptr);
                std::cout << std::endl;
                continue;
            }
            std::cout << "<" << topNode->value << "," ;
            if(topNode->color == Color::BLACK) 
                std::cout << "BLACK";
            else
                std::cout << "RED";
            if(topNode->parent)
                std::cout << topNode->parent->value << ",";
            else
                std::cout << "-1,";
            std::cout << "> ";
            if(topNode->left)
                q.push(topNode->left);
            if(topNode->right)
                q.push(topNode->right);
        }
        std::cout << std::endl;
    }
    void print_inOrder() { inOrder(root_); std::cout << std::endl; }
    void inOrder(node_pointer node)
    {
        if(!node)   return;
        inOrder(node->left);
        std::cout << node->value << " ";
        inOrder(node->right);
    }

private:
    node_pointer insertAux(const value_type& value, InsertPattern pattern);
    node_pointer insertAux(node_pointer root_, const value_type& value, InsertPattern pattern);
    
    node_pointer eraseAux(node_pointer node, const value_type& value, size_type& eraseNums);
    node_pointer eraseLeafNode(node_pointer);
    node_pointer eraseNodeWithOneChild(node_pointer);
    node_pointer leftDBModify(node_pointer node, Color color);
    node_pointer rightDBModify(node_pointer node, Color color);
    node_pointer DB0Modify(node_pointer node);
    node_pointer DB1Modify(node_pointer node);
    node_pointer DB2Modify(node_pointer node);

    void handleDBNode(node_pointer node);
    void setDBNode(node_pointer node) { dbnode_ = node; }
    bool isDBNode(node_pointer node) { return dbnode_ != nullptr && dbnode_ == node; }
    bool isFarNephewNode(node_pointer node1, node_pointer node2)
    {
        if(node1->parent->left == node1)
            return node2 == node1->parent->right->right;
        else
            return node2 == node1->parent->left->left;
    }
    node_pointer brotherNode(node_pointer node)
    {
        if(node->parent && node->parent->left == node)
            return node->parent->right;
        else if(node->parent)
            return node->parent->left;
        else
            return nullptr;
    }
    void swapNodePosition(node_pointer node1, node_pointer node2)
    {
        /* std::cout << " swapNode()" << std::endl; */
        /* print_levelOrder(); */
        node_pointer parent1 = node1->parent;
        node_pointer parent2 = node2->parent;
        /* if(node2->parent == node1) */
        /* { */
        /*     if(parent1 && parent1->left == node1) */
        /*         parent1->left = node2; */
        /*     else if(parent1 && parent1->right == node1) */
        /*         parent1->right = node2; */
        /*     else */
        /*         root_ = node2; */
        /*     node2->parent = parent1; */
        /*     node1->parent = node2; */
        /* } */
        /* else if(node1->parent == node2) */
        /* { */
        /*     if(parent2 && parent2->left == node2) */
        /*         parent2->left = node1; */
        /*     else if(parent2 && parent2->right == node2) */
        /*         parent2->right = node1; */
        /*     else */
        /*         root_ = node1; */
        /*     node1->parent = parent2; */
        /*     node2->parent = node1; */
        /* } */
        /* else */
        /* { */
        /*     if(parent1 && parent1->left == node1) */
        /*         parent1->left = node2; */
        /*     else if(parent1 && parent1->right == node1) */
        /*         parent1->right = node2; */
        /*     else */
        /*         root_ = node2; */
        /*     if(parent2 && parent2->left == node2) */
        /*         parent2->left = node1; */
        /*     else if(parent2 && parent2->right == node2) */
        /*         parent2->right = node1; */
        /*     else */
        /*         root_ = node1; */
        /*     node2->parent = parent1; */
        /*     node1->parent = parent2; */
        /* } */
        std::swap(node1->parent, node2->parent);
        if(node1->parent == node1)
            node1->parent = node2;
        if(node2->parent == node2)
            node2->parent = node1;
        std::swap(node1->left, node2->left);
        if(node1->left == node1)
            node1->left = node2;
        if(node2->left == node2)
            node2->left = node1;
        std::swap(node1->right, node2->right);
        if(node1->right == node1)
            node1->right = node2;
        if(node2->right == node2)
            node2->right = node1;

        if(node1->parent && node1->parent->left == node2)
            node1->parent->left = node1;
        else if(node1->parent && node1->parent->right == node2)
            parent2->right = node1;
        else if(!parent2)
            root_ = node1;

        if(node2->parent && node2->parent->left == node1)
            node2->parent->left = node2; 
        else if(node2->parent && node2->parent->right == node1)
            node2->parent->right = node2;
        else if(!parent1)
            root_ = node2;
        
        if(node1->left)
            node1->left->parent = node1;
        if(node2->left)
            node2->left->parent = node2;
        
        if(node1->right)
            node1->right->parent = node1;
        if(node2->right)
            node2->right->parent = node2;

        std::swap(node1->color, node2->color);
        /* print_levelOrder(); */
    }
    void eraseNodeWithOneChildAux(node_pointer node)
    {

        /* print_levelOrder(); */
        if(node->left)
        {
            if(node->parent && node->parent->left == node)
                node->parent->left = node->left;
            else if(node->parent && node->parent->right == node)
                node->parent->right = node->left;
            else
                root_ = node->left;
            node->left->parent = node->parent;
        }
        else
        {
            if(node->parent && node->parent->left == node)
                node->parent->left = node->right;
            else if(node->parent && node->parent->right == node)
                node->parent->right = node->right;
            else
                root_ = node->right;
            if(node->right)
                node->right->parent = node->parent;
        }
        node->parent = node->left = node->right = nullptr;
        destroy(node);
        nodeAllocator::deallocate(node);
        /* print_levelOrder(); */
    }

    void handleNodeWithTwoRedNode(node_pointer node);
    void enableColorBalance(node_pointer node, InsertPos position);

    bool isMultiInsertPattern(InsertPattern pattern)
    {
        return pattern == InsertPattern::MULTI;
    }
    node_pointer leftRotate(node_pointer node)
    {
        node_pointer parentNode = node->parent;
        node_pointer rightNode = node->right;
        node_pointer rightLeftNode = rightNode->left;
        node->right = rightLeftNode;
        if(rightLeftNode)
            rightLeftNode->parent = node;
        rightNode->left = node;
        node->parent = rightNode;
        if(parentNode && parentNode->left == node)
            parentNode->left = rightNode;
        else if(parentNode && parentNode->right == node)
            parentNode->right = rightNode;
        else
            root_ = rightNode;
        rightNode->parent = parentNode;
        return rightNode;
    }
    
    node_pointer rightRotate(node_pointer node)
    {
        node_pointer parentNode = node->parent;
        node_pointer leftNode = node->left;
        node_pointer leftRightNode = leftNode->right;
        node->left = leftRightNode;
        if(leftRightNode)
            leftRightNode->parent = node;
        leftNode->right = node;
        node->parent = leftNode;
        if(parentNode && parentNode->left == node)
            parentNode->left = leftNode;
        else if(parentNode && parentNode->right == node)
            parentNode->right = leftNode;
        else
            root_ = leftNode;
        leftNode->parent = parentNode;
        return leftNode;
    }
    
    node_pointer leftRightRotate(node_pointer node)
    {
        leftRotate(node->left);
        return rightRotate(node);
    }

    node_pointer rightLeftRotate(node_pointer node)
    {
        rightRotate(node->right);
        return leftRotate(node);
    }

private:
    node_pointer minKey(node_pointer node) const;
    node_pointer maxKey(node_pointer node) const;

    Color color(node_pointer node) { return node->color; } 
    bool isBlackNode(node_pointer node) { return node == nullptr || color(node) == Color::BLACK; }
    bool isRedNode(node_pointer node) { return node && color(node) == Color::RED; }

    node_pointer createNodeWithColor(const value_type& value, Color color)
    {
        node_pointer node = nodeAllocator::allocate(1);
        construct(node, node_type(value, color));
        ++nodeSize_;
        return node;
    }
    void setBlackNode(node_pointer node)
    {
        node->color = Color::BLACK;
    }
    template <class... Args>
    void setBlackNode(node_pointer node, Args... args)
    {
        node->color = Color::BLACK;
        setBlackNode(args...);
    }

    void setRedNode(node_pointer node)
    {
        node->color = Color::RED;
    }
    template <class... Args>
    void setRedNode(node_pointer node, Args... args)
    {
        node->color = Color::RED;
        setRedNode(args...);
    }

    void setNodeColor(node_pointer node, Color color)
    {
        node->color = color;
    }
};



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::minKey(node_pointer node) const
{
    return __minKey(node);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::maxKey(node_pointer node) const
{
    return __maxKey(node);
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::insertAux(const value_type& value, InsertPattern pattern)
{
    if(!root_)
    {
        root_ = createNodeWithColor(value, Color::BLACK);
        return root_;
    }
    else
    {
        return insertAux(root_, value, pattern);
    }
}



template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::handleNodeWithTwoRedNode(node_pointer node)
{
    if(node && node->left && node->right)
    {
        if(isBlackNode(node) && isRedNode(node->left) && isRedNode(node->right))
        {
            if(node != root_)
                setRedNode(node);
            setBlackNode(node->left, node->right);
            if(node->parent && isRedNode(node->parent) && node->parent->parent)
            {
               if(node->parent->parent->left == node->parent && 
                  node->parent->left == node)
               {
                   rightRotate(node->parent->parent);
                   setRedNode(node, node->parent->right);
                   setBlackNode(node->parent);
                    
               }
               else if(node->parent->parent->left == node->parent && 
                       node->parent->right == node)
               {
                   leftRightRotate(node->parent->parent);
                   setRedNode(node->left, node->right);
                   setBlackNode(node);
               }
               else if(node->parent->parent->right == node->parent &&
                       node->parent->right == node)
               {
                   leftRotate(node->parent->parent);
                   setRedNode(node, node->parent->left);
                   setBlackNode(node->parent);
               }
               else if(node->parent->parent->right == node->parent &&
                       node->parent->left == node)
               {
                   rightLeftRotate(node->parent->parent);
                   setRedNode(node->left, node->right);
                   setBlackNode(node);
               }
            }
        }
    }
    if(isRedNode(root_))
        setBlackNode(root_);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::enableColorBalance(node_pointer node, InsertPos position)
{
    if(isRedNode(node))
    {
        if(position == InsertPos::RIGHT)
        {
            if(node->parent->left == node)
            {
                leftRightRotate(node->parent);
                setRedNode(node, node->parent->right);
                setBlackNode(node->parent);
            }
            else
            {
                leftRotate(node->parent);
                setRedNode(node->left, node->right);
                setBlackNode(node);
            }
        }
        else
        {
            if(node->parent->left == node)
            {
                rightRotate(node->parent);
                setRedNode(node->left, node->right);
                setBlackNode(node);
            }
            else
            {
                rightLeftRotate(node->parent);
                setRedNode(node, node->parent->left);
                setBlackNode(node->parent);
            }
        }
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::insertAux(node_pointer node, const value_type& value, InsertPattern pattern)
{
    if(!node)   return nullptr;
    handleNodeWithTwoRedNode(node);
    node_pointer insertNode = nullptr;
    if(comp_(KeyOfValue()(node->value), value))
    {
        insertNode = insertAux(node->right, value, pattern);
        if(!insertNode)
        {
            node->right = createNodeWithColor(value, Color::RED);
            node->right->parent = node;
            insertNode = node->right;
            enableColorBalance(node, InsertPos::RIGHT);
        }
    }
    else
    {
        if(comp_(value, KeyOfValue()(node->value)) || isMultiInsertPattern(pattern))
        {
            insertNode = insertAux(node->left, value, pattern);
            /* if(!node->left)会出错，原因是插入后会进行旋转，
             * 之前node->left != nullptr的node通过旋转可能会导致node->left == nullptr */
            if(!insertNode)
            {
                node->left = createNodeWithColor(value, Color::RED);
                node->left->parent = node;
                insertNode = node->left;
                enableColorBalance(node, InsertPos::LEFT);
            }
        }
        else
        {
            insertNode = node;
        }
    }
    return insertNode;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::handleDBNode(node_pointer node)
{
    /* std::cout << "handleDBNode()" << std::endl; */
    /* print_levelOrder(); */
    setDBNode(nullptr);
    if(!node)   return;
    if(node == root_)   return;
    node_pointer brother = brotherNode(node);
    if(!brother)
    {
        if(node->parent && isRedNode(node->parent))
        {
            setBlackNode(node->parent);
        }
        else
        {
            setDBNode(node->parent);
        }
    }
    else
    {
        if(isBlackNode(brother))
        {
            if(brother->right && isRedNode(brother->right))
            {
                if(isFarNephewNode(node, brother->right))
                {
                    setNodeColor(brother, color(node->parent));
                    setBlackNode(brother->right, node->parent);
                    leftRotate(node->parent);
                }
                else
                {
                    rightRotate(brother);
                    brother = brotherNode(node);
                    setNodeColor(brother, color(node->parent));
                    setBlackNode(node->parent);
                    leftRotate(node->parent);
                }
            }
            else if(brother->left && isRedNode(brother->left))
            {
                if(isFarNephewNode(node, brother->left))
                {
                    setNodeColor(brother, color(node->parent));
                    setBlackNode(brother->left, node->parent);
                    rightRotate(node->parent);
                }
                else
                {
                    leftRotate(brother);
                    brother = brotherNode(node);
                    setNodeColor(brother, color(node->parent));
                    setBlackNode(node->parent);
                    rightRotate(node->parent);
                }
            }
            else if(isBlackNode(brother->left) && isBlackNode(brother->right))
            {
                setRedNode(brother);
                if(isBlackNode(node->parent))
                    setDBNode(node->parent);
                setBlackNode(node->parent);
            }
            else
            {
                setRedNode(brother);
                if(isBlackNode(node->parent))
                    setDBNode(node->parent);
                else
                    setDBNode(nullptr);
                setBlackNode(node->parent);
            }
            
        }
        else
        {
            if(node->parent->right == brother)
            {
                leftRotate(node->parent);
                setRedNode(node->parent);
                setBlackNode(brother);
            }
        }
    }

    /* print_levelOrder(); */
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::DB0Modify(node_pointer node)
{
    std::cout << "DB0Modify()" << std::endl;
    print_levelOrder();
    if((!node->left || isDBNode(node->left)) && node->right && isBlackNode(node->right)) 
    {
        node_pointer brother = node->right;
        if(brother->right && isRedNode(brother->right))
        {
            setDBNode(nullptr);
            setNodeColor(brother, color(node));
            setBlackNode(brother->right, node);
            node = leftRotate(node);
        }
        else if(brother->left && isRedNode(brother->left))
        {
            setDBNode(nullptr);
            rightRotate(brother);
            brother = node->right;
            setNodeColor(brother, color(node));
            setBlackNode(node);
            node = leftRotate(node);
        }
    }
    else if((!node->right || isDBNode(node->right)) && node->left && isBlackNode(node->left)) 
    {
        node_pointer brother = node->left;
        if(brother->left && isRedNode(brother->left))
        {
            setDBNode(nullptr);
            setNodeColor(brother, color(node));
            setBlackNode(brother->left, node);
            node = rightRotate(node);
        }
        else if(brother->right && isRedNode(brother->right))
        {
            setDBNode(nullptr);
            leftRotate(brother);
            brother = node->left;
            setNodeColor(brother, color(node));
            setBlackNode(node);
            node = rightRotate(node);
        }
    }
    print_levelOrder();
    return node;
}
 

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::DB1Modify(node_pointer node)
{
    std::cout << "DB1Modify()" << std::endl;
    print_levelOrder();
    if((!node->left || isDBNode(node->left)) && node->right && isBlackNode(node->right)) 
    {
        if(isBlackNode(node->right->left) && isBlackNode(node->right->right))
        {
            setDBNode(nullptr);
            node_pointer brother = node->right;
            if(isBlackNode(node))
                setDBNode(node);
            setRedNode(brother);
            setBlackNode(node);
        }
    
    }
    else if((!node->right || isDBNode(node->right)) && node->left && isBlackNode(node->left))
    {
        if(isBlackNode(node->left->left) && isBlackNode(node->left->right))
        {
            setDBNode(nullptr);
            node_pointer brother = node->left;
            if(isBlackNode(node))
                setDBNode(node);
            setRedNode(brother);
            setBlackNode(node);
        }
    }

    print_levelOrder();
    return node;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::DB2Modify(node_pointer node)
{
    std::cout << "DB2Modify()" << std::endl;
    print_levelOrder();
    if((!node->left || isDBNode(node->left)) && node->right && isRedNode(node->right))
    {
        setDBNode(nullptr);
        leftRotate(node);
        setBlackNode(node, node->parent);
        setRedNode(node->right);
        if((node->right->left && isRedNode(node->right->left)) ||
           (node->right->right && isRedNode(node->right->right)))
        {
            leftRotate(node);
            setBlackNode(node->parent->right);
            node = node->parent;
        }
        node = node->parent;
    }
    else if((!node->right || isDBNode(node->right)) && node->left && isRedNode(node->left))
    {
        setDBNode(nullptr);
        rightRotate(node);
        setBlackNode(node, node->parent);
        setRedNode(node->left);
        if((node->left->left && isRedNode(node->left->left)) ||
           (node->left->right && isRedNode(node->left->right)))
        {
            rightRotate(node);
            setBlackNode(node->parent->left);
            node = node->parent;
        }
        node = node->parent;
    }
    print_levelOrder();
    return node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::rightDBModify(node_pointer node, Color color)
{
    if((!node->right && color == Color::BLACK) || isDBNode(node->right))
    {
        if(!node->right && node->left)
            return node;
        if(node->left && isBlackNode(node->left))
        {
            node = DB0Modify(node);
            node = DB1Modify(node);
        }
        else if(node->left)
        {
            node = DB2Modify(node);
        }
        else
        {
            setDBNode(nullptr);
            if(isRedNode(node))
                setBlackNode(node);
            else
                setDBNode(node);
        }
    }
    return node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::leftDBModify(node_pointer node, Color color)
{
    if((!node->left && color == Color::BLACK) || isDBNode(node->left))
    {
        if(!node->left && node->right)
            return node;
        if(node->right && isBlackNode(node->right))
        {
            node = DB0Modify(node);
            node = DB1Modify(node);
        }
        else if(node->right)
        {
            node = DB2Modify(node);
        }
        else
        {
            setDBNode(nullptr);
            if(isRedNode(node))
                setBlackNode(node);
            else
                setDBNode(node);
        }
    }
    return node;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::eraseLeafNode(node_pointer node)
{
    if(node)
    {
        if(node->parent && node->parent->left == node)
            node->parent->left = nullptr;
        else if(node->parent)
            node->parent->right = nullptr;
        else
            root_ = nullptr;
        destroy(node);
        nodeAllocator::deallocate(node);
    }
    return nullptr;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::eraseNodeWithOneChild(node_pointer node)
{
    node_pointer res = nullptr;
    if(node->left)
    {
        if(isBlackNode(node) && isBlackNode(node->left))
            setDBNode(node->left);
        else if(isBlackNode(node))
            setBlackNode(node->left);
        node->left->parent = node->parent;
        if(node->parent && node->parent->left == node)
            node->parent->left = node->left;
        else if(node->parent)
            node->parent->right = node->left;
        else
            root_ = node->left;
        res = node->left;
        destroy(node);
        nodeAllocator::deallocate(node);
    }
    else
    {
        if(isBlackNode(node) && isBlackNode(node->right))
            setDBNode(node->right);
        else if(isBlackNode(node))
            setBlackNode(node->right);
        node->right->parent = node->parent;
        if(node->parent && node->parent->left == node)
            node->parent->left = node->right;
        else if(node->parent)
            node->parent->right = node->right;
        else
            root_ = node->right;
        res = node->right;
        destroy(node);
        nodeAllocator::deallocate(node);
    }
    return res;

}
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::eraseAux(node_pointer node, const value_type& value, size_type& eraseNums)
{
    if(!node)   return 0;
    if(comp_(KeyOfValue()(node->value), value))
    {
        Color rightColor = color(node->right);
        node->right = eraseAux(node->right, value, eraseNums);
        if(node->right) node->right->parent = node;
        node = rightDBModify(node, rightColor);
    }
    else if(comp_(value, KeyOfValue()(node->value)))
    {
        Color leftColor = color(node->left);
        node->left = eraseAux(node->left, value, eraseNums);
        if(node->left) node->left->parent = node;
        node = leftDBModify(node, leftColor);
    }
    else
    {
        if(!node->left && !node->right)
        {
            std::cout << "erase leaf node " << node->value << std::endl;
            node = eraseLeafNode(node);
            ++eraseNums;
        }
        else if((node->left && !node->right) || 
                (!node->left && node->right))
        {
            std::cout << "erase  node with one child " << node->value << std::endl;
            node = eraseNodeWithOneChild(node);
            ++eraseNums;
        }
        else
        {
            node_pointer rightMinNode = minKey(node->right);
            swapNodePosition(node, rightMinNode);
            node = rightMinNode;
            Color rightColor = color(node->right);
            node->right = eraseAux(node->right, value, eraseNums);
            if(node->right)  node->right->parent = node;
            node = rightDBModify(node, rightColor);
        }
    }
    if(isDBNode(root_))
        setDBNode(nullptr);
    return node;
        /* if(comp_(value, KeyOfValue()(node->value))) */
        /* { */
        /*     eraseNums = eraseAux(node->left, value); */
        /* } */
        /* else */
        /* { */
        /*     /1* if(isDBNode(node)) *1/ */
        /*     /1*     handleDBNode(node); *1/ */
        /*     node_pointer rightMinNode = minKey(node->right); */
        /*     if(rightMinNode) */
        /*     { */
        /*         swapNodePosition(node, rightMinNode); */
        /*         eraseNums = eraseAux(rightMinNode->right, value); */ 
        /*         node = rightMinNode; */
        /*         eraseNums += eraseAux(node, value); */
        /*     } */
        /*     else */
        /*     { */
        /*         node_pointer leftNode = node->left; */
        /*         if(isBlackNode(node)) */
        /*         { */
        /*             if(node->left && isRedNode(node->left)) */
        /*             { */
        /*                 setBlackNode(node->left); */
        /*             } */
        /*             else if(node->left && isBlackNode(node->right)) */
        /*             { */
        /*                 setDBNode(node->left); */
        /*             } */
        /*             else */
        /*             { */
        /*                 if(!brotherNode(node)) */
        /*                 { */
        /*                     setDBNode(node->parent); */
        /*                 } */
        /*             } */
        /*             /1* { *1/ */
        /*             /1*     if(node->parent && isRedNode(node->parent)) *1/ */
        /*             /1*         setBlackNode(node->parent); *1/ */
        /*             /1*     else if(node->parent) *1/ */
        /*             /1*         setDBNode(node->parent); *1/ */
        /*             /1* } *1/ */
        /*             /1* else *1/ */
        /*             /1* { *1/ */
        /*             /1*     if(!brotherNode(node) && node->parent) *1/ */
        /*             /1*     { *1/ */
        /*             /1*        setDBNode(node->parent); *1/ */ 
        /*             /1*     } *1/ */
        /*             /1* } *1/ */
        /*         } */
        /*         eraseNodeWithOneChildAux(node); */
        /*         ++eraseNums; */
        /*         node = leftNode; */
        /*         eraseNums += eraseAux(node, value); */
        /*     } */
        /* } */
    /* } */
    /* return eraseNums; */
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const value_type& value)
{
    setDBNode(nullptr);
    size_type eraseNums = 0;
    eraseAux(root_, value, eraseNums);
    return eraseNums;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename RBTree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
RBTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator it)
{
    iterator eraseIt = it++;
    eraseAux(*eraseIt);
    return it;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last)
{
    while(first != last)
    {
        eraseAux(first.node_, *first);
        ++first;
    }
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
bool RBTree<Key, Value, KeyOfValue, Compare, Alloc>::isBalance()
{
    tinystl::vector<int> blackNums;
    tinystl::vector<tinystl::string> paths;
    tinystl::string cur;
    isBalance(root_, 0, cur, blackNums, paths);
    for(auto& path : paths)
    {
        std::cout << path << std::endl;
    }
    for(size_type i = 1; i < blackNums.size(); ++i)
    {
        if(blackNums[i] != blackNums[i - 1])
            return false;
    }
    return true;
}


template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void RBTree<Key, Value, KeyOfValue, Compare, Alloc>::isBalance(node_pointer node, int num, tinystl::string cur, tinystl::vector<int>& blackNums, tinystl::vector<tinystl::string>& paths)
{
    if(!node)   return;
    if(isBlackNode(node))
    {
        ++num;
        cur += "<BLACK>";
    }
    else
    {
        if((node->left && isRedNode(node->left)) ||
           (node->right && isRedNode(node->right)))
           num = INT32_MIN;
        cur += "<RED>";
    }
    if(!node->left && !node->right)
    {
        blackNums.push_back(num);
        paths.push_back(cur);
        return;
    }
    isBalance(node->left, num, cur, blackNums, paths);
    isBalance(node->right, num, cur, blackNums, paths);
}

}


