#ifndef ADMIN_H
#define ADMIN_H
using namespace std;
#include "user.h"
#include "product.h"
#include <vector>

//Αρκεί μόνο forward declaration
class UserManagment;

//κληρονομούμε τα στοιχεια της user
class Admin: public User{
    public:
        //constructor
        Admin(const string& username, const string& password);

        //επανοηματοδότηση της displayRole
        void displayRole() const override;

        //istream αναφορά, οπουδήποτε απαιτείται input για να λειτουργήσει η συνάρτηση
        //αναζήτηση προιόντων
        void searchProducts(istream& input, const vector<Product>& products) const;
        //προσθήκη
        void addProduct(istream& input,vector<Product>& products, UserManagment* userManagement);
        //αφαίρεση
        void removeProduct(istream& input,vector<Product>& products, UserManagment* userManagement);
        //Επεξεργασία μεμονομένων στοιχείων υπάρχοντων προιόντων
        void editProduct(istream& input,vector<Product>& products, UserManagment* userManagement);
        //εκτύπωση χωρίς απόθεμα
        void displayOutOfStock(const vector<Product>& products) const;
        //Εκτύπωση των πιο δημοφιλών 
        void displayPopularProducts(istream& input, const vector<Product>& Products) const;
};

#endif // ADMIN_H