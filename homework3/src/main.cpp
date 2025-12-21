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
