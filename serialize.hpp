#ifndef __SERIALIZE_HPP_INCLUDED__
#define __SERIALIZE_HPP_INCLUDED__

#include <string>
#include <fstream>

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/map.hpp>

#include "exc.hpp"

namespace markov {

template <typename T>
void save(const T &s, std::string filename)
{
    std::ios_base::openmode mode = std::ios::out | std::ios::binary | std::ios::trunc;

    std::ofstream ofs(filename.c_str(), mode);
    THROW_EXC_IF_FAILED(!ofs.fail(), "couldn't open file %s", filename.c_str());

    boost::archive::binary_oarchive oa(ofs);

    oa << s;
}

template <typename T>
void load(T &s, std::string filename)
{
    std::ios_base::openmode mode = std::ios::in | std::ios::binary;
    
    std::ifstream ifs(filename.c_str(), mode);
    THROW_EXC_IF_FAILED(!ifs.fail(), "couldn't open file %s", filename.c_str());

    boost::archive::binary_iarchive ia(ifs);

    ia >> s;
}

} // namespace

#endif
