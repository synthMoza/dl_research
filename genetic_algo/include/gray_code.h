#include <bitset>

namespace dl
{

template<typename From, typename To>
struct Encoder
{
    using FromType = From;
    using ToType = To; 

    virtual To encode(const From& from) = 0;
    virtual From decode(const To& to) = 0;
};

template <typename IntegerType = uint32_t, typename CodeType = uint32_t>
struct GrayEncoder : Encoder<IntegerType, CodeType>
{
    CodeType encode(const IntegerType& from) override
    {
        return from ^ (from >> 1);
    }

    IntegerType decode(const CodeType& to) override
    {
        IntegerType from = 0;
        CodeType coded = to;
        for (; coded; coded >>= 1)
            from ^= coded;
        return from;
    }
};

} // namespace ga
