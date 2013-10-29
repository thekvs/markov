#ifndef __SERIALIZE_HPP_INCLUDED__
#define __SERIALIZE_HPP_INCLUDED__

#include <string>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

namespace markov {

template <typename T>
void save(const T &s, std::string filename)
{
    std::ios_base::openmode mode = std::ios::out | std::ios::binary | std::ios::trunc;
    std::ofstream ofs(filename.c_str(), mode);
    boost::archive::binary_oarchive oa(ofs);

    oa << s;
}

template <typename T>
void load(T &s, std::string filename)
{
    std::ios_base::openmode mode = std::ios::in | std::ios::binary;
    std::ifstream ifs(filename.c_str());
    boost::archive::binary_iarchive ia(ifs, mode);

    ia >> s;
}

} // namespace

#endif
