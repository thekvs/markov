#ifndef __MODEL_HPP_INCLUDED__
#define __MODEL_HPP_INCLUDED__

#include <cmath>
#include <limits>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/discrete_distribution.hpp>

#include "sequence.hpp"
#include "numerator.hpp"

namespace markov {

class ModelBuilder {
public:

    ModelBuilder(size_t dim):
        dimention(dim),
        sequence(dimention)
    {
    }

    ModelBuilder() = delete;

    void add_word(const std::string &word);
    void build();
    void print();
    std::vector<std::string> generate(const std::vector<std::string> &start_sequence, size_t count);

private:

    typedef std::map<uint32_t, size_t> Frequencies;

    struct SequenceStat {
        size_t      total_suffixes_count;
        Frequencies frequencies;
    };

    // typedef std::map<Sequence, Frequencies> TextStat;
    typedef std::map<Sequence, SequenceStat> TextStat;

    class Transition {
    public:

        void add(uint32_t id, double probability) {
            words_id.push_back(id);
            probabilities.push_back(probability);
        }

        std::vector<uint32_t> words_id;
        std::vector<double>   probabilities;
    };

    typedef std::map<Sequence, Transition> TransitionTable;

    size_t    dimention;
    Sequence  sequence;
    Numerator numerator;
    TextStat  text_stat;

    TransitionTable transition_table;

    double kahan_sum(const std::vector<double> &data);
};

} // namespace

#endif
