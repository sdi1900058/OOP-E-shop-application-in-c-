#ifndef product_h
#define product_h

using namespace std;
#include <string>

class Product{
    private:
        string title;
        string description;
        string category;
        string subCategory;
        string unitMesurement;
        double price;
        int quantity;
        //για τα popularProducts
        int ordercount;

    public:

        Product(const string& title, const string& description, const string&category,
                const string &subcategory, const string& unitMesurement, double price, int quantity);

        //SETTERS GETTERS
        string getTitle() const;
        string getDescription() const;
        string getCategory() const;
        string getSubCategory() const;
        string getUnitMesurement() const;
        double getPrice() const;
        int getQuantity() const;
        int getOrderCount() const;

        void setTitle(const string& newtitle);
        void setCategory(const string& newCategory);
        void setSubCategory(const string& newsubcategory);
        void setDescription(const string& newDescription);
        void setPrice( double newPrice);
        void setQuantity(int newQuantity);


        //ελεγχος αν είναι διαθέσιμο και αλλαγή του quantity
        bool checkAndupdateQuantity(int quantity);
        //απελευθέρωση
        void releaseQuantity(int quantityincart);
        //ενημέρωση του order count
        void increaseOrderCount();
        //με ελεγχο αν ειναι admin
        void displayProductChecking(const Product& product, bool isAdmin) const;
        //εκτυπωση προιοντος
        void displayProductDetails() const;

};


#endif //product.h