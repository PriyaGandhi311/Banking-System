#ifndef AUTHENTICATION_H
#define AUTHENTICATION_H

#include "Storage.h"
#include <string>

// Password hashing class
class PasswordHasher {
private:
    static std::string generateSalt(size_t length = 16);
    static std::string simpleHash(const std::string& input);

public:
    static std::string hashPassword(const std::string& password);
    static bool verifyPassword(const std::string& password, const std::string& storedHash);
};

// Authentication manager class
class AuthenticationManager {
private:
    Storage storage;
    std::string currentSessionToken;
    int currentUserId;
    
    // Generate a random session token
    std::string generateSessionToken();

public:
    AuthenticationManager();
    
    // User management
    bool registerUser(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    bool logout();
    bool isLoggedIn() const;
    
    // Session management
    std::string getCurrentSessionToken() const;
    int getCurrentUserId() const;
    bool validateSession(const std::string& token);
};

#endif