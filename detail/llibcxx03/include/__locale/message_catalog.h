//===----------------------------------------------------------------------===//
// Part of the LLVM Project (Apache-2.0 WITH LLVM-exception).
//  ccwrap llibcxx03: the store behind std::messages (X/Open gencat source form).
//===----------------------------------------------------------------------===//
#ifndef _CCW_LIBCPP___LOCALE_MESSAGE_CATALOG_H
#define _CCW_LIBCPP___LOCALE_MESSAGE_CATALOG_H

#include "../__config"

#define _CCW_HAS_MESSAGE_CATALOG 1

#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cstring>

_CCW_LIBCPP_BEGIN_NAMESPACE_STD

#if defined(_CCW_OS_WIN) || defined(_CCW_OS_DOS) || defined(_WIN32) || defined(__DOS__)
#  define _CCW_NLSPATH_SEP ';'
#else
#  define _CCW_NLSPATH_SEP ':'
#endif

struct __ccw_msg_entry {
    int              __set_;
    int              __id_;
    _CCW_STD::string __text_;
};

class __ccw_msg_catalog {
public:
    _CCW_STD::vector<__ccw_msg_entry> __e_;

    _CCW_LIBCPP_HIDE_FROM_ABI const _CCW_STD::string* __find(int __set, int __id) const {
        for (_CCW_STD::size_t __i = 0; __i < __e_.size(); ++__i)
            if (__e_[__i].__set_ == __set && __e_[__i].__id_ == __id) return &__e_[__i].__text_;
        return 0;
    }

    _CCW_LIBCPP_HIDE_FROM_ABI static void __unescape(const _CCW_STD::string& __in_, _CCW_STD::string& __out_) {
        _CCW_STD::size_t __i = 0;
        __out_.erase();
        while (__i < __in_.size()) {
            char __c = __in_[__i++];
            if (__c != '\\' || __i >= __in_.size()) { __out_.push_back(__c); continue; }
            char __e = __in_[__i++];
            switch (__e) {
            case 'n': __out_.push_back('\n'); break;
            case 't': __out_.push_back('\t'); break;
            case 'v': __out_.push_back('\v'); break;
            case 'b': __out_.push_back('\b'); break;
            case 'r': __out_.push_back('\r'); break;
            case 'f': __out_.push_back('\f'); break;
            case '\\': __out_.push_back('\\'); break;
            default:
                if (__e >= '0' && __e <= '7') {
                    int __v = __e - '0', __k = 1;
                    while (__k < 3 && __i < __in_.size() && __in_[__i] >= '0' && __in_[__i] <= '7') {
                        __v = __v * 8 + (__in_[__i++] - '0'); ++__k;
                    }
                    __out_.push_back((char)(unsigned char)__v);
                } else {
                    __out_.push_back(__e);
                }
                break;
            }
        }
    }

    _CCW_LIBCPP_HIDE_FROM_ABI bool __load(const char* __path) {
        _CCW_STD::FILE* __f = _CCW_STD::fopen(__path, "rb");
        if (!__f) return false;

        _CCW_STD::string __line;
        _CCW_STD::string __text;
        int              __set   = 1;
        char             __quote = 0;
        bool             __cont  = false;
        int              __ch;

        __line.erase();
        for (;;) {
            __ch = _CCW_STD::fgetc(__f);
            if (__ch != EOF && __ch != '\n') {
                if (__ch != '\r') __line.push_back((char)__ch);
                continue;
            }
            if (!__line.empty() && __line[__line.size() - 1] == '\\') {
                __line.erase(__line.size() - 1);
                if (__ch == EOF) break;
                continue;
            }
            if (__cont) { }
            if (!__line.empty()) {
                const char* __p = __line.c_str();
                while (*__p == ' ' || *__p == '\t') ++__p;
                if (*__p == '$') {
                    ++__p;
                    if (_CCW_STD::strncmp(__p, "set", 3) == 0 && (__p[3] == ' ' || __p[3] == '\t')) {
                        __set = (int)_CCW_STD::strtol(__p + 3, 0, 10);
                    } else if (_CCW_STD::strncmp(__p, "quote", 5) == 0) {
                        const char* __q = __p + 5;
                        while (*__q == ' ' || *__q == '\t') ++__q;
                        __quote = *__q;
                    }
                } else if (*__p >= '0' && *__p <= '9') {
                    char* __end = 0;
                    long  __id  = _CCW_STD::strtol(__p, &__end, 10);
                    const char* __t = __end;
                    while (*__t == ' ' || *__t == '\t') ++__t;
                    _CCW_STD::string __raw(__t);
                    if (__quote && !__raw.empty() && __raw[0] == __quote) {
                        __raw.erase(0, 1);
                        if (!__raw.empty() && __raw[__raw.size() - 1] == __quote) __raw.erase(__raw.size() - 1);
                    }
                    __unescape(__raw, __text);
                    __e_.push_back(__ccw_msg_entry());
                    __e_[__e_.size() - 1].__set_  = __set;
                    __e_[__e_.size() - 1].__id_   = (int)__id;
                    __e_[__e_.size() - 1].__text_ = __text;
                }
            }
            __line.erase();
            if (__ch == EOF) break;
        }
        _CCW_STD::fclose(__f);
        return true;
    }
};

_CCW_LIBCPP_HIDE_FROM_ABI inline _CCW_STD::vector<__ccw_msg_catalog*>& __ccw_msg_slots() {
    static _CCW_STD::vector<__ccw_msg_catalog*> __v;
    return __v;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_msg_expand(const _CCW_STD::string& __tmpl, const _CCW_STD::string& __name,
                                                       const _CCW_STD::string& __loc, _CCW_STD::string& __out_) {
    _CCW_STD::string __lang, __terr, __code;
    _CCW_STD::size_t __i;
    _CCW_STD::size_t __u = __loc.find('_');
    _CCW_STD::size_t __d = __loc.find('.');
    __lang = __loc.substr(0, __u != _CCW_STD::string::npos ? __u
                             : (__d != _CCW_STD::string::npos ? __d : __loc.size()));
    if (__u != _CCW_STD::string::npos)
        __terr = __loc.substr(__u + 1, (__d != _CCW_STD::string::npos ? __d - __u - 1 : __loc.size() - __u - 1));
    if (__d != _CCW_STD::string::npos) __code = __loc.substr(__d + 1);

    __out_.erase();
    for (__i = 0; __i < __tmpl.size(); ++__i) {
        if (__tmpl[__i] != '%' || __i + 1 >= __tmpl.size()) { __out_.push_back(__tmpl[__i]); continue; }
        char __c = __tmpl[++__i];
        switch (__c) {
        case 'N': __out_ += __name; break;
        case 'L': __out_ += __loc;  break;
        case 'l': __out_ += __lang; break;
        case 't': __out_ += __terr; break;
        case 'c': __out_ += __code; break;
        case '%': __out_.push_back('%'); break;
        default:  __out_.push_back('%'); __out_.push_back(__c); break;
        }
    }
}

_CCW_LIBCPP_HIDE_FROM_ABI inline bool __ccw_msg_is_path(const _CCW_STD::string& __n) {
    return __n.find('/') != _CCW_STD::string::npos || __n.find('\\') != _CCW_STD::string::npos;
}

_CCW_LIBCPP_HIDE_FROM_ABI inline long __ccw_msg_open(const _CCW_STD::string& __name, const _CCW_STD::string& __loc) {
    __ccw_msg_catalog* __cat = new __ccw_msg_catalog();
    bool               __ok  = false;
    _CCW_STD::string   __path;

    if (__name.empty()) { delete __cat; return -1; }

    if (__ccw_msg_is_path(__name)) {
        __ok = __cat->__load(__name.c_str());
    } else {
        const char* __np = _CCW_STD::getenv("NLSPATH");
        if (__np) {
            _CCW_STD::string __all(__np);
            _CCW_STD::size_t __b = 0;
            while (__b <= __all.size() && !__ok) {
                _CCW_STD::size_t __e = __all.find(_CCW_NLSPATH_SEP, __b);
                if (__e == _CCW_STD::string::npos) __e = __all.size();
                if (__e > __b) {
                    __ccw_msg_expand(__all.substr(__b, __e - __b), __name, __loc, __path);
                    __ok = __cat->__load(__path.c_str());
                }
                __b = __e + 1;
            }
        }
        if (!__ok) __ok = __cat->__load(__name.c_str());
        if (!__ok) { __path = __name; __path += ".cat"; __ok = __cat->__load(__path.c_str()); }
        if (!__ok) { __path = __name; __path += ".msg"; __ok = __cat->__load(__path.c_str()); }
    }
    if (!__ok) { delete __cat; return -1; }

    _CCW_STD::vector<__ccw_msg_catalog*>& __s = __ccw_msg_slots();
    for (_CCW_STD::size_t __i = 0; __i < __s.size(); ++__i)
        if (__s[__i] == 0) { __s[__i] = __cat; return (long)__i; }
    __s.push_back(__cat);
    return (long)(__s.size() - 1);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline const _CCW_STD::string* __ccw_msg_get(long __c, int __set, int __id) {
    _CCW_STD::vector<__ccw_msg_catalog*>& __s = __ccw_msg_slots();
    if (__c < 0 || (_CCW_STD::size_t)__c >= __s.size() || __s[(_CCW_STD::size_t)__c] == 0) return 0;
    return __s[(_CCW_STD::size_t)__c]->__find(__set, __id);
}

_CCW_LIBCPP_HIDE_FROM_ABI inline void __ccw_msg_close(long __c) {
    _CCW_STD::vector<__ccw_msg_catalog*>& __s = __ccw_msg_slots();
    if (__c < 0 || (_CCW_STD::size_t)__c >= __s.size()) return;
    delete __s[(_CCW_STD::size_t)__c];
    __s[(_CCW_STD::size_t)__c] = 0;
}

_CCW_LIBCPP_END_NAMESPACE_STD

#endif // _CCW_LIBCPP___LOCALE_MESSAGE_CATALOG_H
