#ifndef TREENODE_H
#define TREENODE_H

#include <memory>
#include <list>
#include <type_traits>
#include <stdexcept>
#include <algorithm>

#include <memory>
#include <list>

template <class T>
class Binary_Tree_Node
{
public:
	Binary_Tree_Node(T val)
		: m_value(val)
		, m_pLeftchild(nullptr)
		, m_pRightchild(nullptr)
	{}

	static std::shared_ptr<Binary_Tree_Node> createNode(T val)
	{
		return std::make_shared<Binary_Tree_Node>(val);
	}

	Binary_Tree_Node(const Binary_Tree_Node& other)
		: m_value(other.m_value)
		, m_pLeftchild(other.m_pLeftchild)
		, m_pRightchild(other.m_pRightchild)
	{}

	~Binary_Tree_Node() {}

	Binary_Tree_Node& operator=(const Binary_Tree_Node& other)
	{
		m_value = other.m_value;
		m_pLeftchild = other.m_pLeftchild;
		m_pRightchild = other.m_pRightchild;
		return *this;
	}

	bool operator==(const Binary_Tree_Node& rhs)
	{
		return m_value == rhs.m_value;
	}
	bool operator!=(const Binary_Tree_Node& rhs)
	{
		return !(m_value == rhs.m_value);
	}
	bool operator<(const Binary_Tree_Node& rhs)
	{
		return m_value < rhs.m_value;
	}

public:
	std::shared_ptr<Binary_Tree_Node> leftChild()  { return m_pLeftchild; }
	std::shared_ptr<Binary_Tree_Node> rightChild() { return m_pRightchild; }

	void setLeftChild(std::shared_ptr<Binary_Tree_Node> pChild) { m_pLeftchild = pChild; }
	void setRightChild(std::shared_ptr<Binary_Tree_Node> pChild) { m_pRightchild = pChild; }

	T getValue() const { return m_value; }
	void setValue(T val) { m_value = val; }

	void swapChildren()
	{
		auto pTempLeft = m_pLeftchild;
		m_pLeftchild = m_pRightchild;
		m_pRightchild = pTempLeft;
	}

private:

	T m_value;

	std::shared_ptr<Binary_Tree_Node> m_pLeftchild;
	std::shared_ptr<Binary_Tree_Node> m_pRightchild;
};

#endif // !TREENODE_H