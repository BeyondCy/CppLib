#ifndef STRING_HELPER_H_INCLUDED
#define STRING_HELPER_H_INCLUDED

#include <string>
#include <vector>
#include <algorithm>
#include <stdlib.h>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include <mutex>
#include <set>
#include <map>

#define HEX_STR_TO_NUM(c) (((0X40&(c))>>6)*9+(0X0F&(c))) //you must make sure the c is hex str

#if defined(_MSC_VER)
#ifndef THROW
#define THROW(x) throw(...)
#endif // !THROW
#elif defined(__GNUC__)
#include <string.h>
#ifndef THROW
#define THROW(x) throw(x)
#endif // !THROW
#else
#error unsupported compiler
#endif

#define CP_UTF16LE 1200
#define CP_UTF16BE 1201
#define CP_UTF32LE 12000
#define CP_UTF32BE 12001

class StringHelper
{
public:
    enum io_base
    {
        hex = 0,
        oct = 1,
        dec = 2,
    };

    // TEMPLATE CLASS is_signed
    template<class _Ty1>
    struct _is_signed
        : std::false_type
    {	// determine whether _Ty1 is signed type
    };

    // TEMPLATE CLASS is_signed
    template<class _Ty1>
    struct is_signed : _is_signed<typename std::remove_cv<_Ty1>::type>
    {

    };
        
public:
    /**
    *remove the blank at both end of the string
    *s(in): the string that you want to rm the blank
    */
    static std::string& trim(std::string &s);
    /**
    *remove the blank at both end of the string
    *s(in): the string that you want to rm the blank
    */
    static std::string trim(const std::string &s);
    /**
    *remove the blank at right end of the string
    *s(in): the string that you want to rm the blank at right end
    */
    static std::string& rtrim(std::string &s);
    /**
    *remove the blank at right end of the string
    *s(in): the string that you want to rm the blank at right end
    */
    static std::string rtrim(const std::string &s);
    /**
    *remove the blank at left end of the string
    *s(in): the string that you want to rm the blank at left end
    */
    static std::string& ltrim(std::string &s);
    /**
    *remove the blank at left end of the string
    *s(in): the string that you want to rm the blank at left end
    */
    static std::string ltrim(const std::string &s);
    /**
    *split the string with the delim
    *s(in): the string you want to split with the delim
    *delim(in): the delim that you want to use to split the string
    */
    static std::vector<std::string> split(const std::string& s, const std::string& delim);
    /**
    *connect the ele in vec with the connector
    *vec(in): the set you want deal to join them together
    *connector(in), the connector that you want to use to join the set
    */
    template<typename T>
    static std::string join(const std::vector<T>& vec, const std::string &connector)
    {
        std::string result;
        if (vec.size() == 0)
        {
            return result;
        }

        auto cur = vec.begin();
        auto next = cur + 1;
        for (; next != vec.end(); cur = next, ++next)
        {
            result += *cur + connector;
        }
        result += *cur;
        return result;
    }

    /**
    *replace the substr in the str with dest, if you input src is empty, it will replace ervery character with the dest
    *str(in): the str that you want to replace substr
    *src(in): the substr that you want to replace
    *dest(in): the substr that you want to replace with
    */
    static std::string replace(const std::string& str, const std::string& src, const std::string& dest);

    /**
    *upcase the string
    *str(in): the string that you want to deal
    */
    static std::string& toupper(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](const char c) { return static_cast<char>(::toupper(c)); });
        return str;
    }

    /**
    *upcase the string
    *str(in): the string that you want to deal
    */
    static std::string toupper(const std::string &str)
    {
        std::string result = str;
        std::transform(str.begin(), str.end(), result.begin(), [](const char c) { return static_cast<char>(::toupper(c)); });
        return result;
    }
    /**
    *lowcase the string
    *str(in): the string that you want to deal
    */
    static std::string& tolower(std::string &str)
    {
        std::transform(str.begin(), str.end(), str.begin(), [](const char c) { return static_cast<char>(::tolower(c)); });
        return str;
    }
    
    /**
    *lowcase the string
    *str(in): the string that you want to deal
    */
    static std::string tolower(const std::string &str)
    {
        std::string result = str;
        std::transform(str.begin(), str.end(), result.begin(), [](const char c) { return static_cast<char>(::tolower(c)); });
        return result;
    }
    /**
    *get default user locale
    */
    static std::string defaultlocale();
    /**
    *change multi char to wchar by using charset, like HTTP resp
    *str(in): the multi need to convert
    *charset(in): the charset of the str in http header, like"Content-Type: text/html; charset=utf-8", if empty use default locale 
    */
    static std::wstring towcharbycharset(const std::string &str, const std::string &charset = "UTF-8");
    /**
    *change multi char to wchar, using specify charactor set to convert, it is strict
    *str(in): the multi need to convert
    *locale(in): the language of the string��use it to decode, chinese is windows"zh_CN" linux"zh_CN.UTF-8", english is "C", use NULL for user locale
    */
    static std::wstring towchar(const std::string &str, const char *locale);
    /**
    *change wchar to multi char , using specify charactor set to convert, it is strict
    *wstr(in): the multi need to convert
    *locale(in): the language of the string��use it to encode, chinese is windows"zh_CN" linux"zh_CN.UTF-8", english is "C", use NULL for user locale
    */
    static std::string tochar(const std::wstring &wstr, const char *locale);
    /**
    *change multi char to wchar, using specify charactor set to convert, it is strict
    *str(in): the multi need to convert
    */
    static std::wstring towchar(const std::string &str);
    /**
    *change wchar to multi char , using specify charactor set to convert, it is strict
    *wstr(in): the multi need to convert
    */
    static std::string tochar(const std::wstring &wstr);
    /**
    *convert the utf8 string to wchar
    */
    static std::wstring utf8towchar(const std::string &str);
    /**
    *convert the wchar string to utf8
    */
    static std::string wchartoutf8(const std::wstring &wstr);
    /**
    *convert the utf7 string to wchar
    */
    static std::wstring utf7towchar(const std::string &str);
    /**
    *convert the wchar string to utf7
    */
    static std::string wchartoutf7(const std::wstring &wstr);
    /**
    *convert the utf16le string to wchar
    */
    static std::wstring utf16letowchar(const std::string &str);
    /**
    *convert the wchar string to utf16le
    */
    static std::string wchartoutf16le(const std::wstring &wstr);
    /**
    *convert the utf16be string to wchar
    */
    static std::wstring utf16betowchar(const std::string &str);
    /**
    *convert the wchar string to utf16be
    */
    static std::string wchartoutf16be(const std::wstring &wstr);
    /**
    *convert the utf32le string to wchar
    */
    static std::wstring utf32letowchar(const std::string &str);
    /**
    *convert the wchar string to utf32le
    */
    static std::string wchartoutf32le(const std::wstring &wstr);
    /**
    *convert the utf32be string to wchar
    */
    static std::wstring utf32betowchar(const std::string &str);
    /**
    *convert the wchar string to utf32be
    */
    static std::string wchartoutf32be(const std::wstring &wstr);

    /**
    *convert string to other type
    *Target: the type you want to convert to
    *Source: the type you want to be converted, it can be every type that can use operator <<
    *src: the source to be converted
    */
    template<typename Target, typename Source>
    static Target convert(const Source src) THROW(std::bad_cast)
    {
        if (std::is_same<typename std::remove_cv<Target>::type, char>::value)
        {
            short ret = _lexical_cast<short, typename std::remove_cv<Source>::type
                , std::is_same<char, typename std::remove_cv<Source>::type>::value>::cast(src);
            if (ret > 127 || ret < -128) throw std::bad_cast();
            return *(Target*)&ret;
        }
        else if (std::is_same<typename std::remove_cv<Target>::type, unsigned char>::value)
        {
            unsigned short ret = _lexical_cast<unsigned short, typename std::remove_cv<Source>::type
                , std::is_same<unsigned char, typename std::remove_cv<Source>::type>::value>::cast(src);
            if (ret > 255) throw std::bad_cast();
            return *(Target*)&ret;
        }
        else
        {
            return _lexical_cast<typename std::remove_cv<Target>::type, typename std::remove_cv<Source>::type
                , std::is_same<typename std::remove_cv<Target>::type, typename std::remove_cv<Source>::type>::value>::cast(src);
        }
    }
    /**
    *convert the hex string to its mean byte, like string "12" to byte 18(1*16+2)
    *hex(in): the hex string need to convert, can not have 0x prefix, should only have hex string
    *out(out): the byte of the hex string
    *out_size(in): the size of out
    */
    static bool hex2byte(const std::string& hex, char *out, const size_t &out_size);
    /**
    *convert the byte array to base(hex oct dec) string
    *byte(in): the byte need to convert
    *byte_size(in): the num of the byte
    *delim(in): the delim between two bytes, like if byte 18 19 to hex string, delim ":", then hex string is "12:13"
    *io_base(in): the base you want to choose to use, see io_base
    *width(in): the width of the base string
    *fill(in): if the width is not enough, then will use the fill char to fill
    *upcase(in): the hex string should be upcase
    */
    static std::string byte2basestr(const unsigned char* byte, const size_t &byte_size, const std::string &delim = std::string(), const io_base base = hex, const size_t width = 0,const char fill = '0', bool upcase = true);
    /**
    *get a stable string copy of the input, the return string will not loss all the program life.
    *s(in): string input format, like "%s"
    *args(in): args for s
    *return a ptr to the string which is equal to the input
    */
    template<typename ... Types>
    static const char* getstaticstring(const char *s, Types... args)
    {
        static std::set<std::string> pool;
        static std::mutex pool_lock;
        char buf[4096];
        if (s == NULL) {
            return NULL;
        }
        snprintf(buf, sizeof(buf), s, args...);
        buf[sizeof(buf) - 1] = 0;
        std::unique_lock<std::mutex> lock(pool_lock);
        std::pair<std::set<std::string>::iterator, bool> pair = pool.insert(buf);
        return pair.first->c_str();
    }
    /**
    *get a stable string copy of the input, the return string will not loss all the program life.
    *s(in): string input
    *return a ptr to the string which is equal to the input
    */
    static const char* getstaticstring(const std::string &s)
    {
        return getstaticstring(s.c_str());
    }

    /**
    *change the unable print string to \xFF format
    *s(in): string need to change
    *return print able string
    */
    static std::string escapeunableprint(const std::string &s)
    {
        std::string r;

        for (unsigned int i = 0; i < s.size(); i++) {
            char buf[5];
            unsigned char c = s[i];
            // Printable and some whitespace ok. "\r" not ok because it overwrites the line.
            if (c == '\t' || c == '\n' || (0x20 <= c && c <= 0x7e)) {
                r += c;
            }
            else {
                snprintf(buf, sizeof(buf), "\\x%02X", c);
                r += buf;
            }
        }
        return r;
    }

#if defined(_MSC_VER)
    /**
    *change multi byte str to wchar str
    *str(in): multi byte str
    *code_page(in): using code page, like CP_ACP
    */
    static std::wstring transbytestowchar(const std::string &str, unsigned int code_page);
    /**
    *change wchar str to multi byte str
    *wstr(in): wchar str
    *code_page(in): using code page, like CP_ACP
    */
    static std::string wchartotransbytes(const std::wstring &wstr, unsigned int code_page);
#elif defined(__GNUC__)
    /**
    *change multi byte str to wchar str
    *str(in): multi byte str
    *character(in): character of str, like UTF-8
    */
    static std::wstring transbytestowchar(const std::string &str, const std::string &character);
    /**
    *change wchar str to multi byte str
    *wstr(in): wchar str
    *character(in): character of dst str, like UTF-8
    */
    static std::string wchartotransbytes(const std::wstring &wstr, const std::string &character);
    /**
    *change multi byte str to multi byte str
    *str(in): multi byte str
    *f_character(in): character of str, like zh_CN
    *t_character(in): character of dst str, like UTF-8
    */
    static std::string transbytestotransbytes(const std::string &str, const std::string &f_character, const std::string &t_character);
#else
#error unsupported compiler
#endif

private:
    template<class _Elem,class _Traits>
    static std::basic_ostream<_Elem, _Traits>& changebase(std::basic_ostream<_Elem, _Traits>& ostream, const io_base base)
    {
        switch (base)
        {
        case hex:
            ostream << std::hex;
            return ostream;
        case oct:
            ostream << std::oct;
            return ostream;
        case dec:
            ostream << std::dec;
            return ostream;
        default:
            ostream << std::dec;
            return ostream;
        }
    }

    template<class _Elem, class _Traits>
    static std::basic_ostream<_Elem, _Traits>& changewidth(std::basic_ostream<_Elem, _Traits>& ostream, const size_t width)
    {
        ostream << std::setw(width);
        return ostream;
    }

    template<class _Elem, class _Traits>
    static std::basic_ostream<_Elem, _Traits>& setfill(std::basic_ostream<_Elem, _Traits>& ostream, const char fill)
    {
        ostream << std::setfill(fill);
        return ostream;
    }

    template <typename Target, typename Source, bool Same>
    class _lexical_cast
    {
    public:
        static Target cast(const Source &arg) THROW(std::bad_cast)
        {
            Target ret;
            std::stringstream ss;
            ss << arg;
            if (std::is_integral<Target>::value && !is_signed<Target>::value && ss.str().size() > 1 && ss.str()[0] == '-') throw std::bad_cast();
            if (!(ss >> ret && ss.eof() && !ss.fail())) throw std::bad_cast();
            return ret;
        }
    };

private:
    static std::map<std::string, std::string> html_charset_to_locale_map;
    static std::mutex char_wchar_lock;
};

template<>
struct StringHelper::_is_signed<bool>
    : std::false_type
{	// determine whether _Ty1 is signed type
};

template<>
struct StringHelper::_is_signed<char>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<unsigned char>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<wchar_t>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<signed short>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<unsigned short>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<char16_t>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<signed int>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<unsigned int>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<char32_t>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<signed long>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<unsigned long>
    : std::false_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<long long>
    : std::true_type
{	// determine whether _Ty is signed type
};

template<>
struct StringHelper::_is_signed<unsigned long long>
    : std::false_type
{	// determine whether _Ty is signed type
};

template <typename Target, typename Source>
class StringHelper::_lexical_cast<Target, Source, true>
{
public:
    static Target cast(const Source &arg)
    {
        return arg;
    }
};

template <typename Source>
class StringHelper::_lexical_cast<std::string, Source, false>
{
public:
    static std::string cast(const Source &arg)
    {
        std::ostringstream ss;
        ss << arg;
        return ss.str();
    }
};

template <typename Target>
class StringHelper::_lexical_cast<Target, std::string, false>
{
public:
    static Target cast(const std::string &arg) THROW(std::bad_cast)
    {
        Target ret;
        std::istringstream ss(arg);
        if (std::is_integral<Target>::value && !is_signed<Target>::value && ss.str().size() > 1 && ss.str()[0] == '-') throw std::bad_cast();
        if (!(ss >> ret && ss.eof() && !ss.fail())) throw std::bad_cast();
        return ret;
    }
};

#endif
