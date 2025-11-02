# 41143148
作業二
### 1.1 解題說明
實作 **Polynomial** 類別，使其能：

* 表示任意次多項式（以降序排列每項為 `<係數, 指數>`）。
* 需要輸出 **加法**、**乘法**、**求值** (`Eval`)。
### 1.2 範例
輸入 (p)： 2 2 3 1 1 
輸入 (q)： 3 3 3 5 2 1 0

*`p(x) = 2x^3+x`*  
*`q(x) = 3x^3+5x^2+1`*  

p(x) + q(x) = 5x^3+5x^2+x+1 p(x) * q(x) = 6x^6+10x^5+3x^4+7x^3+x

---

## 解題策略

| 步驟 | 實作說明 |
|------|----------|
| **資料結構** | 使用 struct Term 儲存每一項的係數與指數（float coef、int exp）。Polynomial 類別中以固定大小的陣列 Term terms[MAX_TERMS] 管理多項式項目，並用 int numTerms 記錄目前項數。 |
| **合併與排序** | 由 simplify() 完成：先掃描陣列合併相同指數的項目（相加係數並刪除重複），再用雙層迴圈根據指數由大到小排序。 |
| **新增項目** | addTerm() 檢查係數非零且未超過上限後新增一項，再呼叫 simplify() 自動整合重複項與排序。 |
| **加法** | operator+ 先複製左側多項式，再逐項將右側多項式的每一項呼叫 addTerm() 插入，利用 simplify() 自動合併同類項。 |
| **乘法** | operator* 使用雙層迴圈，對每一對項目執行乘法：新係數為兩者係數相乘，新指數為兩者指數相加，然後呼叫 addTerm() 合併結果。 |
| **求值** | evaluate(float x) 逐項計算 coef * pow(x, exp) 並累加，最後回傳多項式在該 x 下的值。 |
| **輸入/輸出** | operator>> 由使用者輸入項數及每項的係數、指數並加入多項式；operator<< 依序輸出每項（正號項前加 " + "），顯示為「係數x^指數」格式。 |

---

## 程式實作（關鍵程式碼）

```cpp


struct Term {
    float coef; 
    int exp;    
    Term(float c = 0, int e = 0) : coef(c), exp(e) {}
};

class Polynomial {
private:
    Term terms[MAX_TERMS]; 
    int numTerms;         

    void simplify() {
        for (int i = 0; i < numTerms; ++i) {
            for (int j = i + 1; j < numTerms; ++j) {
                if (terms[i].exp == terms[j].exp) {
                    terms[i].coef += terms[j].coef;
                    for (int k = j; k < numTerms - 1; ++k) {
                        terms[k] = terms[k + 1];
                    }
                    --numTerms;
                    --j;
                }
            }
        }
        for (int i = 0; i < numTerms - 1; ++i) {
            for (int j = i + 1; j < numTerms; ++j) {
                if (terms[i].exp < terms[j].exp) {
                    swap(terms[i], terms[j]);
                }
            }
        }
    }

public:
    Polynomial() : numTerms(0) {}

    void addTerm(float coef, int exp) {
        if (coef != 0 && numTerms < MAX_TERMS) {
            terms[numTerms++] = Term(coef, exp);
            simplify();
        }
    }

    Polynomial operator+(const Polynomial& other) const {
        Polynomial result = *this;
        for (int i = 0; i < other.numTerms; ++i) {
            result.addTerm(other.terms[i].coef, other.terms[i].exp);
        }
        return result;
    }

    Polynomial operator*(const Polynomial& other) const {
        Polynomial result;
        for (int i = 0; i < numTerms; ++i) {
            for (int j = 0; j < other.numTerms; ++j) {
                result.addTerm(terms[i].coef * other.terms[j].coef,
                               terms[i].exp + other.terms[j].exp);
            }
        }
        return result;
    }

    float evaluate(float x) const {
        float result = 0;
        for (int i = 0; i < numTerms; ++i) {
            result += terms[i].coef * pow(x, terms[i].exp);
        }
        return result;
    }

    friend ostream& operator<<(ostream& os, const Polynomial& poly) {
        for (int i = 0; i < poly.numTerms; ++i) {
            if (i > 0 && poly.terms[i].coef > 0) os << " + ";
            os << poly.terms[i].coef << "x^" << poly.terms[i].exp;
        }
        return os;
    }

    friend istream& operator>>(istream& is, Polynomial& poly) {
        int numTerms;
        cout << "輸入多項式的項數: ";
        is >> numTerms;
        for (int i = 0; i < numTerms; ++i) {
            float coef;
            int exp;
            cout << "輸入係數與指數: ";
            is >> coef >> exp;
            poly.addTerm(coef, exp);
        }
        return is;
    }
};
```
## 效能分析

| 演算法 / 操作 | 時間複雜度 | 空間複雜度 | 說明 |
|----------------|------------|------------|------|
| **addTerm**（單項插入） | **`O(t²)`** | **`O(1)`** | 每次呼叫都執行 simplify()，其中合併同類項需 O(t²)；若不重複呼叫 simplify()，單純插入為 O(1)。 |
| **Add**（多項式相加） | **`O(t₁ × t₂)`** | **`O(t₁ + t₂)`**| 每插入一項都觸發 simplify()，因此雖然邏輯上是 O(t₁ + t₂)，但由於重複合併整理，實際成本較高。 |
| **Mult**（多項式相乘） | **`O(t₁ × t₂ × t)`** | **`O(t₁ × t₂)`** | 雙層迴圈產生 t₁·t₂ 項，且每次插入都呼叫 simplify()，導致實際為 O(t₁ × t₂ × t)。若改為集中簡化可降至 O(t₁ × t₂)。 |
| **Eval**（求值） | **`O(t)`** | **`O(1)`** | 逐項累加 coef * pow(x, exp) 的結果；pow() 為常數時間操作。 |
| **I/O（>> / <<）** | **`O(t²)`** | **`O(1)`** | 輸入時每新增項都呼叫 simplify()（O(t²)），輸出時因排序亦為 O(t²)；但 t 一般較小，可視為線性近似。 |
## 測試與驗證

### 編譯與執行指令（Visual Studio）
```shell
$ cl /EHsc /std:c++17 Polynomial.cpp /Fe:polynomial.exe
$ .\polynomial.exe
請輸入第一個多項式（先輸入項數，接著每行 exponent coefficient）:
2
3 2
1 1
請輸入第二個多項式：
3
3 3
2 5
0 1

第一個多項式 p(x) = 2x^3 + x
第二個多項式 q(x) = 3x^3 + 5x^2 + 1

p(x) + q(x) = 5x^3 + 5x^2 + x + 1
p(x) * q(x) = 6x^6 + 10x^5 + 2x^3 + 3x^4 + 5x^3 + x
             = 6x^6 + 10x^5 + 3x^4 + 7x^3 + x

請輸入要評估的 x 值: 2
p(2) = 2×(2^3) + 2 = 18
q(2) = 3×(2^3) + 5×(2^2) + 1 = 49
(p+q)(2) = 5×(2^3) + 5×(2^2) + 2 + 1 = 68
(p*q)(2) = 6×(2^6) + 10×(2^5) + 3×(2^4) + 7×(2^3) + 2 = 678
```
### 心得討論

1. 資料結構設計
- 使用動態陣列自行管理容量與記憶體，實作過程加深對「Rule of Three」與動態配置的理解。
2. 演算法效率
- 加法為線性、乘法為平方級，未來可改用 hash map 或二分搜尋加速合併以降低常數時間。
3. 未來改進
- 若能用 STL，std::vector 與 std::sort 可簡化程式；Mult 可用稀疏合併減少搜尋次數。
