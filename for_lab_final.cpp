#include <bits/stdc++.h>
using namespace std;

int main()
{

    FILE *file = fopen("input.c", "r");
    if (file == NULL)
    {
        cout << "File Not Found" << endl;
        return 0;
    }
    string str;
    char c;
    while ((c = fgetc(file)) != EOF)
        str += c;

    fclose(file);

    return 0;
}