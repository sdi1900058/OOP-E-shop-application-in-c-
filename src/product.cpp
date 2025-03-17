#include "product.h"
#include <iostream>
using namespace std;

//constructor
Product::Product(const string& title, const string& description, const string &category,
                 const string &subcategory, const string& unitMesurement, double price, int quantity)
                 :title(title), description(description), category (category), subCategory(subcategory),
                  unitMesurement(unitMesurement) ,price(price), quantity(quantity), ordercount(0){};
//setters getter
string Product::getTitle()const{return title;}
string Product::getDescription()const{return description;}
string Product::getCategory()const{return category;}
string Product::getSubCategory()const{return subCategory;}
string Product::getUnitMesurement()const{return unitMesurement;}
double Product::getPrice()const{return price;}
int Product::getQuantity()const{return quantity;}
int Product::getOrderCount()const{return ordercount;}

void Product::setTitle(const string& newtitle){
    title = newtitle;
}
void Product::setCategory(const string& newCategory){
    category = newCategory;
}
void Product::setSubCategory(const string& newsubcategory){
    subCategory = newsubcategory;
}
void Product::setDescription(const string& newDescription){
    description = newDescription;
}

void Product::setPrice(double newPrice){
    price= newPrice;
}

void Product::setQuantity(int newQuantity){
    quantity = newQuantity;
}

//ελεγχος και ενημερωση quantity in stock
bool Product::checkAndupdateQuantity(int newQuantity){
    if(quantity>=newQuantity){
        quantity-=newQuantity;
        return true;
    }else{
        return false;
    }
}
//απελευθερωση quantity
void Product::releaseQuantity(int quantityincart){
    quantity+=quantityincart;
}

void Product::increaseOrderCount(){ordercount++;}

void Product::displayProductDetails()const{
    cout << "Title: " << title << endl;
    cout << "Description: " << description << endl;
    cout << "Category: " << category << endl;
    cout << "Subcategory: " << subCategory << endl;
    cout << "Price: " << price << endl;
    cout << "Quantity: " << quantity << endl;
    cout << "Measured in: " << unitMesurement << endl;
}

void Product::displayProductChecking(const Product& product, bool isAdmin)const{
    if(isAdmin){
        product.displayProductDetails();
        cout << "Order count: " << product.getOrderCount() << endl;
    }else{
        product.displayProductDetails();
    };
}