#include "user.h"
using namespace std;

// Constructor
User::User(const string& username, const string& password)
     :username(username), password(password) {}
     

// Getter για το username και password
string User::getUsername() const {
    return username;
}
string User::getpassword() const {
    return password;
}

//εξακρίβωση password για το login
bool User::validatePassword(const std::string& inputPassword) const {
    return password == inputPassword;
}
