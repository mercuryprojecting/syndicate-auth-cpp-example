
#include <Windows.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <fstream>
#include "../SyndicateAuthCPP/syndicate/SyndicateAuthLib.h"
#include "syndicate/skStr.h"



#pragma comment(lib, "SyndicateAuth.lib")


// Place your user API key, and secret key found in the subscription tab  
std::string apiKey = skCrypt("toP6M9KZcEgManeA2DtZEwvLVUHcmByp").decrypt();
std::string secretKey = skCrypt("4fffeb7abf20a66f6f8f74aeeb8e689602133190dd362332aa9572f29c6f0479").decrypt();
std::string product_example = skCrypt("1750e983719fe31b3b2469ec6bf839a7").decrypt(); // rename product_example to whatever you like
std::string product_example2 = skCrypt("000aa186b74125442a4c5f3414a04328").decrypt(); // rename product_example to whatever you like
SyndicateAuthLib::Auth auth(apiKey, secretKey);


std::atomic<bool> licenseValid(false);
std::mutex licenseMutex;

void OnLicenseValidated() {

    std::cout << skCrypt("Hello, welcome to Syndicate Auth example 1!").decrypt() << std::endl;


    // Display users Expiration Date, and or HWID
    std::cout << skCrypt("Expiration Date: ").decrypt() << auth.GetExpirationDate() << std::endl;
    std::cout << skCrypt("HWID: ").decrypt() << auth.GetHwid() << std::endl;

    std::string fileId = skCrypt("41a8da1135a1c82152996a756a2f529b").decrypt(); // Replace with your actual file ID provided in file stream
    std::vector<std::uint8_t> fileBytes = auth.DownloadFile(fileId);

    if (!fileBytes.empty()) {
        std::cout << skCrypt("File downloaded successfully. Size: ").decrypt() << fileBytes.size() << " bytes" << std::endl;
        system("pause");
        exit(1);
    }
    else {
        std::cerr << skCrypt("File download failed: ").decrypt() << auth.GetLastError() << std::endl;
    }

}

void OnLicenseValidated2() {

        std::cout << skCrypt("Hello, welcome to Syndicate Auth example 2!").decrypt() << std::endl;


        // Display users Expiration Date, and or HWID
        std::cout << skCrypt("Expiration Date: ").decrypt() << auth.GetExpirationDate() << std::endl;
        std::cout << skCrypt("HWID: ").decrypt() << auth.GetHwid() << std::endl;
        

        std::string fileId = skCrypt("41a8da1135a1c82152996a756a2f529b").decrypt(); // Replace with your actual file ID provided in file stream
        std::vector<std::uint8_t> fileBytes = auth.DownloadFile(fileId);

        if (!fileBytes.empty()) {
            std::cout << skCrypt("File downloaded successfully. Size: ").decrypt() << fileBytes.size() << " bytes" << std::endl;
            system("pause");
            exit(1);
        }
        else {
            std::cerr << skCrypt("File download failed: ").decrypt() << auth.GetLastError() << std::endl;
        }

}

int main() {
    std::string licenseKey;

    std::cout << skCrypt("Enter your License Key: ").decrypt();
    std::getline(std::cin, licenseKey);

    if (!auth.Initialize()) {
        std::cerr << skCrypt("Failed to initialize: ").decrypt() << auth.GetLastError() << std::endl;
        return 1;
    }



    if (auth.ValidateLicense(licenseKey, product_example)) {
        {
            std::lock_guard<std::mutex> lock(licenseMutex);
            licenseValid = true;
        }
        OnLicenseValidated();

      

    }
    // If validation with the first product ID fails, try the second one
    else if (auth.ValidateLicense(licenseKey, product_example2)) {
        {
            std::lock_guard<std::mutex> lock(licenseMutex);
            licenseValid = true;
        }
        OnLicenseValidated2();
      

    }
    else {
        // License validation failed for all product IDs
        std::cerr << skCrypt("License validation failed: ").decrypt() << auth.GetLastError() << std::endl;
        if (auth.GetLastError().find(skCrypt("HWID mismatch").decrypt()) != std::string::npos) {
            std::cout << skCrypt("Please contact support to reset your HWID.").decrypt() << std::endl;
        }
    }

    std::cout << skCrypt("Press Enter to exit...").decrypt();
    std::cin.get();
    return 0;
}