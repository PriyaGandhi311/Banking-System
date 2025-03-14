#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Authentication.h"
#include "BankAccount.h"

// Function prototypes
void displayMainMenu(bool isLoggedIn);
void handleAuthentication(AuthenticationManager& authManager);
void handleBankingOperations(AuthenticationManager& authManager, Bank& bank);
void createAccount(AuthenticationManager& authManager, Bank& bank);
void deposit(Bank& bank, int userId);
void withdraw(Bank& bank, int userId);
void checkBalance(Bank& bank, int userId);
void displayUserAccounts(Bank& bank, int userId);
void clearInputBuffer();

int main() {
    // Initialize authentication manager
    AuthenticationManager authManager;
    
    // Initialize bank system
    Bank bank;
    bank.loadAccounts();
    
    bool running = true;
    while (running) {
        displayMainMenu(authManager.isLoggedIn());
        
        int choice;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        clearInputBuffer();
        
        if (authManager.isLoggedIn()) {
            // User is logged in, show banking options
            switch (choice) {
                case 1: // Create new account
                    createAccount(authManager, bank);
                    break;
                case 2: // Deposit
                    deposit(bank, authManager.getCurrentUserId());
                    break;
                case 3: // Withdraw
                    withdraw(bank, authManager.getCurrentUserId());
                    break;
                case 4: // Check balance
                    checkBalance(bank, authManager.getCurrentUserId());
                    break;
                case 5: // View all accounts
                    displayUserAccounts(bank, authManager.getCurrentUserId());
                    break;
                case 6: // Logout
                    authManager.logout();
                    std::cout << "Logged out successfully.\n";
                    break;
                case 7: // Exit
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        } else {
            // User is not logged in, show authentication options
            switch (choice) {
                case 1: { // Login
                    std::string username, password;
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    
                    if (authManager.login(username, password)) {
                        std::cout << "Login successful!\n";
                    } else {
                        std::cout << "Login failed. Please check your credentials.\n";
                    }
                    break;
                }
                case 2: { // Register
                    std::string username, password;
                    std::cout << "Enter username: ";
                    std::getline(std::cin, username);
                    std::cout << "Enter password: ";
                    std::getline(std::cin, password);
                    
                    if (authManager.registerUser(username, password)) {
                        std::cout << "Registration successful! You can now login.\n";
                    } else {
                        std::cout << "Registration failed. Username might already exist.\n";
                    }
                    break;
                }
                case 3: // Exit
                    running = false;
                    break;
                default:
                    std::cout << "Invalid option. Please try again.\n";
                    break;
            }
        }
    }
    
    std::cout << "Thank you for using our Banking System. Goodbye!\n";
    return 0;
}

void displayMainMenu(bool isLoggedIn) {
    std::cout << "\n===== Banking System =====\n";
    
    if (isLoggedIn) {
        std::cout << "1. Create New Account\n";
        std::cout << "2. Deposit\n";
        std::cout << "3. Withdraw\n";
        std::cout << "4. Check Balance\n";
        std::cout << "5. View All Accounts\n";
        std::cout << "6. Logout\n";
        std::cout << "7. Exit\n";
    } else {
        std::cout << "1. Login\n";
        std::cout << "2. Register\n";
        std::cout << "3. Exit\n";
    }
}

