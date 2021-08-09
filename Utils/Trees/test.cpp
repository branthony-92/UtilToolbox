#include "stdafx.h"
#include "BinaryTree.h"

typedef Binary_Tree<int>                 BinaryTree_Int;
typedef std::shared_ptr<BinaryTree_Int>  BinaryTree_Int_Ptr;

typedef Binary_Tree_Node<int>                 BinaryTreeNode_Int;
typedef std::shared_ptr<BinaryTreeNode_Int>   BinaryTreeNode_Int_Ptr;

struct StudentInfo
{
	uint32_t ID;
	std::string name;
	StudentInfo(uint32_t id, std::string n) : ID(id), name(n) {}
	bool operator<(const StudentInfo& rhs)
	{
		return ID < rhs.ID;
	}
	bool operator>(const StudentInfo& rhs)
	{
		return ID > rhs.ID;
	}
	bool operator==(const StudentInfo& rhs)
	{
		return ID == rhs.ID;
	}
};

void printTree(BinaryTree_Int_Ptr& t)
{
	std::list<BinaryTreeNode_Int_Ptr> queue;

	queue.push_back(t->head());
}

void printBT(const std::string& prefix, const BinaryTreeNode_Int_Ptr node, bool isLeft)
{
	if (node != nullptr)
	{
		std::cout << prefix;

		std::cout << (isLeft ? ":-" : "'-");

		// print the value of the node
		std::cout << node->getValue() << std::endl;

		// enter the next tree level - left and right branch
		printBT(prefix + (isLeft ? ":   " : "    "), node->leftChild(), true);
		printBT(prefix + (isLeft ? ":   " : "    "), node->rightChild(), false);
	}
}

int main()
{
	auto pTree = Binary_Tree<int>::createTree();
	for (auto i = 1; i <= 5; i++)
	{
		pTree->push(i);
		printBT("", pTree->head(), false);
	}
}