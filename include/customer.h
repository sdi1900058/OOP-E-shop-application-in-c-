#ifndef CUSTOMER_H
#define CUSTOMER_H

#include "user.h"
#include "product.h"
#include <vector>
using namespace std;

class UserManagment;


class Customer: public User{
    public:
        //constructor
        Customer(const string& username, const string& password);

        void displayRole() const override;
        //αναζήτηση 
        void searchProducts(const vector<Product>& products, const string& title, const string& category, const string& subcategory) const;
        //Aγορά
        void checkout(vector<Product> &Products, UserManagment& userManagment);
        //Εκτύπωση vector καλαθιού σε format
        void displaycart(const vector<Product>& products) const;
        // προσθήκη στο vector καλαθιού
        void addToCart(const string& productName, double requestedQuantity, vector<Product>& products);
        //αφαίρεση από το καλάθι
        void removeFromCart(const string& productName, vector<Product>& products);
        //Αλλαγή ποσότητας στο καλάθι
        void changeProductQuantity(const string& productName, double updatedQuantity, vector<Product>& products);
        //ιστορικό εκτύπωση
        void viewOrderHistory(UserManagment& userManagment) const;
    private:
        //καλάθι vector με pair τίτλος προιόντος - τιμής
        vector<pair <string,double>> cart;

};

#endif // customer.h