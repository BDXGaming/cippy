#include <math.h>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <cstring>


using namespace std;

class math{
    public:
        static vector<string> get_module_cmds(){
            vector<string> cmds;
            cmds.push_back("math.calc");
            return cmds;
        }

        static int priority(char op) {
            if (op == '+' || op == '-') {
                return 1;
            } else if (op == '*' || op == '/') {
                return 2;
            } else {
                return 0;
            }
        }

        static double eval(double a, double b, char op) {
            switch (op) {
                case '+': return a + b;
                case '-': return a - b;
                case '*': return a * b;
                case '/': return a / b;
                default: return 0;
            }
        }

        static double calc(string expr) {
            stack<char> opStack;
            stack<double> valStack;
            for (int i = 0; i < expr.length(); i++) {
                if (expr[i] == '(') {
                    opStack.push(expr[i]);
                } else if (expr[i] == ')') {
                    while (opStack.top() != '(') {
                        char op = opStack.top();
                        opStack.pop();
                        double b = valStack.top();
                        valStack.pop();
                        double a = valStack.top();
                        valStack.pop();
                        double res = eval(a, b, op);
                        valStack.push(res);
                    }
                    opStack.pop(); // remove the '('
                } else if (isdigit(expr[i])) {
                    int j = i;
                    while (j < expr.length() && isdigit(expr[j])) {
                        j++;
                    }
                    double val = stod(expr.substr(i, j - i));
                    valStack.push(val);
                    i = j - 1;
                } else {
                    while (!opStack.empty() && priority(opStack.top()) >= priority(expr[i])) {
                        char op = opStack.top();
                        opStack.pop();
                        double b = valStack.top();
                        valStack.pop();
                        double a = valStack.top();
                        valStack.pop();
                        double res = eval(a, b, op);
                        valStack.push(res);
                    }
                    opStack.push(expr[i]);
                }
            }
            while (!opStack.empty()) {
                char op = opStack.top();
                opStack.pop();
                double b = valStack.top();
                valStack.pop();
                double a = valStack.top();
                valStack.pop();
                double res = eval(a, b, op);
                valStack.push(res);
            }
            return valStack.top();
        }

};

