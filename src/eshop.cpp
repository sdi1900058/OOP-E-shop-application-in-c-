#include "eshop.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

// ο constructor δεχεται το input
Eshop::Eshop(istream& input) : input(input), userManagement(input) {}

//φορτωση αρχειων και εκκινηση
void Eshop::run(const string& categoriesFile, const string& productsFile, const string& usersFile) {
   //αρχικοποιηση
    userManagement.loadUsersFromFile(usersFile);
    userManagement.loadCategoriesFromFile(categoriesFile);
    userManagement.loadProductsFromFile(productsFile);

    cout << "Welcome to the e-shop!!" << endl;
    displayMainMenu();
    //σε περιπτωση που κατι ξεφευγει / last resort
    userManagement.saveUsersToFile(usersFile);
    userManagement.saveProductsToFile(productsFile);
}

void Eshop::displayMainMenu() {
    while (true) {
        string choice;
        cout << "Do you want to login or register? (enter option):";
        input >> choice;

        if (choice == "login") {
            string username, password;
            cout << "Please enter your username: ";
            input >> username;
            cout << "Please enter your password: ";
            input >> password;

            User* user = userManagement.login(username, password);
            if (user) {
                cout << "Welcome " << username << "!" << endl;
                //έλεγχος αν το αντικείμενο user έχει ιδιότητες admin 
                if (Admin* admin = dynamic_cast<Admin*>(user)) {
                    adminMenu(admin);
                //ελεγχος αν ustomer
                } else if (Customer* customer = dynamic_cast<Customer*>(user)) {
                    customerMenu(customer);
                }
                return; 
            } else {
                cout << "Invalid username or password, please try again." << endl;
            }
        } else if (choice == "register") {
            User* newUser = userManagement.registerUser();
             if (newUser) {
                //μεταβαση στον νεο χρηστη
                cout << "Thanks for signing up! You are automatically logged in as " << newUser->getUsername() << endl;

                // αντιστοιχος ελεγχος με πριν
                if (Admin* admin = dynamic_cast<Admin*>(newUser)) {
                    adminMenu(admin);
                } else if (Customer* customer = dynamic_cast<Customer*>(newUser)) {
                    customerMenu(customer);
                }
                return; 
            }
        } else if (choice == "exit") {
            //τερματισμός
            cout << "Goodbye!" << endl;
            return; 
        } else {
            cout << "try again." << endl;
        }
    }
}

void Eshop::adminMenu(User* admin) {
    //ελεγχος σε περιπτωση που κατι στραβωσε
    Admin* adminPtr = dynamic_cast<Admin*>(admin);
    if (!adminPtr) {
        cout << "Invalid admin user!" << endl;
        return;
    }
    //αρχικοποιηση του vector
    vector<Product>& products = userManagement.getProducts();  

    while (true) {
        cout << "---Admin Menu---" << endl;
        cout << "1. Add Product\n2. Edit Product\n3. Remove Product\n""4. Search Product\n5. Show Unavailable Products\n"
             << "6. Show Top 5 Products\n7. Exit\nEnter your choice: ";
        int choice;
        input >> choice;
        switch (choice) {
            case 1:
                adminPtr->addProduct(input ,products, &userManagement);  
                break;
            case 2:
                adminPtr->editProduct(input  , products, &userManagement);
                break;
            case 3:
                adminPtr->removeProduct(input,products, &userManagement);
                break;
            case 4:
                adminPtr->searchProducts(input ,products);
                break;
            case 5:
                adminPtr->displayOutOfStock(products);
                break;
            case 6:
                adminPtr->displayPopularProducts(input ,products);
                break;
            case 7:
                cout << "exiting the e-shop" << endl;
                exit(0);
            default:
                cout << "try again." << endl;
        }
    }
}



//menu πελατών
void Eshop::customerMenu(Customer* customer) {
    //ελεγχος και μετατροπη customer σε Customer*
    Customer* customerPtr = dynamic_cast<Customer*>(customer);
    while (true) {
        cout << "---Customer Menu---" << endl;
        cout << "1. Search for a Product\n2.Add product to cart\n3.Update product from cart\n"
             <<"4.Remove product from cart\n5.Complete order\n6.View order history\n7.View cart\n8.Exit\n ";
        cout << "Enter your choice: ";
        int choice;
        input >> choice;
        //αρχικοποιηση του vector products
        vector<Product>& products = userManagement.getProducts();

        switch (choice) {
            case 1:
                {string title, category, subcategory;
                cout << "Enter the title of the product or press Enter to skip: ";
                input.ignore();
                getline(input, title);
                cout << "Enter the category or press Enter to skip: ";
                getline(input, category);
                cout << "Enter the subcategory or press Enter to skip: ";
                getline(input, subcategory);
                //κληση βαση επιλογων την αναζητηση
                customer->searchProducts(products, title, category, subcategory);
                break;}
            case 2:
                {string productName;
                double quantity;
                //κληση βαση ονοματος την προσθηκη
                cout << "Which product do you want to add? ";
                input.ignore();
                getline(input, productName);
                cout<<"Enter quantity: ";
                input>>quantity;
                customerPtr->addToCart(productName,quantity,products);
                break;}
            case 3:
                {string productName;
                double quantity;
                //κληση βαση ονοματος και ποσοτητας την αλλαγη ποσοστητας
                cout << "Which product do you want to update? ";
                input.ignore();
                getline(input, productName);
                cout << "Enter new quantity: ";
                input >> quantity;
                customerPtr->changeProductQuantity(productName, quantity, products);
                break;}
            case 4:
                {string productName;
                cout << "Which product do you want to remove? ";
                //κληση βαση ονοματος την αφαιρεση
                input.ignore();
                getline(input, productName);
                customerPtr->removeFromCart(productName, products);
                break;}
            case 5:
                //αγορα
                customerPtr->checkout(products, userManagement);
                break;
            case 6:
            //ιστορικο
                customerPtr->viewOrderHistory(userManagement);
                break;
            case 7:
                //εκτυπωση τρεχον vector cart
                customerPtr->displaycart(products);
                break;
            case 8:
                cout<< "Goodbye!"<<endl;
                exit(0);
            default:
                cout << "Invalid choice. Try again." << endl;
                break;
        }
    }
}
