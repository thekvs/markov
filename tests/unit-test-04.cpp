#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include "serialize.hpp"
#include "numerator.hpp"
#include "exc.hpp"

using namespace markov;

std::string
mk_temp_filename()
{
    char path[PATH_MAX];

    strncpy(path, "/tmp/unit-test-04.data.XXXXXX", sizeof(path));
    int status = mkstemp(path);
    THROW_EXC_IF_FAILED(status != -1, "mkstemp() failed: %s", strerror(errno));

    return path;
}

void
test()
{
    Numerator numerator1, numerator2;

    std::vector<std::string> data = {"aaaa", "bbbb", "cccc", "dddd"};
    std::vector<uint32_t>    expected = {1, 2, 3, 4};
    std::vector<uint32_t>    id;

    for (const auto &w: data) {
        id.push_back(numerator1.enumerate(w));
    }

    THROW_EXC_IF_FAILED(id == expected, "unit test failed");

    std::string data_file = mk_temp_filename();

    save(numerator1, data_file);
    load(numerator2, data_file);

    id.clear();

    for (const auto &w: data) {
        id.push_back(numerator2.enumerate(w));
    }

    THROW_EXC_IF_FAILED(id == expected, "unit test failed");
    THROW_EXC_IF_FAILED(numerator2.enumerate("eeee") == 5, "unit test failed");
}

int
main()
{
    int status = EXIT_SUCCESS;

    try {
        test();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        status = EXIT_FAILURE;
    }

    return status;
}
