#include "model.hpp"

namespace markov {

static const double   one = 1.0;

void
Model::add_word(const std::string &word)
{
    auto id = numerator.enumerate(word);

    if (sequence.complete()) {
        auto seq_found = text_stat.find(sequence);
        if (seq_found == text_stat.end()) {
            Frequencies freq = { {id, 1} };
            SequenceStat sequence_stat = {1, freq};
            text_stat[sequence] = sequence_stat;
        } else {
            auto &sequence_stat = seq_found->second;
            auto freq_found = sequence_stat.frequencies.find(id);

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
Model::build(bool sanity_check)
{
    for (const auto &ts_elem: text_stat) {
        const TextStat::mapped_type &sequence_stat = ts_elem.second;
        Transition transition;
        for (const auto &f: sequence_stat.frequencies) {
            double probability = static_cast<double>(f.second) / sequence_stat.total_suffixes_count;
            transition.add(f.first, probability);
        }

        if (sanity_check) {
            // Sanity check: sum of probabilities have to be 1.
            //
            // Since we are summing doubles we have to use a special
            // summation algorithm to compensate for accumulating error
            // and than compare difference with epsilon for double type.
            auto sum = kahan_sum(transition.probabilities);
            auto diff = std::fabs(sum - one);
            THROW_EXC_IF_FAILED(diff < std::numeric_limits<double>::epsilon(),
                "broken invariant: sum of probs. (=%f) is not eq. to 1", sum);
        }

        transition_table[ts_elem.first] = transition;
    }
}

void
Model::build(const std::string &data, bool sanity_check)
{
    Separator separator(kTextSeparators.c_str());
    Tokenizer tokenizer(data, separator);

    boost::locale::generator gen;
    std::locale locale = gen("ru_RU.UTF-8");

    for (const auto &token: tokenizer) {
        add_word(boost::locale::to_lower(token, locale));
    }

    build(sanity_check);
}

void
Model::print()
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

std::vector<std::string>
Model::generate(const std::vector<std::string> &start_sequence, size_t count)
{
    std::vector<std::string> result;

    THROW_EXC_IF_FAILED(start_sequence.size() == dimention,
        "number of words in the start sequence (=%zu)"
        " have to be equal to the model's dimention (=%zu)",
        start_sequence.size(), dimention);

    Sequence seq(dimention);

    for (const auto &w: start_sequence) {
        seq.add(numerator.enumerate(w));
    }

    boost::mt19937 generator;
    generator.seed(static_cast<unsigned int>(std::time(0)));

    size_t idx = 0;
    TransitionTable::iterator found = transition_table.find(seq);

    while (found != transition_table.end() && idx < count) {
        boost::random::discrete_distribution<> dist(found->second.probabilities);
        auto next_word_id = found->second.words_id[dist(generator)];
        result.push_back(numerator.denumerate(next_word_id));
        seq.add(next_word_id);
        idx++;
        found = transition_table.find(seq);
    }

    return result;
}

double
Model::kahan_sum(const std::vector<double> &data)
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
