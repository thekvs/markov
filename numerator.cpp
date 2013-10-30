#include "numerator.hpp"

namespace markov {

uint32_t
Numerator::enumerate(const std::string &s)
{
    uint32_t result;

    auto found = data.left.find(s);

    if (found == data.left.end()) {
        counter++;
        data.insert(DataStorageEntry(s, counter));
        result = counter;
    } else {
        result = found->second;
    }

    return result;
}

std::string
Numerator::denumerate(uint32_t id)
{
    auto found = data.right.find(id);
    THROW_EXC_IF_FAILED(found != data.right.end(), "broken invariant");

    return found->second;
}

} // namespace