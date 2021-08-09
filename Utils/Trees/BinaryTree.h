#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "TreeNode.h"

template <class T>
class Binary_Tree
{
public:

private:
	std::shared_ptr<Binary_Tree_Node<T>> m_pHead;

	unsigned int m_leftDepth;
	unsigned int m_rightDepth;

public:
	// Binary tree iterator class
	class Iterator {
	public:
		using iterator_category = std::forward_iterator_tag;
		using difference_type   = std::ptrdiff_t;
		using value_type        = Binary_Tree_Node<T>;
		using pointer           = Binary_Tree_Node<T>*;  // or also value_type*
		using reference         = Binary_Tree_Node<T>&;  // or also value_type&

		Iterator(pointer ptr) : m_ptr(ptr) {}

		reference operator*() const { return *m_ptr; }
		pointer operator->()        { return m_ptr; }

		// no real use for increment operators here
		Iterator& operator++() = delete;
		Iterator operator++(int) = delete;

		// this is how we step through
		Iterator& nextLeft()  { m_ptr = m_ptr->leftChild().get();  return *this; }
		Iterator& nextRight() { m_ptr = m_ptr->rightChild().get(); return *this; }

		friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
		friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };

	private:
		pointer m_ptr;
	};

	// Iterator access 
	Iterator begin() { return Iterator(m_pHead.get()); }
	Iterator end()   { return Iterator(nullptr); }

public:
	// static tree factory methods
	static std::shared_ptr<Binary_Tree> createTree(std::shared_ptr<Binary_Tree_Node<T>> head = nullptr) {  return std::make_shared<Binary_Tree>(head); }
	static std::shared_ptr<Binary_Tree> createTree(T firstValue) { return std::make_shared<Binary_Tree>(firstValue); }

	Binary_Tree(const Binary_Tree& other) 
		: m_pHead(other.m_pHead)
		, m_leftDepth(other.m_leftDepth)
		, m_rightDepth(other.m_rightDepth) {}

	virtual ~Binary_Tree() {}

	Binary_Tree& operator=(const Binary_Tree& other) {
		m_pHead		 = other.m_pHead;
		m_leftDepth  = other.m_leftDepth;
		m_rightDepth = other.m_rightDepth;
		return *this;
	}

	// Constructors
	Binary_Tree(std::shared_ptr<Binary_Tree_Node<T>> head)
		: m_pHead(head)
		, m_leftDepth(0u)
		, m_rightDepth(0u) {}
	Binary_Tree(T val)
		: m_pHead(nullptr)
		, m_leftDepth(0u)
		, m_rightDepth(0u)
	{
		m_pHead = Binary_Tree_Node<T>::createNode(val);
	}
public:
	std::shared_ptr<Binary_Tree_Node<T>> head() { return m_pHead; }

	// add a value to the tree
	void push(T val);
	

	// access values
	Iterator find(T val) {
		auto iter = begin();
		return binarySearch(iter, val);
	}

	// erase values from the tree, return an iterator
	Iterator& erase(Iterator& valIter) {

		return valIter;
	}

	// clear the tree
	void clear() { m_pHead = nullptr; }

private:
	void balanceLeft(std::shared_ptr<Binary_Tree_Node<T>> pNode);
	void balanceRight(std::shared_ptr<Binary_Tree_Node<T>> pNode);
	
	Iterator& binarySearch(Iterator& valIter, T target) 
	{
		if (valIter == end())			        { return valIter; }
		else if (valIter->getValue() == target) { return valIter; }
		else if (target > valIter->getValue())  { return binarySearch(valIter.nextRight(), target); }
		else								    { return binarySearch(valIter.nextLeft(), target);  }
	}

	std::shared_ptr<Binary_Tree_Node<T>> findFirstLeaf(std::shared_ptr<Binary_Tree_Node<T>> pNode, T target, uint32_t& depth);
	
};



template<class T>
void Binary_Tree<T>::push(T val)
{
	if (find(val) != end()) return; // no duplicates

	if (!m_pHead)
	{
		m_pHead = Binary_Tree_Node<T>::createNode(val);
	}
	else
	{
		uint32_t depth = 0u;
		findFirstLeaf(m_pHead, val, depth);
		if (val < m_pHead->getValue())
		{
			if (depth > m_leftDepth) m_leftDepth = depth;
		}
		else
		{
			if (depth > m_rightDepth) m_rightDepth = depth;
		}
	}
	int balance = m_rightDepth - m_leftDepth;
	if (balance < -1)
	{
		balanceLeft(m_pHead->leftChild());
	}
	else if (balance > 1)
	{
		balanceRight(m_pHead->rightChild());
	}
}

template<class T>
void Binary_Tree<T>::balanceLeft(std::shared_ptr<Binary_Tree_Node<T>> pNode)
{
	// find the first node in the left side with an empty right leaf
	std::list<std::shared_ptr<Binary_Tree_Node<T>>> nodeStack;

	std::shared_ptr<Binary_Tree_Node<T>> pNewHead = nullptr;
	std::shared_ptr<Binary_Tree_Node<T>> pTempHead = nullptr;
	std::shared_ptr<Binary_Tree_Node<T>> pTempNodeParent = pNode;

	m_leftDepth--;

	if (!pNode->rightChild())
	{
		pNewHead = pNode;
		m_pHead->setLeftChild(nullptr);
		pNewHead->setRightChild(m_pHead);
		m_pHead = pNewHead;
		return;
	}
	else
	{
		nodeStack.push_front(pNode->rightChild());
		while (!nodeStack.empty())
		{
			auto pTempNode = nodeStack.front();
			nodeStack.pop_front();

			if (!pTempNode->rightChild())
			{
				pNewHead = pTempNode;
				break;
			}
			pTempNodeParent = pTempNode;
			nodeStack.push_front(pTempNode->rightChild());
		}
	}

	// cache the head's left child
	std::shared_ptr<Binary_Tree_Node<T>> pTempLeft = m_pHead->leftChild();

	// remove the old parent's left child
	m_pHead->setLeftChild(nullptr);

	// set the new heads right child to the old head
	pNewHead->setRightChild(m_pHead);

	// replace the new head with the the new head's left value
	pTempNodeParent->setRightChild(pNewHead->leftChild());

	// set the new heads right child to the old head
	pNewHead->setLeftChild(pTempLeft);

	// set the head to the new value
	m_pHead = pNewHead;
}

template<class T>
void Binary_Tree<T>::balanceRight(std::shared_ptr<Binary_Tree_Node<T>> pNode)
{
	// find the first node in the left side with an empty right leaf
	std::list<std::shared_ptr<Binary_Tree_Node<T>>> nodeStack;

	std::shared_ptr<Binary_Tree_Node<T>> pNewHead = nullptr;
	std::shared_ptr<Binary_Tree_Node<T>> pTempHead = nullptr;
	std::shared_ptr<Binary_Tree_Node<T>> pTempNodeParent = pNode;

	m_rightDepth--;

	if (!pNode->leftChild())
	{
		pNewHead = pNode;
		m_pHead->setRightChild(nullptr);
		pNewHead->setLeftChild(m_pHead);
		m_pHead = pNewHead;
		return;
	}
	else
	{
		nodeStack.push_front(pNode->leftChild());
		while (!nodeStack.empty())
		{
			auto pTempNode = nodeStack.front();
			nodeStack.pop_front();

			if (!pTempNode->leftChild())
			{
				pNewHead = pTempNode;
				break;
			}
			pTempNodeParent = pTempNode;
			nodeStack.push_front(pTempNode->leftChild());
		}
	}

	// cache the head's right child
	std::shared_ptr<Binary_Tree_Node<T>> pTempRight = m_pHead->rightChild();

	// remove the old parent's right child
	m_pHead->setRightChild(nullptr);

	// set the new heads right child to the old head
	pNewHead->setRightChild(m_pHead);

	// replace the new head with the the new head's right value
	pTempNodeParent->setLeftChild(pNewHead->rightChild());

	// set the new heads right child to the old head
	pNewHead->setRightChild(pTempRight);

	// set the head to the new value
	m_pHead = pNewHead;
}

template<class T>
std::shared_ptr<Binary_Tree_Node<T>> Binary_Tree<T>::findFirstLeaf(std::shared_ptr<Binary_Tree_Node<T>> pNode, T target, uint32_t& depth)
{
	depth++;

	if (target < pNode->getValue())
	{
		if (!pNode->leftChild())
		{
			pNode->setLeftChild(Binary_Tree_Node<T>::createNode(target));
			return pNode;
		}
		return (findFirstLeaf(pNode->leftChild(), target, depth));
	}
	else
	{
		if (!pNode->rightChild())
		{
			pNode->setRightChild(Binary_Tree_Node<T>::createNode(target));
			return pNode;
		}
		return (findFirstLeaf(pNode->rightChild(), target, depth));
	}
}

#endif // !BINARYTREE_H