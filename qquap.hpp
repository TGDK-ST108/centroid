#ifndef QUMA_QQUAP_HPP
#define QUMA_QQUAP_HPP

#include <string>
#include <vector>
#include <cstdint>
#include "mes144.hpp"
#include "faustian.hpp"

namespace QUMA {

    struct QQUApContext {
        MES144Seed seed;
        FaustianVector fv;
        std::string key;
    };

    class QQUAp {
    public:
        static void Init();
        static void Shutdown();

        // derive a complete context from a key
        static QQUApContext GenerateContext(const std::string& key);

        // primary encryption/decryption
        static std::string Encrypt(const std::string& input, const QQUApContext& ctx);
        static std::string Decrypt(const std::string& input, const QQUApContext& ctx);

        // hash / digest generation
        static std::string Digest(const std::string& input, const QQUApContext& ctx);
    };
}

#endif // QUMA_QQUAP_HPP