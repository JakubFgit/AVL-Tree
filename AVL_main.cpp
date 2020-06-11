#include <iostream>
#include <iomanip>

using namespace std;

template <typename key, typename info>
class Dictionary {
	struct Node{
		key k;
		info i;
		int balance_factor;
		Node* left;
		Node* right;
		Node(const key& new_k, const info& new_i) : k(new_k), i(new_i), left(NULL), right(NULL) {};
	};
	Node*root;

 void insert(Node* &n, const key &k, const info &i)
    {
        if(n == NULL)
        {
            n = new Node(k, i);
            set_balance_factors(root);
        }
        else if(k < n->k)
            insert(n->left, k, i);
        else if(k > n->k)
            insert(n->right, k, i);
        else
           return;
    }
     void RRotation(Node* n)
    {
        if (n == root)
        {
            root = n->left;
            n->left = n->left->right;
            root->right = n;
            return;
        }

        Node * prev = root;
        while (prev->right != n && prev->left != n)
        {
           if (n->k < prev->k)
              prev = prev->left;
           else if (n->k > prev->k)
              prev = prev->right;
        }

        if (prev->right == n)
            prev->right = n->left;
        if (prev->left == n)
            prev->left = n->left;
        Node * temp = n->left->right;
        n->left->right = n;
        n->left = temp;
    }

    void LRotation(Node* n)
    {
        if (n == root) {
        root = n->right;
        n->right = n->right->left;
        root->left = n;
        return;
     }

     Node * prev = root;
     while (prev->right != n && prev->left != n)
        {
           if (n->k < prev->k)
              prev = prev->left;
           else if (n->k > prev->k)
              prev = prev->right;
        }

     if (prev->right == n)
        prev->right = n->right;
     if (prev->left == n)
        prev->left = n->right;
     Node * temp = n->right->left;
     n->right->left = n;
     n->right = temp;
    }

	void clear(Node* n) {
		if (!n) {
			return;
		}
		clear(n->left);
		clear(n->right);
		delete n;
	}

Node *copy(Node* &lhs, Node* const &rhs)
    {
        if(rhs == NULL)
            return NULL;

        lhs = new Node(rhs->key, rhs->info);
        lhs->left = copy(lhs->left, rhs->left);
        lhs->right = copy(lhs->right, rhs->right);
        return lhs;
    }
    int difference(Node* n)
    {
        int leftHeight = height(n->left);
        int rightHeight = height(n->right);
        int balance_factor = leftHeight - rightHeight;
        return balance_factor;
    }
	int set_balance_factors(Node * const & n)
    {
        if(n == NULL)
            return 0;
        int heightLeft = set_balance_factors(n->left);
        int heightRight = set_balance_factors(n->right);
        n->balance_factor = heightLeft - heightRight;
        //Balance//
        if(n->balance_factor > 1)
        {
            if(difference(n->left) > 0)
            {
                RRotation(n);
                return set_balance_factors(n);
            }
            else
            {
                if(n->right == NULL)
                    return 0;
                RRotation(n->right);
                LRotation(n);
                return set_balance_factors(n);
            }
        }
        if(n->balance_factor < -1)
        {
            if(difference(n->right) > 0)
            {
                if(n->left == NULL)
                    return 0;
                LRotation(n->left);
                RRotation(n);
                return set_balance_factors(n);
            }
            else
            {
                LRotation(n);
                return set_balance_factors(n);
            }
        }
        //return heightMax + 1
        if(height(n->left) < height(n->right))
            return (height(n->right) + 1);
        return height(n->left) + 1;
    }
	int height(Node* n)
	{
		int h = 0;
		if (n != NULL)
		{
			int left_Height = height(n->left);
			int right_Height = height(n->right);
			int max_Height = 0;
            if(left_Height < right_Height)
                max_Height = right_Height;
            else
                max_Height = left_Height;
			h = max_Height + 1;
		}
		return h;
	}
	 void print(Node* const & n, int space) const
    {
        if (n != NULL)
        {
            print(n->right, space+10);
            cout << setw(space) << " " << n->k << " " << n->i << " " << n->balance_factor;
            print(n->left, space+10);
        }
        else
            cout << setw(space) << " " <<  endl;
    }
public:
    Dictionary();
    ~Dictionary();
    friend ostream &operator << (ostream &o, const Dictionary<key, info> &Dictionary)
    {
        Dictionary.print();
        return o;
    };
    Dictionary(const Dictionary<key, info> &Dictionary);
    Dictionary<key, info> &operator = (const Dictionary<key, info> &Dictionary);
    bool operator == (const Dictionary<key, info> &Dictionary);
    bool operator != (const Dictionary<key, info> &Dictionary);
    bool isEmpty()const {return (root == NULL);};
    void addNode(const key& k, const info& i);
    void remove(const key &k);
    void clear() {clear(root); root = NULL;};
    void print() const {print(root, 0);};
    bool isExist(const key &k) const;
    bool compare(Node* const &lhs, Node* const & rhs);
};

template<typename key, typename info>
Dictionary<key, info>::Dictionary() //constructor
{

    root = NULL;
}

template<typename key, typename info>
Dictionary<key, info>::~Dictionary() //destructor
{

    if(root)
        clear(root);
    root = NULL;
}

template<typename key, typename info>
Dictionary<key, info>::Dictionary(const Dictionary<key, info> &Dictionary) //copyconstructor
{

    root = copy(Dictionary.root);
}
template <typename key, typename info>
void Dictionary<key, info>::addNode(const key &k, const info &i)
{
    insert(root, k, i);
}
template <typename key, typename info>
void Dictionary<key, info>::remove(const key &k)
{
    if(root == NULL)
    {

        return;
    }
    if(!isExist(k))
    {
        return;
    }
    Node *n = root;
    Node *n_parent= NULL;
    if((n->left == NULL) && (n->right == NULL)) // remove root only
    {
        this->~Dictionary();
        return;
    }
    while(n)
    {
        if(n->k != k)
        {
            if (n == NULL)
                break;
            else
            {
                if(n->k > k)
                {
                    n_parent = n;
                    n = n->left;
                }
                else if(n->k < k)
                {
                    n_parent = n;
                    n = n->right;
                }
            }
        }
        else // n->key == key
        break;
    }
    cout<< "Remove node: "<< n->k <<" "<< n->i << " " << n->balance_factor <<endl;
    cout << "---"<< endl;
    if((n->left) && (n->right)) // 2 children
    {
        Node *succesor = n->right;
        n_parent = n;
        while(succesor->left)
        {
              n_parent = succesor;
              succesor = succesor->left;
        }
        n->k = succesor->k;
        n->i = succesor->i;
        n = succesor;
    }
    Node *subtree = n->left; // 0 or 1 child
    if(subtree == NULL)
        subtree = n->right;
    if((n_parent == NULL) && (n)) // two nodes scenario
    {
        n->k = subtree->k;
        n->i = subtree->i;
        n->left = NULL;
        n->right = NULL;
        delete subtree;
        set_balance_factors(root);
        return;
    }
    if(n_parent->left == n)
        n_parent->left = subtree;
    else
        n_parent->right = subtree;

    delete n;
    set_balance_factors(root);
}



template<typename key, typename info>
bool Dictionary<key, info>::isExist(const key& k) const
{
    Node *n = root;
    while(n != NULL)
    {
        if(n->k != k)
        {
            if (n == NULL)
                break;
            else
            {
                if(n->k > k)
                    n = n->left;
                else if(n->k < k)
                    n = n->right;
            }
        }
        else // n->key == key
            return true;
    }
    cout << "Key: " << k << " not exist" << endl;
    return false;
}
template<typename key, typename Info>
bool Dictionary<key, Info> :: compare(Node* const &lhs, Node* const & rhs)
{
    if(lhs == NULL && rhs == NULL)
        return true;
    else if(lhs == NULL && rhs)
        return false;
    else if(lhs && rhs == NULL)
        return false;
    else if(lhs && rhs)
    {
        if(lhs->key != rhs->key)
        {
            if(lhs->info != rhs->info)
                return false;
        }
    }
    else if(!compare(lhs->left, rhs->left))
    {
        if(!compare(lhs->right, rhs->right))
            return false;
    }
    else
        return true;
}
template<typename key, typename Info>
Dictionary<key, Info>&Dictionary<key, Info> :: operator = (const Dictionary<key, Info> &Dictionary)
{
    if(root == Dictionary.root)
        return *this;
    this->~Dictionary();
    if(Dictionary.root == NULL)
        return *this;
    root = copy(root, Dictionary.root);
     set_balance_factors(root);
    return *this;
}

template<typename key, typename Info>
bool Dictionary<key, Info> :: operator == (const Dictionary<key, Info> &Dictionary)
{
    return compare(root, Dictionary.root);
}

template<typename key, typename Info>
bool Dictionary<key, Info> :: operator != (const Dictionary<key, Info> &Dictionary)
{
    return !(*this == Dictionary);
}

int main() {
	Dictionary<int, int> tree;
	tree.addNode(1, 1);
	tree.addNode(2, 2);
	tree.addNode(2, 2);
	tree.addNode(3, 3);
	tree.addNode(4, 4);
	tree.addNode(5, 5);
	tree.addNode(6, 6);
	tree.addNode(7, 7);
    tree.print();
	tree.remove(2);


	tree.print();

	return 0;
}
