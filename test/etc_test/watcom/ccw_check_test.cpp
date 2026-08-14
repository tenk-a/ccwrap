// Corrupts a string / tree / vector the way A1'' does. Aborting is the pass.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <set>
#include <string>
#include <vector>

int main(int argc, char** argv) {
    int what = (argc > 1) ? std::atoi(argv[1]) : 0;

    if (what == 1) {
        std::string s = "0123456789abcdefghij";
        unsigned char* raw = (unsigned char*)(void*)&s;
        raw[0] = 0x01;
        std::printf("string: no check fired\n");
        return 0;
    }
    if (what == 2) {
        std::set<int> t;
        for (int i = 0; i < 8; ++i) t.insert(i);
        void** raw = (void**)(void*)&t;
        void** root = (void**)raw[1];
        if (!root) { std::printf("tree: layout changed\n"); return 0; }
        root[1] = (void*)root;
        (void)(t.find(-99) != t.end());
        std::printf("tree: no check fired\n");
        return 0;
    }
    if (what == 3) {
        std::vector<std::string> v;
        v.push_back("aaaaaaaaaaaaaaaaaaaaaa");
        void** raw = (void**)(void*)&v;
        void* t = raw[1]; raw[1] = raw[2]; raw[2] = t;
        v.clear();
        std::printf("vector: no check fired\n");
        return 0;
    }
    std::printf("usage: ccw_check_test 1|2|3\n");
    return 0;
}
