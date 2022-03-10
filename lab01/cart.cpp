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

class ShoppingCart
{
public:
    void add(Product product, int qty)
    {
        Item item(product, qty);
        items.push_back(item);
    }
    double total()
    {
        double sum = 0;
        for (auto item : items)
        {
            sum += item.getProduct().getPrice() * item.getQuantity();
        }
        return sum;
    }
    vector<Item> getItems() {
        return items;
    }
private:
    vector<Item> items;
};

int main()
{
    Product coffeeCup("Coffee cup", 4.80);
    Product wineGlass("Wine glass", 7.20);

    ShoppingCart cart;
    cart.add(coffeeCup, 2);
    cart.add(wineGlass, -2);

    cout << fixed << setprecision(2);

    for (auto item : cart.getItems())
    {
        cout << item.getProduct().getName() << " ";
        cout << "x" << item.getQuantity() << " ";
        cout << "$" << item.getProduct().getPrice() << endl;
    }
    cout << "Total $" << cart.total() << endl;

    return 0;
}