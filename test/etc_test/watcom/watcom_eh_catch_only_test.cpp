// C10: this TU must only CATCH. A throw would odr-use the constructor and hide it.
#include <exception>
#include <new>
#include <stdexcept>
#include <system_error>
#include <optional>
#include <variant>
#include <functional>
#include <memory>
#include <typeinfo>
#include <any>
#include <cstdio>

int ccw_eh_no_throw(int n);

int main()
{
    int seen = 0;
    try {
        seen = ccw_eh_no_throw(1);
    }
    catch (std::bad_array_new_length&) { seen = -1; }
    catch (std::bad_alloc&)            { seen = -1; }
    catch (std::bad_exception&)        { seen = -1; }
    catch (std::bad_any_cast&)         { seen = -1; }
    catch (std::bad_cast&)             { seen = -1; }
    catch (std::bad_typeid&)           { seen = -1; }
    catch (std::bad_optional_access&)  { seen = -1; }
    catch (std::bad_variant_access&)   { seen = -1; }
    catch (std::bad_function_call&)    { seen = -1; }
    catch (std::bad_weak_ptr&)         { seen = -1; }
    catch (std::domain_error&)         { seen = -1; }
    catch (std::invalid_argument&)     { seen = -1; }
    catch (std::length_error&)         { seen = -1; }
    catch (std::out_of_range&)         { seen = -1; }
    catch (std::logic_error&)          { seen = -1; }
    catch (std::range_error&)          { seen = -1; }
    catch (std::overflow_error&)       { seen = -1; }
    catch (std::underflow_error&)      { seen = -1; }
    catch (std::system_error&)         { seen = -1; }
    catch (std::runtime_error&)        { seen = -1; }
    catch (std::exception&)            { seen = -1; }

    if (seen != 1) {
        std::printf("watcom_eh_catch_only: FAILED (seen=%d)\n", seen);
        return 1;
    }
    std::printf("watcom_eh_catch_only: ok\n");
    return 0;
}
