#ifndef __EXC_HPP_INCLUDED__
#define __EXC_HPP_INCLUDED__

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <exception>

namespace markov {

class Exc: public std::exception {
public:

    Exc(const char *file, int line, const char *fmt, ...)
    {
        va_list ap;
        char    buff[1024];

        va_start(ap, fmt);
        vsnprintf(buff, sizeof(buff), fmt, ap);
        va_end(ap);

        snprintf(msg, sizeof(msg), "%s:%i %s", sanitize_file_name(file), line, buff);
    }

    virtual ~Exc() throw() {
    };

    virtual const char *what() const throw() {
        return msg;
    }

private:

    char msg[1024];

    Exc() {}

    const char* sanitize_file_name(const char *file)
    {
        const char *s;
        char       *p;

        p = const_cast<char*>(strrchr(file, '/'));

        if (p == NULL) {
            s = file;
        } else {
            p++;
            s = p;
        }

        return s;
    }
};

#define THROW_EXC(args...)  (throw Exc(__FILE__, __LINE__, args))

#define THROW_EXC_IF_FAILED(status, args...) do {       \
    if (!(status)) {                                    \
        throw Exc(__FILE__, __LINE__, args);            \
    }                                                   \
} while(0)

} // namespace

#endif
