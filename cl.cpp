#include "cl.hpp"
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>

namespace QUMA {

void CL::Init() { /* initialize kernel entropy, logging, etc. */ }
void CL::Shutdown() { /* release allocated entropy pools */ }

std::string CL::Base64Encode(const std::vector<uint8_t>& data) {
    static const char tbl[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    int val = 0, valb = -6;
    for (uint8_t c : data) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back(tbl[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    while (valb > -6) {
        out.push_back(tbl[((val << 8) >> (valb + 8)) & 0x3F]);
        valb -= 6;
    }
    while (out.size() % 4) out.push_back('=');
    return out;
}

std::vector<uint8_t> CL::Base64Decode(const std::string& input) {
    static const int tbl[256] = {
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,
        -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,62,-1,-1,-1,63,
        52,53,54,55,56,57,58,59,60,61,-1,-1,-1, 0,-1,-1,
        -1, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,
        15,16,17,18,19,20,21,22,23,24,25,-1,-1,-1,-1,-1,
        -1,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,
        41,42,43,44,45,46,47,48,49,50,51
    };
    std::vector<uint8_t> out;
    int val = 0, valb = -8;
    for (unsigned char c : input) {
        if (tbl[c] == -1) break;
        val = (val << 6) + tbl[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

std::string CL::HMACSign(const std::string& data, const std::string& key) {
    unsigned char* result;
    unsigned int len = 20;
    result = HMAC(EVP_sha1(), key.c_str(), key.length(),
                  (unsigned char*)data.c_str(), data.length(), nullptr, nullptr);
    std::ostringstream ss;
    for (unsigned int i = 0; i < len; i++)
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)result[i];
    return ss.str();
}

bool CL::VerifyHMAC(const std::string& data, const std::string& key, const std::string& signature) {
    return HMACSign(data, key) == signature;
}

std::string CL::HexQUApEncrypt(const std::string& input, const std::string& key) {
    // placeholder: real HexQUAp would use TGDK entropy and clause transforms
    std::ostringstream ss;
    for (char c : input)
        ss << std::hex << ((int)c ^ key.length());
    return ss.str();
}

std::string CL::HexQUApDecrypt(const std::string& cipher, const std::string& key) {
    std::string out;
    for (size_t i = 0; i < cipher.size(); i += 2) {
        std::string byteStr = cipher.substr(i, 2);
        char ch = (char)(std::stoi(byteStr, nullptr, 16) ^ key.length());
        out.push_back(ch);
    }
    return out;
}

} // namespace QUMA