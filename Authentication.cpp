#include "Authentication.h"
#include <sstream>
#include <iomanip>
#include <random>
#include <chrono>

// Password Hasher implementation
std::string PasswordHasher::generateSalt(size_t length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);
    
    std::string salt;
    salt.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        salt += alphanum[dis(gen)];
    }
    
    return salt;
}

std::string PasswordHasher::simpleHash(const std::string& input) {
    std::hash<std::string> hasher;
    auto hashValue = hasher(input);
    
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << hashValue;
    return ss.str();
}

std::string PasswordHasher::hashPassword(const std::string& password) {
    std::string salt = generateSalt();
    std::string combined = salt + password;
    
    // Multiple iterations to strengthen the hash
    std::string result = combined;
    for (int i = 0; i < 1000; i++) {
        result = simpleHash(result);
    }
    
    // Return salt:hash format
    return salt + ":" + result;
}

bool PasswordHasher::verifyPassword(const std::string& password, const std::string& storedHash) {
    size_t delimiterPos = storedHash.find(':');
    if (delimiterPos == std::string::npos) {
        return false;
    }
    
    std::string salt = storedHash.substr(0, delimiterPos);
    std::string storedHashValue = storedHash.substr(delimiterPos + 1);
    
    std::string combined = salt + password;
    std::string result = combined;
    for (int i = 0; i < 1000; i++) {
        result = simpleHash(result);
    }
    
    return storedHashValue == result;
}

// Authentication Manager implementation
AuthenticationManager::AuthenticationManager() : currentUserId(0) {}

std::string AuthenticationManager::generateSessionToken() {
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value = now_ms.time_since_epoch().count();
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 999999);
    
    std::stringstream ss;
    ss << std::hex << std::setw(16) << std::setfill('0') << value << dis(gen);
    return ss.str();
}

bool AuthenticationManager::registerUser(const std::string& username, const std::string& password) {
    // Check if username already exists
    User existingUser = storage.getUserByUsername(username);
    if (existingUser.getId() != 0) {
        return false; // User already exists
    }
    
    // Hash password
    std::string hashedPassword = PasswordHasher::hashPassword(password);
    
    // Create new user
    int userId = storage.getNextUserId();
    User newUser(userId, username, hashedPassword);
    
    // Save user
    return storage.saveUser(newUser);
}

bool AuthenticationManager::login(const std::string& username, const std::string& password) {
    User user = storage.getUserByUsername(username);
    if (user.getId() == 0) {
        return false; // User not found
    }
    
    if (user.isLocked()) {
        return false; // Account locked
    }
    
    if (!PasswordHasher::verifyPassword(password, user.getPasswordHash())) {
        // Update failed attempts count
        if (!user.attemptLogin("")) { // Pass empty string to ensure it fails but updates counter
            storage.saveUser(user);
        }
        return false; // Incorrect password
    }
    
    // Create session
    currentUserId = user.getId();
    currentSessionToken = generateSessionToken();
    Session session(currentUserId, currentSessionToken);
    storage.saveSession(session);
    
    return true;
}

bool AuthenticationManager::logout() {
    if (!isLoggedIn()) {
        return false;
    }
    
    bool result = storage.deleteSession(currentSessionToken);
    currentSessionToken = "";
    currentUserId = 0;
    return result;
}

bool AuthenticationManager::isLoggedIn() const {
    return !currentSessionToken.empty() && currentUserId > 0;
}

std::string AuthenticationManager::getCurrentSessionToken() const {
    return currentSessionToken;
}

int AuthenticationManager::getCurrentUserId() const {
    return currentUserId;
}

bool AuthenticationManager::validateSession(const std::string& token) {
    Session session = storage.getSessionByToken(token);
    if (session.getUserId() == 0 || !session.isValid()) {
        return false;
    }
    
    // Renew session
    session.renew();
    storage.saveSession(session);
    
    return true;
}