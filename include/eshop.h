#ifndef ESHOP_H
#define ESHOP_H
using namespace std;
#include "UserManagment.h"

class Eshop{
    private:
        //input 
        istream& input;
        //για τη φορτωση και αποθηκευση αρχειων
        UserManagment userManagement;
    public:
        //παμε με cin αν δεν αναγνωριστει input απο αλλου στον constructor
        Eshop(istream& input = cin); 
        //αρχικοποιηση/φορτωση των αρχειων και "τρεξιμο του μενου"
        void run(const string& categoriesFile, const string& productsFile, const string& usersFile );
        //main menu login/register
        void displayMainMenu();
        //admin menu
        void adminMenu(User* admin);
        //customer menu
        void customerMenu(Customer* customer);
};
#endif //ESHOP_H