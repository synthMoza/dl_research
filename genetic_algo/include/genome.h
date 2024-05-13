#ifndef GENOME_HEADER
#define GENOME_HEADER

#include <bitset>

#include "rng.h"
#include "gray_code.h"

namespace dl
{

// Genome class holds genome as encoded value using Gray Code
class Genome
{
public:
    using GenomeType = uint64_t;
private:
    GrayEncoder<GenomeType, GenomeType> m_encoder{};
    GenomeType m_encoded_value;

    inline static std::uniform_int_distribution<GenomeType> m_distribution{};
public:
    // be default, we generate random genome
    Genome() {
        m_encoded_value = m_encoder.encode(m_distribution(random_engine));
    }

    Genome(GenomeType value) : m_encoded_value(m_encoder.encode(value)) {}

    GenomeType getEncodedGenome() const {
        return m_encoded_value;
    }

    GenomeType getDecodedGenome() const {
        return m_encoder.decode(m_encoded_value);
    }

    static GenomeType getDistributionMaximumValue() {
        return m_distribution.b();
    }

    static GenomeType getDistributionMinimumValue() {
        return m_distribution.a();
    }

    static size_t getGenomeSize() {
        return sizeof(GenomeType) * 8;
    }

    void mutate(double mutation_probability) {
        std::uniform_real_distribution distribution;
        std::bitset<sizeof(GenomeType) * 8> bits(m_encoded_value);

        for (size_t i = 0; i < sizeof(GenomeType) * 8; ++i) {
            const auto current_probability = distribution(random_engine);
            if (current_probability < mutation_probability) {
                bits.flip(i);
            }
        }

        m_encoded_value = bits.to_ullong();
    }
};

} // namespace dl

#endif // #define GENOME_HEADER
