#ifndef BSTREE
#define BSTREE
#define COPY_WORKING 0

#include <vector>
#include <iostream>

class BSTree {
	private:
		class Node {
			public:
				Node *left;
				Node *right;
				Node *parent;
				int data; /* 0 - hot left, cold right; 1 - hot right, cold left */
				Node(int val, Node *parent){this->data = val; this->parent = parent;
					this->left = NULL; this->right = NULL;};
				~Node(){delete left; delete right;};
		};
		Node *root;
		bool insertHelper(int val, Node *next);
		bool findHelper(int val, Node *next);
		void sortedArrayHelper(std::vector<int> &list, Node *next);
		bool removeTwoChildren(Node *n);
		bool removeOneChildren(Node *n);
		bool removeNoChildren(Node *n);
		bool removeHelper(int val, Node *n);
		void destroy(Node *n);
		void cloneTree(Node *n);
		void balanceHelper(std::vector<int> list, int start, int end);
		int findHeight(Node *node);
	public:
		BSTree();
		BSTree(const BSTree &obj);
		~BSTree();
		bool empty();
		bool insert(int val);
		bool find(int val);
		void sortedArray(std::vector<int> &list);
		bool remove(int val);
		void balance();
		int height();
};

#endif
