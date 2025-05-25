#pragma once


#define SYNDICATEAUTHLIB_API __declspec(dllexport) 



#include <string>
#include <vector>

namespace SyndicateAuthLib {


    class SYNDICATEAUTHLIB_API Auth {
    public:
        Auth(const std::string& apiKey, const std::string& secretKey);
        ~Auth();

        bool Initialize();
        bool ValidateLicense(const std::string& licenseKey, const std::string& productId);
        std::string GetLastError() const;
        std::vector<std::uint8_t> DownloadFile(const std::string& fileId);

        // Getter methods for expiration date and HWID
        std::string GetExpirationDate() const;
        std::string GetHwid() const;


    private:
        std::string m_apiKey;
        std::string m_secretKey;
        std::string m_previousResponse;
        std::string m_lastError;
        bool m_initialized;
        bool m_isAuthenticated;

        std::string m_expirationDate;
        std::string m_hwid;



    };
}