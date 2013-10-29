#include <iostream>
#include <cstring>
#include <cerrno>
#include <cstdlib>

#include "model.hpp"

using namespace markov;

std::string data_file = "data/Chehov_Dama_S_Sobachkoy.txt";

void
test()
{
    Model model(1);

    std::ifstream stream(data_file.c_str(), std::ios::in);
    THROW_EXC_IF_FAILED(!stream.fail(), "couldn't open data file %s", data_file.c_str());

    std::string line, data;

    while (std::getline(stream, line)) {
        data.append(line);
        data.append("\n");
    }

    bool with_sanity_check = true;
    model.build(data, with_sanity_check);
}

int
main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;

    try {
        THROW_EXC_IF_FAILED(argc >= 2, "unit test failed: not enough args");
        data_file = std::string(argv[1]) + "/" + data_file;

        test();
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        status = EXIT_FAILURE;
    }

    return status;
}
