#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

//Base User class
class User { 
    protected:
        //Όνομα χρήστη και κωδικός στα protected 
        //θέλουμε να κληρονομηθούν από admin/customer
        string username;
        string password;
    public:
        User(const string& username , const string& password);
        //virtual destructor για διασφλάλιση σωστής απελευθέρωσης μνήμης από τις Derived κλάσεις και συντόμευση κώδικα
        virtual ~User()=default;

        //για χρήση κατά βάση από την UserManagment
        string getUsername() const;
        string getpassword()    const;
        bool validatePassword(const string& inputPassword) const;
      
        //=0 γιατί δεν έχουμε υλοποίηση σε αυτή τη κλάση
        virtual void displayRole() const =0;


};



#endif