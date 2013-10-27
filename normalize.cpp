// g++ -Wall -W -std=c++11 normalize.cpp -o /tmp/nl -lboost_locale
#include <string>
#include <vector>

#include <boost/tokenizer.hpp>
#include <boost/locale.hpp>

#include "exc.hpp"

namespace markov {

static const char *separators = " .,;:!?-*\"\'()\n\r";

typedef std::vector<std::string>    Tokens;
typedef boost::char_separator<char> Separator;
typedef boost::tokenizer<Separator> Tokenizer;

Tokens
tokenize(const std::string &data, const char *sep)
{
    Separator separator(sep);
    Tokenizer tokenizer(data, separator);

    boost::locale::generator gen;
    std::locale locale = gen("ru_RU.UTF-8");

    Tokens tokens;

    for (auto token: tokenizer) {
        tokens.push_back(boost::locale::to_lower(token, locale));
    }

    return tokens;
}

} // namespace

using namespace markov;

#include <iostream>
#include <fstream>

int
main(int argc, char **argv)
{
    std::ifstream file(argv[1], std::ios::in);
    std::string   line, data;

    while (std::getline(file, line)) {
        data.append(line);
        data.append("\n");
    }

    Tokens tokens = tokenize(data, separators);

    for (auto token: tokens) {
        std::cout << token << std::endl;
    }

    return 0;
}
