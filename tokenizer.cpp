#include "tokenizer.hpp"

namespace markov {

std::vector<std::string>
tokenize(const std::string &data, const std::string &sep, bool to_lower)
{
    Separator separator(sep.c_str());
    Tokenizer tokenizer(data, separator);

    boost::locale::generator gen;
    std::locale locale = gen("ru_RU.UTF-8");

    std::vector<std::string> tokens;

    for (const auto &token: tokenizer) {
        if (to_lower) {
            tokens.push_back(boost::locale::to_lower(token, locale));
        } else {
            tokens.push_back(token);
        }
    }

    return tokens;
}

} // namespace