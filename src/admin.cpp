#include "admin.h"
#include "UserManagment.h"
#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <algorithm>
using namespace std;

class Product;

//Constructor
Admin :: Admin (const string& username1, const string& password1)
       :User(username1, password1) {}

void Admin :: displayRole() const {
     cout << "Admin privileges granted" << endl;
}

void Admin :: searchProducts (istream& input, const vector<Product> &products) const{

    string title , category, subcategory;
    cout<< "=== Product Search ==="<< endl;

    //οποιδήποτε prompt απαντηθει με enter θα μείνει κενό 
    cout<< "Enter the title of the product you are looking for or press Enter to skip: "<<endl;
    //clean buffer
    input.ignore();
    getline(input, title);
    cout<< "Enter the category of the product you are looking for or press Enter to skip: "<<endl;
    getline(input, category);
    cout<< "Enter the subcategory of the product you are looking for or press Enter to skip: "<<endl;
    getline(input, subcategory);

    cout<< "=== Search Results ==="<< endl;
    //auto για κάθε μεταβλητή ενός μέλους της product
    for (const auto& product : products){
        //οποιοδήποτε πεδίο είναι κενό επιστρέφει true
        if ((title.empty() || product.getTitle() == title) &&
            (category.empty() || product.getCategory() == category) &&
            (subcategory.empty() || product.getSubCategory() == subcategory)){
                //έλεγχος αν είναι admin αντί για σκέτο display
                product.displayProductChecking(product, true);}
    }
}

void Admin::removeProduct(istream& input, vector<Product>& products, UserManagment* userManagement) {

    string title;
    cout << "===Remove a Product===" << endl;
    cout << "Enter the title of the product you want to remove: ";
    //clean buffer
    input.ignore();
    getline(input, title);

    //Από το πρώτο στο τελευταίο στοιχείο του vector 
    auto it = find_if(products.begin(), products.end(), [&title](const Product& product) {
        //σύγκριση με καθε τίτλο
        //το [&title] διασφαλίζει ότι έχουμε πρόσβαση στον εκάστοτε
        return product.getTitle() == title;
    });

    //Αν πλέον έχουμε φτάσει στο τέλος και η συνάρτηση δεν έχει βρει
    //σημαινει οτι ο τιτλος ειναι λαθος
    if (it == products.end()) {
        cout << "Product not found!" << endl;
        return;
    }

    //Αποδεσμεύουμε τον pointer και αν έγιναν αλλαγές τις αποθηκεύουμε
    products.erase(it);
    if (userManagement != nullptr) {
        userManagement->saveProductsToFile("files/products_copy.txt");
    }
    cout << "Product successfully removed!" << endl;
}


void Admin::addProduct(istream& input, vector<Product>& products, UserManagment* userManagement) {
    string title,description,category, subcategory,unitMeasurement;
    double price, quantity;

    //Φόρτωση των κατηγοριών και των υποκατηγοριών σε map
    map<string,vector<string>> categories;
    ifstream categoryFile("files/categories.txt");
    if (!categoryFile.is_open()){
        cout << "Error: Could not open categories.txt." << endl;
        return;
    }
    string line;
    //αντλούμε τις κατηγορίες από το πρώτο στοιχείο της γραμμής 
    while (getline(categoryFile, line)) {
        //αντλούμε τις υποκατηγορίες και την ορθη αντιστοίχιση απο την παρένθεση που ακολουθεί
        size_t pos = line.find('(');
        string mainCategory = trim(line.substr(0, pos - 1)); 
        //trim για να κοψουμε κενα που θα οδηγησουν σε σφαλματα
        string subcategoriesString = trim ( line.substr(pos + 1, line.size() - pos - 2)); 
        stringstream ss(subcategoriesString);
        string sub;
        vector<string> subcategories;
        while (getline(ss, sub, '@')) {
            subcategories.push_back(trim(sub));
        }
        categories[mainCategory] = subcategories;
    }
    cout << "Give product title: ";
    //clean buffer
    input.ignore();
    getline(input, title);
    title = trim(title);  

    cout << "Give product description: ";
    getline(input, description);
    description = trim(description); 

    // Εμφάνιση όλων των αποθηκευμένων κατηγοριώνκαι εισαγωγή κατηγορίας
    cout << "Give one of the following categories: ";
    for (const auto& [mainCategory, _] : categories) {
        cout << mainCategory << " ";
    }
    cout << endl;

    getline(input, category);
    //trim για κενα
    category = trim(category);  
    if (categories.find(category) == categories.end()) {
        cout << "Invalid category. Please try again." << endl;
        return;
    }
    //αντίστοιχα με subcategory
    cout << "Give one of the following subcategories: ";
    for (const auto& sub : categories[category]) {
        cout << sub << " ";
    }
    cout << endl;

    getline(input, subcategory);
    subcategory = trim(subcategory); 
    auto it = find(categories[category].begin(), categories[category].end(), subcategory);
    if (it == categories[category].end()) {
        cout << "Invalid subcategory. Please try again." << endl;
        return;
    }

    cout << "Give product price: ";
    input >> price;
    if (price <= 0) {
        cout << "Invalid price. Please try again." << endl;
        return;
    }

    cout << "Give measurement type[Kg/Unit]: ";
    //buffer clean γτ δε βαλαμε πριν getline
    input.ignore();
    getline(input, unitMeasurement);
    unitMeasurement = trim(unitMeasurement); 
    if (unitMeasurement != "Kg" && unitMeasurement != "Unit") {
        cout << "Invalid measurement type. Please try again." << endl;
        return;
    }

    cout << "Give amount of " << unitMeasurement << ": ";
    input >> quantity;
    if (quantity < 0) {
        cout << "Invalid quantity. Please try again." << endl;
        return;
    }

    //αποθήκευση στο vector / στο αρχείο
    products.emplace_back(title, description, category, subcategory, unitMeasurement, price, quantity);
    if (userManagement) {
        userManagement->saveProductsToFile("files/products_copy.txt");
    }
    cout << "Product added successfully!" << endl;
}

void Admin:: editProduct(istream& input,vector<Product>& products, UserManagment* userManagement){
    //title για την αναζήτηση
    string title;
    cout<< "=== Edit a Product ==="<<endl;
    cout<< "Enter the title of the product you want to edit: "<<endl;
    input.ignore();
    getline(input, title );

    //σειριακή αναζητηση στο vector
    auto it = find_if(products.begin(), products.end(), [&title](const Product& product){
        return product.getTitle() == title;
    });

    if(it ==products.end()){
        cout << "Product not found" << endl;
        return;
    }

    cout<<"Enter number of field you want to edit: 1.Title 2.Description 3.Category and Subcategory 4.Price 5.Available Kg 6.Nothing"<<endl;
    int choice;
    input>>choice;
   switch (choice) {
    case 1: {
        cout << "Enter new title: ";
        //clean buffer + απαραίτητο για να αγνοηθεί οποιοδηποτε υπόλοιπο
        input.ignore(numeric_limits<streamsize>::max(), '\n'); 

        string newTitle;
        getline(input, newTitle);
        if (!newTitle.empty()) {
            it->setTitle(newTitle);
        } else {
            cout << "Title was not changed " << endl;
        }
        break;
    }
    case 2: {
        cout << "Enter new description: "<<endl;
        input.ignore(numeric_limits<streamsize>::max(), '\n');
        string newDescription;
        getline(input, newDescription);
        if (!newDescription.empty()) {
            it->setDescription(newDescription);
        } else {
            cout << "Description was not changed" << endl;
        }
        break;
    }
    case 3: {
        //na prosthesw mesw categories.txt///////////////////
        cout << "Enter new category: "<<endl;
        input.ignore(numeric_limits<streamsize>::max(), '\n');
        string newCategory;
        getline(input, newCategory);
        if (!newCategory.empty()) {
            it->setCategory(newCategory);
        }
        cout << "Enter new subcategory: ";
        string newSubCategory;
        getline(input, newSubCategory);
        if (!newSubCategory.empty()) {
            it->setSubCategory(newSubCategory);
        }
        break;
    }
    case 4: {
        cout << "Enter new price: ";
        double newPrice;
        if (input >> newPrice && newPrice >= 0) {
            it->setPrice(newPrice);
        } else {
            cout << "Price was not changed"  << endl;
        }
        input.ignore(numeric_limits<streamsize>::max(), '\n'); 
        break;
    }
    case 5: {
        cout << "Enter new available quantity: ";
        double newQuantity;
        if (input >> newQuantity && newQuantity >= 0) {
            it->setQuantity(newQuantity);
        } else {
            cout << "Quantity was not changed" << endl;
        }
        input.ignore(numeric_limits<streamsize>::max(), '\n'); 
        break;
    }
    case 6:
        cout << "No changes made" << endl;
        break;
    default:
        cout << "Invalid choice" << endl;
        break;
}
    //Αποθήκευση στα αρχεία
    userManagement->saveProductsToFile("files/products_copy.txt");
    cout << "editing has been completed" <<endl;
}

void Admin :: displayOutOfStock (const vector<Product>& products) const{
    cout<< "\n===Out of stock Products===\n"<< endl;
    bool found= false; //flag

    for(const auto& product: products){
        if(product.getQuantity()==0){
            product.displayProductDetails();
            found = true;
        }
    }
    //όλα έχουν απόθεμα
    if(!found){
        cout<< "No out of stock products found" <<endl;
    }
}

void Admin:: displayPopularProducts(istream& input,const vector<Product>& products)const{
    cout << "===Most Popular Products==="<<endl;

    //temp array για sorting με σύγκριση ένα προς ένα
    vector<Product> sortedProducts = products;
    sort(sortedProducts.begin(), sortedProducts.end(), [](const Product& a, const Product& b){
        return a.getOrderCount() > b.getOrderCount();
        });
    //δυνατότητα στον χρήστη να δει τα top3/5/10 κλπ
    size_t posa;
    cout <<" How many products would you like me to print " << endl;
    input>> posa;

    if(posa > sortedProducts.size()){
        cout << "the number you entered exceeds the total products sold number, will print them all" <<endl;
        posa = sortedProducts.size();
    }
    for(size_t i = 0; i<posa; ++i){
        cout<< "\nRank number "<< i+1 <<endl;
        sortedProducts[i].displayProductDetails();
        cout<< "Times ordered: "<<sortedProducts[i].getOrderCount()<<endl;
    }
    if(posa==0) cout <<"invalid choise" <<endl;
}