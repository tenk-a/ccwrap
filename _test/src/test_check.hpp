#ifndef TEST_CHECK_HPP
#define TEST_CHECK_HPP

#ifndef TEST_CHECK
#define  TEST_CHECK(cc) do {                                                            \
        try {                                                                           \
            ((cc) || printf("%s (%d) : `%s' is failed.\n", __FILE__, __LINE__, #cc));   \
        } catch (...) {                                                                 \
            printf("%s (%d) : `%s' thew.\n", __FILE__, __LINE__, #cc);                  \
        }                                                                               \
    } while (0)
#define  TEST_EQ(a,b)                                                                   \
    do {                                                                                \
        try {                                                                           \
            if ((a) != (b)) {                                                           \
                std::cout << __FILE__ << " (" << __LINE__ << ") : "                     \
                  << #a << "(" << a << ") and " << #b << "(" << b << ") are not equal.\n";  \
            }                                                                           \
        } catch (...) {                                                                 \
            std::cout << __FILE__ << " (" << __LINE__ << ") : `"                        \
                << #a << "(" << a << ") == " << #b << "(" << b << ")' are threw.\n";    \
        }                                                                               \
    } while (0)

#define TEST_THROW(exp,E)   \
    do { bool f=0; (void)f; \
        try { exp; } catch (E) { f=1; } catch (...) {}  \
        if (!f) { printf("%s (%d): (%s) threw not %s.\n", __FILE__, __LINE__, #exp, #E); } \
    } while (0)
#define TEST_NO_THROW(exp)  \
    do { bool f=0; (void)f; \
        try { exp; } catch (...) {f=1;} \
        if (f) { printf("%s (%d): (%s) threw.\n", __FILE__, __LINE__, #exp); } \
    } while (0)

#define TEST_OUT_OF_RANGE(a)        TEST_THROW(a, std::out_of_range)
#endif

#endif
