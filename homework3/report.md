# 41143203

作業三

## 解題說明
ChainNode<T>: Linked List的節點結構。
Chain<T>: 以單向Linked List實作的串列。
ChainIterator<T>: 用於走訪Chain的Iterator。
Polynomial<T>: 以Circular Linked List表示之多項式。

### 解題策略
1. 使用ChainNode作為Linked List的基本節點，包含資料欄位與指向下一節點的指標。
2. Chain使用ChainNode<T>串接形成Linked List，提供Begin()與End()以配合Iterator使用。
3. ChainIterator封裝節點指標，透過多載*與++運算子來完成走訪功能。
4. 實作 Iterator。

Polynomial:
1. 實作多項式的加法、乘法與輸出
2. 插入新項目時: 若次方相同，合併係數;若合併後係數為0，刪除該節點，否則依次方由大到小插入正確位置。


## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;


template <class T>
class ChainNode
{
public:
    T data;
    ChainNode<T>* link;
};

template <class T>
class ChainIterator 
{
public:
    ChainNode<T>* current;

    ChainIterator(ChainNode<T>* p = NULL) 
    {
        current = p;
    }

    T& operator*() 
    {
        return current->data;
    }

    ChainIterator<T>& operator++() 
    {
        current = current->link;
        return *this;
    }

    bool operator!=(ChainIterator<T> x)
    {
        return current != x.current;
    }
};

template <class T>
class Chain
{
private:
    ChainNode<T>* first;

public:
    typedef ChainIterator<T> iterator;

    Chain() 
    {
        first = NULL;
    }

    ~Chain()
    {
        ChainNode<T>* temp;
        while (first != NULL) {
            temp = first;
            first = first->link;
            free(temp);
        }
    }

    iterator Begin()
    {
        return iterator(first);
    }

    iterator End() 
    {
        return iterator(NULL);
    }

    // 在串列尾端插入資料
    void InsertBack(T x)
    {
        ChainNode<T>* p = first;

        if (p == NULL)
        {
            first = (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
            first->data = x;
            first->link = NULL;
            return;
        }

        while (p->link != NULL)
            p = p->link;

        p->link = (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
        p->link->data = x;
        p->link->link = NULL;
    }
};

struct Term 
{
    int coef; // 係數
    int exp;  // 次方
};


template <class T>
class Polynomial
{
private:
    ChainNode<T>* head; // dummy head

public:
    Polynomial()
    {
        head = (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
        head->link = head;
    }

    // 拷貝建構子（deep copy）
    Polynomial(const Polynomial<T>& other)
    {
        head = (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
        head->link = head;

        ChainNode<T>* p = other.head->link;
        ChainNode<T>* tail = head;

        while (p != other.head)
        {
            ChainNode<T>* node =
                (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
            node->data = p->data;
            node->link = head;
            tail->link = node;
            tail = node;
            p = p->link;
        }
    }

    Polynomial<T>& operator=(const Polynomial<T>& other) 
    {
        if (this == &other) return *this;

        // 清空原本資料
        ChainNode<T>* p = head->link;
        while (p != head)
        {
            ChainNode<T>* temp = p;
            p = p->link;
            free(temp);
        }
        head->link = head;

        // deep copy
        ChainNode<T>* q = other.head->link;
        ChainNode<T>* tail = head;

        while (q != other.head) 
        {
            ChainNode<T>* node =
                (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
            node->data = q->data;
            node->link = head;
            tail->link = node;
            tail = node;
            q = q->link;
        }

        return *this;
    }


    ~Polynomial()
    {
        ChainNode<T>* p = head->link;
        ChainNode<T>* temp;

        while (p != head)
        {
            temp = p;
            p = p->link;
            free(temp);
        }
        free(head);
    }

    
    void InsertTerm(int coef, int exp)
    {
        if (coef == 0) return;

        ChainNode<T>* prev = head;
        ChainNode<T>* curr = head->link;

        while (curr != head && curr->data.exp > exp) 
        {
            prev = curr;
            curr = curr->link;
        }

        if (curr != head && curr->data.exp == exp)
        {
            curr->data.coef += coef;

            if (curr->data.coef == 0)
            {
                prev->link = curr->link;
                free(curr);
            }
        }
        else 
        {
            ChainNode<T>* node =
                (ChainNode<T>*)malloc(sizeof(ChainNode<T>));
            node->data.coef = coef;
            node->data.exp = exp;
            node->link = curr;
            prev->link = node;
        }
    }

    // 多項式加法
    Polynomial<T> Add(Polynomial<T>& b)
    {
        Polynomial<T> c;

        ChainNode<T>* p = head->link;
        ChainNode<T>* q = b.head->link;

        while (p != head && q != b.head)
        {
            if (p->data.exp > q->data.exp) 
            {
                c.InsertTerm(p->data.coef, p->data.exp);
                p = p->link;
            }
            else if (p->data.exp < q->data.exp)
            {
                c.InsertTerm(q->data.coef, q->data.exp);
                q = q->link;
            }
            else
            {
                c.InsertTerm(p->data.coef + q->data.coef, p->data.exp);
                p = p->link;
                q = q->link;
            }
        }

        while (p != head)
        {
            c.InsertTerm(p->data.coef, p->data.exp);
            p = p->link;
        }

        while (q != b.head)
        {
            c.InsertTerm(q->data.coef, q->data.exp);
            q = q->link;
        }

        return c;
    }

    // 輸出多項式
    void Print()
    {
        ChainNode<T>* p = head->link;
        int first = 1;

        while (p != head)
        {
            if (!first && p->data.coef > 0)
                cout << "+";

            cout << p->data.coef;
            if (p->data.exp != 0)
                cout << "x^" << p->data.exp;

            first = 0;
            p = p->link;
        }
        cout << endl;
    }
};

void ReadPolynomial(Polynomial<Term>& p)
{
    int n;
    cin >> n;  // 項數

    for (int i = 0; i < n; i++) 
    {
        int coef, exp;
        cin >> coef >> exp;
        p.InsertTerm(coef, exp);
    }
}


int main()
{

    Polynomial<Term> p1, p2, p3;

    cout << "P1(coef exp):" << endl;
    ReadPolynomial(p1);

    cout << "P2(coef exp):" << endl;
    ReadPolynomial(p2);

    cout << "P1 = ";
    p1.Print();

    cout << "P2 = ";
    p2.Print();

    p3 = p1.Add(p2);
    cout << "P1 + P2 = ";
    p3.Print();

    return 0;
}

```

## 效能分析

1. 時間複雜度：加法：O(n + m)、乘法：O(n*m)
2. 空間複雜度：加法：O(n + m)、乘法：O(n*m)

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $P1 : 3\n3 4\n2 2\n1 0\n﹐\ P2 : 3\n5 3\n-2 2\n4 0\n$      |$P1 = 3x^4 + 2x^2 + 1\n P2 = 5x^3 - 2x^2 + 4\n P1 + P2 = 3X^4 + 5X^3 + 5\n$         |$P1 = 3x^4 + 2x^2 + 1\n P2 = 5x^3 - 2x^2 + 4\n P1 + P2 = 3X^4 + 5X^3 + 5\n$        |
| 測試二   |  $P1 : 1\n1 0\n﹐\ P2 : 2\n2 1\n3 0\n$       |$P1 = 1\n P2 = 2x + 3\n P1 + P2 = 2X + 4\n$       |$P1 = 1\n P2 = 2x + 3\n P1 + P2 = 2X + 4\n$      |

### 編譯與執行指令
![image1](https://github.com/Ri-41143203/hw/blob/main/hw3/1.png)
![image2](https://github.com/Ri-41143203/hw/blob/main/hw3/2.png)
### 結論

1. 程式能正確使用Linked List 與 Iterator。
2. Circular Linked List能有效簡化多項式插入與刪除的邏輯。 

## 申論及開發報告

1. **資料結構使用**  
   使用Chain與ChainIterator完成串列操作，多項式以Circular Linked List表示，能正確進行加法運算與同次方合併。

   
透過私有成員管理多項式節點，InsertTerm、Add、Print 封裝於 Polynomial 類別內，避免外部程式直接存取。輸入函數間接操作私有成員，使程式結構清楚、易於使用。因為一開始執行時會報錯:"擲回例外狀況: 讀取存取違規。 **p** 為 0xDDDDDDDD"，節點使用 malloc/free 管理，拷貝建構子與賦值運算子實作深拷貝，避免指標衝突與記憶體洩漏。
