#pragma once

#include "alloc.h"
#include "iterator.h"
#include "queue.h"
#include "utility.h"
#include "algorithm.h"

#include <algorithm>
#include <utility>

namespace tinystl
{

template <class Key, class Value>
struct TreeNode
{
    TreeNode(const Key& key, 
             const Value& value, 
             TreeNode* parentNode = nullptr, 
             TreeNode* leftNode = nullptr, 
             TreeNode* rightNode = nullptr)
        : p(key, value), parent(parentNode), left(leftNode), right(rightNode)
    {  }


    tinystl::pair<Key, Value> p;
    TreeNode* parent;
    TreeNode* left;
    TreeNode* right;
};

namespace 
{
    template <class Key, class Value>
    TreeNode<Key, Value>* __minKey(TreeNode<Key, Value>* root) 
    {
        while(root && root->left)
            root = root->left;
        return root;
    }
    template <class Key, class Value>
    TreeNode<Key, Value>* __maxKey(TreeNode<Key, Value>* root) 
    {
        while(root && root->right)
            root = root->right;
        return root;
    }

}
template <class Key, class Value>
class TreeIterator
{
public:
    typedef bidirectional_iterator_tag      iterator_category;
    typedef tinystl::pair<Key, Value>           data_type;
    typedef std::size_t                     size_type;
    typedef std::ptrdiff_t                  difference_type;
    typedef TreeNode<Key, Value>*           pointer;
    typedef TreeNode<Key, Value>            node_type;
    typedef TreeIterator<Key, Value>        Self;
    typedef Key                             value_type;
    typedef value_type&                     reference;

public:
    pointer node_;
    pointer root_;

public:
    TreeIterator(pointer node = nullptr, pointer root = nullptr) 
        : node_(node), root_(root)
    {  }
    Key& operator*()
    {
        return node_->p.first;
    }
    const Key& operator*() const
    {
        return node_->p.first;
    }
    Key* operator->()
    {
        return &node_->p.first;
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

    bool operator==(const Self& rhs) const { return node_ == rhs.node_; }
    bool operator!=(const Self& rhs) const { return !operator==(rhs); }
    

private:
    void increment()
    {
        if(!node_)  return;
        if(node_->right)
        {
            pointer right = node_->right;
            while(right->left)
                right = right->left;
            node_ = right;
        }
        else
        {
            pointer parent = node_->parent;
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
            pointer left = node_->left;
            while(left->right)
                left = left->right;
            node_ = left;
        }
        else
        {
            pointer parent = node_->parent;
            while(parent && parent->left == node_)
            {
                node_ = parent;
                parent = parent->parent;
            }
            node_ = parent;
        }
    }
};



template <class Key, class Value = Key, class Compare = std::less<Key>, class Alloc = alloc>
class avl_tree
{
public:
    typedef Key     key_type;
    typedef Value   value_type;
    typedef tinystl::pair<key_type, value_type> data_type;
    typedef Compare compare_type;
    typedef std::size_t size_type;
    typedef TreeNode<key_type, value_type>      node_type;
    typedef TreeNode<key_type, value_type>*     node_pointer;

    typedef TreeIterator<key_type, value_type>  iterator;
    typedef const iterator                      const_iterator;
    typedef tinystl::reverse_iterator<iterator>          reverse_iterator;
    typedef const reverse_iterator              const_reverse_iterator;

    typedef SimpleAlloc<node_type, Alloc>       nodeAllocator;
protected:
    node_pointer root_;
    size_type nodeSize_;
    compare_type comp_;
public:
    avl_tree() : root_(nullptr), nodeSize_(0) {}
    avl_tree(const avl_tree&) = delete;
    avl_tree& operator=(const avl_tree&) = delete;

    ~avl_tree()
    {
        erase(begin(), end());     
    }

    size_type height() const { return height(root_); }
    size_type size() const { return nodeSize_; }
    bool empty() const { return nodeSize_ == 0; }

    iterator root() { return iterator(root_); }

    const_iterator find_min() const { return const_iterator(minKey(root_), root_); }
    const_iterator find_max() const { return const_iterator(maxKey(root_), root_); }
    const_iterator find(const key_type& key) const; 

    iterator begin() { return iterator(minKey(root_), root_); }
    const_iterator begin() const noexcept { return iterator(minKey(root_), root_); }
    const_iterator cbegin() const  { return const_iterator(minKey(root_), root_); }

    iterator end() { return iterator(nullptr); }
    const_iterator end() const noexcept{ return iterator(minKey(root_), root_); }
    const_iterator cend() const noexcept { return const_iterator(nullptr, root_); }

    iterator insert(const key_type& key, const value_type& val = value_type());
    template <class InputIterator>
    void insert(InputIterator first, InputIterator last)
    {
        while(first != last)
            insert(*first++);
    }

    iterator erase(const key_type& key) { return iterator(eraseAux(root_, key)); }
    iterator erase(const_iterator pos) { return iterator(eraseAux(pos.node_, *pos)); }
    iterator erase(iterator first, iterator last)
    {
        while(first != last)
            erase(first++);
        return last;
    }
    
    void print_preorder(const std::string& delim = " ", std::ostream& os = std::cout) const { preOrderAux(root_, delim, os); }
    void print_inorder(const std::string& delim = " ", std::ostream& os = std::cout) const { inOrderAux(root_, delim, os); }
    void print_postorder(const std::string& delim = " ", std::ostream& os = std::cout) const { postOrderAux(root_, delim, os); }
    void print_levelorder(const std::string& delim = " ", std::ostream& os = std::cout) const { levelOrderAux(root_, delim, os); }
private:
    node_pointer insertAux(node_pointer node, const key_type& key, const value_type&);
    node_pointer eraseAux(node_pointer node, const key_type& key);

    void preOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const;
    void inOrderAux(node_pointer node, const std::string& delim, std::ostream& os)const ;
    void postOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const;
    void levelOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const;
private:
    node_pointer createNode(const key_type& key, const value_type& value)
    {
        node_pointer node = nodeAllocator::allocate(1);
        construct(node, node_type(key, value));
        return node;
    }
    node_pointer minKey(node_pointer root) const
    {
        return __minKey(root);
    }
    node_pointer maxKey(node_pointer root) const
    {
        return __maxKey(root);
    }
    void createChildNode(node_pointer node, const key_type& key, const value_type& value, bool isLeftNode)
    {
        node_pointer childNode = nodeAllocator::allocate(1);
        construct(childNode, node_type(key, value, node));
        if(isLeftNode)
            node->left = childNode;
        else
            node->right = childNode;
        ++nodeSize_;
    }
    void eraseNodeWithOneChild(node_pointer node)
    {
        node_pointer nextNode = node->left ? node->left : node->right;
        node_pointer parent = node->parent;
        if(!parent)
        {
            root_ = nextNode;
        }
        else
        {
            if(parent->left == node)
                parent->left = nextNode;
            else
                parent->right = nextNode;
        }
        if(nextNode)
            nextNode->parent = parent;
        destroy(node);
        nodeAllocator::deallocate(node);
        --nodeSize_;
    }

    void swapNode(node_pointer lhs, node_pointer rhs)
    {
        tinystl::swap(lhs->left, rhs->left);
        tinystl::swap(lhs->right, rhs->right);
        tinystl::swap(lhs->parent, rhs->parent);
        if(lhs->left == lhs)
            lhs->left = rhs;
        if(rhs->left == rhs)
            rhs->left = lhs;
        if(lhs->right == lhs)
            lhs->right = rhs;
        if(rhs->right == rhs)
            rhs->right = lhs;
        if(lhs->parent == lhs)
            lhs->parent = rhs;
        if(rhs->parent == rhs)
            rhs->parent = lhs;
    }
private:
    node_pointer enableHeightBalance(node_pointer node)
    {
        int leftH = height(node->left);
        int rightH = height(node->right);
        if(std::abs(leftH - rightH) < 2)
            return node;
        if(leftH > rightH)
        {
            if(node->left->left)
                node = rightRotate(node);
            else
                node = leftRightRotate(node);
        }
        else
        {
            if(node->right->right)
                node = leftRotate(node);
            else
                node = rightLeftRotate(node);
        }
        return node;
    }
 
    size_type height(node_pointer root) const
    {
        if(!root)   return 0;
        if(!root->left && !root->right)
            return tinystl::max(height(root->left), height(root->right)) + 1;
        else if(root->left && !root->right)
            return height(root->left) + 1;
        else
            return height(root->right) + 1;
    }
    node_pointer leftRotate(node_pointer node)
    {
        node_pointer parentNode = node->parent;
        node_pointer rightNode = node->right;
        node_pointer rightLeftNode = rightNode->left;
        rightNode->left = node;
        node->parent = rightNode;
        node->right = rightLeftNode;
        if(rightLeftNode)
            rightLeftNode->parent = node;
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
        leftNode->right = node;
        node->parent = leftNode;
        node->left = leftRightNode;
        if(leftRightNode)
            leftRightNode->parent = node;
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
};


template <class Key, class Value, class Compare, class Alloc>
typename avl_tree<Key, Value, Compare, Alloc>::iterator 
avl_tree<Key, Value, Compare, Alloc>::insert(const key_type& key, const value_type& value)
{

    node_pointer insertNode(nullptr); 
    if(!root_)
    {
        root_ = createNode(key, value);
        insertNode = root_;
    }
    else
    {
        insertNode = insertAux(root_, key, value);
    }
    return iterator(insertNode);
}

template <class Key, class Value, class Compare, class Alloc>
typename avl_tree<Key, Value, Compare, Alloc>::node_pointer 
avl_tree<Key, Value, Compare, Alloc>::insertAux(node_pointer node, const key_type& key, const value_type& value)
{
    if(!node)   
        return nullptr;
    if(!comp_(node->p.first, key) && !comp_(key, node->p.first))
        return node;
    if(comp_(node->p.first, key))
    {
        node->right = insertAux(node->right, key, value);
        if(!node->right)
        {
            createChildNode(node, key, value, false);
        }
    }
    else 
    {
        node->left = insertAux(node->left, key, value);
        if(!node->left)
        {
            createChildNode(node, key, value, true);
        }
    }
    node = enableHeightBalance(node);
    return node;
}


template <class Key, class Value, class Compare, class Alloc>
typename avl_tree<Key, Value, Compare, Alloc>::node_pointer 
avl_tree<Key, Value, Compare, Alloc>::eraseAux(node_pointer node, const key_type& key)
{
    if(!node)   
        return nullptr;
    node_pointer nextNode = nullptr;
    if(!comp_(node->p.first, key) && !comp_(key, node->p.first))
    {
        if(node->left)
        {
            node_pointer leftMaxNode = maxKey(node->left);
            swapNode(node, leftMaxNode);
            nextNode = leftMaxNode;
        }
        else if(node->right)
        {
            node_pointer rightMinNode = minKey(node->right);
            swapNode(node, rightMinNode);
            nextNode = rightMinNode;
        }
        eraseNodeWithOneChild(node);
        if(nextNode)
            nextNode = enableHeightBalance(nextNode); 
    }
    else
    {
        if(comp_(node->p.first, key))
        {
            nextNode = eraseAux(node->right, key);
        }
        else
        {
            nextNode = eraseAux(node->left, key);
        }
    }
    return nextNode;
}


template <class Key, class Value, class Compare, class Alloc>
typename avl_tree<Key, Value, Compare, Alloc>::const_iterator 
avl_tree<Key, Value, Compare, Alloc>::find(const key_type& key) const
{
    node_pointer node = root_;
    while(!node)
    {
        if(!comp_(node->p.first, key) && !comp_(key, node->p.first))
            return const_iterator(node);
        if(comp_(node->p.first, key))
            node = node->right;
        else
            node = node->left;
    }
    return static_cast<const_iterator>(nullptr, root_);
}

template <class Key, class Value, class Compare, class Alloc>
void avl_tree<Key, Value, Compare, Alloc>::preOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const
{
    if(!node)   return;
    os << node->p.first << delim;
    preOrderAux(node->left, delim, os);
    preOrderAux(node->right, delim, os);
}
template <class Key, class Value, class Compare, class Alloc>
void avl_tree<Key, Value, Compare, Alloc>::inOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const
{
    if(!node)   return;
    inOrderAux(node->left, delim, os);
    os << node->p.first << delim;
    inOrderAux(node->right, delim, os);
}
template <class Key, class Value, class Compare, class Alloc>
void avl_tree<Key, Value, Compare, Alloc>::postOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const
{
    if(!node)   return;
    postOrderAux(node->left, delim, os);
    postOrderAux(node->right, delim, os);
    os << node->p.first << delim;
}
template <class Key, class Value, class Compare, class Alloc>
void avl_tree<Key, Value, Compare, Alloc>::levelOrderAux(node_pointer node, const std::string& delim, std::ostream& os) const
{
    tinystl::queue<node_pointer> q;
    q.push(node);
    while(!q.empty())
    {
        node_pointer top = q.front();
        q.pop();
        os << top->p.first << delim;
        if(top->left)
            q.push(top->left);
        if(top->right)
            q.push(top->right);
    }
}
}
