#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include "serialize.hpp"
#include "sequence.hpp"
#include "exc.hpp"

using namespace markov;

std::string
mk_temp_filename()
{
    char path[PATH_MAX];

    strncpy(path, "/tmp/unit-test-03.data.XXXXXX", sizeof(path));
    int status = mkstemp(path);
    THROW_EXC_IF_FAILED(status != -1, "mkstemp() failed: %s", strerror(errno));

    return path;
}

void
test()
{
    Sequence seq1(4), seq2(4);

    seq1.add(3);
    seq1.add(7);
    seq1.add(17);
    seq1.add(11);

    std::string data_file = mk_temp_filename();

    save(seq1, data_file);
    load(seq2, data_file);

    THROW_EXC_IF_FAILED(seq1 == seq2, "unit test failed");
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
