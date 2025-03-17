#include "eshop.h"
#include <iostream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    //αν δεν εχει το ./oop και 3 αρχεία για να φορτωσει τερματιζει
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " <categories_file> <products_file> <users_file>" << endl;
        return 1;
    }
    //αρχικοποιηση των αρχειων
    string categoriesFile = argv[1];
    string productsFile = argv[2];
    string usersFile = argv[3];

    //κληση constructor eshop και ορισμα input
    Eshop eshop(cin);

    //ξεκιναμε το eshop
    eshop.run(categoriesFile, productsFile, usersFile);

    return 0;
}
