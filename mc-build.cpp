#include <getopt.h>

#include <boost/lexical_cast.hpp>

#include "model.hpp"
#include "tokenizer.hpp"
#include "serialize.hpp"

using namespace markov;

static struct option longopts[] = {
    { "urls",       required_argument,  NULL,   'u' },
    { "files",      required_argument,  NULL,   'f' },
    { "dimension",  required_argument,  NULL,   'd' },
    { "model",      required_argument,  NULL,   'm' },
    { "help",       no_argument,        NULL,   'h' },
    { NULL,         0,                  NULL,   0   }
};

struct Args {
    std::string urls;
    std::string files;
    std::string model;
    size_t      dimension;

    Args(): dimension(0) {}
};

const std::string kContentFetcher = "curl -s -S --compress ";

void
help(const char *program)
{
    std::cout << "Usage: " << program << " options" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -h,--help           write this help message and exit" << std::endl;
    std::cout << "  -u,--urls args      comma separated list of urls to fetch data for a model" << std::endl;
    std::cout << "  -f,--files args     comma separated list of files with data for a model" << std::endl;
    std::cout << "  -d,--dimension arg  model's dimension" << std::endl;
    std::cout << "  -m,--model arg      file where to store compiled model" << std::endl;
    std::cout << std::endl;

    exit(EXIT_SUCCESS);
}

Args
parse_args(int argc, char **argv)
{
    if (argc < 5) {
        help(argv[0]);
    }

    int  opt, optidx;
    Args args;

    while ((opt = getopt_long(argc, argv, "u:f:d:m:h", longopts, &optidx)) != -1) {
        switch (opt) {
            case 'u':
                args.urls = optarg;
                break;
            case 'f':
                args.files = optarg;
                break;
            case 'd':
                args.dimension = boost::lexical_cast<size_t>(optarg);
                break;
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

    THROW_EXC_IF_FAILED(args.dimension > 0, "model's dimension must be positive");
    THROW_EXC_IF_FAILED((!args.urls.empty()) || (!args.files.empty()),
        "at least one parameter (--urls or --files) must be specified");
    THROW_EXC_IF_FAILED(!args.model.empty(), "output file with model's data must be specified");

    return args;
}

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
        // Yeah, I know there is a potential resource leak, but the best thing
        // we can do in such situation is to die quickly (and dirty).
        THROW_EXC_IF_FAILED(bytes_read != -1, "failed to read from the child process");
        content.append(buffer, bytes_read);
    }

    free(buffer);

    int status = pclose(stream);
    THROW_EXC_IF_FAILED(status != -1, "pclose() failed: %s", strerror(errno));

    return content;
}

void
build_model(const std::string &data, Model &model)
{
    Separator separator(kTextSeparators.c_str());
    Tokenizer tokenizer(data, separator);

    boost::locale::generator gen;
    std::locale locale = gen("ru_RU.UTF-8");

    for (const auto &token: tokenizer) {
        model.add_word(boost::locale::to_lower(token, locale));
    }

    model.build();
}

void
run(int argc, char **argv)
{
    Args args = parse_args(argc, argv);

    std::string train_data;

    if (!args.urls.empty()) {
        std::vector<std::string> urls = tokenize(args.urls, kUrlSeparators, false);
        for (const auto &url: urls) {
            auto data = download(url);
            train_data.append(data);
            train_data.append("\n");
        }
    }

    if (!args.files.empty()) {
        std::vector<std::string> files = tokenize(args.urls, kUrlSeparators, false);
        for (const auto &file: files) {
            std::ifstream stream(file.c_str(), std::ios::in);
            std::string   line, data;

            while (std::getline(stream, line)) {
                data.append(line);
                data.append("\n");
            }

            train_data.append(data);
            train_data.append("\n");
        }
    }

    Model model(args.dimension);
    build_model(train_data, model);
    save(model, args.model);
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
