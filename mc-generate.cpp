#include <getopt.h>

#include <boost/lexical_cast.hpp>

#include "model.hpp"
#include "tokenizer.hpp"
#include "serialize.hpp"

using namespace markov;

static struct option longopts[] = {
    { "model",      required_argument,  NULL,   'm' },
    { "seed",       required_argument,  NULL,   's' },
    { "seed-file",  required_argument,  NULL,   'S' },
    { "count",      required_argument,  NULL,   'c' },
    { "help",       no_argument,        NULL,   'h' },
    { NULL,         0,                  NULL,   0   }
};

struct Args {
    std::string model;
    std::string seed;
    std::string seed_file;
    size_t      count;

    Args(): count(0) {}
};

void
help(const char *program)
{
    std::cout << "Usage: " << program << " options" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h,--help           write this help message and exit" << std::endl;
    std::cout << "  -m,--model arg      file where to store compiled model" << std::endl;
    std::cout << "  -s,--seed arg       start sequence" << std::endl;
    std::cout << "  -S,--seed-file arg  read start sequence(s) (one per line) from a given file" << std::endl;
    std::cout << "  -c,--count arg      number of words to generate (without start sequence)" << std::endl;
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}

Args
parse_args(int argc, char **argv)
{
    if (argc < 4) {
        help(argv[0]);
    }

    int  opt, optidx;
    Args args;

    while ((opt = getopt_long(argc, argv, "m:s:S:c:h", longopts, &optidx)) != -1) {
        switch (opt) {
            case 'm':
                args.model = optarg;
                break;
            case 's':
                args.seed = optarg;
                break;
            case 'S':
                args.seed_file = optarg;
                break;
            case 'c':
                args.count = boost::lexical_cast<size_t>(optarg);
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
    THROW_EXC_IF_FAILED((!args.seed.empty()) || (!args.seed_file.empty()),
        "at least one start sequence source must be specified");
    THROW_EXC_IF_FAILED(args.count > 0, "number of words to generate must be positive integer");

    return args;
}

void
generate(Model &model, const std::string &seed, size_t count)
{
    auto start_sequence = tokenize(seed, kTextSeparators);
    THROW_EXC_IF_FAILED(start_sequence.size() == model.order(),
        "number of words in the seed sequence (=%zu) must coinside with model's dimention (=%zu)",
        start_sequence.size(), model.order());

    auto gibberish = model.generate(start_sequence, count);
    
    if (gibberish.size()) {
        for (const auto &word: start_sequence) {
            std::cout << word << " ";
        }
        for (const auto &word: gibberish) {
            std::cout << word << " ";
        }
    }

    std::cout << std::endl;    
}

void
run(int argc, char **argv)
{
    auto args = parse_args(argc, argv);

    Model model;

    load(model, args.model);

    if (!args.seed.empty()) {
        generate(model, args.seed, args.count);
    }

    if (!args.seed_file.empty()) {
        std::ifstream stream(args.seed_file.c_str(), std::ios::in);
        THROW_EXC_IF_FAILED(!stream.fail(), "couldn't open file %s", args.seed_file.c_str());

        std::string line;

        while (std::getline(stream, line)) {
            generate(model, line, args.count);
        }
    }
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
