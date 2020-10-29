#include "machine.h"

#include <ctype.h>

Machine::Machine()
{
}

bool Machine::check(char c) {
    current_c = c;

    if (c == '(' || c == ')') {
        current_type = SCOPE;
        if (c == '(')
            count_scope++;
        else
            count_scope--;
        if (count_scope < 0)   // ()))) -> false
            return false;
    } else if (c == '+' || c == '-' || c == '*' || c == '/') {
        current_type = OP;
    } else if (isdigit(c)) {
        current_type = DIGIT;
    } else {
        current_type = OTHER;
    }

    if ((c == '(' && previous_type == DIGIT) || (c == '(' && previous_c == ')'))    // 9( or )( - > false
        return false;

    if ((c == ')' && previous_type == OP) || (c == ')' && previous_c == '('))     // +) or () -> false
        return false;

    switch (current_type) {
    case OP:
        if (previous_type == OP) //   -+ -> false
            return false;
        if (previous_c == '(')  // (* -> false
            return false;
        if (previous_type == NONE)
            return false;
        break;
    case DIGIT:
        if (previous_c == ')') // )9 -> false
            return false;
        break;
    case OTHER: // ? * @ ....-> false
        return false;
    default:
        return true;
    }
    return true;
}

void Machine::update() {
    previous_c = current_c;
    previous_type = current_type;
}

bool Machine::checkEnd() {
    return count_scope == 0 && current_type != OP;
}
