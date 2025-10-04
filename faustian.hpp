#ifndef QUMA_FAUSTIAN_HPP
#define QUMA_FAUSTIAN_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace QUMA {

    struct FaustianVector {
        double a, b, c, d;
    };

    class Faustian {
    public:
        static void Init();
        static void Shutdown();

        // build the pairwise duoquadratalized vector
        static FaustianVector Generate(const std::string& key, double bias = 1.0);

        // reversible trapdoor transformation
        static std::string TrapdoorEncrypt(const std::string& input, const FaustianVector& fv);
        static std::string TrapdoorDecrypt(const std::string& input, const FaustianVector& fv);

        // verification signature
        static std::string Signature(const FaustianVector& fv);
    };
}

#endif // QUMA_FAUSTIAN_HPP