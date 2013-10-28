#include "model.hpp"

namespace markov {

static const double one = 1.0;

void
ModelBuilder::add_word(const std::string &word)
{
    uint32_t id = numerator.enumerate(word);

    if (sequence.complete()) {
        TextStat::iterator seq_found = text_stat.find(sequence);
        if (seq_found == text_stat.end()) {
            Frequencies freq = { {id, 1} };
            SequenceStat sequence_stat = {1, freq};
            text_stat[sequence] = sequence_stat;
        } else {
            SequenceStat &sequence_stat = seq_found->second;
            Frequencies::iterator freq_found = sequence_stat.frequencies.find(id);

            if (freq_found == sequence_stat.frequencies.end()) {
                sequence_stat.frequencies[id] = 1;
            } else {
                freq_found->second++;
            }

            sequence_stat.total_suffixes_count++;
        }
    }

    sequence.add(id);
}

void
ModelBuilder::build()
{
    for (const auto &ts_elem: text_stat) {
        const TextStat::mapped_type &sequence_stat = ts_elem.second;
        Transition transition;
        for (const auto &f: sequence_stat.frequencies) {
            double probability = static_cast<double>(f.second) / sequence_stat.total_suffixes_count;
            transition.add(f.first, probability);
        }

        // Sanity check: sum of probabilities have to be 1.
        //
        // Since we are summing doubles we have to use a special
        // summation algorithm to compensate for accumulating error
        // and than compare difference with epsilon for double type.
        double sum = kahan_sum(transition.probabilities);
        double diff = std::fabs(sum - one);
        THROW_EXC_IF_FAILED(diff < std::numeric_limits<double>::epsilon(),
            "broken invariant: sum of probs. (=%f) is not eq. to 1", sum);

        transition_table[ts_elem.first] = transition;
    }
}

void
ModelBuilder::print()
{
    for (const auto &elem: transition_table) {
        for (const auto &id: elem.first.data) {
            std::cout << numerator.denumerate(id) << " ";
        }
        std::cout << "-> ";
        for (size_t i = 0; i < elem.second.words_id.size(); i++) {
            std::cout << numerator.denumerate(elem.second.words_id[i]) << ":" << 
                elem.second.probabilities[i] << " ";
        }
        std::cout << std::endl;
    }
}

double
ModelBuilder::kahan_sum(const std::vector<double> &data)
{
    // directly stolen from here: http://en.wikipedia.org/wiki/Kahan_summation_algorithm
    double sum = 0.0;
    double c = 0;

    double y;
    double t;

    for (auto &v: data) {
        y = v - c;
        t = sum + y;
        c = (t - sum) - y;
        sum = t;
    }

    return sum;
}

} // namespace
