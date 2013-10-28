#ifndef __NUMERATOR_HPP_INCLUDED__
#define __NUMERATOR_HPP_INCLUDED__

#include <cstdint>
#include <string>
#include <vector>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/utility.hpp>
#include <boost/bimap.hpp>

#include "exc.hpp"

namespace markov {

class Numerator: boost::noncopyable {
public:

    Numerator(): counter(0) {}

    uint32_t enumerate(const std::string &str);
    std::string denumerate(uint32_t id);

    bool exist(const std::string &str) const;
    bool exist(uint32_t id) const;

private:

    friend class boost::serialization::access;

    typedef boost::bimap<std::string, uint32_t> DataStorage;
    typedef DataStorage::value_type             DataStorageEntry;

    uint32_t    counter;
    DataStorage data;

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & counter;
        ar & data;
    }
};

}

#endif
