#include <bits/stdc++.h>
using namespace std;

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

        // Handle whitespace
        if (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
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

    cout << endl;
    cout << "Sample : " << "output.txt" << endl;

    p2 = fopen("output.txt", "w");
    cout << ans;
    for (int i = 0; i < ans.length(); i++)
    {
        fputc(ans[i], p2);
    }

    fclose(p2);
    return 0;
}