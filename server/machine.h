#ifndef MACHINE_H
#define MACHINE_H

using namespace std;

class Machine // check string is valid expression or not
{
public:
    Machine();
    bool check(char c);
    void update();
    bool checkEnd();
private:
    enum TypeChar{
        NONE = 0,
        DIGIT,
        SCOPE,
        OP,
        OTHER
    };

    char current_c = 0, previous_c = 0;
    TypeChar current_type = NONE, previous_type = NONE;
    int count_scope = 0;
};

#endif // MACHINE_H
