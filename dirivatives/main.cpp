//
//  main.cpp
//  dirivatives
//
//  Created by Vadim on 25.12.16.
//  Copyright © 2016 Vadim. All rights reserved.
//

#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstdlib>
// #include <bits/stdc++.h>
using namespace std;
#define function kekesmen
string s;

string substring(int l, int r) {
    if (r < l) return "";
    return s.substr(l, r - l + 1);
}

bool isIntegerNumber(string &str, int l, int r) {
    for (int i = l; i <= r; ++i)
        if (str[i] < '0' || str[i] > '9')
            return 0;
    return 1;
}

bool isDoubleNumber(string &str, int l, int r) {
    for (int i = l; i <= r; ++i)
        if ((str[i] < '0' || str[i] > '9') &&  s[i] != '.')
            return 0;
    return 1;
}

string dirivative_expr(int, int);

string dirivative_multiplyement(int l, int r) {
    int bal = 0;
    for (int i = l; i <= r; ++i) {
        if (s[i] == '(') {++bal;continue;}
        if (s[i] == ')') {--bal;continue;}
        if (!bal && s[i] == '/') {
            return "(" + dirivative_expr(l, i - 1) + "*" + substring(i + 1, r) + "-" + substring(l, i - 1) + "*" + dirivative_expr(i + 1, r) + ")/" + substring(i + 1, r) + "^2";
        }
    }
    for (int i = l; i <= r; ++i) {
        if (s[i] == '(') {++bal;continue;}
        if (s[i] == ')') {--bal;continue;}
        if (!bal && s[i] == '^') {
            if (substring(l, i - 1) == "x" && isIntegerNumber(s, i + 1, r)) {
                return substring(i + 1, r) + "*x^" + to_string(stoi(substring(i + 1, r)) - 1);
            }
            if (substring(l, i - 1) == "x" && isDoubleNumber(s, i + 1, r)) {
                return substring(i + 1, r) + "*x^(" + to_string(stof(substring(i + 1, r)) - 1) + ")";
            }
            int l0 = (int)s.size();
            s += substring(i + 1, r) + "*ln(" + substring(l, i - 1) + ")";
            int r0 = (int)s.size() - 1;
            return substring(l, r) + "*(" + dirivative_expr(l0, r0) + ")";
        }
    }
    if (s[l] == '(' && s[r] == ')') {
        return "(" + dirivative_expr(l + 1, r - 1) + ")";
    }
    if (l == r && s[l] == 'x') return "1";
    if (l == r && s[l] == 'y') return "1";
    if (l == r && s[l] == 'z') return "1";
    if (s[l] >= '0' && s[l] <= '9') return "0";
    if (s[l] == '-') return "-(" + dirivative_expr(l + 1, r) + ")";
    if (s[l] == 's') {
        l += 4, --r;
        return "cos(" + substring(l, r) + ")*(" + dirivative_expr(l, r) + ")";
    }
    if (s[l] == 'c' && s[l + 1] == 'o') {
        l += 4, --r;
        return "(-sin(" + substring(l, r) + "))*(" + dirivative_expr(l, r) + ")";
    }
    if (s[l] == 'c' && s[l + 1] == 't') {
        l += 4, --r;
        return "(-1/sin(" + substring(l, r) + ")^2)*(" + dirivative_expr(l, r) + ")";
    }
    if (s[l] == 't') {
        l += 3, --r;
        return "(1/cos(" + substring(l, r) + ")^2)*(" + dirivative_expr(l, r) + ")";
    }
    if (s[l] == 'l') {
        l += 3, --r;
        return "(" + dirivative_expr(l, r) + ")/(" + substring(l, r) + ")";
    }
    if (s[l] == 'a') {
        l += 3;
        if (s[l] == 's') {
            l += 4, --r;
            return "((" + dirivative_expr(l, r) + ")/sqrt(1-(" + substring(l, r) + ")^2))";
        }
        if (s[l] == 'c' && s[l + 1] == 'o') {
            l += 4, --r;
            return "(-(" + dirivative_expr(l, r) + ")/sqrt(1-(" + substring(l, r) + ")^2))";
        }
        if (s[l] == 'c' && s[l + 1] == 't') {
            l += 4, --r;
            return "(-(" + dirivative_expr(l, r) + ")/(1-(" + substring(l, r) + ")^2))";
        }
        if (s[l] == 't') {
            l += 3, --r;
            return "(-(" + dirivative_expr(l, r) + ")/(1-(" + substring(l, r) + ")^2))";
        }
    }
    return "ERROR";
}

string dirivative_summarant(int l, int r) {
    int bal = 0, last = l;
    string ans = "";
    for (int i = l; i <= r; ++i) {
        if (s[i] == '(') {++bal;continue;}
        if (s[i] == ')') {--bal;continue;}
        if (!bal && s[i] == '*') {
            string leftPart = substring(l, last - 1);
            string rightPart = "*" + substring(i + 1, r);
            ans += "(" + leftPart + dirivative_expr(last, i - 1) + rightPart + ")+";
            last = i + 1;
        }
    }
    if (last == l) return dirivative_multiplyement(l, r);
    else {
        string leftPart = substring(l, last - 1);
        ans += "(" + leftPart + dirivative_expr(last, r) + ")";
    }
    return ans;
}

string dirivative_expr(int l, int r) {
    if (r < l) return "";
    if (l == r && s[l] == '-') return "-";
    if (l == r && s[l] == '(') return "(";
    if (l == r && s[l] == '(') return ")";
    int bal = 0, last = l;
    string ans = "";
    for (int i = l; i <= r; ++i) {
        if (s[i] == '(') {++bal;continue;}
        if (s[i] == ')') {--bal;continue;}
        if (!bal && (s[i] == '+' || s[i] == '-')) {
            ans += dirivative_expr(last, i - 1);
            ans.push_back(s[i]);
            last = i + 1;
        }
    }
    if (last == l) return dirivative_summarant(l, r);
    else ans += dirivative_expr(last, r);
    return ans;
}

int main(int argc, const char * argv[]) {
    //freopen("deriv.in", "r", stdin);
    //freopen("deriv.out", "w", stdout);
    bool making_readable_enabled = 0;
    while (1) {
        getline(cin, s);
        string s2 = "";
        for (int i = 0; i < (int) s.size(); ++i) {
            if (s[i] == '*' && s[i + 1] == '*') {
                s2.push_back('^'), ++i;
                continue;
            }
            if (s[i] != ' ')
                s2.push_back(s[i]);
        }
        s = s2;
        string ans = dirivative_expr(0, (int)s.size() - 1);
        string ans2 = "";
        for (int i = 0; i < (int) ans.size(); ++i) {
            if (false && ans[i] == '^') {
                ans2.push_back('*');
                ans2.push_back('*');
                continue;
            }
            ans2.push_back(ans[i]);
        }
        cout << ans2 << endl;
    }
    return 0;
}
// (u^v)' = (e^(vln(u)))' = u^v * (vln(u))'
// x^x' = x^x (xlnx)' = x^x * (lnx + x/x)
