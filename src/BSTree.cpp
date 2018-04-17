#include "BSTree.h"
#include <vector>

BSTree::BSTree()
{
	this->root = NULL;
}

BSTree::BSTree(const BSTree &obj)
{
	if(!obj.root)
		this->root = NULL;
	else
	{
		this->root = new Node(obj.root->data, NULL);
		cloneTree(obj.root);
	}
}

void BSTree::cloneTree(Node *next)
{
	std::vector<int> list;
	std::vector<int>::iterator it;
	sortedArrayHelper(list, next);
	
	for(it = list.begin(); it != list.end(); it++)
	{
		insert(*it);
	}
}

BSTree::~BSTree()
{
	destroy(this->root);
}

void BSTree::destroy(Node *n)
{
	if(n != NULL)
	{
		destroy(n->left);
		n->left = NULL;
		destroy(n->right);
		n->right = NULL;
		delete n;
		n = NULL;
	}
	this->root = NULL;
}

bool BSTree::empty()
{
	if(!this->root)
		return true;
	return false;
}

bool BSTree::insertHelper(int val, Node *next)
{
	if(next->data > val) /* go left */
	{
		if(!next->left)
		{
			next->left = new Node(val, next);
			return true;
		}else
		{
			return insertHelper(val, next->left);
		}
	}else if(next->data < val) /* go right */
	{
		if(!next->right)
		{
			next->right = new Node(val, next);
			return true;
		}else
		{
			return insertHelper(val, next->right);
		}
	}
	return false;
}

bool BSTree::insert(int val)
{
	if(!this->root)
	{
		this->root = new Node(val, NULL);
		return true;
	}
	return insertHelper(val, this->root);
}

bool BSTree::findHelper(int val, Node *next)
{
	if(next->data == val)
	{
		return true;
	}else
	{
		if(next->data > val) /* go left */
		{
			if(!next->left)
			{
				return false;
			}else
			{
				return findHelper(val, next->left);
			}
		}else if(next->data < val) /* go right */
		{
			if(!next->right)
			{
				return false;
			}else
			{
				return findHelper(val, next->right);
			}
		}
	}
	return false;
}

bool BSTree::find(int val)
{
	if(empty())
		return false;
	
	return findHelper(val, this->root);
}

void BSTree::sortedArrayHelper(std::vector<int> &list, Node *next)
{
	if(next)
	{
		if(next->left)
			sortedArrayHelper(list, next->left);
		
		list.push_back(next->data);
		
		if(next->right)
			sortedArrayHelper(list, next->right);
	}
	return;
}

void BSTree::sortedArray(std::vector<int> &list)
{
	if(!empty())
	{
		sortedArrayHelper(list, this->root);
	}
}

bool BSTree::removeTwoChildren(Node *n)
{
	/* find least value in right sub-tree */
	/* replace to-be deleted node with least value */
	/* remove node that least value came from */
	int least;
	Node *tmp = n->right;
	while(tmp)
	{
		if(tmp->data < least || !least)
			least = tmp->data;
		
		if(tmp->left)
			tmp = tmp->left;
		else if(tmp->right)
			tmp = tmp->right;
		else
			tmp = NULL;
	}
	n->data = least;
	bool dontCare = removeHelper(least, n->right);
	return true;
}

bool BSTree::removeOneChildren(Node *n)
{
	if(n->parent)
	{
		if(n->left) /* child is left */
		{
			if(n->parent->left == n) /* next is the parent's left node child  */
			{
				n->parent->left = n->left;
				n->left->parent = n->parent;
			}else 						   /* next is the parent's right node child */
			{
				n->parent->right = n->left;
				n->left->parent = n->parent;
			}
		}else /* child is right */
		{
			if(n->parent->left == n) /* next is the parent's left node child  */
			{
				n->parent->left = n->right;
				n->right->parent = n->parent;
			}else  						   /* next is the parent's right node child */
			{
				n->parent->right = n->right;
				n->right->parent = n->parent;
			}
		}
	}else /* dealing with the root */
	{
		if(n->left)
		{
			this->root = n->left;
			n->left->parent = NULL;
		}else
		{
			this->root = n->right;
			n->right->parent = NULL;
		}
	}
	n->left = NULL;
	n->right = NULL;
	delete n;
	n = NULL;
	
	return true;
}

bool BSTree::removeNoChildren(Node *n)
{
	if(n->parent)
	{
		if(n->parent->left == n)
		{
			n->parent->left = n->left;
		}else
		{
			n->parent->right = n->left;
		}
	}else
	{
		this->root = NULL;
	}
	delete n;
	n = NULL;
	return true;
}

bool BSTree::removeHelper(int val, Node *n)
{
	if(n->data == val) /* FOUND CASE */
	{
		
		if(n->left && n->right) /* 2 Children */
		{
			return removeTwoChildren(n);
		}else if(n->left || n->right) /* 1 Child */
		{
			return removeOneChildren(n);
		}else /* leaf */
		{
			return removeNoChildren(n);
		}
	}else
	{
		if(n->data > val) /* go left */
		{
			if(!n->left)
			{
				return false;
			}else
			{
				return removeHelper(val, n->left);
			}
		}else if(n->data < val) /* go right */
		{
			if(!n->right)
			{
				return false;
			}else
			{
				return removeHelper(val, n->right);
			}
		}
	}
	return false;
}

bool BSTree::remove(int val)
{
	if(!find(val))
		return false;
	return removeHelper(val, this->root);
}

void BSTree::balanceHelper(std::vector<int> list, int start, int end)
{
	int mid = (start + end) / 2;
	bool retVal = insert(list[mid]);
	if(retVal)
	{
		balanceHelper(list, start, mid-1);
		balanceHelper(list, mid+1, end);
	}
	
	return;
}

void BSTree::balance()
{
	if(!this->root)
		return;
	std::vector<int> list;
	sortedArrayHelper(list, this->root);
	destroy(this->root);
	balanceHelper(list, 0, list.size()-1);
}

int BSTree::height(){
    return findHeight(this->root);
}
int BSTree::findHeight(Node* node){
   // base case tree is empty
   if(node == NULL)
       return 0;
	int lh, rh, max;
   // If tree is not empty then height = 1 + max of left height and right heights
    lh = findHeight(node->left);
    rh = findHeight(node->right);
    max = (lh >= rh) ? lh : rh;
    return 1 + max;
}
