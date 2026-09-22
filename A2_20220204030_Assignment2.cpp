#include <bits/stdc++.h>
using namespace std;

set<string> keywords = {
    "auto", "break", "case", "char", "const",
    "continue", "default", "do", "double", "else",
    "enum", "extern", "float", "for", "goto",
    "if", "int", "long", "register", "return",
    "short", "signed", "sizeof", "static", "struct",
    "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while"};

set<string> doubleOperators = {
    "<=", ">=", "==", "!=", "++", "--",
    "&&", "||", "+=", "-=", "*=", "/=",
    "%=", "<<", ">>"};

set<char> singleOperators = {
    '+', '-', '*', '/', '%',
    '=', '<', '>', '!', '&',
    '|', '^', '~'};

set<char> separators = {
    '\'', '\"', ';', ','};

set<char> paranthesis = {
    '(', ')', '{', '}', '[', ']'};

string removeCommentAndSpaces(string str)
{
    string ans;
    bool inSingleComment = false;
    bool inMultiComment = false;
    bool prevSpace = false;

    for (int i = 0; i < str.length(); i++)
    {
        // End of single-line comment
        if (inSingleComment)
        {
            if (str[i] == '\n')
                inSingleComment = false;
            continue;
        }

        // End of multi-line comment
        if (inMultiComment)
        {
            if (str[i] == '*' && i + 1 < str.length() && str[i + 1] == '/')
            {
                inMultiComment = false;
                i++;
            }
            continue;
        }

        // Start of comments
        if (str[i] == '/' && i + 1 < str.length())
        {
            if (str[i + 1] == '/')
            {
                inSingleComment = true;
                i++;
                continue;
            }

            if (str[i + 1] == '*')
            {
                inMultiComment = true;
                i++;
                continue;
            }
        }

        // Remove extra whitespace
        if (isspace(str[i]))
        {
            if (!prevSpace)
            {
                ans += ' ';
                prevSpace = true;
            }
        }
        else
        {
            ans += str[i];
            prevSpace = false;
        }
    }

    return ans;
}

string addSpaces(string ans)
{
    string temp;

    for (int i = 0; i < ans.length(); i++)
    {
        // Check two-character operators first
        if (i + 1 < ans.length())
        {
            string op = "";
            op += ans[i];
            op += ans[i + 1];

            if (doubleOperators.count(op))
            {
                temp += ' ';
                temp += op;
                temp += ' ';
                i++;
                continue;
            }
        }

        // Separators
        if (separators.count(ans[i]))
        {
            temp += ' ';
            temp += ans[i];
            temp += ' ';
        }

        // paranthesis
        else if (paranthesis.count(ans[i]))
        {
            temp += ' ';
            temp += ans[i];
            temp += ' ';
        }

        // Single-character operators
        else if (singleOperators.count(ans[i]))
        {
            temp += ' ';
            temp += ans[i];
            temp += ' ';
        }

        // Everything else
        else
        {
            temp += ans[i];
        }
    }

    string result;
    bool prevSpace = false;

    for (char ch : temp)
    {
        if (isspace(ch))
        {
            if (!prevSpace)
            {
                result += ' ';
                prevSpace = true;
            }
        }
        else
        {
            result += ch;
            prevSpace = false;
        }
    }

    return result;
}

string getLexeme(const string &ans, int &i)
{
    string lexeme;

    while (i < ans.length())
    {
        // Stop at delimiters
        if (isspace(ans[i]))
        {
            break;
        }

        lexeme += ans[i];
        i++;
    }

    i--;
    return lexeme;
}

string tokenPrint(string kw, string chr)
{
    string temp = "";

    temp += "[ " + kw + " " + chr + " ] ";

    return temp; // Return the processed string for output
}

string numDFA(string lexeme)
{
    int i = 0;
    int l = lexeme.length();
    int state = 0;

    // Start State
    if (isdigit(lexeme[i]))
    {
        state = 1;
        i++;
    }
    else if (lexeme[i] == '.')
    {
        state = 2;
        i++;
    }
    else
    {
        state = 0;
    }

    // State 1 : Integer part
    if (state == 1)
    {
        for (; i < l; i++)
        {
            if (isdigit(lexeme[i]))
            {
                state = 1;
            }
            else if (lexeme[i] == '.')
            {
                state = 2;
                i++;
                break;
            }
            else
            {
                state = 0;
                break;
            }
        }
    }

    // State 2 : Decimal point
    if (state == 2)
    {
        if (i < l && isdigit(lexeme[i]))
        {
            state = 3;
            i++;
        }
        else
        {
            state = 0;
        }
    }

    // State 3 : Fraction part
    if (state == 3)
    {
        for (; i < l; i++)
        {
            if (isdigit(lexeme[i]))
            {
                state = 3;
            }
            else
            {
                state = 0;
                break;
            }
        }
    }

    // Accepting states
    if (state == 1)
        return tokenPrint("int", lexeme);

    if (state == 3)
        return tokenPrint("float", lexeme);

    return tokenPrint("unkn", lexeme);
}

string idDFA(string lexeme)
{
    if (!(isalpha(lexeme[0]) || lexeme[0] == '_')) // dead state
        return tokenPrint("unkn", lexeme);

    for (int i = 1; i < lexeme.length(); i++)
    {
        if (!(isalnum(lexeme[i]) || lexeme[i] == '_')) // dead state
            return tokenPrint("unkn", lexeme);
    }

    if (keywords.count(lexeme))
        return tokenPrint("kw", lexeme); // for keywords

    return tokenPrint("id", lexeme); // state 1
}

string tokenize(string ans)
{
    string token;

    for (int i = 0; i < ans.length(); i++)
    {
        if (isspace(ans[i]))
            continue;

        // Double Operators
        if (i + 1 < ans.length())
        {
            string op = "";
            op += ans[i];
            op += ans[i + 1];

            if (doubleOperators.count(op))
            {
                token += tokenPrint("op", op);
                i++;
                continue;
            }
        }

        // Single Operators
        if (singleOperators.count(ans[i]))
        {
            token += tokenPrint("op", string(1, ans[i]));
            continue;
        }

        // Separators
        if (separators.count(ans[i]))
        {
            token += tokenPrint("sep", string(1, ans[i]));
            continue;
        }

        // Parentheses
        if (paranthesis.count(ans[i]))
        {
            token += tokenPrint("par", string(1, ans[i]));
            continue;
        }

        string lexeme = getLexeme(ans, i);

        if (lexeme.empty())
            continue;

        // Starts with digit or '.'
        if (isdigit(lexeme[0]) || lexeme[0] == '.') // state 0
        {
            token += numDFA(lexeme);
        }
        else if (isalpha(lexeme[0]) || lexeme[0] == '_') // state 0
        {
            token += idDFA(lexeme);
        }
        else
        {
            token += tokenPrint("unkn", lexeme);
        }
    }

    return token;
}
int main()
{
    FILE *p1, *p2;
    int c;
    string str;

    p1 = fopen("input.c", "r");

    if (!p1)
    {
        cout << "File can't open!\n";
        return 0;
    }
    cout << "Sample : " << "input.c" << endl;

    while ((c = fgetc(p1)) != EOF)
    {
        str += (char)c;
        cout << (char)c;
    }
    cout << endl;

    fclose(p1);

    // for assignment 1,
    string ans = removeCommentAndSpaces(str);

    // assingment 2 , step 1: separate the string in seperator operators and operands and keywords
    ans = addSpaces(ans);

    // step 2: tokenize

    string token = tokenize(ans);

    cout << endl;
    cout << "Sample : " << "step1.txt" << endl;

    p2 = fopen("step1.txt", "w");
    cout << ans;
    for (int i = 0; i < ans.length(); i++)
    {
        fputc(ans[i], p2);
    }
    cout << endl;

    fclose(p2);
    cout << endl;

    cout << "Sample : " << "step2.txt" << endl;
    cout << token;
    p2 = fopen("step2.txt", "w");

    for (int i = 0; i < token.length(); i++)
    {
        fputc(token[i], p2);
    }
    fclose(p2);

    return 0;
}