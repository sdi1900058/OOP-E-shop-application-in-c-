#include "UserManagment.h"
#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

//τριμαρει αχρείαστα κενά / επιστρεφει κενο string αν ειναι πολλα κενα
string trim(const string& str) {
    //απο το πρωτο μη κενο
    size_t first = str.find_first_not_of(' ');
    //κενο string αν δεν εχει χαρακτηρες
    if (first == string::npos) return ""; 
    //στο τελευταιο μη κενο
    size_t last = str.find_last_not_of(' ');
    //επιστροφη τροποποιημενο
    return str.substr(first, (last - first + 1));
}
//αρχικοποιηση ληψης input στο constructor
UserManagment::UserManagment(istream& input):input(input){}

UserManagment::~UserManagment(){
    for(User* user: users){
        delete user;
    }
}

User* UserManagment::registerUser() {
    //αρχικοποιηση απαραιτητων μεταβλητων
    string username, password;
    bool isAdmin = false; 
    string option;

    cout << "Enter username: ";
    input >> username;
    cout << "Enter password: ";
    input >> password;

    while (true) {
        cout << "Is user an admin? (y for yes/n for no): ";
        input >> option;
        //case sensitive
        if (option == "Y" || option == "y") {
            isAdmin = true;
            break;
        } else if (option == "N" || option == "n") {
            isAdmin = false;
            break;
        } else {
            cout << "Invalid input. Please try again." << endl;
            //clear buffer
            input.clear();
            input.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    
    auto it = find_if(users.begin(), users.end(), [&](const User* user) {
        return user->getUsername() == username;
    });
    //ελεγχος για ιδιο username
    if (it != users.end()) {
        cout << "Username already exists." << endl;
        return nullptr;
    }
    //αρχικοποιηση του user και δημιουργια νεου αντικειμενου
    User* newUser = nullptr;
    if (isAdmin) {
        newUser = new Admin(username, password);
    } else {
        newUser = new Customer(username, password);
    }
    //αποθηκευση
    users.push_back(newUser);
    saveUsersToFile("files/users_copy.txt");
    return newUser; //επιστροφη ως τρεχων χρηστης
}


User* UserManagment::login(const string&username, const string& password){
    //ελεγχος αν υπαρχει username σε αντιστοιχια password σειριακα
   auto it= find_if(users.begin(), users.end(), [&](const User* user){
        return user->getUsername()==username&&user->validatePassword(password);
   });
    if(it!=users.end()){
        return *it;
    }
    //αν απετυχε επιστροφη κενου pointer
    return nullptr;
}

void UserManagment::saveUsersToFile(const string& filename){
    //ελεγχος του αρχειου αν ανοιγει σωστα
    ofstream outfile(filename);
    if(!outfile){
        cout<<"Error with file"<<filename<<endl;
        return;
    }
    for(const auto& user:users){
        //αποθηκευση βάση isAdmin με ζητουμενο format
        string userType = dynamic_cast<Admin*>(user) ? "1" : "0";
        outfile<<user->getUsername() << "," <<user->getpassword() << "," << userType << endl;}
        cout<<"users saved to "<< filename <<endl;
        outfile.close();
}

void UserManagment::loadUsersFromFile(const string& filename){
    //ελεγχος του αρχειου αν ανοιγει σωστα
    ifstream inFile(filename);
    if(!inFile){
        cout<< "Error opening file line 81 usermanagment.cpp "<<filename<<endl;
        return;
    }
    string line;
    while(getline(inFile,line)){
        //αναζήτηση στο αρχείο βάση γραμμής χωρίς περιττά σύμβολα ( , )
        stringstream ss (line);
        string username, password, userType;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, userType, ',');
        bool isAdmin = userType == "1";
        if(isAdmin){
            //κληση αντιστοιχου constructor
            users.push_back(new Admin(username, password));
        } else {
            users.push_back(new Customer(username, password));
        }
    }
    inFile.close();
    cout<<" users loaded from file "<< filename <<endl;
}

///Products//////////////////// 

//προσθήκη στο vector 
void UserManagment::addProduct(const Product& product){
    products.push_back(product);
    cout<<"product added"<<endl;
}

void UserManagment::saveProductsToFile(const string& filename){
    //ελεγχος του αρχειου αν ανοιγει σωστα
    ofstream outFile(filename);
    if(!outFile){
        cout<<"Error with file"<<filename<<endl;
        return;
    }
    //αν δεν εχει αρχικοποιηθει σωστα error
    for (const auto& product : products){
        if (product.getTitle().empty() || product.getCategory().empty() || 
            product.getSubCategory().empty() || product.getUnitMesurement().empty()) {
            cout << "error invalid data" << endl;
        continue;}
        //αποθήκευση με το δοσμένο format/ εκαδικους οπου χρειαζεται @ μεταξυ προιοντων/ κενα μεταξυ @
        outFile << product.getTitle() << " @ " 
        << product.getDescription() << " @ "
        << product.getCategory() << " @ "
        << product.getSubCategory() << " @ "
        << fixed << setprecision(2) << product.getPrice() << " @ " 
        << product.getUnitMesurement() << " @ " 
        << product.getQuantity() << endl;
    }
    outFile.close();
    cout<<"Products saved to file "<< filename <<endl;
}
//για χρηση ολου του vector
vector<Product>& UserManagment::getProducts() {
    return products;
}


void UserManagment::loadCategoriesFromFile(const string& filename) {
    //ελεγχος του αρχειου αν ανοιγει σωστα
    ifstream inFile(filename);
    if (!inFile) {
        cout << "error with " << filename << endl;
        return;
    }
    //ελεγχος αν εχει σωστο φορματ βαση ()
    string line;
    while (getline(inFile, line)) {
        size_t pos = line.find('(');
        if (pos == string::npos) {
            cout << "wrong format" << endl;
            continue;
        }
        //με τη χρηση του trim για περικοπη περιττων κενων
        string mainCategory = trim(line.substr(0, pos - 1)); 
        string subcategoriesString = line.substr(pos + 1, line.size() - pos - 2); 
        stringstream ss(subcategoriesString);
        string subcategory;
        vector<string> subcategories;
       //σωστο allignement category - subcategories 
        while (getline(ss, subcategory, '@')) {
            subcategory = trim(subcategory);
            subcategories.push_back(subcategory);
        }
        categories[mainCategory] = subcategories;
    }

    cout << "categories loaded successfully from " << filename << endl;
}


void UserManagment::loadProductsFromFile(const string& filename){
    //ελεγχος του αρχειου αν ανοιγει σωστα
    ifstream inFile(filename);
    if(!inFile){
        cout << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    while(getline(inFile, line)){
        stringstream ss(line);
        string title, description, category, subCategory, unitMesurement;
        double price, quantity;
        //φορτωση των αρχειων χωρις περιττα συμβολα, χωρις κενα με trin
        getline(ss, title, '@');
        title = trim(title);

        getline(ss, description, '@');
        description = trim(description);

        getline(ss, category, '@');
        category = trim(category);

        getline(ss, subCategory, '@');
        subCategory = trim(subCategory);

        ss >> price;
        ss.ignore(1, '@');
        getline(ss, unitMesurement, '@');
        unitMesurement = trim(unitMesurement);

        ss >> quantity;
        //προσθήκη σε vector
        products.emplace_back(title, description, category, subCategory, unitMesurement, price, quantity);
    }
    inFile.close();
    cout << "Products loaded successfully from " << filename << endl;
}

void UserManagment::saveOrderHistory(const string& username, const string& orderData){
    //αποθήκευση στον κατάλληλο φάκελο/αρχείο
    string filename ="files/order_history/" + username + "_history.txt";
    ofstream outFile(filename, ios::app);
    if(!outFile){
        //ελεγχος του αρχειου αν ανοιγει σωστα
        cout<<"error opening file "<<filename << " to save order history "<<username <<endl;
        return;
    }
    outFile<<orderData<<endl;
    outFile.close();
    cout<<"order saved to file "<< filename <<endl;
}

string UserManagment::loadOrderHistory(const string& username){
    //cout debug
    cout <<"username order history "<<username<<endl;
    string filename = "files/order_history/" + username + "_history.txt";
    cout <<"orders from file "<< filename <<endl;
    ifstream inFile(filename);
    if(!inFile){
        cout<<"error opening file "<<filename << " to save order history "<<username <<endl;
        return "";
    }
    //αναγνωση και επιστροφη ως συμβολοσειρα αρχειου
    stringstream buffer;
    buffer << inFile.rdbuf();
    return buffer.str();
}