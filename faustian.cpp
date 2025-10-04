#include "faustian.hpp"
#include <sstream>
#include <iomanip>
#include <cmath>

namespace QUMA {

void Faustian::Init() { /* allocate local policy matrices */ }
void Faustian::Shutdown() { /* free matrices */ }

FaustianVector Faustian::Generate(const std::string& key, double bias) {
    FaustianVector fv{0,0,0,0};
    for (size_t i = 0; i < key.size(); ++i) {
        double k = static_cast<double>(static_cast<unsigned char>(key[i]));
        fv.a += sin(k + i*bias);
        fv.b += cos(k * 0.5 + i);
        fv.c += sin(k * 0.33 + fv.b);
        fv.d += cos(k * 0.25 + fv.a);
    }
    // normalize and apply duoquadratalized fold (fold 5 – 8)
    fv.a = fmod(fabs(fv.a), 5.0);
    fv.b = fmod(fabs(fv.b), 8.0);
    fv.c = fmod(fabs(fv.c), 13.0);
    fv.d = fmod(fabs(fv.d), 21.0);
    return fv;
}

std::string Faustian::TrapdoorEncrypt(const std::string& input, const FaustianVector& fv) {
    std::ostringstream ss;
    for (size_t i = 0; i < input.size(); ++i) {
        uint8_t c = static_cast<uint8_t>(input[i]);
        double rot = (fv.a + fv.b + fv.c + fv.d) * (i + 1);
        c ^= static_cast<uint8_t>(static_cast<int>(rot) & 0xFF);
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)c;
    }
    return ss.str();
}

std::string Faustian::TrapdoorDecrypt(const std::string& input, const FaustianVector& fv) {
    std::string out;
    for (size_t i = 0; i < input.size(); i += 2) {
        std::string byteStr = input.substr(i, 2);
        uint8_t c = static_cast<uint8_t>(std::stoi(byteStr, nullptr, 16));
        double rot = (fv.a + fv.b + fv.c + fv.d) * ((i / 2) + 1);
        c ^= static_cast<uint8_t>(static_cast<int>(rot) & 0xFF);
        out.push_back(static_cast<char>(c));
    }
    return out;
}

std::string Faustian::Signature(const FaustianVector& fv) {
    std::ostringstream ss;
    ss << std::hex << std::setw(4) << std::setfill('0')
       << static_cast<int>((fv.a * 17) + (fv.b * 19) + (fv.c * 23) + (fv.d * 29)) & 0xFFFF;
    return ss.str();
}

} // namespace QUMA