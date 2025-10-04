#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

//Problem1

//���j
int F(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return F(m - 1, 1);
    return F(m - 1, F(m, n - 1));
}

//�����j
int F_2(int m, int n) {
    const int MAX_STACK = 100000;
    int* stack = (int*)malloc(sizeof(int) * MAX_STACK);
    int top = -1;

    // ��l���A
    stack[++top] = m;

    while (top >= 0) {
        m = stack[top--]; // ���X��e�h

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            m = m - 1;
            n = 1;
            stack[++top] = m; // ���^���j�I�s A(m-1,1)
        }
        else {
            // ���� A(m-1, A(m, n-1))
            // ���p�� A(m, n-1)
            stack[++top] = m - 1; // �~�h���j
            stack[++top] = m;     // ���h���j
            n = n - 1;
        }
    }

    free(stack);
    return n;
}



//Problem2

void print_subset(char* current, int current_len) {
    cout << "{";
    for (int i = 0; i < current_len; ++i) {
        if (i > 0) cout << ",";
        cout << current[i];
    }
    cout << "}" << endl;
}

void powerset_recursive(char* set, int set_size, int index, char* current, int current_len) {
    if (index == set_size) {
        print_subset(current, current_len);
        return;
    }
    // Exclude the current element
    powerset_recursive(set, set_size, index + 1, current, current_len);
    // Include the current element
    current[current_len] = set[index];
    powerset_recursive(set, set_size, index + 1, current, current_len + 1);
}

int main() {

    //Problem1
    int m, n;
    cin >> m >> n;
    cout << "A(" << m << ", " << n << ")(���j) = " << F(m, n) << endl;
    cout << "A(" << m << ", " << n << ")(�D���j) = " << F_2(m, n) << endl;


    //Problem2
    char set[] = { 'a', 'b', 'c' };
    int set_size = 3;
    char current[10];
    memset(current, 0, sizeof(current));
    powerset_recursive(set, set_size, 0, current, 0);

    return 0;
}

