#include <iostream>
#include <cmath>
using namespace std;

#define MAX_TERMS 100

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
int main() {
    Polynomial poly1, poly2;

    cout << "輸入第一個多項式:\n";
    cin >> poly1;

    cout << "輸入第二個多項式:\n";
    cin >> poly2;

    cout << "多項式1: " << poly1 << endl;
    cout << "多項式2: " << poly2 << endl;

    
    Polynomial sum = poly1 + poly2;
    cout << "相加結果: " << sum << endl;

    
    Polynomial product = poly1 * poly2;
    cout << "相乘結果: " << product << endl;

    float x;
    cout << "輸入一個 x 的值，用來計算多項式 1 的結果: ";
    cin >> x;
    cout << "當 x = "<<x<<"時，多項式 1 的值為:"<< poly1.evaluate(x)<< endl;

    return 0;
}

