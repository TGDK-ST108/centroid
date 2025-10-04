#ifndef QUMA_CL_HPP
#define QUMA_CL_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace QUMA {
    class CL {
    public:
        static std::string Base64Encode(const std::vector<uint8_t>& data);
        static std::vector<uint8_t> Base64Decode(const std::string& input);

        static std::string HexQUApEncrypt(const std::string& input, const std::string& key);
        static std::string HexQUApDecrypt(const std::string& cipher, const std::string& key);

        static std::string HMACSign(const std::string& data, const std::string& key);
        static bool VerifyHMAC(const std::string& data, const std::string& key, const std::string& signature);

        static void Init();     // kernel initialization
        static void Shutdown(); // kernel cleanup
    };
}

#endif // QUMA_CL_HPP