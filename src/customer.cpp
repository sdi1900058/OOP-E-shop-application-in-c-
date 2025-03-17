#include "customer.h"
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <algorithm>
#include "product.h"
#include "UserManagment.h"


using namespace std;

//constructor
Customer::Customer(const string& username, const string& password)
       :User(username, password) {}

void Customer::displayRole() const {
    cout << "Customer welcome" << endl;
}
void Customer::searchProducts(const vector<Product>& products, const string& title, const string& category, const string& subcategory) const {
    cout << "\n===Search Results===\n" << endl;
    //δίνονται τα ορίσματα, απλά ψάχνει στο vector βάση κενών και αντιστοιχιών
    for (const auto& product : products) {
        if ((title.empty() || product.getTitle() == title) &&
            (category.empty() || product.getCategory() == category) &&
            (subcategory.empty() || product.getSubCategory() == subcategory)) {
            product.displayProductChecking(product, false);
        }
    }
}

void Customer::checkout(vector<Product>& products, UserManagment& userManagement) {
    
    if (cart.empty()) {
        cout << "The cart is empty. Add products before checking out." << endl;
        return;
    }

    //αρχικοποίηση συνολικής τιμής - string προς αποθήκευση - flag για ολοκλήρωση
    double totalCost = 0.0;
    string orderData = "\n\n---CART START---\n";
    bool hasValidItems = false;

    //αναζήτηση όπως και σε άλλα, με πρόσθετη παράμετρο quantity
    for (const auto& [productName, quantity] : cart) {
        auto it = find_if(products.begin(), products.end(), [&](Product& product) {
            return product.getTitle() == productName;
        });

        if (it != products.end()) {
            //αν υπαρχει διαθέσιμη ποσότητα (το checkAndupdateQuantity είναι bool)
            //ίσως οδηγεί σε διπλοκράτηση ποσότητας / να τσεκάρω
            if (it->checkAndupdateQuantity(quantity)) {
                // Αποθήκευση ποσότητας όπως ζητείται / κουσούρι από όταν είχα τη ποσότητα double
                ostringstream formattedQuantity;
                formattedQuantity << fixed << setprecision(0) << quantity;

                //ενημέρωση string προς αποθήκευση
                orderData += formattedQuantity.str() + " " + productName + "\n";

                // υπολογισμός κόστους
                totalCost += it->getPrice() * quantity;
                it->increaseOrderCount();
                hasValidItems = true;
            } else {
                cout << "Not enough of " << productName << endl;
            }
        }
    }

    if (hasValidItems) {
        //προσθήκη Total Cost με ακρίβεια 2 δεκαδικών όπως ζητείται 
        ostringstream formattedTotalCost;
        formattedTotalCost << std::fixed << std::setprecision(2) << totalCost;

        orderData += "---CART END---\nTotal Cost: " + formattedTotalCost.str() + "\n\n";

        //αποθήκευση παραγγελίας
        userManagement.saveOrderHistory(getUsername(), orderData);
        userManagement.saveProductsToFile("files/products_copy.txt");
        cout << "Checkout completed successfully!" << endl;
    } else {
        cout << "Checkout failed. No valid items in the cart." << endl;
    }

    //μηδενισμός vector
    cart.clear(); 
}

void Customer::addToCart(const string& productName, double requestedQuantity, vector<Product>& products){
    //αναζήτηση σειριακά
    auto it=find_if(products.begin(),products.end(), [&](Product& product){
        return product.getTitle() == productName;
    });
    if (it ==products.end()){
        cout << "Product not found" << productName <<endl;
        return;
    }
    //ελέγχει και δεσμεύει τη ποσότητα
    if (!it->checkAndupdateQuantity(requestedQuantity)){
        cout <<"No stock available for "<<productName<<endl;
        return;
    }

    //αν υπάρχει ήδη ενημερώνει το ήδη υπάρχων (μειονέκτημα του vector)
    for (auto& [name, quantity] : cart){
        if( name == productName){
            quantity +=requestedQuantity;
            cout<< "updated quantity for "<< productName << " to " <<quantity<< endl;
            return;
        }
    }
    //προσθηκη στο καλάθι
    cart.emplace_back(productName, requestedQuantity);
    cout << "added "<< productName << " to cart with quantity "<< requestedQuantity <<endl;
}

void Customer::removeFromCart(const string& productName, vector<Product>& products) {
    //σειριακη στο products
    auto it = find_if(products.begin(), products.end(), [&](Product& product) {
        return product.getTitle() == productName;
    });

    if (it == products.end()) {
        cout << "Product not found in cart: " << productName << endl;
        return;
    }

    // Εύρεση του προϊόντος στο καλάθι
    for (size_t i = 0; i < cart.size(); ++i) {
        //σύγκριση τίτλων
        if (cart[i].first == productName) { 
            //επαναφορά ποσότητας καλαθιού
            it->releaseQuantity(cart[i].second);
            //αφαίρεση 
            cart.erase(cart.begin() + i); 
            cout << " Removed " << productName << " from cart" << endl;
            return;
        }
    }

    cout << " Product not found in cart: " << productName << endl;
}

void Customer::changeProductQuantity(const string& productName, double updatedQuantity, vector<Product>& products){
    //σειριακη
    auto it = find_if(products.begin(), products.end(), [&](Product& product){
        return product.getTitle()== productName;
    });

    if(it==products.end()){
        cout<<"Product not found in stock: "<<productName<<endl;
        return;
    }

    //έλεγχος βάση ονόματος- ποσότητας
    for(auto& [name, quantity] : cart){
        if(name ==productName){
            //απελευθέρωση ποσότητας που μένει
            it->releaseQuantity(quantity);
            //προσθήκη ποσότητας αν είναι εφικτό
            if ( !it -> checkAndupdateQuantity(updatedQuantity)){
                cout <<"Not enough stock left "<< endl;
                return;
            }
        //αλλαγή
        quantity = updatedQuantity;
        cout <<"updated quantity for "<<productName<< " to " << quantity<< endl;
        return;
        }
    }
    cout << productName << " not found in cart." << endl;
}

void Customer::displaycart(const vector<Product>& products) const{
    cout << "---CART START---"<<endl;
    double totalCost=0.0;
    //όπως και στο checkout με διαφορά ότι δεν χρειάζεται string αποθήκευσης
    for (const auto&[productName, quantity]:cart){
        auto it=find_if(products.begin(),products.end(), [&](const Product& product){
            return product.getTitle()==productName;
        });

        if(it !=products.end()){
            cout<< quantity<<" "<<productName<<endl;
            totalCost+= it->getPrice()* quantity;
        }else{
            cout<<"Product "<<productName << "Not found" <<endl;

        }
    }
    //διατήρηση του επιθυμητου φορματ
    cout<<"--CART END---"<<endl;
    cout<<"Total Cost: "<<totalCost<<endl;
}

void Customer::viewOrderHistory(UserManagment& UserManagment)const{
    //φορτωνουμε απο τα αρχεια
    string history = UserManagment.loadOrderHistory(getUsername());

    if(history.empty()){
        cout<< "No order history"<<endl;
    }else{
        cout<<"=== Order History ==="<<endl;
        cout<< history<<endl;
    }
}