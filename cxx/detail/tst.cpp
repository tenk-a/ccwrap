//#include "string_view.hpp"
#include "string_view_test.cpp"
#include "string_util_test.cpp"

std::string_view    test = "test";

int main()
{
    try {
        string_view_test();
        string_util_test();
    } catch (...) {
        printf("unkown throw...\n");
    }
    return 0;
}
