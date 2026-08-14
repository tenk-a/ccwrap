// bugchk-id:     A16
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no type-trait intrinsics (__is_enum/__is_class/__is_base_of/__underlying_type ...)
// Without these and without SFINAE (B01) there is no way to implement is_enum / is_class /
// is_union / is_empty / is_polymorphic / is_abstract / is_base_of / underlying_type.
enum E { e0 };
struct C { int x; };
struct D : C {};
union U { int x; };

int main() {
    int r = 0;
    r += __is_enum(E);
    r += __is_class(C);
    r += __is_union(U);
    r += __is_empty(C);
    r += __is_polymorphic(C);
    r += __is_base_of(C, D);
    typedef __underlying_type(E) UT;
    r += (int)(UT)0;
    return r == 5 ? 0 : 1;
}
