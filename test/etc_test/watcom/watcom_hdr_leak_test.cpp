// Compiling this file IS the check: <algorithm> must not drag <locale> in.
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <string>

using namespace std;

int main() {
    string text = "Open WATCOM";
    transform(text.begin(), text.end(), text.begin(), ::tolower);
    if (text != "open watcom") {
        printf("hdr_leak: transform(::tolower) gave \"%s\"\n", text.c_str());
        return 1;
    }

    string up = "abc";
    transform(up.begin(), up.end(), up.begin(), ::toupper);
    if (up != "ABC") {
        printf("hdr_leak: transform(::toupper) gave \"%s\"\n", up.c_str());
        return 1;
    }

    printf("hdr_leak: ok\n");
    return 0;
}
