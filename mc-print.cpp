#include <getopt.h>

#include <boost/lexical_cast.hpp>

#include "model.hpp"
#include "tokenizer.hpp"
#include "serialize.hpp"

using namespace markov;

static struct option longopts[] = {
    { "model",      required_argument,  NULL,   'm' },
    { "help",       no_argument,        NULL,   'h' },
    { NULL,         0,                  NULL,   0   }
};

struct Args {
    std::string model;
};

void
help(const char *program)
{
    std::cout << "Usage: " << program << " options" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h,--help           write this help message and exit" << std::endl;
    std::cout << "  -m,--model arg      file with compiled model" << std::endl;
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}

Args
parse_args(int argc, char **argv)
{
    if (argc < 2) {
        help(argv[0]);
    }

    int  opt, optidx;
    Args args;

    while ((opt = getopt_long(argc, argv, "m:h", longopts, &optidx)) != -1) {
        switch (opt) {
            case 'm':
                args.model = optarg;
                break;
            case 'h':
                help(argv[0]);
                break;
            default:
                std::cout << "for help message run with --help argument." << std::endl;
                exit(EXIT_FAILURE);
        }
    }

    THROW_EXC_IF_FAILED(!args.model.empty(), "file with model's data must be specified");

    return args;
}

void
run(int argc, char **argv)
{
    Args args = parse_args(argc, argv);

    Model model;

    load(model, args.model);
    model.print();
}

int
main(int argc, char **argv)
{
    int status = EXIT_SUCCESS;

    try {
        run(argc, argv);
    } catch (std::exception &exc) {
        std::cerr << "Error: " << exc.what() << std::endl;
        status = EXIT_FAILURE;
    }

    return status;
}
