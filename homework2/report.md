# 41143203

作業二

## 解題說明
Problem1: 實作多項式類別Polynomial，包含加法、乘法與代入運算。
Problem2: 使用輸入輸出運算子 >> ﹑<< 讓多項式可以輸入與輸出。


### 解題策略
Problem1:
遞迴:
1. 設計Term類別表示多項式中的單項（係數 + 指數）。
2. Polynomial使用動態陣列儲存Term，並保持降冪排序。
3. 插入新的一項時，若指數相同則合併係數；若陣列容量不足，自動增加容量。
4. 實作加法與乘法，分別合併或兩兩相乘後再合併。
5. 實作Eval(x)，計算多項式在x的值。


Problem2:
1. 運算子多載>>讀入多項式，第一行輸入項數n，讓接著n行輸入每項係數與指數。
2. 運算子多載<<列印多項式，強制輸出X^0/X^1的形式，並依正負號格式化。

## 程式實作

以下為主要程式碼：

```cpp
#include <iostream>
#include <cmath>
using namespace std;

//Problem1
class Polynomial;

class Term {
    friend class Polynomial;
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
    friend istream& operator>>(istream& is, Polynomial& poly);
private:
    float coef;
    int exp;
};

class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    void expand() {
        capacity *= 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++)
            temp[i] = termArray[i];
        delete[] termArray;
        termArray = temp;
    }

public:

    Polynomial(int cap = 2) : capacity(cap), terms(0) {
        termArray = new Term[capacity];
    }

    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
    }

    Polynomial& operator=(const Polynomial& other) {
        if (this == &other) return *this;
        delete[] termArray;
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
        return *this;
    }

    ~Polynomial() { delete[] termArray; }

    void newTerm(float coef, int exp) {
        if (fabs(coef) < 1e-6) return;
        if (terms == capacity) expand();

        int i;
        for (i = 0; i < terms; i++) {
            if (termArray[i].exp < exp) break;
            if (termArray[i].exp == exp) {
                termArray[i].coef += coef;
                if (fabs(termArray[i].coef) < 1e-6) {
                    for (int j = i; j < terms - 1; j++)
                        termArray[j] = termArray[j + 1];
                    terms--;
                }
                return;
            }
        }
        for (int j = terms; j > i; j--)
            termArray[j] = termArray[j - 1];
        termArray[i].coef = coef;
        termArray[i].exp = exp;
        terms++;
    }

    Polynomial Add(const Polynomial& b) const {
        Polynomial c;
        int aPos = 0, bPos = 0;
        while (aPos < terms && bPos < b.terms) {
            if (termArray[aPos].exp == b.termArray[bPos].exp) {
                float sum = termArray[aPos].coef + b.termArray[bPos].coef;
                if (fabs(sum) > 1e-6) c.newTerm(sum, termArray[aPos].exp);
                aPos++; bPos++;
            }
            else if (termArray[aPos].exp > b.termArray[bPos].exp) {
                c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
                aPos++;
            }
            else {
                c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
                bPos++;
            }
        }
        while (aPos < terms) {
            c.newTerm(termArray[aPos].coef, termArray[aPos].exp);
            aPos++;
        }
        while (bPos < b.terms) {
            c.newTerm(b.termArray[bPos].coef, b.termArray[bPos].exp);
            bPos++;
        }
        return c;
    }

    // 乘法
    Polynomial Mult(const Polynomial& b) const {
        Polynomial result(terms * b.terms + 2);
        for (int i = 0; i < terms; i++)
            for (int j = 0; j < b.terms; j++)
                result.newTerm(
                    termArray[i].coef * b.termArray[j].coef,
                    termArray[i].exp + b.termArray[j].exp
                );
        return result;
    }

    float Eval(float x) const {
        float sum = 0.0f;
        for (int i = 0; i < terms; i++)
            sum += termArray[i].coef * pow(x, termArray[i].exp);
        return sum;
    }

    friend istream& operator>>(istream& is, Polynomial& poly);
    friend ostream& operator<<(ostream& os, const Polynomial& poly);
};

//Problem2

istream& operator>>(istream& is, Polynomial& poly) {
    int n;
    is >> n;
    for (int i = 0; i < n; i++) {
        float c; int e;
        is >> c >> e;
        poly.newTerm(c, e);
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& poly) {
    if (poly.terms == 0) { os << "0"; return os; }
    bool first = true;
    for (int i = 0; i < poly.terms; i++) {
        float coef = poly.termArray[i].coef;
        int exp = poly.termArray[i].exp;
        if (fabs(coef) < 1e-6) continue;
        if (first) {
            if (coef < 0) os << "-";
        }
        else {
            os << (coef > 0 ? " + " : " - ");
        }
        float absCoef = fabs(coef);
        if (absCoef != floor(absCoef)) os << absCoef;
        else os << (int)absCoef;
        if (exp == 1) os << "X";
        else if (exp != 0) os << "X^" << exp;
        first = false;
    }
    return os;
}

int main() {
    Polynomial A, B;
    cout << "Enter Polynomial A:" << endl;
    cin >> A;
    cout << "Enter Polynomial B:" << endl;
    cin >> B;
    cout << "A(x) = " << A << endl;
    cout << "B(x) = " << B << endl;
    Polynomial C = A.Add(B);
    cout << "A(x)+B(x) = " << C << endl;
    Polynomial D = A.Mult(B);
    cout << "A(x)*B(x) = " << D << endl;
    cout << "A(2) = " << A.Eval(2) << endl;
    return 0;
}

```

## 效能分析

1. 時間複雜度：加法：O(n + m)、乘法：O(n*m)
2. 空間複雜度：O(n*m)。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $A:2\n2 2\n1 0\n﹐\ B : 3\n1 2\n3 1\n4 0\n$      |A(x) = 2x^2 + 1\nB(x) = 1x^2 + 3x + 4\nA(x) + B(x) = 3X^2 + 3X + 5\nA(x)*B(x) = 2X^4 + 6X^3 + 9X^2 + 3X + 4\nA(2) = 9\n         | A(x) = 2x^2 + 1\nB(x) = 1x^2 + 3x + 4\nA(x) + B(x) = 3X^2 + 3X + 5\nA(x)*B(x) = 2X^4 + 6X^3 + 9X^2 + 3X + 4\nA(2) = 9\n        |
| 測試二   |  $A:1\n1 0\n﹐\ B : 4\n2 3\n5 2\n6 1\n3 0\n$       |A(x) = 1\nB(x) = 2X^3 + 5X^2 + 6X + 3\nA(x)+B(x) = 2X^3 + 5X^2 + 6X + 4\nA(x)*B(x) = 2X^3 + 5X^2 + 6X + 3\nA(2) = 1        |A(x) = 1\nB(x) = 2X^3 + 5X^2 + 6X + 3\nA(x)+B(x) = 2X^3 + 5X^2 + 6X + 4\nA(x)*B(x) = 2X^3 + 5X^2 + 6X + 3\nA(2) = 1       |

### 編譯與執行指令
![image1](https://github.com/Ri-41143203/hw/blob/main/hw2/1.png)
![image2](https://github.com/Ri-41143203/hw/blob/main/hw2/2.png)
### 結論

1. 程式能正確進行多項式的加法、乘法與代入運算。
2. 運算子多載能輸入輸出，結果格式清晰。 

## 申論及開發報告

1. **資料結構使用**  
   使用Term與Polynomial結構，動態陣列管理多項式項目，插入新項時自動排序與合併，確保多項式簡潔。

   
透過friend管理輸入輸出，避免外部程式直接存取內部私有成員。將add、mult、eval封裝於Polynomial內，。
