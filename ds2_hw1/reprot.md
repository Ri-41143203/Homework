# 41143203

作業一

## 解題說明
MinPQ<T>: Priority Queue的抽象類別，定義基本操作介面。
MinHeap<T>: 以Binary Heap實作的最小優先佇列。
Node: Binary Search Tree的節點結構。
BST: Binary Search Tree的基本操作與高度計算。

### 解題策略
MinHeap:
1. 使用vector作為底層儲存結構，模擬Binary Heap。
2. 插入元素時，先放到陣列尾端，再透過HeapifyUp向上調整。
3. 刪除最小元素時，將最後一個元素移到 root，再透過 HeapifyDown 向下調整。
4. Heap中最小元素永遠位於root，因此Top()可以直接回傳第一個元素。

BST:
1. 使用Node結構表示BST的節點，包含key、left與right指標。
2. 插入新項目時: 依照BST特性，小於放左子樹，大於放右子樹。


## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

1. 時間複雜度：Push: O($\log n$)、Pop: O($\log n$)、Top: O(1)
2. 空間複雜度：O(n)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $Push(10), Push(4), Push(15), Push(2)$      |$Top element: 2$         |$Top element: 2$        |
| 測試二   |  $Pop()$       |$Top after pop: 4$       |$Top after pop: 4$      |

### 編譯與執行指令
![image1]([https://github.com/Ri-41143203/hw/blob/main/hw3/1.png](https://github.com/Ri-41143203/hw/blob/main/hw1/%E8%9E%A2%E5%B9%95%E6%93%B7%E5%8F%96%E7%95%AB%E9%9D%A2%202026-03-28%20232858.png))
### 結論

1. MinHeap能正確維持最小堆積的特性，Push與Pop操作皆能保持heap結構。
2. Binary Search Tree在隨機插入資料時，其高度大致與$\log_2 n$成比例。

## 申論及開發報告

1. **資料結構使用**  
   使用Vector來實作Binary Heap，使得節點存取可以透過 index 計算父節點與子節點位置。
  
透過隨機數產生不同大小的資料集合，插入BST並測量樹高，再與$\log_2 n$進行比較，觀察BST在平均情況下的高度成長趨勢。
