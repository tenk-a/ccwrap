// bugchk-id:     B05
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no lambdas (workaround: named function objects)
int main() {
    int base = 1;
    int (*f)(int) = [](int x) { return x; };
    int r = f(0);
    int captured = [base](int x) { return x + base; }(1);
    return r + captured - 2;
}
