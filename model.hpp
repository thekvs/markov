#ifndef __MODEL_HPP_INCLUDED__
#define __MODEL_HPP_INCLUDED__

#include <cmath>
#include <limits>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>

#include "sequence.hpp"
#include "numerator.hpp"
#include "tokenizer.hpp"

namespace markov {

class Model {
public:

    Model(size_t dim):
        dimention(dim),
        sequence(dimention)
    {
    }

    Model(): Model(0) {}

    void add_word(const std::string &word);
    void build(bool sanity_check=false);
    void build(const std::string &data, bool sanity_check=false);
    void print();
    std::vector<std::string> generate(const std::vector<std::string> &start_sequence, size_t count);

    size_t order() const {
        return dimention;
    }

private:

    friend class boost::serialization::access;

    using Frequencies = std::map<uint32_t, size_t>;

    struct SequenceStat {
        size_t      total_suffixes_count;
        Frequencies frequencies;
    };

    using TextStat = std::map<Sequence, SequenceStat>;

    class Transition {
    public:

        void add(uint32_t id, double probability)
        {
            words_id.push_back(id);
            probabilities.push_back(probability);
        }

        std::vector<uint32_t> words_id;
        std::vector<double>   probabilities;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int)
        {
            ar & words_id;
            ar & probabilities;
        }
    };

    using TransitionTable = std::map<Sequence, Transition>;

    size_t          dimention;
    TransitionTable transition_table;
    Numerator       numerator;

    Sequence        sequence;
    TextStat        text_stat;

    double kahan_sum(const std::vector<double> &data);

    template<typename Archive>
    void serialize(Archive &ar, const unsigned int)
    {
        ar & dimention;
        ar & transition_table;
        ar & numerator;
    }
};

} // namespace

#endif
