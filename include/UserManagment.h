#ifndef USERMANAGMENT_H
#define USERMANAGMENT_H

#include "admin.h"
#include "user.h"
#include "customer.h"
#include "product.h"
#include <vector>
#include <string>
#include <vector>
#include <iostream>
#include <map>
using namespace std;

//τροποποιηση string για φορματ που ζητειται
string trim(const string& str);

class UserManagment{
    private:
        //προς χρηση και αποθηκευση χρηστων
        vector <User*> users;
        //προς αποθηκευση προιοντων
        vector <Product> products;
        //map με κατηγοριες σε συνδεση με υποκατηγοριες
        map<string, vector<string>> categories;
        //pointer δειχνει ποιον τρεχει το προγραμμα
        User* currentUser = nullptr;
        //input source(files/terminal)
        istream& input;
    public:
        //αρχικοποιηση input στον constructor
        UserManagment(istream& input = cin);
        ~UserManagment();

        //εγγραφη
        User* registerUser();
        //login
        User* login(const string& username, const string& password);
        //αποθηκευση και φορτωση χρηστων
        void saveUsersToFile(const string& filename);
        void loadUsersFromFile(const string& filename);

        //προσθηκη προιοντος σε vector
        void addProduct(const Product& product);
        //αποθηκευση vector στα αρχεια
        void saveProductsToFile(const string& filename);
        //φορτωση προιοντων και κατηγοριων
        void loadProductsFromFile(const string& filename);
        void loadCategoriesFromFile(const string& filename);

        //προς αποθηκευση vector
        vector<Product>& getProducts();
        //αποθηκευση ιστορικου και φορτωση ιστορικου
        void saveOrderHistory(const string& username, const string& orderData);
        string loadOrderHistory(const string& username);
};
#endif // USERMANAGMENT_H