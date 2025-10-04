#ifndef QUMA_MES144_HPP
#define QUMA_MES144_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace QUMA {
    struct MES144Seed {
        uint64_t hi;
        uint64_t lo;
    };

    class MES144 {
    public:
        static void Init();
        static void Shutdown();

        // create a 144-bit seed from a string key
        static MES144Seed GenerateSeed(const std::string& key);

        // entropy sample (pseudo-random 0-1 range)
        static double EntropyRatio(const MES144Seed& s, uint32_t offset = 0);

        // 144-bit symmetric encrypt/decrypt (toy model)
        static std::string Encrypt(const std::string& input, const MES144Seed& seed);
        static std::string Decrypt(const std::string& input, const MES144Seed& seed);
    };
}

#endif // QUMA_MES144_HPP