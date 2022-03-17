#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

class Product
{
public:
    Product() : price(0) {}
    Product(string name, double price) : name(name), price(price) {}
    string getName() {
        return name;
    }
    double getPrice() {
        return price;
    }
private:
    string name;
    double price;
};

class Item
{
public:
    Item(Product product, int qty) : product(product), quantity(qty) {
        if (quantity <= 0) {
            quantity = 1;
        }
    }
    Product getProduct() {
        return product;
    }
    int getQuantity() {
        return quantity;
    }
private:
    Product product;
    int quantity;
};

class Discount {
public:
    Discount(string desc) : desc(desc) {}
    virtual double calculate(vector<Item> items) {
        return 0;
    }
    string getDesc() {
        return desc;
    }
private:
    string desc;
};

class PercentageDiscount : public Discount {
public:
    PercentageDiscount(string desc, double off) : Discount(desc), off(off) {}
    double calculate(vector<Item> items) {
        double sum = 0;
        for (auto item : items)
        {
            sum += item.getProduct().getPrice() * item.getQuantity();
        }
        return sum * off;
    }
private:
    double off;
};

class TakeTwoGetOneFreeDiscount : public Discount {
public:
    TakeTwoGetOneFreeDiscount(string desc) : Discount(desc) {}
    double calculate(vector<Item> items) {
        double sum = 0;
        for (auto item : items)
        {
            int freeItemsCount = item.getQuantity() / 3;
            sum += freeItemsCount * item.getProduct().getPrice();
        }
        return sum;
    }
};

class ShoppingCart
{
public:
    void add(Product product, int qty)
    {
        Item item(product, qty);
        items.push_back(item);
    }
    void add(Discount *discount) {
        discounts.push_back(discount);
    }
    double total()
    {
        double sum = 0;
        for (auto item : items)
        {
            sum += item.getProduct().getPrice() * item.getQuantity();
        }
        for (auto discount : discounts) {
            sum -= discount->calculate(items);
        }
        return sum;
    }
    vector<Item> getItems() {
        return items;
    }
    vector<Discount*> getDiscounts() {
        return discounts;
    }
private:
    vector<Item> items;
    vector<Discount*> discounts;
};

int main()
{
    Product coffeeCup("Coffee cup", 4.80);
    Product wineGlass("Wine glass", 7.20);

    ShoppingCart cart;
    cart.add(coffeeCup, 3); // 3 / 3 = 1 * 4.80
    cart.add(wineGlass, 6); // 6 / 3 = 2 * 7.20 = 14.40

    PercentageDiscount *off5p = new PercentageDiscount("5% off", 0.05);
    // cart.add(off5p);

    TakeTwoGetOneFreeDiscount *take2get1free = new TakeTwoGetOneFreeDiscount("Take 2 get 1 for free");
    cart.add(take2get1free); // final discount should be 19.20

    cout << fixed << setprecision(2);

    for (auto item : cart.getItems())
    {
        cout << item.getProduct().getName() << " ";
        cout << "x" << item.getQuantity() << " ";
        cout << "$" << item.getProduct().getPrice() << endl;
    }
    for (auto discount : cart.getDiscounts()) {
        cout << discount->getDesc() << " -$" << discount->calculate(cart.getItems()) << endl;
    }
    cout << "Total $" << cart.total() << endl;

    return 0;
}