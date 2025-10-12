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

void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void createAccount(AuthenticationManager& authManager, Bank& bank) {
    if (!authManager.isLoggedIn()) {
        std::cout << "You must be logged in to create an account.\n";
        return;
    }
    
    std::string accountName;
    double initialBalance;
    
    std::cout << "Enter account name: ";
    std::getline(std::cin, accountName);
    
    std::cout << "Enter initial balance: $";
    std::cin >> initialBalance;
    clearInputBuffer();
    
    if (initialBalance < 0) {
        std::cout << "Initial balance cannot be negative.\n";
        return;
    }
    
    // Generate a simple account ID
    static int accountCounter = 1000;
    std::string accountId = "ACC" + std::to_string(accountCounter++);
    
    BankAccount newAccount(authManager.getCurrentUserId(), accountId, accountName, initialBalance);
    bank.addAccount(newAccount);
    
    std::cout << "Account created successfully!\n";
    std::cout << "Account ID: " << accountId << "\n";
    std::cout << "Account Name: " << accountName << "\n";
    std::cout << "Initial Balance: $" << initialBalance << "\n";
}

void deposit(Bank& bank, int userId) {
    std::string accountId;
    double amount;
    
    std::cout << "Enter account ID: ";
    std::getline(std::cin, accountId);
    
    std::cout << "Enter amount to deposit: $";
    std::cin >> amount;
    clearInputBuffer();
    
    BankAccount* account = bank.findAccount(accountId);
    if (!account) {
        std::cout << "Account not found.\n";
        return;
    }
    
    if (account->getUserId() != userId) {
        std::cout << "You can only deposit to your own accounts.\n";
        return;
    }
    
    if (account->deposit(amount)) {
        bank.saveAccounts(); // Save the updated balance
        std::cout << "Deposit successful! New balance: $" << account->getBalance() << "\n";
    } else {
        std::cout << "Deposit failed. Amount must be positive.\n";
    }
}

void withdraw(Bank& bank, int userId) {
    std::string accountId;
    double amount;
    
    std::cout << "Enter account ID: ";
    std::getline(std::cin, accountId);
    
    std::cout << "Enter amount to withdraw: $";
    std::cin >> amount;
    clearInputBuffer();
    
    BankAccount* account = bank.findAccount(accountId);
    if (!account) {
        std::cout << "Account not found.\n";
        return;
    }
    
    if (account->getUserId() != userId) {
        std::cout << "You can only withdraw from your own accounts.\n";
        return;
    }
    
    if (account->withdraw(amount)) {
        bank.saveAccounts(); // Save the updated balance
        std::cout << "Withdrawal successful! New balance: $" << account->getBalance() << "\n";
    } else {
        std::cout << "Withdrawal failed. Insufficient funds or invalid amount.\n";
    }
}

void checkBalance(Bank& bank, int userId) {
    std::string accountId;
    
    std::cout << "Enter account ID: ";
    std::getline(std::cin, accountId);
    
    BankAccount* account = bank.findAccount(accountId);
    if (!account) {
        std::cout << "Account not found.\n";
        return;
    }
    
    if (account->getUserId() != userId) {
        std::cout << "You can only check balance of your own accounts.\n";
        return;
    }
    
    account->displayBalance();
}

void displayUserAccounts(Bank& bank, int userId) {
    std::vector<BankAccount> userAccounts = bank.findAccountsByUserId(userId);
    
    if (userAccounts.empty()) {
        std::cout << "No accounts found for this user.\n";
        return;
    }
    
    std::cout << "\nYour Accounts:\n";
    std::cout << "==============\n";
    for (const auto& account : userAccounts) {
        account.displayBalance();
    }
}

