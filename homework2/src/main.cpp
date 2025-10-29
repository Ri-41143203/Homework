#include <iostream>
#include <cmath>
using namespace std;

class Polynomial; // 前向宣告

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
    // 預設建構子
    Polynomial(int cap = 2) : capacity(cap), terms(0) {
        termArray = new Term[capacity];
    }

    // 拷貝建構子（深拷貝）
    Polynomial(const Polynomial& other) {
        capacity = other.capacity;
        terms = other.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++)
            termArray[i] = other.termArray[i];
    }

    // 賦值運算子（深拷貝）
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

    // 多項式乘法
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

// 主程式測試
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
