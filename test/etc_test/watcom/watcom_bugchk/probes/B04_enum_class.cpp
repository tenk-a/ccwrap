// bugchk-id:     B04
// bugchk-kind:   compile
// bugchk-expect: bug
// bugchk-desc:   no enum class (workaround: struct { enum {...} } -- see detail/system_error.hpp errc)
enum class Color { red, green, blue };
enum class Small : unsigned char { a, b };

int main() {
    Color c = Color::red;
    Small s = Small::b;
    return (int)c + (int)s - 1;
}
