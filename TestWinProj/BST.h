#pragma once
#include <memory>
#include <string>

template <typename T1, typename T2>
struct myPair
{
	T1 first;
	T2 second;
};

//template <typename T1, typename T2>
//myPair<T1, T2>& make_myPair(const T1& _first, const T2& _second)
//{
//	myPair<T1, T2> pair { _first, _second};
//	return pair;
//}


template <typename T1, typename T2>
myPair<T1, T2> make_myPair(const T1& _first, const T2& _second)
{	
	return myPair<T1, T2> { _first, _second};
}


template <typename K, typename V>
struct Node
{
	myPair<K, V> pair;

	Node* parent;
	Node* LeftChild;
	Node* RightChild;
};

template <typename Key, typename Value>
class BST 
{
public:
	BST()
		: RootNode(nullptr), nodeCount(0)
	{

	}

	~BST()
	{
		// post-order travel
		post_order_release(RootNode);
		//RootNode = nullptr;
	}

	void insert(const myPair<Key, Value>& _pair);
	void post_order_release(Node<Key, Value>* node);
	Node<Key, Value>* GetInOrderSuccessor(Node<Key, Value>* _Node);

	bool isLeft(Node<Key, Value>* curNode)
	{
		if (curNode->parent->LeftChild == curNode)
			return true;
		else
			return false;
	}

	bool isRight(Node<Key, Value>* curNode)
	{
		if (curNode->parent->RightChild == curNode)
			return true;
		else
			return false;
	}

public:
	class iterator
	{
	public:
		friend class BST;

		iterator()
			: bst(nullptr),
			 curNode(nullptr)
		{}

		iterator(BST<Key, Value>* _bst, Node<Key, Value>* _node)
			: bst(_bst),
			  curNode(_node)
		{}

		/*
		* ���� �ڵ�
		* iterator(const BST<Key, Value>& _bst, const Node<Key, Value>& _node)
			: bst(_bst),
			  curNode(_node)
		{}
		*/

		bool operator == (const iterator& _other)
		{
			if (bst == _other.bst && curNode == _other.curNode)
				return true;
			else
				return false;
		}

		bool operator != (const iterator& _other)
		{
			return !(*this == _other);
		}

		const myPair<Key, Value>* operator ->()
		{
			return &curNode->pair;
		}
					 
		iterator& operator ++()
		{
			curNode = bst->GetInOrderSuccessor(curNode);
			return *this;
		}

		bool hasLeftChild()
		{
			if (nullptr != curNode->LeftChild)
				return true;
			else
				return false;
		}

		bool hasRightChild()
		{
			if (nullptr != curNode->RightChild)
				return true;
			else
				return false;
		}

		const Node<Key, Value>* GetNode()
		{
			return curNode;
		}

	private:
		BST<Key, Value>* bst;
		Node<Key, Value>* curNode;
	};

	iterator begin();
	iterator end();
	iterator find(const Key& first);
	bool erase(const Key& _key);

	Node<Key, Value>* GetRoot() { return RootNode; }

private:
	Node<Key, Value>* RootNode;
	int nodeCount;
};

template<typename Key, typename Value>
inline void BST<Key, Value>::insert(const myPair<Key, Value>& _pair)
{
	Node<Key, Value>* newNode = new Node<Key, Value>;

	newNode->pair = _pair;
	newNode->parent = nullptr;
	newNode->LeftChild = nullptr;
	newNode->RightChild = nullptr;

	Node<Key, Value>* pNode = RootNode;

	if (nullptr == pNode)
	{
		RootNode = newNode;
	}
	else
	{
		while (true)
		{
			if (pNode->pair.first > newNode->pair.first)
			{
				if (nullptr == pNode->LeftChild)
				{
					newNode->parent = pNode;
					pNode->LeftChild = newNode;
					break;
				}
				pNode = pNode->LeftChild;
			}
			else if (pNode->pair.first < newNode->pair.first)
			{
				if (nullptr == pNode->RightChild)
				{
					newNode->parent = pNode;
					pNode->RightChild = newNode;
					break;
				}
				pNode = pNode->RightChild;
			}
			else
			{
				return;
			}
		}	
	}

	nodeCount++;
}

template<typename Key, typename Value>
inline void BST<Key, Value>::post_order_release(Node<Key, Value>* node)
{
	if (nullptr != node)
	{
		post_order_release(node->LeftChild);
		post_order_release(node->RightChild);
		// std::cout << ">>" << node->pair.first << "\n";
		delete node;
	}	
}

template<typename Key, typename Value>
inline Node<Key, Value>* BST<Key, Value>::GetInOrderSuccessor(Node<Key, Value>* _Node)
{
	Node<Key, Value>* Successor = nullptr;

	// 1. ������ �ڽ��� �ִ� ���
	// ������ �ڽ����� �� �� ���� �ڽ� ��尡 ���� �� ���� ���� �ڽ����� ������
	if (nullptr != _Node->RightChild)
	{
		Successor = _Node->RightChild;

		bool b = Successor->LeftChild;

		while (Successor->LeftChild)
		{
			Successor = Successor->LeftChild;
		}
	}
	// 2. ������ �ڽ��� ���� ���
	// �θ� �ļ����ε� ���� ��尡 �θ��� ���� �ڽ��̿��� ��.
	else
	{
		Successor = _Node;

		while (true)
		{
			// ��� �ö󰬴µ� ��Ʈ ����� -> �ļ��� ���� -> end iterator
			if (nullptr == Successor->parent)
			{
					return nullptr;
			}

			// ���� �θ��� ���� �ڽ��̴�?
			if (Successor->parent->LeftChild == Successor)
			{
				Successor = Successor->parent;
				break;
			}
			else
			{
				Successor = Successor->parent;
			}
		}

	}

	return Successor;
}

template<typename Key, typename Value>
inline typename BST<Key, Value>::iterator BST<Key, Value>::begin()
{
	Node<Key, Value>* pNode = RootNode;

	while (pNode->LeftChild)
	{
		pNode = pNode->LeftChild;
	}

	return iterator(this, pNode);
}

template<typename Key, typename Value>
inline typename BST<Key,Value>::iterator BST<Key, Value>::end()
{
	return iterator(this, nullptr);
}

template<typename Key, typename Value>
inline typename BST<Key, Value>::iterator BST<Key, Value>::find(const Key& first)
{
	Node<Key, Value>* pNode = RootNode;

	while (true)
	{
		if (pNode->pair.first > first)
		{
			if (nullptr == pNode->LeftChild)
			{
				// ��ã��
				pNode = nullptr;
				break;
			}
			pNode = pNode->LeftChild;
		}
		else if (pNode->pair.first < first)
		{
			if (nullptr == pNode->RightChild)
			{
				// ��ã��
				pNode = nullptr;
				break;
			}
			pNode = pNode->RightChild;
		}
		else
		{
			// ã��
			break;
		}
	}

	return iterator(this, pNode);
}

template<typename Key, typename Value>
inline bool BST<Key, Value>::erase(const Key& _key)
{
	BST<Key, Value>::iterator iter = find(_key);

	if (end() == iter)
	{
		return false;
	}
	else
	{
		// ������� ��尡 �ܸ� ����� ���
		// �ڽ��� �����Ƿ� �θ� ����� �ڽ��� nullptr�� ����������.
		if (nullptr == iter.curNode->LeftChild && nullptr == iter.curNode->RightChild)
		{
			if (isLeft(iter.curNode))
				iter.curNode->parent->LeftChild = nullptr;
			else
				iter.curNode->parent->RightChild = nullptr;
						
			delete iter.curNode;
		}
		// ������� ��尡 �ܸ� ��尡 �ƴ� ���
		else
		{
			// �������� ���� ������ϴ� ����� �ļ���
			Node<Key, Value>* nextNode = GetInOrderSuccessor(iter.curNode);

			// �ļ��ڰ� �ڽ��� ���� ���
			// ���⵵ ���� �ڽ��� �����Ƿ� nullptr �ִ� �� ����
			if (nullptr == nextNode->LeftChild && nullptr == nextNode->RightChild)
			{
				iter.curNode->pair = nextNode->pair;

				if (isLeft(nextNode))
					nextNode->parent->LeftChild = nullptr;
				else
					nextNode->parent->RightChild = nullptr;

				delete nextNode;

			}
			// �ļ��ڰ� �ڽ��� �ִ� ���
			else
			{
				iter.curNode->pair = nextNode->pair;

				// ������ ���� �� �ļ��� ��带 �����ؾ� �Ѵ�. 
				// �ļ��ڰ� �ڽ��� �־ �ڽİ� �ļ����� �θ� ��������� �Ѵ�.
				nextNode->RightChild->parent = nextNode->parent;

				// �ļ��ڰ� ���� �ڽ��̸� �ļ����� �ڽ� ��带 �ļ����� �θ����� ���� �ڽ����� ����
				// �ƴϸ� �� �ݴ�
				if (isLeft(nextNode))
					nextNode->parent->LeftChild = nextNode->LeftChild;
				else
					nextNode->parent->RightChild = nextNode->RightChild;
					
				

				delete nextNode;
			}
		}
	}	

	return true;
}



