// bugchk-id:     A18
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no __builtin_LINE/__builtin_FILE -- source_location::current() cannot capture its call site
struct source_location {
    int line_;
    const char* file_;
    static source_location current(int l = __builtin_LINE(), const char* f = __builtin_FILE()) {
        source_location s; s.line_ = l; s.file_ = f; return s;
    }
};

int main() {
    source_location s = source_location::current();
    return (s.line_ > 0 && s.file_ && s.file_[0]) ? 0 : 1;
}
