// g++ -Wall -W -std=c++11 normalize.cpp -o /tmp/nl -lboost_locale
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/locale.hpp>
#include <boost/lexical_cast.hpp>

#include "exc.hpp"
#include "model.hpp"
#include "tokenizer.hpp"

namespace markov {

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

} // namespace

using namespace markov;

#include <iostream>
#include <fstream>

int
main(int argc, char **argv)
{
    if (argc < 5) {
        std::cout << "usage: " << argv[0] << " <dimension> <datafile> <number of words> <start sequence>" << std::endl;
        return EXIT_SUCCESS;
    }

    size_t      model_dimension = boost::lexical_cast<size_t>(argv[1]);
    std::string data_file = argv[2];
    size_t      words_count = boost::lexical_cast<size_t>(argv[3]);
    std::string seed = argv[4];

    std::ifstream file(data_file.c_str(), std::ios::in);
    std::string   line, data;

    while (std::getline(file, line)) {
        data.append(line);
        data.append("\n");
    }

    // for (auto token: tokens) {
    //     std::cout << token << std::endl;
    // }

    Model model(model_dimension);

    build_model(data, model);

    std::vector<std::string> start_sequence = tokenize(seed, kTextSeparators);
    std::vector<std::string> gibberish = model.generate(start_sequence, words_count);
    
    if (gibberish.size()) {
        for (const auto &word: start_sequence) {
            std::cout << word << " ";
        }
        for (const auto &word: gibberish) {
            std::cout << word << " ";
        }
        std::cout << std::endl;
    }
    //model.print();

    return 0;
}
