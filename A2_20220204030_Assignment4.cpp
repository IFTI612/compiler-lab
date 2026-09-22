
#include <bits/stdc++.h>
using namespace std;

set<string> keywords =
    {
        "auto", "break", "case", "char", "const",
        "continue", "default", "do", "double", "else",
        "enum", "extern", "float", "for", "goto",
        "if", "int", "long", "register", "return",
        "short", "signed", "sizeof", "static", "struct",
        "switch", "typedef", "union", "unsigned", "void",
        "volatile", "while", "printf", "scanf"};

set<string> doubleOperators =
    {
        "<=", ">=", "==", "!=", "++", "--",
        "&&", "||", "+=", "-=", "*=", "/=",
        "%=", "<<", ">>"};

set<char> singleOperators =
    {
        '+', '-', '*', '/', '%',
        '=', '<', '>', '!', '&',
        '|', '^', '~'};

set<char> separators =
    {
        '\'', '\"', ';', ','};

set<char> paranthesis =
    {
        '(', ')', '[', ']'};

set<char> braces =
    {
        '{', '}'};

string removeCommentAndSpaces(string str)
{
    string ans;
    bool inSingleComment = false;
    bool inMultiComment = false;
    bool prevSpace = false;

    int lineNo = 1;

    ans += "#LINE" + to_string(lineNo) + "# ";

    for (int i = 0; i < str.length(); i++)
    {
        // End of single-line comment
        if (inSingleComment)
        {
            if (str[i] == '\n')
            {
                inSingleComment = false;
                lineNo++;

                ans += '\n';
                ans += "#LINE" + to_string(lineNo) + "# ";
                prevSpace = false;
            }
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

            if (str[i] == '\n')
            {
                lineNo++;
                ans += '\n';
                ans += "#LINE" + to_string(lineNo) + "# ";
                prevSpace = false;
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

        // New line
        if (str[i] == '\n')
        {
            lineNo++;
            ans += '\n';
            ans += "#LINE" + to_string(lineNo) + "# ";
            prevSpace = false;
            continue;
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
        // Keep line number as a separate token
        if (isdigit(ans[i]) && (i == 0 || ans[i - 1] == '\n'))
        {
            while (i < ans.length() && isdigit(ans[i]))
            {
                temp += ans[i];
                i++;
            }

            temp += '.';
            temp += ' ';
            continue;
        }
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
        // Line number token
        if (ans.compare(i, 5, "#LINE") == 0)
        {
            int j = i + 5;
            string lineNo = "";

            while (j < ans.length() && isdigit(ans[j]))
            {
                lineNo += ans[j];
                j++;
            }

            if (j < ans.length() && ans[j] == '#')
            {
                token += tokenPrint("line", lineNo);
                i = j;
                continue;
            }
        }

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

        // String literal
        if (ans[i] == '"')
        {
            string str = "\"";
            i++;

            while (i < ans.length() && ans[i] != '"')
            {
                str += ans[i];
                i++;
            }

            if (i < ans.length())
                str += '"';

            token += tokenPrint("str", str);
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

        // Braces
        if (braces.count(ans[i]))
        {
            token += tokenPrint("brc", string(1, ans[i]));
            continue;
        }

        string lexeme = getLexeme(ans, i);

        if (lexeme.empty())
            continue;

        // Starts with digit or '.'
        if (isdigit(lexeme[0]) || lexeme[0] == '.')
        {
            token += numDFA(lexeme);
        }
        else if (isalpha(lexeme[0]) || lexeme[0] == '_')
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
// =========================================================
// ASSIGNMENT 3 : SYMBOL TABLE

// =========================================================

struct TokenA3
{
    string category; // "kw" | "id" | "op" | "sep" | "par" | "num" | "unkn"
    string text;     // the raw lexeme / symbol
    int line;
};

set<string> dataTypeKeywords =
    {
        "int", "float", "double", "char", "long", "short",
        "unsigned", "signed", "void", "struct", "enum", "union"};

// -------------------- Symbol Table --------------------
struct Symbol
{
    int id;
    string name;
    string idType;   // "var" or "func"
    string dataType; // int / float / double / ...
    string scope;    // "global", or the enclosing function's name
    string value;    // literal value if assigned a number, else ""
};

class SymbolTable
{
public:
    vector<Symbol> table;

    // insert(): adds a new (name, scope) pair if not already present.
    int insert(string name, string idType, string dataType, string scope)
    {
        int existing = lookupInScope(name, scope);
        if (existing != -1)
            return existing;

        Symbol s;
        s.id = (int)table.size() + 1;
        s.name = name;
        s.idType = idType;
        s.dataType = dataType;
        s.scope = scope;
        s.value = "";
        table.push_back(s);
        return s.id;
    }

    // lookup(): search for a name and return its id, or -1 if not found.
    int lookup(string name)
    {
        for (int i = 0; i < (int)table.size(); i++)
        {
            if (table[i].name == name)
                return table[i].id;
        }
        return -1;
    }

    // lookupInScope(): internal helper used while resolving identifie
    int lookupInScope(string name, string scope)
    {
        for (int i = 0; i < (int)table.size(); i++)
        {
            if (table[i].name == name && table[i].scope == scope)
                return table[i].id;
        }
        return -1;
    }

    // set_attribute(): associate/update an attribute of an existing entry.
    void set_attribute(int id, string attrName, string attrValue)
    {
        for (int i = 0; i < (int)table.size(); i++)
        {
            if (table[i].id == id)
            {
                if (attrName == "idType")
                    table[i].idType = attrValue;
                else if (attrName == "dataType")
                    table[i].dataType = attrValue;
                else if (attrName == "scope")
                    table[i].scope = attrValue;
                else if (attrName == "value")
                    table[i].value = attrValue;
                return;
            }
        }
    }

    string display()
    {
        // Column headers
        vector<string> headers = {"Sl. No.", "Name", "Id Type", "Data Type", "Scope", "Value"};

        // Compute each column's row values
        vector<vector<string>> rows;
        for (int i = 0; i < (int)table.size(); i++)
        {
            rows.push_back({to_string(table[i].id), table[i].name, table[i].idType,
                            table[i].dataType, table[i].scope, table[i].value});
        }

        vector<size_t> width(headers.size());
        for (size_t c = 0; c < headers.size(); c++)
        {
            width[c] = headers[c].length();
            for (size_t r = 0; r < rows.size(); r++)
                width[c] = max(width[c], rows[r][c].length());
        }

        auto makeBorder = [&]()
        {
            string line = "+";
            for (size_t c = 0; c < width.size(); c++)
                line += string(width[c] + 2, '-') + "+";
            line += "\n";
            return line;
        };

        auto makeRow = [&](const vector<string> &cells)
        {
            string line = "|";
            for (size_t c = 0; c < cells.size(); c++)
            {
                string padded = " " + cells[c] + string(width[c] - cells[c].length(), ' ') + " ";
                line += padded + "|";
            }
            line += "\n";
            return line;
        };

        string out;
        out += makeBorder();
        out += makeRow(headers);
        out += makeBorder();
        for (size_t r = 0; r < rows.size(); r++)
            out += makeRow(rows[r]);
        out += makeBorder();

        return out;
    }

    bool existsInScope(string name, string scope) /// for assignment 4
    {
        return lookupInScope(name, scope) != -1;
    }

    // free(): remove all entries and free storage of the symbol table.
    void free()
    {
        table.clear();
    }
};

// Prints a single-field token in the "[ category lexeme ]" format, for Step 1 and Step 4 streams.
string tokenPrintA3(string val)
{
    string temp = "";
    temp += "[ " + val + " ] ";
    return temp;
}

// -------------------- Pass 1 : classify tokens --------------------

vector<TokenA3> classifyTokensA3(string token)
{
    vector<TokenA3> tokens;

    int i = 0;
    int currentLine = 1;

    while (i < token.length())
    {
        // Find the next '['
        while (i < token.length() && token[i] != '[')
            i++;

        if (i >= token.length())
            break;

        i++; // Skip '['

        // Read everything until ']'
        string inside = "";
        while (i < token.length() && token[i] != ']')
        {
            inside += token[i];
            i++;
        }

        i++; // Skip ']'

        // Extract category and lexeme
        stringstream ss(inside);
        string category, lexeme;

        ss >> category; // First word
        ss >> lexeme;   // Second word

        if (category == "line")
        {
            currentLine = stoi(lexeme);
            continue;
        }

        // Convert int and float into num
        if (category == "int" || category == "float")
            category = "num";

        tokens.push_back({category, lexeme, currentLine});
    }

    return tokens;
}
/// Assignment 4 : Syntax Error Detection
struct SyntaxError
{
    int line;
    string message;
};

vector<SyntaxError> errors;

void addError(int line, string message)
{
    errors.push_back({line, message});
}

/// assignment 4 : Syntax Error Detection

// -------------------- Pass 2 : scopes, symbol table, streams --------------------
void buildSymbolTableAndStreams(vector<TokenA3> &tokens, SymbolTable &symTab,
                                string &step1, string &step4)
{
    int n = (int)tokens.size();
    vector<int> resolvedId(n, -1); // symbol table id for each "id" token, if any

    vector<string> scopeStack;
    scopeStack.push_back("global");

    // Bookkeeping so a function's scope is popped at the right ')' or '}'.
    struct ScopeFrame
    {
        string name;
        int pushParenDepth; // paren depth value right after the opening '('
        bool confirmed;     // true once we know it's a definition (saw '{')
        int pushBraceDepth; // brace depth value right after that '{'
    };
    vector<ScopeFrame> frames;

    int parenDepth = 0, braceDepth = 0;
    string pendingFuncPush = ""; // set when we just declared "id(" as a function

    for (int i = 0; i < n; i++)
    {
        string cat = tokens[i].category;
        string txt = tokens[i].text;

        if (cat == "par" && txt == "(")
        {
            parenDepth++;
            if (!pendingFuncPush.empty())
            {
                scopeStack.push_back(pendingFuncPush);
                frames.push_back({pendingFuncPush, parenDepth, false, -1});
                pendingFuncPush = "";
            }
        }
        else if (cat == "par" && txt == ")")
        {
            parenDepth--;
            if (!frames.empty() && !frames.back().confirmed &&
                frames.back().pushParenDepth - 1 == parenDepth)
            {
                bool bodyFollows = (i + 1 < n && tokens[i + 1].category == "brc" && tokens[i + 1].text == "{");
                if (bodyFollows)
                    frames.back().confirmed = true; // keep scope until matching '}'
                else
                {
                    scopeStack.pop_back(); // prototype only, no body
                    frames.pop_back();
                }
            }
        }
        else if (cat == "brc" && txt == "{")
        {
            braceDepth++;
            if (!frames.empty() && frames.back().confirmed && frames.back().pushBraceDepth == -1)
                frames.back().pushBraceDepth = braceDepth;
        }
        else if (cat == "brc" && txt == "}")
        {
            if (!frames.empty() && frames.back().confirmed && frames.back().pushBraceDepth == braceDepth)
            {
                scopeStack.pop_back();
                frames.pop_back();
            }
            braceDepth--;
        }
        else if (cat == "id")
        {
            bool isDeclaration = (i > 0 && tokens[i - 1].category == "kw" &&
                                  dataTypeKeywords.count(tokens[i - 1].text));

            int symId;
            if (isDeclaration)
            {
                string dataType = tokens[i - 1].text;

                string scope = scopeStack.back();

                if (symTab.existsInScope(txt, scope))
                {
                    addError(tokens[i].line,
                             "Duplicate identifier declaration: " + txt);
                }

                bool isFunc = (i + 1 < n &&
                               tokens[i + 1].category == "par" &&
                               tokens[i + 1].text == "(");

                string idType = isFunc ? "func" : "var";

                symId = symTab.insert(txt, idType, dataType, scope);

                if (isFunc)
                    pendingFuncPush = txt;
            }
            else
            {
                // Reference to an already-declared identifier: resolve by scope,
                // falling back to global (mirrors normal C scoping rules).
                symId = symTab.lookupInScope(txt, scopeStack.back());
                if (symId == -1)
                    symId = symTab.lookupInScope(txt, "global");
                if (symId == -1)
                {
                    addError(tokens[i].line,
                             "Undeclared identifier: " + txt);
                }
            }

            resolvedId[i] = symId;

            // "[id] [=] [num]" -> record/refresh the Value column.
            if (i + 2 < n && tokens[i + 1].category == "op" && tokens[i + 1].text == "=" &&
                tokens[i + 2].category == "num")
            {
                symTab.set_attribute(symId, "value", tokens[i + 2].text);
            }
        }
    }

    // -------------------- Build Step 1 and Step 4 streams --------------------
    for (int i = 0; i < n; i++)
    {
        string cat = tokens[i].category;
        string txt = tokens[i].text;

        if (cat == "id")
        {
            step1 += tokenPrintA3("id " + txt);
            step4 += tokenPrintA3("id " + to_string(resolvedId[i]));
        }
        else if (cat == "unkn")
        {
            step1 += tokenPrintA3("unkn " + txt);
            step4 += tokenPrintA3("unkn " + txt);
        }
        else // kw, op, sep, par, num -> bare symbol, same in both streams
        {
            step1 += tokenPrintA3(txt);
            step4 += tokenPrintA3(txt);
        }
    }
}

/// Assignment 4

void sortErrors()
{
    sort(errors.begin(), errors.end(),
         [](SyntaxError a, SyntaxError b)
         {
             return a.line < b.line;
         });
}

void checkSyntaxErrors(vector<TokenA3> &tokens)
{
    stack<int> parenStack;
    stack<int> braceStack;
    stack<int> ifStack;

    for (int i = 0; i < tokens.size(); i++)
    {
        string cat = tokens[i].category;
        string txt = tokens[i].text;
        int line = tokens[i].line;

        // ---------------------------------
        // Parentheses
        // ---------------------------------
        if (cat == "par" && txt == "(")
        {
            parenStack.push(line);
        }
        else if (cat == "par" && txt == ")")
        {
            if (parenStack.empty())
                addError(line, "Unmatched ')'");

            else
                parenStack.pop();
        }

        // ---------------------------------
        // Braces
        // ---------------------------------
        else if (cat == "brc" && txt == "{")
        {
            braceStack.push(line);
        }
        else if (cat == "brc" && txt == "}")
        {
            if (braceStack.empty())
                addError(line, "Unmatched '}'");

            else
                braceStack.pop();
        }

        // ---------------------------------
        // if / else
        // ---------------------------------
        else if (cat == "kw" && txt == "if")
        {
            ifStack.push(line);
        }
        else if (cat == "kw" && txt == "else")
        {
            if (ifStack.empty())
            {
                addError(line, "Else without previous if");
            }
            else
            {
                // Check if next token is "if"
                if (i + 1 < tokens.size() &&
                    tokens[i + 1].category == "kw" &&
                    tokens[i + 1].text == "if")
                {
                    // else-if: keep the previous if available
                }
                else
                {
                    ifStack.pop();
                }
            }
        }

        // ---------------------------------
        // Duplicate tokens
        // ---------------------------------
        if (i > 0)
        {
            if (tokens[i].text == tokens[i - 1].text &&
                tokens[i].category != "par" &&
                tokens[i].category != "brc" &&
                tokens[i].category != "sep" &&
                tokens[i].category != "op" &&
                tokens[i].category != "line")
            {
                addError(line,
                         "Duplicate token: " + tokens[i].text);
            }
        }

        // ---------------------------------
        // for(;;)
        // ---------------------------------
        if (cat == "kw" && txt == "for")
        {
            if (i + 3 < tokens.size() &&
                tokens[i + 1].text == "(" &&
                tokens[i + 2].text == ";" &&
                tokens[i + 3].text == ";")
            {
                addError(line,
                         "Invalid for loop: double ';'");
            }
        }
    }

    // Remaining unmatched parentheses
    while (!parenStack.empty())
    {
        addError(parenStack.top(),
                 "Unmatched '('");
        parenStack.pop();
    }

    // Remaining unmatched braces
    while (!braceStack.empty())
    {
        addError(braceStack.top(),
                 "Unmatched '{'");
        braceStack.pop();
    }
}

void displayErrors()
{
    sortErrors();

    cout << endl;
    cout << "Syntax Errors:" << endl;

    if (errors.empty())
    {
        cout << "No syntax errors found." << endl;
        return;
    }

    for (SyntaxError e : errors)
    {
        cout << "Line " << e.line << ": "
             << e.message << endl;
    }
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
    cout << endl;
    for (int i = 0; i < token.length(); i++)
    {
        fputc(token[i], p2);
    }
    fclose(p2);

    // ---------------------------------------------------
    // ASSIGNMENT 3  symbol table + Step 1 / Step 4 streams
    // ---------------------------------------------------
    vector<TokenA3> tokensA3 = classifyTokensA3(token);

    SymbolTable symTab;
    string step1Stream, step4Stream;
    buildSymbolTableAndStreams(tokensA3, symTab, step1Stream, step4Stream);

    cout << endl;
    cout << "Symbol Table:" << endl;
    string symTabOutput = symTab.display();
    cout << symTabOutput;

    cout << endl;
    p2 = fopen("symtable.txt", "w");
    for (int i = 0; i < (int)symTabOutput.length(); i++)
    {
        fputc(symTabOutput[i], p2);
    }
    fclose(p2);

    // ---------------------------------------------------
    // ASSIGNMENT 4 : Syntax Error Detection
    // ---------------------------------------------------
    checkSyntaxErrors(tokensA3);
    displayErrors();

    symTab.free();

    return 0;
}
