#ifndef __TOKENIZER_HPP_INCLUDED__
#define __TOKENIZER_HPP_INCLUDED__

#include <vector>
#include <string>

#include <boost/tokenizer.hpp>
#include <boost/locale.hpp>

namespace markov {

const std::string kTextSeparators = " .,;:!?-*\"\'()[]{}&$\n\r";
const std::string kUrlSeparators = " ,";

typedef boost::char_separator<char> Separator;
typedef boost::tokenizer<Separator> Tokenizer;

std::vector<std::string> tokenize(const std::string &data, const std::string &sep, bool to_lower=true);

} // namespace

#endif
