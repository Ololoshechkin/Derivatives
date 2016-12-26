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

bool isNumber(string &str, int l, int r) {
    for (int i = l; i <= r; ++i)
        if (str[i] < '0' || str[i] > '9')
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
            if (substring(l, i - 1) == "x" && isNumber(s, i + 1, r)) {
                return substring(i + 1, r) + "*x^" + to_string(stoi(substring(i + 1, r)) - 1);
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

char get(string &ans, int i) {
    if (i < 0) return '!';
    if (i >= (int) ans.size()) return '?';
    return ans[i];
}

void delete_x1(string &ans) {
    // "*1
    // 1*?
    // ?*1*?
    string answer = "";
    for (int i = 0; i < (int) ans.size(); ++i) {
        
        if (get(ans, i) == '*' && get(ans, i + 1) == '1' && (get(ans, i + 2) < '0' || get(ans, i + 2) > '9')) {
            ++i;
            continue;
        }
        if (get(ans, i) == '1' && get(ans, i + 1) == '*' && (get(ans, i - 1) < '0' || get(ans, i - 1) > '9')) {
            ++i;
            continue;
        }
        
        if (get(ans, i) == '+' && get(ans, i + 1) == '0' && get(ans, i + 2) == '+') {
            i += 2;
            continue;
        }
        if (get(ans, i) == '0' && get(ans, i + 1) == '+' && (get(ans, i - 1) < '0' || get(ans, i - 1) > '9')) {
            ++i;
            continue;
        }
        if (get(ans, i) == '+' && get(ans, i + 1) == '0') {
            ++i;
            continue;
        }
        
        answer.push_back(ans[i]);
    }
    ans = answer;
}

bool is_simple_expression(string ans, int l, int r) {
    string cur = ans.substr(l, r - l + 1);
    if (isNumber(cur, 0, r - l)) return 1;
    if (cur == "x") return 1;
    if (cur == "sin(x)" || cur == "sinx") return 1;
    if (cur == "cos(x)" || cur == "cosx") return 1;
    if (cur == "tg(x)" || cur == "tgx") return 1;
    if (cur == "ctg(x)" || cur == "ctgx") return 1;
    if (cur == "arcsin(x)" || cur == "arcsinx") return 1;
    if (cur == "arccos(x)" || cur == "arccosx") return 1;
    if (cur == "arctg(x)" || cur == "arctgx") return 1;
    if (cur == "arcctg(x)" || cur == "arcctgx") return 1;
    if (cur == "ln(x)" || cur == "lnx") return 1;
    return 0;
}

void delete_brackets(string &ans) {
    vector<int> right_pair((int) ans.size());
    vector<int> st;
    for (int i = 0; i < (int) ans.size(); ++i) {
        if (ans[i] == '(') st.push_back(i);
        else if (ans[i] == ')') {
            right_pair[st.back()] = i;
            st.pop_back();
        }
    }
    vector<bool> deleted((int) ans.size());
    for (int i = 0; i < (int) ans.size() - 1; ++i) {
        if (ans[i] == '(' && ans[i + 1] == '(' && right_pair[i] - 1 == right_pair[i + 1]) {
            deleted[i] = deleted[right_pair[i]] = 1;
        }
    }
    int func_bal = 0;
    for (int i = 0; i < (int) ans.size(); ++i) {
        if (ans.substr(i, 4) == "cos(") i = right_pair[i + 3];
        if (ans.substr(i, 4) == "ctg(") i = right_pair[i + 3];
        if (ans.substr(i, 4) == "sin(") i = right_pair[i + 3];
        if (ans.substr(i, 3) == "tg(") i = right_pair[i + 2];
        if (ans.substr(i, 3) == "ln(") i = right_pair[i + 2];
        if (ans.substr(i, 7) == "arcsin(") i = right_pair[i + 6];
        if (ans.substr(i, 7) == "arccos(") i = right_pair[i + 6];
        if (ans.substr(i, 7) == "arcctg(") i = right_pair[i + 6];
        if (ans.substr(i, 6) == "arctg(") i = right_pair[i + 5];
        if (ans[i] == '(' && is_simple_expression(ans, i + 1, right_pair[i] - 1)) {
            deleted[i] = deleted[right_pair[i]] = 1;
        }
    }
    bool should_delete_external = (ans[0] == '(');
    int bal = 0;
    for (int i = 0; i < (int) ans.size() - 1; ++i) {
        if (ans[i] == '(') ++bal;
        if (ans[i] == ')') --bal;
        if (!bal) should_delete_external = 0;
    }
    if (should_delete_external) deleted[0] = deleted[right_pair[0]] = 1;
    string answer = "";
    for (int i = 0; i < (int) ans.size(); ++i)
        if (!deleted[i])
            answer.push_back(ans[i]);
    ans = answer;
}

void make_readable(string &ans) {
    for (int i = 0; i < 30; ++i) {
        delete_brackets(ans);
        delete_x1(ans);
    }
}

bool psp(string &ans) {
    int bal = 0;
    for (int i = 0; i < (int)ans.size(); ++i) {
        if (ans[i] == '(') ++bal;
        if (ans[i] == ')') --bal;
        if (bal < 0) return 0;
    }
    if (bal) return 0;
    return 1;
}

int main(int argc, const char * argv[]) {
    freopen("deriv.in", "r", stdin);
    freopen("deriv.out", "w", stdout);
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
        if (s == "enable") {making_readable_enabled = 1; continue;}
        if (s == "disable") {making_readable_enabled = 0; continue;}
        string ans = dirivative_expr(0, (int)s.size() - 1);
        if (making_readable_enabled) make_readable(ans);
        string ans2 = "";
        for (int i = 0; i < (int) ans.size(); ++i) {
            if (ans[i] == '^') {
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
