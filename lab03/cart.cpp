#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>

using namespace std;

class Product
{
public:
    Product() : id(0), price(0) {}
    Product(int id, string name, double price) : id(id), name(name), price(price) {}
    int getID() {
        return id;
    }
    string getName() {
        return name;
    }
    double getPrice() {
        return price;
    }
    bool notFound() {
        return id == 0;
    }
private:
    int id;
    string name;
    double price;
};

class Catalog {
public:
    bool load(string path) {
        ifstream fin(path);
        if (fin.fail()) {
            cerr << "Could not open file " << path << endl;
            return false;
        }

        string line;
        string nop;
        while(getline(fin, line)) {
            stringstream ss(line);
            int id;
            string name;
            double price;
            ss >> id;
            getline(ss, nop, ','); // get rid of the first ,
            getline(ss, name, ',');
            ss >> price;
            if (ss.fail()) {
                cerr << "Cannot parse product from '" << line << "'" << endl;
            } else {
                products.push_back(Product(id, name, price));
            }
        }

        return true;
    }
    Product get(int id) {
        for (auto product : products) {
            if (product.getID() == id) {
                return product;
            }
        }
        return Product();
    }
private:
    vector<Product> products;
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

void print(ShoppingCart cart, ostream &out) {
    out << fixed << setprecision(2);

    for (auto item : cart.getItems())
    {
        out << item.getProduct().getName() << " ";
        out << "x" << item.getQuantity() << " ";
        out << "$" << item.getProduct().getPrice() << endl;
    }
    for (auto discount : cart.getDiscounts()) {
        out << discount->getDesc() << " -$" << discount->calculate(cart.getItems()) << endl;
    }
    out << "Total $" << cart.total() << endl;
}

int main(int argc, char * argv[])
{
    cout << "argc:" << argc << endl;
    for (int i=0; i<argc; i++) {
        cout << "argv[" << i << "]:" << argv[i] << endl;
    }
    if (argc < 2) {
        cerr << "Expected path to the catalog file as a first argument. Got none." << endl;
        return 2;
    }
    Catalog catalog;
    bool ok = catalog.load(argv[1]);
    if (!ok) {
        cerr << "Could not load catalog" << endl;
        return 1;
    }
    ShoppingCart cart;

    Product teaCup = catalog.get(2);
    if (teaCup.notFound()) {
        cerr << "Cannot find product with ID 2" << endl;
    } else {
        cart.add(teaCup, 3);
    }

    Product wineGlass = catalog.get(3);
    cart.add(wineGlass, 6);

    TakeTwoGetOneFreeDiscount *take2get1free = new TakeTwoGetOneFreeDiscount("Take 2 get 1 for free");
    cart.add(take2get1free);
    
    print(cart, cout);

    return 0;
}