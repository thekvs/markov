#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include "model.hpp"

using namespace markov;

std::string
mk_temp_filename()
{
    char path[PATH_MAX];

    strncpy(path, "/tmp/unit-test-05.data.XXXXXX", sizeof(path));
    int status = mkstemp(path);
    THROW_EXC_IF_FAILED(status != -1, "mkstemp() failed: %s", strerror(errno));

    return path;
}

void
test()
{
    Model model1(1), model2(1);

    std::vector<std::string> words = {"aaaa", "bbbb", "cccc", "dddd"};

    for (const auto &w: words) {
        model1.add_word(w);
    }

    model1.build();

    std::string data_file = mk_temp_filename();

    save(model1, data_file);
    load(model2, data_file);

    std::vector<std::string> start = {"aaaa"};
    std::vector<std::string> expected_gibberish = {"bbbb", "cccc", "dddd"};

    std::vector<std::string> gibberish1 = model1.generate(start, 3);
    std::vector<std::string> gibberish2 = model2.generate(start, 3);

    THROW_EXC_IF_FAILED(gibberish1 == gibberish2, "unit test failed");
    THROW_EXC_IF_FAILED(gibberish1 == expected_gibberish, "unit test failed");

    gibberish2 = model2.generate(start, 10);

    THROW_EXC_IF_FAILED(gibberish2 == expected_gibberish, "unit test failed");

    std::vector<std::string> unknown_start = {"unknown"};
    gibberish2 = model2.generate(unknown_start, 10);

    THROW_EXC_IF_FAILED(gibberish2.size() == 0, "unit test failed");
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
