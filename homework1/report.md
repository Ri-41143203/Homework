# 41143203

作業一

## 解題說明
Problem1:實作Ackermann函數的遞迴與非遞迴。
Problem2:以遞迴方式列印出集合S的所有子集合。


### 解題策略
Problem1:
遞迴:
1. 直接依照定義呼叫自身函數。
2. 遇到m=0回傳n+1，否則呼叫F(m-1,F(m,n-1))。

非遞迴
1.使用陣列模擬堆疊(Stack)來存放待計算的m值。

Problem2:
1.使用遞迴方式產生所有子集合。
2.每次遞迴針對當前元素選擇包含或不包含。
3.遞迴結束時印出當前組合。

## 程式實作

以下為主要程式碼：

```cpp
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
```

## 效能分析

1. 時間複雜度：$O(2^n)$。
2. 空間複雜度：空間複雜度為 Problem1:$O(m\+n)$;Problem2:$O(\n)$。

## 測試與驗證

### 測試案例

| 測試案例 | 輸入參數 $n$ | 預期輸出 | 實際輸出 |
|----------|--------------|----------|----------|
| 測試一   | $m = 0\ n = 1$      | 2        | 2        |
| 測試二   | $m = 1\ n = 1$      | 3        | 3        |
| 測試三   | $m = 1\ n = 2$      | 4        | 4        |
| 測試四   | $m = 2\ n = 1$      | 5        | 5        |
| 測試五   | $m = 3\ n = 2$      | 29       | 29       |

### 編譯與執行指令

```shell
$ g++ -std=c++17 -o sigma sigma.cpp
$ ./sigma
6
```

### 結論

1. 程式能正確計算結果正確。
2. 能正確列舉所有可能。  


## 申論及開發報告

1. **資料結構使用**  
   Problem1非遞迴版本使用堆疊模擬遞迴呼叫，避免溢位;Problem2 使用「陣列」暫存當前子集，搭配遞迴生成所有子集。

透過遞迴實作能直接對應函數數學定義，非遞迴要用堆疊模擬避免溢位，然而，對遞迴的深度可能超過系統限制的情況，當遞迴深度過高時應使用非遞迴版本避免程式崩潰;暫存陣列current保證產生子集合過程中不破壞其他狀態。
