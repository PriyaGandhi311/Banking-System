#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>
#include <map>
#include <ctime>

// Forward declarations to avoid circular dependencies
class User;
class Session;

// Utility function to split strings
std::vector<std::string> split(const std::string& s, char delimiter);

// User class to store authentication information
class User {
private:
    int id;
    std::string username;
    std::string passwordHash;
    int failedAttempts;
    bool locked;
    time_t lockTime;

public:
    User();
    User(int userId, const std::string& user, const std::string& hash);
    
    // Getters
    int getId() const;
    std::string getUsername() const;
    std::string getPasswordHash() const;
    bool isLocked() const;
    
    // Authentication methods
    bool checkPassword(const std::string& hashedPassword);
    bool attemptLogin(const std::string& hashedPassword);
    
    // Serialization
    std::string serialize() const;
    static User deserialize(const std::string& data);
};

// Session class to manage user sessions
class Session {
private:
    std::string token;
    int userId;
    time_t creationTime;
    time_t expiryTime;
    
public:
    Session();
    Session(int id, const std::string& sessionToken, int durationSeconds = 3600);
    
    // Getters
    std::string getToken() const;
    int getUserId() const;
    time_t getExpiryTime() const;
    
    // Session management
    bool isValid() const;
    void renew(int durationSeconds = 3600);
    
    // Serialization
    std::string serialize() const;
    static Session deserialize(const std::string& data);
};

// Storage class to handle file operations
class Storage {
private:
    const std::string USERS_FILE = "users.csv";
    const std::string SESSIONS_FILE = "sessions.csv";
    
    // Helper methods
    std::vector<std::string> readAllLines(const std::string& filename);
    void writeAllLines(const std::string& filename, const std::vector<std::string>& lines);
    
public:
    // User storage methods
    std::vector<User> getAllUsers();
    User getUserById(int id);
    User getUserByUsername(const std::string& username);
    bool saveUser(const User& user);
    int getNextUserId();
    
    // Session storage methods
    std::vector<Session> getAllSessions();
    Session getSessionByToken(const std::string& token);
    bool saveSession(const Session& session);
    bool deleteSession(const std::string& token);
};

#endif