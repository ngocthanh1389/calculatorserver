#include "calculator.h"

#include <unistd.h>
#include <stack>

#include "machine.h"

#define INVALID "INVALID\n"

extern int client_socket[MAX_CLIENT];

// Function to find precedence of
// operators.
int precedence(char op){
    if(op == '+'||op == '-')
        return 1;
    if(op == '*'||op == '/')
        return 2;
    return 0;
}

// Function to perform arithmetic operations.
int applyOp(int a, int b, char op){
    switch(op){
    case '+': return a + b;
    case '-': return a - b;
    case '*': return a * b;
    case '/': if (b != 0) return a / b;
    }
    return 0;
}

// Calculate expression
string cal(int sd) {
    char c, check;

    // stack to store integer values.
    stack <int> values;

    // stack to store operators.
    stack <char> ops;

    Machine machine;

    while((check = read( sd , &c, 1)) != 0 && c != '\n' && c != '\r') {
start:

        // Current token is a whitespace,
        // skip it.
        if(c == ' ')
            continue;

        auto valid = machine.check(c);
        // Remove invalid expression
        if(!valid) {
            while((check = read( sd , &c, 1)) != 0 && c != '\n'){}
            return INVALID;
        }
        machine.update();

        // Current token is an opening
        // brace, push it to 'ops'
        if(c == '('){
            ops.push(c);
        }

        // Current token is a number, push
        // it to stack for numbers.
        else if(isdigit(c)) {
            int val = c-'0';

            // There may be more than one
            // digits in number.
            while((check = read( sd , &c, 1)) != 0 && c != '\n' && c != '\r' && isdigit(c)) {
                val = (val*10) + (c-'0');
            }

            values.push(val);

            // check end of expression
            if (check == 0 || c == '\n' || c == '\r')
                break;
            else
                goto start;
        }

        // Closing brace encountered, solve
        // entire brace.
        else if(c == ')') {
            while(!ops.empty() && ops.top() != '(')
            {
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }

            // pop opening brace.
            if(!ops.empty())
                ops.pop();
        }

        // Current token is an operator.
        else  {
            // While top of 'ops' has same or greater
            // precedence to current token, which
            // is an operator. Apply operator on top
            // of 'ops' to top two elements in values stack.
            while(!ops.empty() && precedence(ops.top()) >= precedence(c)){
                int val2 = values.top();
                values.pop();

                int val1 = values.top();
                values.pop();

                char op = ops.top();
                ops.pop();

                values.push(applyOp(val1, val2, op));
            }
            // Push current token to 'ops'.
            ops.push(c);
        }
    }

    // Entire expression has been parsed at this
    // point, apply remaining ops to remaining
    // values.
    while(!ops.empty()) {
        int val2 = values.top();
        values.pop();

        int val1 = values.top();
        values.pop();

        char op = ops.top();
        ops.pop();

        values.push(applyOp(val1, val2, op));
    }

    // continue read to end expression
    if (c == '\r')
        read( sd , &c, 1);

    if (check == 0) {
        printf("Close socket: %d\n", sd);
        close( sd );
        for (int i = 0; i < MAX_CLIENT; i++)
            if (client_socket[i] == sd){
                client_socket[0] = 0;
            }
    }
    // Top of 'values' contains result, return it.
    if (machine.checkEnd() && values.size() == 1)
        return to_string(values.top()) + "\n";
    else
        return INVALID;
}
