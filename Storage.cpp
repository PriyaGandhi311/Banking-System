#include "Storage.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

// Utility function to split strings
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// User class implementation
User::User() : id(0), failedAttempts(0), locked(false), lockTime(0) {}

User::User(int userId, const std::string& user, const std::string& hash) 
    : id(userId), username(user), passwordHash(hash), failedAttempts(0), locked(false), lockTime(0) {}

int User::getId() const { 
    return id; 
}

std::string User::getUsername() const { 
    return username; 
}

std::string User::getPasswordHash() const { 
    return passwordHash; 
}

bool User::isLocked() const { 
    return locked; 
}

bool User::checkPassword(const std::string& hashedPassword) {
    return passwordHash == hashedPassword;
}

bool User::attemptLogin(const std::string& hashedPassword) {
    if (locked) {
        time_t now = time(nullptr);
        if (now - lockTime < 300) { // 5 minutes lockout
            return false;
        }
        locked = false;
        failedAttempts = 0;
    }
    
    if (passwordHash == hashedPassword) {
        failedAttempts = 0;
        return true;
    }
    
    failedAttempts++;
    if (failedAttempts >= 3) {
        locked = true;
        lockTime = time(nullptr);
    }
    return false;
}

std::string User::serialize() const {
    std::stringstream ss;
    ss << id << "," << username << "," << passwordHash << "," 
       << failedAttempts << "," << (locked ? 1 : 0) << "," << lockTime;
    return ss.str();
}

User User::deserialize(const std::string& data) {
    User user;
    std::vector<std::string> parts = split(data, ',');
    if (parts.size() >= 6) {
        user.id = std::stoi(parts[0]);
        user.username = parts[1];
        user.passwordHash = parts[2];
        user.failedAttempts = std::stoi(parts[3]);
        user.locked = (parts[4] == "1");
        user.lockTime = std::stol(parts[5]);
    }
    return user;
}

// Session class implementation
Session::Session() : userId(0), creationTime(0), expiryTime(0) {}

Session::Session(int id, const std::string& sessionToken, int durationSeconds) 
    : token(sessionToken), userId(id) {
    creationTime = time(nullptr);
    expiryTime = creationTime + durationSeconds;
}

std::string Session::getToken() const { 
    return token; 
}

int Session::getUserId() const { 
    return userId; 
}

time_t Session::getExpiryTime() const { 
    return expiryTime; 
}

bool Session::isValid() const {
    return time(nullptr) < expiryTime;
}

void Session::renew(int durationSeconds) {
    expiryTime = time(nullptr) + durationSeconds;
}

std::string Session::serialize() const {
    std::stringstream ss;
    ss << token << "," << userId << "," << creationTime << "," << expiryTime;
    return ss.str();
}

Session Session::deserialize(const std::string& data) {
    Session session;
    std::vector<std::string> parts = split(data, ',');
    if (parts.size() >= 4) {
        session.token = parts[0];
        session.userId = std::stoi(parts[1]);
        session.creationTime = std::stol(parts[2]);
        session.expiryTime = std::stol(parts[3]);
    }
    return session;
}

// Storage class implementation
std::vector<std::string> Storage::readAllLines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
    }
    return lines;
}

void Storage::writeAllLines(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream file(filename);
    if (file.is_open()) {
        for (const auto& line : lines) {
            file << line << std::endl;
        }
        file.close();
    }
}

std::vector<User> Storage::getAllUsers() {
    std::vector<User> users;
    for (const auto& line : readAllLines(USERS_FILE)) {
        if (!line.empty()) {
            users.push_back(User::deserialize(line));
        }
    }
    return users;
}

User Storage::getUserById(int id) {
    for (const auto& user : getAllUsers()) {
        if (user.getId() == id) {
            return user;
        }
    }
    return User(); // Return empty user if not found
}

User Storage::getUserByUsername(const std::string& username) {
    for (const auto& user : getAllUsers()) {
        if (user.getUsername() == username) {
            return user;
        }
    }
    return User(); // Return empty user if not found
}

bool Storage::saveUser(const User& user) {
    std::vector<User> users = getAllUsers();
    bool updated = false;
    
    for (auto& existingUser : users) {
        if (existingUser.getId() == user.getId()) {
            existingUser = user;
            updated = true;
            break;
        }
    }
    
    if (!updated) {
        users.push_back(user);
    }
    
    std::vector<std::string> lines;
    for (const auto& u : users) {
        lines.push_back(u.serialize());
    }
    
    writeAllLines(USERS_FILE, lines);
    return true;
}

int Storage::getNextUserId() {
    int maxId = 0;
    for (const auto& user : getAllUsers()) {
        maxId = std::max(maxId, user.getId());
    }
    return maxId + 1;
}

std::vector<Session> Storage::getAllSessions() {
    std::vector<Session> sessions;
    for (const auto& line : readAllLines(SESSIONS_FILE)) {
        if (!line.empty()) {
            sessions.push_back(Session::deserialize(line));
        }
    }
    return sessions;
}

Session Storage::getSessionByToken(const std::string& token) {
    for (const auto& session : getAllSessions()) {
        if (session.getToken() == token) {
            return session;
        }
    }
    return Session(); // Return empty session if not found
}

bool Storage::saveSession(const Session& session) {
    std::vector<Session> sessions = getAllSessions();
    bool updated = false;
    
    for (auto& existingSession : sessions) {
        if (existingSession.getToken() == session.getToken()) {
            existingSession = session;
            updated = true;
            break;
        }
    }
    
    if (!updated) {
        sessions.push_back(session);
    }
    
    // Clean up expired sessions
    std::vector<Session> validSessions;
    time_t now = time(nullptr);
    for (const auto& s : sessions) {
        if (s.getExpiryTime() > now) {
            validSessions.push_back(s);
        }
    }
    
    std::vector<std::string> lines;
    for (const auto& s : validSessions) {
        lines.push_back(s.serialize());
    }
    
    writeAllLines(SESSIONS_FILE, lines);
    return true;
}

bool Storage::deleteSession(const std::string& token) {
    std::vector<Session> sessions = getAllSessions();
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(), 
        [&token](const Session& s) { return s.getToken() == token; }), sessions.end());
    
    std::vector<std::string> lines;
    for (const auto& s : sessions) {
        lines.push_back(s.serialize());
    }
    
    writeAllLines(SESSIONS_FILE, lines);
    return true;
}