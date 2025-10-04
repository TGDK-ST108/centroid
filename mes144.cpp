#include "mes144.hpp"
#include <cmath>
#include <sstream>
#include <iomanip>

namespace QUMA {

void MES144::Init() { /* allocate RNG pools or entropy devices */ }
void MES144::Shutdown() { /* cleanup */ }

MES144Seed MES144::GenerateSeed(const std::string& key) {
    uint64_t hi = 0, lo = 0;
    for (size_t i = 0; i < key.size(); ++i) {
        uint8_t c = static_cast<uint8_t>(key[i]);
        if (i % 2 == 0)
            hi = (hi * 131) ^ (c + (hi >> 3));
        else
            lo = (lo * 137) ^ (c + (lo >> 5));
    }
    // mix entropy between hi/lo
    hi ^= (lo << 13) | (lo >> 51);
    lo ^= (hi << 7)  | (hi >> 57);
    return {hi, lo};
}

double MES144::EntropyRatio(const MES144Seed& s, uint32_t offset) {
    uint64_t mix = s.hi ^ (s.lo + offset * 0x9E3779B97F4A7C15ULL);
    // normalize to [0,1)
    return (mix & 0xFFFFFFFFFFFFULL) / static_cast<double>(0xFFFFFFFFFFFFULL);
}

std::string MES144::Encrypt(const std::string& input, const MES144Seed& seed) {
    std::ostringstream ss;
    uint64_t val = seed.hi ^ seed.lo;
    for (size_t i = 0; i < input.size(); ++i) {
        uint8_t ch = static_cast<uint8_t>(input[i]);
        ch ^= (val >> ((i % 8) * 8)) & 0xFF;
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)ch;
        // advance entropy
        val = val * 0x5DEECE66DULL + 0xB;
    }
    return ss.str();
}

std::string MES144::Decrypt(const std::string& input, const MES144Seed& seed) {
    std::string out;
    uint64_t val = seed.hi ^ seed.lo;
    for (size_t i = 0; i < input.size(); i += 2) {
        std::string byteStr = input.substr(i, 2);
        uint8_t byte = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        byte ^= (val >> (((i / 2) % 8) * 8)) & 0xFF;
        out.push_back(static_cast<char>(byte));
        val = val * 0x5DEECE66DULL + 0xB;
    }
    return out;
}

} // namespace QUMA