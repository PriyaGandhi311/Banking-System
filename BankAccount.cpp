#include "BankAccount.h"
#include "Storage.h"
#include <sstream>
#include <fstream>
#include <vector>

// Utility function to split strings (if not already defined in Storage.h)
std::vector<std::string> splitString(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// BankAccount methods implementation
BankAccount::BankAccount(int userId, const std::string& accountId, const std::string& name, double initialBalance)
    : userId(userId), accountId(accountId), name(name), balance(initialBalance) {}

int BankAccount::getUserId() const {
    return userId;
}

std::string BankAccount::getAccountId() const {
    return accountId;
}

std::string BankAccount::getName() const {
    return name;
}

double BankAccount::getBalance() const {
    return balance;
}

bool BankAccount::deposit(double amount) {
    if (amount <= 0) {
        return false;
    }
    balance += amount;
    return true;
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0 || amount > balance) {
        return false;
    }
    balance -= amount;
    return true;
}

void BankAccount::displayBalance() const {
    std::cout << "Account: " << accountId << " | Name: " << name 
              << " | Balance: $" << balance << std::endl;
}

std::string BankAccount::serialize() const {
    std::stringstream ss;
    ss << userId << "," << accountId << "," << name << "," << balance;
    return ss.str();
}

BankAccount BankAccount::deserialize(const std::string& data) {
    auto parts = splitString(data, ',');
    if (parts.size() >= 4) {
        int userId = std::stoi(parts[0]);
        std::string accountId = parts[1];
        std::string name = parts[2];
        double balance = std::stod(parts[3]);
        return BankAccount(userId, accountId, name, balance);
    }
    // Return a default account if data is invalid
    return BankAccount(0, "", "", 0.0);
}

// Bank methods implementation
void Bank::addAccount(const BankAccount& account) {
    accounts.push_back(account);
    saveAccounts();
}

BankAccount* Bank::findAccount(const std::string& accountId) {
    for (auto& account : accounts) {
        if (account.getAccountId() == accountId) {
            return &account;
        }
    }
    return nullptr;
}

std::vector<BankAccount> Bank::findAccountsByUserId(int userId) {
    std::vector<BankAccount> userAccounts;
    for (const auto& account : accounts) {
        if (account.getUserId() == userId) {
            userAccounts.push_back(account);
        }
    }
    return userAccounts;
}

void Bank::loadAccounts() {
    accounts.clear();
    std::ifstream file("accounts.csv");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                accounts.push_back(BankAccount::deserialize(line));
            }
        }
        file.close();
    }
}

void Bank::saveAccounts() {
    std::ofstream file("accounts.csv");
    if (file.is_open()) {
        for (const auto& account : accounts) {
            file << account.serialize() << std::endl;
        }
        file.close();
    }
}