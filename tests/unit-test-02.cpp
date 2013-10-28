#include <iostream>

#include "numerator.hpp"
#include "exc.hpp"

using namespace markov;

void
test()
{
    Numerator numerator;

    std::vector<std::string> strings = {"aaaa", "bbbb", "cccc", "dddd", "aaaa"};
    std::vector<uint32_t>    expected = {1, 2, 3, 4, 1};
    std::vector<uint32_t>    result;

    for (const auto &string: strings) {
        result.push_back(numerator.enumerate(string));
    }

    THROW_EXC_IF_FAILED(result == expected, "unit test failed");

    std::vector<std::string> denumerated;

    for (auto id: result) {
        denumerated.push_back(numerator.denumerate(id));
    }

    THROW_EXC_IF_FAILED(denumerated == strings, "unit test failed");
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
