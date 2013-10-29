#ifndef __SEQUENCE_HPP_INCLUDED__
#define __SEQUENCE_HPP_INCLUDED__

#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cstdint>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>

namespace markov {

class Sequence {
public:

    Sequence(size_t len): length(len) {
        data.reserve(length);
    }

    Sequence(): length(0) {};

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
        return (data < other.data);
    }

    bool operator==(const Sequence &other) const {
        return (data == other.data && length == other.length);
    }

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & length;
        ar & data;
    }

    size_t                length;
    std::vector<uint32_t> data;
};

} // namespace

#endif
