#include <Windows.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <mutex>
#include <fstream>
#include <string>
#include "../SyndicateAuthCPP/syndicate/SyndicateAuthLib.h"
#include "syndicate/skStr.h"

#pragma comment(lib, "SyndicateAuth.lib")

// Place your user API key, and secret key found in the subscription tab  
std::string apiKey = skCrypt("example").decrypt(); // api key here
std::string secretKey = skCrypt("example").decrypt(); // secret key here
std::string productId = skCrypt("example").decrypt(); // product id here
std::string FILE_Id = skCrypt("example ").decrypt(); // file id for file you want to stream here
SyndicateAuthLib::Auth auth(apiKey, secretKey);

std::atomic<bool> licenseValid(false);
std::mutex licenseMutex;

void OnLicenseValidated() {
    std::cout << "\n" << skCrypt("=== License Validation Successful! ===").decrypt() << std::endl;
    std::cout << skCrypt("Welcome to Syndicate Auth!").decrypt() << std::endl;

    // Display users Expiration Date, and or HWID
    std::cout << "\n" << skCrypt("License Information:").decrypt() << std::endl;
    std::cout << skCrypt("  Expiration Date: ").decrypt() << auth.GetExpirationDate() << std::endl;
    std::cout << skCrypt("  HWID: ").decrypt() << auth.GetHwid() << std::endl;

    
    std::cout << "\n" << skCrypt("Downloading file...").decrypt() << std::endl;
    std::vector<std::uint8_t> fileBytes = auth.DownloadFile(FILE_Id);

    if (!fileBytes.empty()) {
        std::cout << skCrypt("File downloaded successfully. Size: ").decrypt() << fileBytes.size() << " bytes" << std::endl;
        
        // Save file to demonstrate
        std::ofstream outFile("downloaded_file.bin", std::ios::binary);
        if (outFile.is_open()) {
            outFile.write(reinterpret_cast<const char*>(fileBytes.data()), fileBytes.size());
            outFile.close();
            std::cout << skCrypt("File saved as 'downloaded_file.bin'").decrypt() << std::endl;
        }
        
        system("pause");
        exit(1);
    } else {
        std::cerr << skCrypt("File download failed: ").decrypt() << auth.GetLastError() << std::endl;
        
        // The DLL now handles all error parsing and provides user-friendly messages
        std::cout << "\n" << skCrypt("=== File Download Failed ===").decrypt() << std::endl;
        std::cout << skCrypt("Message: ").decrypt() << auth.GetLastError() << std::endl;
        std::cout << skCrypt("===============================").decrypt() << std::endl;
    }
}

int main() {
    std::cout << skCrypt("========================================").decrypt() << std::endl;
    std::cout << skCrypt("        Syndicate Auth Example").decrypt() << std::endl;
    std::cout << skCrypt("========================================").decrypt() << std::endl;
    
    std::string licenseKey;
    std::cout << "\n" << skCrypt("Enter your License Key: ").decrypt();
    std::getline(std::cin, licenseKey);

    // Validate input
    if (licenseKey.empty()) {
        std::cout << skCrypt("Error: License key cannot be empty!").decrypt() << std::endl;
        std::cout << skCrypt("Press Enter to exit...").decrypt();
        std::cin.get();
        return 1;
    }

    std::cout << "\n" << skCrypt("Initializing SyndicateAuth...").decrypt() << std::endl;

    if (!auth.Initialize()) {
        std::cout << "\n" << skCrypt("=== Initialization Failed ===").decrypt() << std::endl;
        std::cout << skCrypt("Message: ").decrypt() << auth.GetLastError() << std::endl;
        std::cout << skCrypt("=============================").decrypt() << std::endl;
        
        std::cout << skCrypt("Press Enter to exit...").decrypt();
        std::cin.get();
        return 1;
    }

    std::cout << skCrypt("Initialization successful!").decrypt() << std::endl;
    std::cout << "\n" << skCrypt("Validating license...").decrypt() << std::endl;

    if (auth.ValidateLicense(licenseKey, productId)) {
        {
            std::lock_guard<std::mutex> lock(licenseMutex);
            licenseValid = true;
        }
        OnLicenseValidated();
    }
    else {
        std::cout << "\n" << skCrypt("=== License Validation Failed ===").decrypt() << std::endl;
        std::cout << skCrypt("Message: ").decrypt() << auth.GetLastError() << std::endl;
        std::cout << skCrypt("=================================").decrypt() << std::endl;
    }

    std::cout << "\n" << skCrypt("Press Enter to exit...").decrypt();
    std::cin.get();
    return 0;
}