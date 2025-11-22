#include <Windows.h>
#include <iostream>
#include <thread>
#include <string>
#include <conio.h>
#include "render/render.h"
#include "cheat/esp/esp.h"
#include "auth/auth.hpp"

// Color codes for console
namespace Colors {
    const int RED = 12;
    const int GREEN = 10;
    const int YELLOW = 14;
    const int CYAN = 11;
    const int MAGENTA = 13;
    const int WHITE = 15;
    const int GRAY = 8;
    const int BRIGHT_WHITE = 15;
}

// Console utility class
class ConsoleHelper {
public:
    static void SetColor(int color) {
        HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hConsole, color);
    }

    static void PrintColored(const std::string& text, int color) {
        SetColor(color);
        std::cout << text;
        SetColor(Colors::WHITE);
    }

    static void PrintHeader(const std::string& title) {
        SetColor(Colors::CYAN);
        std::cout << "\n==================================================\n";
        std::cout << "= ";
        SetColor(Colors::BRIGHT_WHITE);
        std::cout << title;
        SetColor(Colors::CYAN);
        for (size_t i = title.length(); i < 45; i++) std::cout << " ";
        std::cout << "  =\n";
        std::cout << "==================================================\n";
        SetColor(Colors::WHITE);
    }

    static void ClearScreen() {
        system("cls");
    }
};

// KeyAuth configuration
std::string name = "";
std::string ownerid = "";
std::string version = "1.0";
std::string url = "https://keyauth.win/api/1.3/";
std::string path = "";
KLAUTH::KLITH api(name, ownerid, version, url, path, false);

// Key authentication manager
class KeyAuthManager {
public:
    bool AuthenticateUser() {
        ConsoleHelper::PrintHeader("AUTHENTICATION REQUIRED");
        std::cout << "\n";

        ConsoleHelper::PrintColored("[1] ", Colors::CYAN);
        std::cout << "Use license key\n";
        ConsoleHelper::PrintColored("[0] ", Colors::RED);
        std::cout << "Exit\n\n";

        ConsoleHelper::PrintColored("Select option: ", Colors::YELLOW);

        int choice;
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
        case 1:
            return HandleLicense();
        case 0:
            ConsoleHelper::PrintColored("\n[!] Exiting...\n", Colors::RED);
            Sleep(1000);
            return false;
        default:
            ConsoleHelper::PrintColored("\n[!] Invalid option!\n", Colors::RED);
            Sleep(1500);
            return AuthenticateUser();
        }
    }

private:

    bool HandleLicense() {
        ConsoleHelper::ClearScreen();
        ConsoleHelper::PrintHeader("LICENSE ACTIVATION");

        std::string key;

        ConsoleHelper::PrintColored("\nEnter License Key: ", Colors::CYAN);
        std::getline(std::cin, key);

        ConsoleHelper::PrintColored("\n[*] Validating license...\n", Colors::YELLOW);

        try {
            api.license(key);

            if (api.response.success) {
                ConsoleHelper::PrintColored("\n[+] License activated successfully!\n", Colors::GREEN);
                Sleep(1500);
                return true;
            }
            else {
                ConsoleHelper::PrintColored("\n[!] License validation failed: ", Colors::RED);
                std::cout << api.response.message << "\n";
                Sleep(2000);
                return false;
            }
        }
        catch (...) {
            ConsoleHelper::PrintColored("\n[!] License validation error!\n", Colors::RED);
            Sleep(2000);
            return false;
        }
    }
};

class ProcessManager {
public:
    bool WaitForTarget() {
        ConsoleHelper::PrintColored("\n[*] Scanning for Fortnite process...\n", Colors::YELLOW);

        std::wstring target = L"FortniteClient-Win64-Shipping.exe";
        int pid = 0;

        while (!pid) {
            pid = mem::find_process(target.c_str());
            if (pid) {
                ConsoleHelper::PrintColored("[+] Fortnite detected! PID: ", Colors::GREEN);
                ConsoleHelper::PrintColored(std::to_string(pid) + "\n", Colors::CYAN);
                mem::process_id = pid;
                break;
            }
            Sleep(500);
        }

        ConsoleHelper::PrintColored("[*] Waiting for process initialization...\n", Colors::YELLOW);
        Sleep(2000);

        return AttachToProcess();
    }

private:
    bool AttachToProcess() {
        ConsoleHelper::PrintColored("[*] Attaching to process...\n", Colors::YELLOW);
        Sleep(200);

        cr3 = mem::fetch_cr3();
        if (!cr3) {
            ConsoleHelper::PrintColored("[!] Failed to resolve DTB\n", Colors::RED);
            return false;
        }

        virtualaddy = mem::find_image();
        if (!virtualaddy) {
            ConsoleHelper::PrintColored("[!] Failed to resolve base address\n", Colors::RED);
            return false;
        }

        Sleep(300);

        ConsoleHelper::PrintColored("[+] Successfully attached to Fortnite\n", Colors::GREEN);
        ConsoleHelper::PrintColored("[+] Base Address: ", Colors::GREEN);
        ConsoleHelper::PrintColored("0x" + std::to_string(virtualaddy) + "\n", Colors::MAGENTA);
        ConsoleHelper::PrintColored("[+] DTB: ", Colors::GREEN);
        ConsoleHelper::PrintColored("0x" + std::to_string(cr3) + "\n", Colors::MAGENTA);

        return true;
    }
};

class Application {
public:
    int Run() {
        SetConsoleTitleA("Kaillaka Base | Enhanced Edition");

        ConsoleHelper::PrintColored("[*] Initializing API...\n", Colors::YELLOW);
        api.init();
        Sleep(500);

        if (!m_keyAuth.AuthenticateUser()) {
            return EXIT_FAILURE;
        }

        ConsoleHelper::ClearScreen();

        if (!InitializeDriver()) return EXIT_FAILURE;

        if (!InitializeProcess()) return EXIT_FAILURE;

        ShowSuccessAndStart();

        return EXIT_SUCCESS;
    }

private:
    KeyAuthManager m_keyAuth;
    ProcessManager m_process;

    bool InitializeDriver() {
        ConsoleHelper::PrintColored("\n[*] Initializing driver...\n", Colors::YELLOW);
        Sleep(200);

        if (!mem::find_driver()) {
            ConsoleHelper::PrintColored("[!] Driver not loaded\n", Colors::RED);
            ConsoleHelper::PrintColored("[!] Please ensure the driver is properly installed\n", Colors::RED);
            std::cout << "\n";
            system("pause");
            return false;
        }

        ConsoleHelper::PrintColored("[+] Driver initialized successfully\n", Colors::GREEN);
        Sleep(500);
        return true;
    }

    bool InitializeProcess() {
        if (!m_process.WaitForTarget()) {
            std::cout << "\n";
            system("pause");
            return false;
        }
        Sleep(500);
        return true;
    }

    void ShowSuccessAndStart() {
        ConsoleHelper::ClearScreen();

        ConsoleHelper::SetColor(Colors::CYAN);
        std::cout << "\n==================================================\n";
        std::cout << "=                                                =\n";
        std::cout << "=         ";
        ConsoleHelper::SetColor(Colors::GREEN);
        std::cout << "CHEAT IS NOW RUNNING";
        ConsoleHelper::SetColor(Colors::CYAN);
        std::cout << "                   =\n";
        std::cout << "=                                                =\n";
        std::cout << "==================================================\n\n";
        ConsoleHelper::SetColor(Colors::WHITE);

        ConsoleHelper::PrintColored("User: ", Colors::CYAN);
        ConsoleHelper::PrintColored(api.user_data.username + "\n", Colors::GREEN);

        ConsoleHelper::PrintColored("HWID: ", Colors::CYAN);
        ConsoleHelper::PrintColored(api.user_data.hwid + "\n", Colors::GRAY);

        ConsoleHelper::PrintColored("\nPress ", Colors::YELLOW);
        ConsoleHelper::PrintColored("INSERT", Colors::GREEN);
        ConsoleHelper::PrintColored(" to toggle the menu\n\n", Colors::YELLOW);

        Sleep(1000);

        ConsoleHelper::PrintColored("[*] Starting overlay...\n", Colors::CYAN);
        Sleep(500);

        setup(); // Start the overlay
    }
};

int main() {
    Application app;
    return app.Run();

}
