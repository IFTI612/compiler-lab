#include <bits/stdc++.h>
using namespace std;

/*
    CSE 4130 - Formal Language and Compilers Lab
    Assignment 5: CFG for simple arithmetic expressions

    <Exp>    -> <Term> + <Term> | <Term> - <Term> | <Term>
    <Term>   -> <Factor> * <Factor> | <Factor> / <Factor> | <Factor>
    <Factor> -> ( <Exp> ) | ID | NUM
    ID       -> a | b | c | d | e
    NUM      -> 0 | 1 | 2 | ... | 9
*/

string str;
bool found;
int length, i;

// forward declarations (Factor calls Exp)
void Exp();
void Term();
void Factor();

// skip blank spaces between tokens
void skipSpace()
{
    while (i < length && str[i] == ' ')
        i++;
}

// <Factor> -> ( <Exp> ) | ID | NUM
void Factor()
{
    skipSpace();
    if (i < length && str[i] == '(')
    {
        i++;
        Exp();
        skipSpace();
        if (found && i < length && str[i] == ')')
        {
            i++;
            found = true;
        }
        else
        {
            found = false;
        }
    }
    else if (i < length && str[i] >= 'a' && str[i] <= 'e')
    { // ID
        i++;
        found = true;
    }
    else if (i < length && isdigit(str[i]))
    { // NUM
        i++;
        found = true;
    }
    else
    {
        found = false;
    }
}

// <Term> -> <Factor> * <Factor> | <Factor> / <Factor> | <Factor>
void Term()
{
    Factor();
    if (!found)
        return;

    skipSpace();
    if (i < length && (str[i] == '*' || str[i] == '/'))
    {
        i++;
        skipSpace();
        Factor();
    }
}

// <Exp> -> <Term> + <Term> | <Term> - <Term> | <Term>
void Exp()
{
    Term();
    if (!found)
        return;

    skipSpace();
    if (i < length && (str[i] == '+' || str[i] == '-'))
    {
        i++;
        skipSpace();
        Term();
    }
}

int main()
{
    cout << "Type 'exit' to quit." << endl;
    while (true)
    {
        found = true;
        i = 0;

        cout << "Enter a string: ";
        getline(cin, str);

        if (str == "exit")
            break;

        length = str.size();
        Exp();

        skipSpace(); // allow trailing spaces
        if (!found || i != length)
            cout << "Rejected" << endl;
        else
            cout << "Accepted" << endl;

        cout << endl;
    }
    return 0;
}