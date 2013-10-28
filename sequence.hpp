#ifndef __SEQUENCE_HPP_INCLUDED__
#define __SEQUENCE_HPP_INCLUDED__

#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cstdint>

namespace markov {

class Sequence {
public:

    Sequence(size_t len): length(len) {
        data.reserve(length);
    }

    Sequence() = delete;

    bool complete() const {
        return (data.size() == length);
    }

    void add(uint32_t n)
    {
        if (complete()) {
            std::copy(data.begin() + 1, data.end(), data.begin());
            data.back() = n;
        } else {
            data.push_back(n);
        }
    }

    bool operator<(const Sequence &other) const {
        return data < other.data;
    }

    std::vector<uint32_t> data;
    size_t                length;
};

typedef std::map<Sequence, int> SequenceMap;

} // namespace

#endif
