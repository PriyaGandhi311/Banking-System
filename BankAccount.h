#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>
#include <iostream>
#include <vector>

// Bank Account class
class BankAccount {
private:
    int userId;            // Added to link with user authentication
    std::string accountId;
    std::string name;
    double balance;

public:
    // Constructor
    BankAccount(int userId, const std::string& accountId, const std::string& name, double initialBalance = 0.0);
    
    // Getters
    int getUserId() const;
    std::string getAccountId() const;
    std::string getName() const;
    double getBalance() const;
    
    // Operations
    bool deposit(double amount);
    bool withdraw(double amount);
    void displayBalance() const;
    
    // Serialization for storage
    std::string serialize() const;
    static BankAccount deserialize(const std::string& data);
};

// Bank class to manage multiple accounts
class Bank {
private:
    std::vector<BankAccount> accounts;
    
public:
    // Add an account
    void addAccount(const BankAccount& account);
    
    // Find account by ID
    BankAccount* findAccount(const std::string& accountId);
    
    // Find accounts by user ID
    std::vector<BankAccount> findAccountsByUserId(int userId);
    
    // Load all accounts from storage
    void loadAccounts();
    
    // Save all accounts to storage
    void saveAccounts();
};

#endif