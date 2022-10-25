#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <sstream>
#include <cstring>
#include <cstdio>


using namespace std;

namespace longNumberNS {

  const int base = 1e9;
  const int minBase = 1e8;
  const int maxLen = 1e6;

  typedef long long ll;

  int min(int a, int b) { return (a < b ? a : b); }
  int max(int a, int b) { return (a > b ? a : b); }
  ll abs(ll x) { return (x > 0 ? x : -x); }

  class longNumber;

  //functions
  const longNumber intToLongNumber(ll x);
  const longNumber binPow(longNumber a, int p);
  const longNumber sqr(const longNumber& x);
  const longNumber abs(const longNumber& a);

  class longNumber {
  private:
    bool negative;
    vector <int> number;

    int addBase(int x) {
      x -= number.size();
      if (!x) x++;
      number.resize(number.size() + x, 0);
      for (int i = number.size() - 1; i - x >= 0 && i != 0; i--)
        number[i] = number[i - x];
      for (int i = 0; i < x; i++)
        number[i] = 0;
      return x;
    }

    void delZero() {
      while ((number.size()) && (!number.back()))
        number.pop_back();
      if (!number.size()) negative = false;
    }

    //sBinSearch - special binary search
    int sBinSearch(const longNumber& a) {
      int ans = 0, l = 0, r = base;
      while (l <= r) {
        int m = (l + r) / 2;
        if (*this >= a * m)
          ans = m, l = m + 1;
        else
          r = m - 1;
      }
      return ans;
    }

    void read() {
      char s[10000]; scanf("%s", s);
      for (int i = strlen(s); i > 0; i -= 9) {
        s[i] = 0;
        number.push_back(atoi(i >= 9 ? s + i - 9 : s));
      }
      if (s[0] == '-') negative = true;
      (*this).delZero();
      if ((number.size()) && (number.back() < 0))
        number[number.size() - 1] *= -1;
    }


    void write() const {
      printf("%d", number.empty() ? 0 : (negative ? -number.back() : number.back()));
      for (int i = number.size() - 2; i >= 0; i--)
        printf("%09d", number[i]);
    }

    //IO streams operators
    friend istream& operator >>(istream& is, longNumber& x);
    friend ostream& operator <<(ostream& os, const longNumber& x);
    //unary operators
    friend const longNumber operator +(const longNumber& a);
    friend const longNumber operator -(const longNumber& a);
    //comparison operators
    friend bool operator ==(const longNumber& a, const longNumber& b);
    friend bool operator !=(const longNumber& a, const longNumber& b);
    friend bool operator <(const longNumber& a, const longNumber& b);
    friend bool operator >(const longNumber& a, const longNumber& b);
    friend bool operator <=(const longNumber& a, const longNumber& b);
    friend bool operator >=(const longNumber& a, const longNumber& b);
    //logical operators
    friend bool operator !(const longNumber& a);
    //increment & decrement
    friend longNumber& operator ++(longNumber& a);
    friend longNumber& operator --(longNumber& a);
    friend longNumber operator ++(longNumber& a, const int);
    friend longNumber operator --(longNumber& a, const int);
    //arithmetic operators
    friend longNumber operator +(const longNumber& a, const longNumber& b);
    friend longNumber operator +(const longNumber& a, const ll& b);
    friend longNumber operator -(const longNumber& a, const longNumber& b);
    friend longNumber operator -(const longNumber& a, const ll& b);
    friend longNumber operator *(const longNumber& a, const longNumber& b);
    friend longNumber operator *(const longNumber& a, const ll& b);
    friend longNumber operator /(const longNumber& a, const longNumber& b);
    friend longNumber operator /(const longNumber& a, const ll& b);
    friend longNumber operator %(const longNumber& a, const longNumber& b);
    friend longNumber operator %(const longNumber& a, const ll& b);
    //functions
    friend const longNumber intToLongNumber(ll x);
  public:
    //constructors
    longNumber() : negative(0), number() {}

    longNumber(const ll& value) : negative(), number() {
      if (!value) {
        negative = 0; return;
      }
      if (abs(value) < base) {
        number.push_back(abs(value));
        negative = (bool)(value - abs(value));
      }
      else *this = intToLongNumber(value);
    }

    //assignment operators

    longNumber& operator =(const longNumber& b) {
      number = b.number;
      negative = b.negative;
      return *this;
    }

    longNumber& operator =(const ll& x) {
      return *this = intToLongNumber(x);
    }

    longNumber& operator +=(const longNumber& b) {
      if ((!negative) && (b.negative))
        return *this -= -b;
      if ((negative) && (!b.negative)) {
        negative ^= 1; *this -= b;
        negative ^= 1; return *this;
      }
      if ((negative) && (b.negative)) {
        negative ^= 1; *this += -b;
        negative ^= 1; return *this;
      }
      bool carry = 0;
      size_t maxN = max(number.size(), b.number.size());
      for (size_t i = 0; (i < maxN) || (carry); i++) {
        if (i == number.size()) number.push_back(0);
        number[i] += carry + (i < b.number.size() ? b.number[i] : 0);
        carry = (number[i] >= base);
        if (carry) number[i] -= base;
      }
      return *this;
    }

    longNumber& operator +=(const ll& b) {
      if (abs(b) >= base)
        return *this += intToLongNumber(b);
      if ((!negative) && (b < 0))
        return *this -= -b;
      if ((negative) && (b >= 0)) {
        negative ^= 1; *this -= b;
        negative ^= 1; return *this;
      }
      if ((negative) && (b < 0)) {
        negative ^= 1; *this += -b;
        negative ^= 1; return *this;
      }
      int carry = b;
      for (size_t i = 0; (i < number.size()) || (carry); i++) {
        if (i == number.size()) number.push_back(0);
        number[i] += carry;
        carry = (number[i] >= base);
        if (carry) number[i] -= base;
      }
      return *this;
    }

    longNumber& operator -=(const longNumber& b) {
      if ((!negative) && (b.negative))
        return *this += -b;
      if ((negative) && (!b.negative)) {
        negative ^= 1; *this += b;
        negative ^= 1; return *this;
      }
      if ((negative) && (b.negative)) {
        negative ^= 1; *this -= (-b);
        negative ^= 1; return *this;
      }
      bool carry = 0;
      int flag = (*this < b ? -1 : 1);
      size_t maxN = max(b.number.size(), 1);
      for (size_t i = 0; (i < maxN) || (carry); i++) {
        if (i == number.size()) number.push_back(0);
        number[i] -= carry * flag + (i < b.number.size() ? b.number[i] : 0);
        number[i] *= flag; carry = (number[i] < 0);
        if (carry) number[i] += base;
      }
      negative = (flag == -1 ? 1 : 0);
      (*this).delZero();
      return *this;
    }

    longNumber& operator -=(const ll& b) {
      if (abs(b) >= base)
        return *this -= intToLongNumber(b);
      if ((!negative) && (b < 0))
        return *this += -b;
      if ((negative) && (b >= 0)) {
        negative ^= 1; *this += b;
        negative ^= 1; return *this;
      }
      if ((negative) && (b < 0)) {
        negative ^= 1; *this -= -b;
        negative ^= 1; return *this;
      }
      bool carry = 0;
      int flag = (*this < intToLongNumber(b) ? -1 : 1);
      size_t maxN = (b ? 1 : 0);
      for (size_t i = 0; (i < maxN) || (carry); i++) {
        if (i == number.size()) number.push_back(0);
        number[i] -= carry * flag + (i < maxN ? b : 0);
        number[i] *= flag; carry = (number[i] < 0);
        if (carry) number[i] += base;
      }
      negative = (flag == -1 ? 1 : 0);
      (*this).delZero();
      return *this;
    }

    longNumber& operator *=(const longNumber& b) {
      longNumber a = *this; *this = longNumber();
      negative = a.negative ^ b.negative;
      number.resize(a.number.size() + b.number.size(), 0);
      for (size_t i = 0; i < a.number.size(); i++)
        for (size_t j = 0, carry = 0; (j < b.number.size()) || (carry); j++) {
          ll cur = 1LL * a.number[i] * (j < b.number.size() ? b.number[j] : 0);
          if (i + j >= number.size()) number.resize(i + j + 1, 0);
          cur += number[i + j] + carry;
          number[i + j] = cur % base;
          carry = cur / base;
        }
      (*this).delZero();
      return *this;
    }

    longNumber& operator *=(const ll& b) {
      if (abs(b) >= base)
        return *this *= intToLongNumber(b);
      int carry = 0;
      for (size_t i = 0; (i < number.size()) || (carry); i++) {
        if (i == number.size()) number.push_back(0);
        ll cur = carry + 1LL * number[i] * abs(b);
        number[i] = cur % base;
        carry = cur / base;
      }
      negative ^= (b >= 0 ? 0 : 1);
      (*this).delZero();
      return *this;
    }

    longNumber& operator /=(const longNumber& b) {
      if (!b) return *this;
      
      longNumber cur;
      for (int k, i = (int)number.size() - 1; i >= 0; i--) {
        k = cur.addBase(min(number.size(), b.number.size())) - 1;
        
        for (; k >= 0 && i >= 0; k--, i--) {
          cur.number[k] = number[i];
          number[i] = 0;
        }
        cur.delZero();
        i++; if (cur < b) continue;
        number[i] = cur.sBinSearch(abs(b));
        cur -= abs(b) * number[i];
      }
      negative ^= b.negative;
      (*this).delZero();
      return *this;
    }

    longNumber& operator /=(const ll& b) {
      if (!b) return *this;
      if (abs(b) >= base)
        return *this /= intToLongNumber(b);
      longNumber cur;
      for (int k, i = number.size() - 1; i >= 0; i--) {
        k = cur.addBase(min(number.size(), 1)) - 1;
        for (; k >= 0; k--, i--) {
          cur.number[k] = number[i];
          number[i] = 0;
        }
        cur.delZero();
        i++; if (cur < b) continue;
        number[i] = cur.sBinSearch(intToLongNumber(abs(b)));
        cur -= intToLongNumber(abs(b) * number[i]);
      }
      negative ^= (b >= 0 ? 0 : 1);
      (*this).delZero();
      return *this;
    }

    longNumber& operator %=(const longNumber& b) {
      if (!b) return *this;
      longNumber cur;
      for (int k, i = number.size() - 1; i >= 0; i--) {
        k = cur.addBase(min(number.size(), b.number.size())) - 1;
        for (; k >= 0; k--, i--)
          cur.number[k] = number[i];
        cur.delZero();
        i++; if (cur < b) continue;
        cur -= abs(b) * cur.sBinSearch(abs(b));
      }
      cur.negative = negative;
      return *this = cur;
    }

    longNumber& operator %=(const ll& b) {
      if (!b) return *this;
      if (abs(b) >= base)
        return *this %= intToLongNumber(b);
      longNumber cur;
      for (int k, i = number.size() - 1; i >= 0; i--) {
        k = cur.addBase(min(number.size(), 1)) - 1;
        for (; k >= 0; k--, i--)
          cur.number[k] = number[i];
        cur.delZero();
        i++; if (cur < b) continue;
        cur -= intToLongNumber(abs(b) * cur.sBinSearch(intToLongNumber(abs(b))));
      }
      cur.negative = negative;
      return *this = cur;
    }

    string toString() {
      stringstream ss;
      if (number.size() == 0) return "";
      ss << number[number.size() - 1];
      for (int i = number.size() - 2; i >= 0; i--) {
        int o = number[i];
        string p = "";
        for (int j = 0; j < 9; j++) {
          p += char((o % 10) + '0');
          o /= 10;
        }
        reverse(p.begin(), p.end());
        ss << p;
      }
      string ans = ss.str();
      if (negative) ans = '-' + ans;
      return ans;
    }
  };

  //IO streams operators

  istream& operator >>(istream& is, longNumber& x) {
    x.read();
    return is;
  }

  ostream& operator <<(ostream& os, const longNumber& x) {
    x.write(); return os;
  }

  //unary operators

  const longNumber operator +(const longNumber& a) {
    return a;
  }

  const longNumber operator -(const longNumber& a) {
    longNumber ans(a);
    ans.negative ^= 1;
    return ans;
  }

  //comparison operators

  bool operator ==(const longNumber& a, const longNumber& b) {
    if ((a.negative == b.negative) && (a.number == b.number))
      return true;
    else return false;
  }

  bool operator !=(const longNumber& a, const longNumber& b) {
    return !(a == b);
  }

  bool operator <(const longNumber& a, const longNumber& b) {
    if (a.negative > b.negative) return true;
    if (a.negative < b.negative) return false;
    if (a.number.size() < b.number.size()) return true;
    if (a.number.size() > b.number.size()) return false;
    for (int i = a.number.size() - 1; i >= 0; i--) {
      if (a.number[i] == b.number[i]) continue;
      if ((a.number[i] < b.number[i]) == !(a.negative)) return true;
      return false;
    }
    return false;
  }

  bool operator >(const longNumber& a, const longNumber& b) {
    if (a.negative < b.negative) return true;
    if (a.negative > b.negative) return false;
    if (a.number.size() > b.number.size()) return true;
    if (a.number.size() < b.number.size()) return false;
    for (int i = a.number.size() - 1; i >= 0; i--) {
      if (a.number[i] == b.number[i]) continue;
      if ((a.number[i] > b.number[i]) == !(a.negative)) return true;
      return false;
    }
    return false;
  }

  bool operator <=(const longNumber& a, const longNumber& b) {
    return ((a < b) || (a == b));
  }

  bool operator >=(const longNumber& a, const longNumber& b) {
    return ((a > b) || (a == b));
  }

  //logical operators

  bool operator !(const longNumber& a) {
    return a.number.empty();
  }

  //increment & decrement

  longNumber& operator ++(longNumber& a) {
    if (a.negative) {
      a.negative ^= 1; --a;
      a.negative ^= 1; return a;
    }
    bool carry = 1;
    for (size_t i = 0; (carry); i++) {
      if (i == a.number.size()) a.number.push_back(0);
      a.number[i] += carry;
      carry = (a.number[i] >= base);
      if (carry) a.number[i] -= base;
    }
    return a;
  }

  longNumber& operator --(longNumber& a) {
    if (a.negative) {
      a.negative ^= 1; ++a;
      a.negative ^= 1; return a;
    }
    if (!a) {
      a.number.push_back(1);
      a.negative = true;
      return a;
    }
    bool carry = 1;
    for (size_t i = 0; (carry); i++) {
      a.number[i] -= carry;
      carry = (a.number[i] < 0);
      if (carry) a.number[i] += base;
    }
    a.delZero();
    return a;
  }

  longNumber operator ++(longNumber& a, const int) {
    longNumber ans = a; ++a;
    return ans;
  }

  longNumber operator --(longNumber& a, const int) {
    longNumber ans = a; --a;
    return ans;
  }

  //arithmetic operators

  longNumber operator +(const longNumber& a, const longNumber& b) {
    if ((!a.negative) && (b.negative))
      return a - (-b);
    if ((a.negative) && (!b.negative))
      return -((-a) - b);
    if ((a.negative) && (b.negative))
      return -((-a) + (-b));
    longNumber ans; bool carry = 0;
    size_t maxN = max(a.number.size(), b.number.size());
    for (size_t i = 0; (i < maxN) || (carry); i++) {
      ans.number.push_back((i < a.number.size() ? a.number[i] : 0));
      ans.number[i] += carry + (i < b.number.size() ? b.number[i] : 0);
      carry = (ans.number[i] >= base);
      if (carry) ans.number[i] -= base;
    }
    return ans;
  }

  longNumber operator +(const longNumber& a, const ll& b) {
    if (abs(b) >= base)
      return a + intToLongNumber(b);
    if ((!a.negative) && (b < 0))
      return a - (-b);
    if ((a.negative) && (b >= 0))
      return -((-a) - b);
    if ((a.negative) && (b < 0))
      return -((-a) + (-b));
    longNumber ans; int carry = b;
    for (size_t i = 0; (i < a.number.size()) || (carry); i++) {
      ans.number.push_back((i < a.number.size() ? a.number[i] : 0));
      ans.number[i] += carry;
      carry = (ans.number[i] >= base);
      if (carry) ans.number[i] -= base;
    }
    return ans;
  }

  longNumber operator -(const longNumber& a, const longNumber& b) {
    if ((!a.negative) && (b.negative))
      return a + (-b);
    if ((a.negative) && (!b.negative))
      return -((-a) + b);
    if ((a.negative) && (b.negative))
      return -((-a) - (-b));
    longNumber ans; bool carry = 0;
    int flag = (a < b ? -1 : 1);
    size_t maxN = max(b.number.size(), 1);
    for (size_t i = 0; (i < maxN) || (carry); i++) {
      ans.number.push_back(i < a.number.size() ? a.number[i] : 0);
      ans.number[i] -= carry * flag + (i < b.number.size() ? b.number[i] : 0);
      ans.number[i] *= flag; carry = (ans.number[i] < 0);
      if (carry) ans.number[i] += base;
    }
    ans.negative = (flag == -1 ? 1 : 0);
    ans.delZero();
    return ans;
  }

  longNumber operator -(const longNumber& a, const ll& b) {
    if (abs(b) >= base)
      return a - intToLongNumber(b);
    if ((!a.negative) && (b < 0))
      return a + (-b);
    if ((a.negative) && (b >= 0))
      return -((-a) + b);
    if ((a.negative) && (b < 0))
      return -((-a) - (-b));
    longNumber ans; bool carry = 0;
    int flag = (a < intToLongNumber(b) ? -1 : 1);
    size_t maxN = (b ? 1 : 0);
    for (size_t i = 0; (i < maxN) || (carry); i++) {
      ans.number.push_back(i < a.number.size() ? a.number[i] : 0);
      ans.number[i] -= carry * flag + (i < maxN ? b : 0);
      ans.number[i] *= flag; carry = (ans.number[i] < 0);
      if (carry) ans.number[i] += base;
    }
    ans.negative = (flag == -1 ? 1 : 0);
    ans.delZero();
    return ans;
  }

  longNumber operator *(const longNumber& a, const longNumber& b) {
    longNumber ans; ans.negative = a.negative ^ b.negative;
    ans.number.resize(a.number.size() + b.number.size(), 0);
    for (size_t i = 0; i < a.number.size(); i++)
      for (size_t j = 0, carry = 0; (j < b.number.size()) || (carry); j++) {
        ll cur = 1LL * a.number[i] * (j < b.number.size() ? b.number[j] : 0);
        if (i + j >= ans.number.size()) ans.number.resize(i + j + 1, 0);
        cur += ans.number[i + j] + carry;
        ans.number[i + j] = cur % base;
        carry = cur / base;
      }
    ans.delZero();
    return ans;
  }

  longNumber operator *(const longNumber& a, const ll& b) {
    if (abs(b) >= base)
      return a * intToLongNumber(b);
    longNumber ans; int carry = 0;
    for (size_t i = 0; (i < a.number.size()) || (carry); i++) {
      ll cur = carry + 1LL * (i < a.number.size() ? a.number[i] : 0) * abs(b);
      ans.number.push_back(cur % base);
      carry = cur / base;
    }
    ans.negative = a.negative ^ (b >= 0 ? 0 : 1);
    ans.delZero();
    return ans;
  }

  longNumber operator /(const longNumber& a, const longNumber& b) {
    if (!b) return a;
    longNumber ans, cur;
    ans.number.resize(a.number.size());
    for (int k, i = (int)a.number.size() - 1; i >= 0; i--) {
      k = cur.addBase(min(a.number.size(), b.number.size())) - 1;
      for (; k >= 0 && i >= 0; k--, i--) {
        cur.number[k] = a.number[i];
        ans.number[i] = 0;
      }
      cur.delZero();
      i++; if (cur < b) continue;
      ans.number[i] = cur.sBinSearch(abs(b));
      cur -= abs(b) * a.number[i];
    }
    ans.negative = a.negative ^ b.negative;
    ans.delZero();
    return ans;
  }

  longNumber operator /(const longNumber& a, const ll& b) {
    if (!b) return a;
    if (abs(b) >= base)
      return a / intToLongNumber(b);
    longNumber ans, cur;
    ans.number.resize(a.number.size());
    for (int k, i = a.number.size() - 1; i >= 0; i--) {
      k = cur.addBase(min(a.number.size(), 1)) - 1;
      for (; k >= 0; k--, i--) {
        cur.number[k] = a.number[i];
        ans.number[i] = 0;
      }
      cur.delZero();
      i++; if (cur < b) continue;
      ans.number[i] = cur.sBinSearch(intToLongNumber(abs(b)));
      cur -= intToLongNumber(abs(b) * a.number[i]);
    }
    ans.negative = a.negative ^ (b >= 0 ? 0 : 1);
    ans.delZero();
    return ans;
  }

  longNumber operator %(const longNumber& a, const longNumber& b) {
    if (!b) return a;
    longNumber cur;
    for (int k, i = a.number.size() - 1; i >= 0; i--) {
      k = cur.addBase(min(a.number.size(), b.number.size())) - 1;
      for (; k >= 0; k--, i--)
        cur.number[k] = a.number[i];
      cur.delZero();
      i++; if (cur < b) continue;
      cur -= abs(b) * cur.sBinSearch(abs(b));
    }
    cur.negative = a.negative;
    return cur;
  }

  longNumber operator %(const longNumber& a, const ll& b) {
    if (!b) return a;
    if (abs(b) >= base)
      return a % intToLongNumber(b);
    longNumber cur;
    for (int k, i = a.number.size() - 1; i >= 0; i--) {
      k = cur.addBase(min(a.number.size(), 1)) - 1;
      for (; k >= 0; k--, i--)
        cur.number[k] = a.number[i];
      cur.delZero();
      i++; if (cur < b) continue;
      cur -= intToLongNumber(abs(b) * cur.sBinSearch(intToLongNumber(abs(b))));
    }
    cur.negative = a.negative;
    return cur;
  }

  //functions

  //intToLongNumber
  const longNumber intToLongNumber(ll x) {
    longNumber ans;
    if (!x) return ans;
    if (x < 0)
      ans.negative = true, x *= -1;
    while (x)
      ans.number.push_back(x % base), x /= base;
    return ans;
  }

  //binPow
  const longNumber binPow(longNumber a, int p) {
    longNumber ans(1);
    while (p) {
      if (p & 1) ans *= a;
      a *= a; p >>= 1;
    }
    return ans;
  }

  //sqr
  const longNumber sqr(const longNumber& x) {
    return x * x;
  }

  //abs
  const longNumber abs(const longNumber& a) {
    return (a >= 0 ? a : -a);
  }
}

using namespace longNumberNS;

#include <windows.h>
#include <conio.h>

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

string emp = "                         ";
vector<string> s;
vector<int> z;

int b = 0;
int i = 0;
int rt = 0;


string round(string s, int t, int k) {
  bool f = 0;
  int z = 0;
  int j = -1;
  string ns = "";
  for (int i = 0; i < s.size(); i++) {
    ns += s[i];
    if (s[i] == '.') {
      f = 1;
      continue;
    }
    if (f) {
      z++;
      if (z > k) {
        j = i;
        break;
      }
    }
  }
  if (j == -1) return s;
  if (t == 0) {
    if (ns[j] >= '5') {
      if (ns[j - 1] == '.') j--;
      ns[j - 1]++;
      j--;
    }   
  }
  if (t == 1) {
    bool f = 0;
    if (ns[j - 1] == '.') {
      if ((ns[j - 2] - '0') % 2 == 1) f = 1;
    }
    else {
      if ((ns[j - 1] - '0') % 2 == 1) f = 1;
    }
    if ((ns[j] == '5' && f) || ns[j] > '5') {
      if (ns[j - 1] == '.') j--;
      ns[j - 1]++;
      j--;
    } 
  }

  while (ns[j] > '9') {
    ns[j] = '0';
    if (j == 0) {
      ns = '1' + ns;
      break;
    }
    else {
      if (ns[j - 1] == '.') j--;
      ns[j - 1] ++;
      j--;
    }
  }

  string ans = "";
  for (int i = 0; i < ns.size() - 1; i++) ans += ns[i];
  return ans;
}

pair<int, longNumber> CalcZ(pair<int, longNumber> a, pair<int, longNumber> b, int z) {
  pair<int, longNumber> ans = { 0, longNumber(0) };
  if (z == 0) {
    while (a.first != b.first) {
      if (a.first < b.first) {
        a.first++;
        a.second *= 10;
      }
      else {
        b.first++;
        b.second *= 10;
      }
    }
    ans.first = a.first;
    ans.second = a.second;
    ans.second += b.second;
    
  }
  if (z == 1) {
    while (a.first != b.first) {
      if (a.first < b.first) {
        a.first++;
        a.second *= 10;
      }
      else {
        b.first++;
        b.second *= 10;
      }
    }
    ans.first = a.first;
    ans.second = a.second;
    ans.second -= b.second;
  }
  if (z == 2) {
    while (a.first != b.first) {
      if (a.first < b.first) {
        a.first++;
        a.second *= 10;
      }
      else {
        b.first++;
        b.second *= 10;
      }
    }
    ans.first = a.first * 2;
    ans.second = a.second;
    ans.second *= b.second;
  }
  if (z == 3) {
    if (b.second == 0) {
      return { -1, longNumber(-1) };
    }
    while (a.first != b.first) {
      if (a.first < b.first) {
        a.first++;
        a.second *= 10;
      }
      else {
        b.first++;
        b.second *= 10;
      }
    }
    for (int i = 0; i < 40; i++) {
      a.first++;
      a.second *= 10;
    }
    ans.first = 40;
    ans.second = a.second;
    ans.second /= b.second;
  }
  return ans;
}

bool check(string s) {
  int kp = 0;
  int km = 0;
  int ks = 0;
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '.') kp++;
    if (s[i] == '-') km++;
    if (s[i] == ' ') ks++;
  }

  if (kp + ks + km == s.size()) return false;
  if (km == 1 && s[0] != '-') return false;
  if (km > 1) return false;
  if (kp == 1 && s[0] == '.') return false;
  if (kp == 1 && s[s.size() - 1] == '.') return false;
  if (kp > 1) return false;
  if (ks) {
    bool f = 0;
    if (kp == 0) f = 1;
    int k = 0;
    for (int i = s.size() - 1; i >= 0; i--) {
      if (f) {
        k++;
        if (k != 4 && s[i] == ' ') return false;
        if (s[i] == '-') break;
        if (k == 4 && s[i] != ' ') return false;
        if (k == 4) k = 0;
      }
      else {
        if (s[i] == ' ') return false;
        if (s[i] == '.') f = 1;
      }
    }
  }
  return true;
}

pair<int, longNumber> toLong(string s) {
  longNumber x(0);
  int f = 1;
  if (s[0] == '-') { f = -1; }

  int k = s.size();
  for (int i = 0; i < s.size(); i++) {
    if (s[i] == '-') continue;
    if (s[i] == ' ') continue;
    if (s[i] == '.') {
      k = i + 1;
      continue;
    }
    x *= 10;
    x += int(s[i] - '0');
  }
  //SetConsoleCursorPosition(hConsole, { 0, 20 });
  //cout << "                                                             ";
  //SetConsoleCursorPosition(hConsole, { 0, 20 });
  // cout << s << " " << f << " " << x << " ";
  x *= f;
  //cout << x;
  return { s.size() - k, x };
}

void OutError(string s) {
  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  SetConsoleCursorPosition(hConsole, { short(29),  short(10) });
  for (int i = 0; i < 95; i++) cout << " ";
  SetConsoleCursorPosition(hConsole, { short(29),  short(14) });
  for (int i = 0; i < 95; i++) cout << " ";
  SetConsoleCursorPosition(hConsole, { short(29),  short(10) });
  cout << s;
  SetConsoleCP(866);
  SetConsoleOutputCP(866);
}

string toString(pair<int, longNumber> o) {
  string ans = "";
  string s = o.second.toString();
  if (s.size() == 0) {
    ans = "0";
  }
  else {
    int i = 0;

    for (; i < int(s.size()) - o.first; i++) ans += s[i];

    if (i == 0 || (i == 1 && s[0] == '-')) ans += "0";

    int m = s.size();

    if (m != i) ans += ".";

    if (o.first > s.size()) for (int i = 0; i < o.first - s.size(); i++)  ans += "0";
    for (; i < m; i++) ans += s[i];



  }
  if (ans == "-0") ans = "0";
  return ans;
}

string NormAns(string o) {
  bool f = 0;

  bool ng = 0;
  if (o[0] == '-') ng = 1;
  string no = "";
  for (int i = 0; i < o.size(); i++) if (o[i] != '-') no += o[i];
  o = no;
  for (int i = 0; i < o.size(); i++) {
    if (o[i] == '.') { f = 1; break; };
  }

  bool nn = 0;

  string s = "";
  int k = 0;
  for (int i = o.size() - 1; i >= 0; i--) {
    if (!f) {
      if (k == 3) s += " ";
      s += o[i];
      k++;
      if (k == 4) k = 1;
    }
    else {
      if (o[i] == '.') {
        f = 0;
        if (nn) s += o[i];
        continue;
      }
      if (o[i] != '0') nn = 1;
      if (nn) s += o[i];
    }
  }
  reverse(s.begin(), s.end());
  if (ng) s = "-" + s;
  return s;
}

void OutAns(pair<int, longNumber> o) {
  SetConsoleCursorPosition(hConsole, { short(29),  short(10) });
  for (int i = 0; i < 95; i++) cout << " ";
  SetConsoleCursorPosition(hConsole, { short(29),  short(14) });
  for (int i = 0; i < 95; i++) cout << " ";

  string ans = toString(o);
  ans = round(ans, rt, 6);
  ans = NormAns(ans);
  SetConsoleCursorPosition(hConsole, { short(29),  short(10) });

  if (ans.size() > 93) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Ошибка: Выходит за диапазон";
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
  } else 
  {
    if (ans == "-0") ans = "0";
    cout << ans;
  }
  
  ans = toString(o);
  ans = round(ans, rt, 0);
  ans = NormAns(ans);
  SetConsoleCursorPosition(hConsole, { short(29),  short(14) });
  if (ans.size() > 93) {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Ошибка: Выходит за диапазон";
    SetConsoleCP(866);
    SetConsoleOutputCP(866);
  }
  else {
    if (ans == "-0") ans = "0";
    cout << ans;
  }

  SetConsoleCursorPosition(hConsole, { short(29),  short(14) });
}

void Calc()
{
  bool f = 1;
  for (int i = 0; i < 4; i++) f = f & check(s[i]);

  if (f == 0) {
    OutError("Ошибка: Неверный синтаксис\n");
    return;
  }
  OutError("");
  SetConsoleCursorPosition(hConsole, { 0, 17 });

  vector<pair<int, longNumber>> o;
  for (int i = 0; i < 4; i++) o.push_back(toLong(s[i]));


  pair<int, longNumber> pr = CalcZ(o[1], o[2], z[1]);

  if (pr.first == -1) {
    OutError("Ошибка: Деление на ноль\n");
    return;
  }
  pr = toLong(round(toString(pr), 0, 10));
  if (pr.first == -1) {
    OutError("Ошибка: Деление на ноль\n");
    return;
  }

  if ((z[2] == 2 || z[2] == 3) && (z[0] == 0 || z[0] == 1)) {
    pr = CalcZ(pr, o[3], z[2]);
    
    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }
    pr = toLong(round(toString(pr), 0, 10));
    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }

    pr = CalcZ(o[0], pr, z[0]);

    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }
  }
  else {
    pr = CalcZ(o[0], pr, z[0]);
    
    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }
    pr = toLong(round(toString(pr), 0, 10));
    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }
    
    pr = CalcZ(pr, o[3], z[2]);    
    if (pr.first == -1) {
      OutError("Ошибка: Деление на ноль\n");
      return;
    }
  }

  OutAns(pr);

}

bool ch(char a) {
  if (a == '.') return 1;
  if ('0' <= a && a <= '9') return 1;
  if (a == '-') return 1;
  if (a == ' ') return 1;
  return 0;
}

void OutS(int j) {
  int pk = 0;
  if (j == 1 || j == 2) pk = 1;
  if (j == 3) pk = 2;
  SetConsoleCursorPosition(hConsole, { short(32 * j + 1 + pk),  4 });
  cout << emp;
  SetConsoleCursorPosition(hConsole, { short(32 * j + 12 - s[j].size() / 2 + 1 + pk),  4 });
  cout << s[j];
  SetConsoleCursorPosition(hConsole, { short(32 * j + 12 - s[j].size() / 2 + i + 1 + pk),  4 });
}

void OutZ(int j) {
  char zn[] = { '+', '-', '*', '/' };
  int pk = 0;
  if (j == 1) pk = 1;
  if (j == 2) pk = 2;
  for (int i = 0; i < 4; i++) {
    SetConsoleCursorPosition(hConsole, { short(26 + 32 * j + 1 + pk),  short(i * 2 + 1) });
    if (z[j] == i) {
      cout << ">>" << zn[i] << "<<";
    }
    else {
      cout << "  " << zn[i] << "  ";
    }
  }
  SetConsoleCursorPosition(hConsole, { short(26 + 32 * j + 2 + 1 + pk),  short(z[j] * 2 + 1) });

}

void OutRType()
{
  string o[] = {"   математическое    " , "   бухгалтерское     ", "      усечение       " };

  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  SetConsoleCursorPosition(hConsole, { short(1),  short(9) });

  cout << "     Вид округления:";
  
  for (int i = 0; i < 3; i++) {
    SetConsoleCursorPosition(hConsole, { short (1),  short(11 + i * 2) });
    if (rt == i) { cout << ">>" + o[i] + "<<"; }
    else cout << "  " + o[i] + "  ";
  
  }
  
  SetConsoleCursorPosition(hConsole, { short(13),  short(11 + rt * 2) });

 
  SetConsoleCP(866);
  SetConsoleOutputCP(866);
}

void Out() {

  cout << char(201);  
  for (int q = 0; q < 4; q++) {
    if (q == 1) cout << char(205);
    for (int i = 0; i < 25; i++) cout << char(205);
    if (q == 2)  cout << char(205);
    if (q != 3) {
      cout << char(203);
      for (int i = 0; i < 5; i++) cout << char(205);
      cout << char(203);
    }
  }
  cout << char(187);  

  
  for (int z = 0; z < 7; z++) {
    cout << char(186);
    for (int q = 0; q < 4; q++) {

      if (q == 1) cout << " ";
      for (int i = 0; i < 25; i++) cout << ' ';
      if (q == 2) cout << " ";
      if (q != 3) {
        cout << char(186);
        for (int i = 0; i < 5; i++) cout << ' ';
        cout << char(186);
      }
    }
    cout << char(186);
  }
  

  
  cout << char(204);
  for (int q = 0; q < 4; q++) {
    if (q == 1) cout << char(205);
    for (int i = 0; i < 25; i++) cout << char(205);
    if (q == 2) cout << char(205);
    if (q != 3) {
      if (q == 0) cout << char(206); else  cout << char(202);
      for (int i = 0; i < 5; i++) cout << char(205);
      cout << char(202);
    }
  }
  cout << char(185);
  for (int z = 0; z < 7; z++) {
    cout << char(186);
    for (int i = 0; i < 25; i++) cout << " ";
    if (z == 3) {
      cout << char(204);
      for (int i = 0; i < 97; i++) cout << char(205);
      cout << char(185);
    }
    else {
      cout << char(186);
      for (int i = 0; i < 97; i++) cout << " ";
      cout << char(186);
    }
  }

  cout << char(204);
  for (int i = 0; i < 25; i++) cout << char(205);
  cout << char(202);
  for (int i = 0; i < 97; i++) cout << char(205);
  cout << char(185);
  
  cout << char(186);
  for (int i = 1; i < 124; i++) cout << " ";
  cout << char(186);

  cout << char(200);
  for (int i = 1; i < 124; i++) cout << char(205);
  cout << char(188);
  
  {

  SetConsoleCursorPosition(hConsole, { short(34),  2});
  cout << "/";
  SetConsoleCursorPosition(hConsole, { short(33),  3 });
  cout << "/";
  SetConsoleCursorPosition(hConsole, { short(33),  4 });
  cout << "|";
  SetConsoleCursorPosition(hConsole, { short(33),  5 });
  cout << "\\";
  SetConsoleCursorPosition(hConsole, { short(34),  6 });
  cout << "\\";

  SetConsoleCursorPosition(hConsole, { short(90),  2 });
  cout << "\\";
  SetConsoleCursorPosition(hConsole, { short(91),  3 });
  cout << "\\";
  SetConsoleCursorPosition(hConsole, { short(91),  4 });
  cout << "|";
  SetConsoleCursorPosition(hConsole, { short(91),  5 });
  cout << "/";
  SetConsoleCursorPosition(hConsole, { short(90),  6 });
  cout << "/";
  }

  SetConsoleCP(1251);
  SetConsoleOutputCP(1251);
  SetConsoleCursorPosition(hConsole, { 69,  17 });
  cout << "Акуленко Вячеслав Владимирович, 4 курс, 4 группа, 2022";
  SetConsoleCP(866);
  SetConsoleOutputCP(866);

  for (int i = 0; i < 4; i++) OutS(i);
  for (int i = 0; i < 3; i++) OutZ(i);
  OutRType();
}

void Delete() {
  if (b % 2 || i == 0) return;
  s[b / 2].erase(i - 1, 1);
  i--;
  OutS(b / 2);  
}

void Arrows() {
  char a = _getch();

  if (a == 81 || a == 73) {
    if (b != 7) b = 7; else b = 0;
    return;
  }
  if (a == 80 && b == 7) {
    rt++;
    rt = (rt + 3) % 3;
    return;
  }
  if (a == 72 && b == 7) {
    rt--;
    rt = (rt + 3) % 3;
    return;
  }
  if (b == 7) return;
  if (a == 79 || (a == 77 && b % 2 == 1 && b != 7)) {
    b++;
    if (b == 7) b = 0;
    if (b % 2 == 0) i = s[b / 2].size();
    return;
  }
  if (a == 71 || (a == 75 && b % 2 == 1 && b != 7)) {
    b--;
    if (b == -1) b = 6;
    if (b % 2 == 0) i = s[b / 2].size();
    return;
  }
  
  
  if (a == 77 && b % 2 == 0) {
    if (i != s[b / 2].size()) i++;
    else {
      b++;
      if (b == 7) b = 0;
    }
    return;
  }
  if (a == 75 && b % 2 == 0) {
    if (i != 0) i--; else {
      b--;
      if (b == -1) b == 6;
    }
    return;
  }
  
  if (a == 80 && b % 2 == 1 && b != 7) {
    z[b / 2]++;
    z[b / 2] = (z[b / 2] + 4) % 4;
    return;
  }
  if (a == 72 && b % 2 == 1 && b != 7) {
    z[b / 2]--;
    z[b / 2] = (z[b / 2] + 4) % 4;
    return;
  }
}

int main() {
  
  
  system("mode con cols=125 lines=19");
  system("cls");


  s.push_back("0");
  s.push_back("0");
  s.push_back("0");
  s.push_back("0");
  
  z.push_back(0);
  z.push_back(0);
  z.push_back(0);

  Out();
  i = s[0].size();
  OutS(0);
  while (1) {
    if (_kbhit()) {
      char a = _getch();
      
      if (a == 13) {
        Calc();
        
        if (b == 7) OutRType(); else 
        if (b % 2 == 0) OutS(b / 2); else 
        if (b % 2 == 1) OutZ(b / 2);
        
        continue;
      }
      if (a == 8) {
        Delete();
        continue;
      }
      if (a == 27) {
        system("cls");
        break;
      }
      if (a == -32) {
        Arrows();
        if (b == 7) OutRType(); else
        if (b % 2 == 0) OutS(b / 2); else
        if (b % 2 == 1) OutZ(b / 2);
        continue;
      }

      if (b % 2 == 0) {
        string o = "";

        if (a == ',')  a = '.';
        if (ch(a) && s[b / 2].size() < 25) {
          o += a;
          s[b / 2].insert(i, o);

          i++; 
          OutS(b / 2);
        }
      }
    }
  }
  
  return 0;
}