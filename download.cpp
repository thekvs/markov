#include <string>
#include <iostream>
#include <memory>
#include <cerrno>
#include <cstring>
#include <cstdlib>
#include <cstdio>

#include "exc.hpp"

namespace markov {

const std::string kContentFetcher = "curl -s -S";

class BufferDeleter {
public:

    void operator() (char *p) {
        free(p);
    }
};

class StreamDeleter {
public:

    void operator() (FILE *s) {
        pclose(s);
    }
};

std::string
download(std::string url)
{
    std::string content;
    std::string command = kContentFetcher + " " + url;

    FILE *stream = popen(command.c_str(), "r");
    THROW_EXC_IF_FAILED(stream != NULL, "popen() failed: %s", strerror(errno));

    char   *buffer = NULL;
    size_t  buffer_size;
    ssize_t bytes_read;

    while ((bytes_read = getline(&buffer, &buffer_size, stream)) > 0) {
        THROW_EXC_IF_FAILED(bytes_read != -1, "failed to read from the child process");
        content.append(buffer, bytes_read);
    }

    int status = pclose(stream);
    THROW_EXC_IF_FAILED(status != -1, "pclose() failed: %s", strerror(errno));

    free(buffer);

    return content;
}

} // namespace

using namespace markov;

int
main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " url" << std::endl;
        return EXIT_SUCCESS;
    }

    std::string data;
    std::string url = argv[1];

    try {
        data = download(url);
    } catch (std::exception &exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Downloaded " << data.size() << " bytes" << std::endl;

    return EXIT_SUCCESS;
}
