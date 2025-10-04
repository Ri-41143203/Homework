#include <iostream>
#include <cstdlib>
#include <cstring>
using namespace std;

//Problem1

//遞迴
int F(int m, int n) {
    if (m == 0) return n + 1;
    if (n == 0) return F(m - 1, 1);
    return F(m - 1, F(m, n - 1));
}

//不遞迴
int F_2(int m, int n) {
    const int MAX_STACK = 100000;
    int* stack = (int*)malloc(sizeof(int) * MAX_STACK);
    int top = -1;

    // 初始狀態
    stack[++top] = m;

    while (top >= 0) {
        m = stack[top--]; // 取出當前層

        if (m == 0) {
            n = n + 1;
        }
        else if (n == 0) {
            m = m - 1;
            n = 1;
            stack[++top] = m; // 推回遞迴呼叫 A(m-1,1)
        }
        else {
            // 模擬 A(m-1, A(m, n-1))
            // 先計算 A(m, n-1)
            stack[++top] = m - 1; // 外層遞迴
            stack[++top] = m;     // 內層遞迴
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
    cout << "A(" << m << ", " << n << ")(遞迴) = " << F(m, n) << endl;
    cout << "A(" << m << ", " << n << ")(非遞迴) = " << F_2(m, n) << endl;


    //Problem2
    char set[] = { 'a', 'b', 'c' };
    int set_size = 3;
    char current[10];
    memset(current, 0, sizeof(current));
    powerset_recursive(set, set_size, 0, current, 0);

    return 0;
}

