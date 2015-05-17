#ifndef __TOKENIZER_HPP_INCLUDED__
#define __TOKENIZER_HPP_INCLUDED__

#include <vector>
#include <string>

#include <boost/tokenizer.hpp>
#include <boost/locale.hpp>

namespace markov {

const std::string kTextSeparators = " .,;:!?-*\"\'()[]{}&$\n\r";
const std::string kUrlSeparators = " ,";
const std::string kFileSeparators = kUrlSeparators;

using Separator = boost::char_separator<char>;
using Tokenizer = boost::tokenizer<Separator>;

std::vector<std::string> tokenize(const std::string &data, const std::string &sep, bool to_lower=true);

} // namespace

#endif
