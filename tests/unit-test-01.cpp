#include <iostream>

#include "sequence.hpp"
#include "exc.hpp"

using namespace markov;

void
test()
{
    Sequence seq(4);
    THROW_EXC_IF_FAILED(seq.complete() == false, "unit test failed");

    seq.add(3);
    THROW_EXC_IF_FAILED(seq.complete() == false, "unit test failed");

    seq.add(7);
    seq.add(17);
    seq.add(11);

    std::vector<uint32_t> expected1 = {3, 7, 17, 11};

    THROW_EXC_IF_FAILED(seq.complete() == true, "unit test failed");
    THROW_EXC_IF_FAILED(seq.data == expected1, "unit test failed");

    seq.add(15);

    std::vector<uint32_t> expected2 = {7, 17, 11, 15};

    THROW_EXC_IF_FAILED(seq.complete() == true, "unit test failed");
    THROW_EXC_IF_FAILED(seq.data == expected2, "unit test failed");

    SequenceMap map;
    map[seq] = 1;
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
