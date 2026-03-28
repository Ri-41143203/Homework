#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

//MinHeap
template <class T>
class MinPQ
{
public:
    virtual ~MinPQ() {}
    virtual bool IsEmpty() const = 0;
    virtual const T& Top() const = 0;
    virtual void Push(const T&) = 0;
    virtual void Pop() = 0;
};

template <class T>
class MinHeap : public MinPQ<T>
{

private:

    vector<T> heap;

    void HeapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;

            if (heap[parent] <= heap[index])
                break;

            swap(heap[parent], heap[index]);
            index = parent;
        }
    }

    void HeapifyDown(int index)
    {
        int size = heap.size();

        while (true)
        {
            int left = index * 2 + 1;
            int right = index * 2 + 2;
            int smallest = index;

            if (left < size && heap[left] < heap[smallest])
                smallest = left;

            if (right < size && heap[right] < heap[smallest])
                smallest = right;

            if (smallest == index)
                break;

            swap(heap[index], heap[smallest]);
            index = smallest;
        }
    }

public:

    bool IsEmpty() const
    {
        return heap.empty();
    }

    const T& Top() const
    {
        return heap.front();
    }

    void Push(const T& x)
    {
        heap.push_back(x);
        HeapifyUp(heap.size() - 1);
    }

    void Pop()
    {
        if (IsEmpty()) return;

        heap[0] = heap.back();
        heap.pop_back();
        HeapifyDown(0);
    }
};

//Binary Search Tree
struct Node 
{

    int key;
    Node* left;
    Node* right;

    Node(int k)
    {
        key = k;
        left = NULL;
        right = NULL;
    }
};

class BST 
{

public:

    Node* root;

    BST()
    {
        root = NULL;
    }

    Node* insert(Node* node, int key)
    {
        if (node == NULL)
            return new Node(key);

        if (key < node->key)
            node->left = insert(node->left, key);

        else
            node->right = insert(node->right, key);

        return node;
    }

    int height(Node* node)
    {
        if (node == NULL)
            return 0;

        int leftH = height(node->left);
        int rightH = height(node->right);

        return 1 + max(leftH, rightH);
    }

    Node* findMin(Node* node)
    {
        while (node->left != NULL)
            node = node->left;

        return node;
    }

    Node* deleteNode(Node* node, int key)
    {
        if (node == NULL)
            return NULL;

        if (key < node->key)
            node->left = deleteNode(node->left, key);

        else if (key > node->key)
            node->right = deleteNode(node->right, key);

        else
        {
            if (node->left == NULL)
            {
                Node* temp = node->right;
                delete node;
                return temp;
            }

            else if (node->right == NULL)
            {
                Node* temp = node->left;
                delete node;
                return temp;
            }

            Node* temp = findMin(node->right);

            node->key = temp->key;

            node->right = deleteNode(node->right, temp->key);
        }

        return node;
    }
};


int main()
{

    srand(time(0));

    cout << "====== MinHeap Test ======" << endl;

    MinHeap<int> heap;

    heap.Push(10);
    heap.Push(4);
    heap.Push(15);
    heap.Push(2);

    cout << "Top element: " << heap.Top() << endl;

    heap.Pop();

    cout << "Top after pop: " << heap.Top() << endl;



    cout << endl;
    cout << "====== BST Height Experiment ======" << endl;

    int sizes[] = { 100,500,1000,2000,3000,4000,5000,6000,7000,8000,9000,10000 };

    for (int n : sizes)
    {
        BST tree;

        for (int i = 0; i < n; i++)
        {
            int value = rand();
            tree.root = tree.insert(tree.root, value);
        }

        int h = tree.height(tree.root);

        double ratio = h / log2(n);

        cout << "n = " << n
            << "  height = " << h
            << "  height/log2(n) = " << ratio
            << endl;
    }

    return 0;
}