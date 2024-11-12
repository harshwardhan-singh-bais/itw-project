#include <bits/stdc++.h>
using namespace std;


// Global vector for dishes and their prices
vector<pair<string, int>> dishes = {
    {"Pasta", 200},
    {"Pizza", 300},
    {"Burger", 150},
    {"Salad", 100}
};

/* Forward declarations
class Table;
class Staff;
*/
// Order class
class Order {
private:
    unordered_map<string, int> order;

public:
    void add_dish(const string& dish) {
        order[dish]++;
    }

    const unordered_map<string, int>& get_order() const {
        return order;
    }

    void remove_unavailable_dishes(const vector<string>& available_dishes) {
        for (auto it = order.begin(); it != order.end(); ) {
            if (find(available_dishes.begin(), available_dishes.end(), it->first) == available_dishes.end()) {
                it = order.erase(it);
            } else {
                ++it;
            }
        }
    }

    bool is_empty() const {
        return order.empty();
    }
};

// Bill class
class Bill {
private:
    Order& order;

public:
    Bill(Order& o) : order(o) {}

    void show_bill() {
        int total = 0;

        cout << "\nBill Summary:\n";
        cout << "Order Name\tQuantity\tTotal Price\n";
        for (const auto& item : order.get_order()) {
            int price = 0;
            for (const auto& dish : dishes) {
                if (dish.first == item.first) {
                    price = dish.second;
                    break;
                }
            }
            int item_total = price * item.second;
            total += item_total;
            cout << item.first << "\t\t" << item.second << "\t\t" << item_total << endl;
        }

        double gst = total * 0.12;
        double final_amount = total + gst;

        cout << "\nSubtotal: " << total << endl;
        cout << "GST (12%): " << gst << endl;
        cout << "Total Amount: " << final_amount << endl;
    }
};

// Table class
class Table {
    friend class Admin;
    friend class Chef;

private:
    vector<string> customers;
    static int table_count;
    Order order;
    string review;

    void customer_service() {
        display_menu();
        get_order();
        cout << "Thank you for your order! Please wait patiently while our chef prepares your food." << endl;
    }

    void display_menu() {
        cout << "Your choices are:" << endl;
        cout << "No.\tDishes\tPrice" << endl;
        for (size_t i = 0; i < dishes.size(); ++i) {
            cout << i + 1 << ".\t" << dishes[i].first << "\t" << dishes[i].second << endl;
        }
    }

    void get_order() {
        int n;

        do {
            cout << "Please enter the choice number of the dishes you want to eat (enter 0 to confirm your order):" << endl;

            while (true) {
                if (!(cin >> n)) {
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number." << endl;
                    continue;
                }

                if (n == 0) {
                    break;
                }

                if (n >= 1 && n <= static_cast<int>(dishes.size())) {
                    order.add_dish(dishes[n - 1].first);
                } else {
                    cout << "Invalid choice. Please select a valid dish number." << endl;
                }
            }

            if (order.is_empty()) {
                cout << "No dishes selected. Please select at least one dish." << endl;
                continue;
            }

            cout << "Your current order is:" << endl;
            for (const auto& item : order.get_order()) {
                cout << item.first << ": " << item.second << endl;
            }

            cout << "Do you want to confirm this order? (yes/no): ";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            bool confirmed = get_user_choice();

            if (confirmed) {
                return;
            }

        } while (true);
    }

    void add_on() {
        cout << "Add-ons feature is not implemented yet." << endl;
    }

   void finalize_order() {
    Bill bill(order);
    bill.show_bill();  // This will print the bill details
    confirm_payment(); // Prompt for payment confirmation
    collect_review();
}


    void confirm_payment() {
        cout << "Do you confirm the payment? (yes/no): ";
        bool payment_confirmed = get_user_choice();
        if (payment_confirmed) {
            cout << "Payment confirmed. Thank you!" << endl;
        } else {
            cout << "Payment not confirmed. Please proceed to payment." << endl;
        }
    }

    void collect_review() {
        cout << "Please provide your review for the dining experience: ";
        getline(cin, review);
        cout << "Thank you for your review!" << endl;
    }

    bool get_user_choice() {
        string choice;
        while (true) {
            cout << "(yes/no): ";
            getline(cin, choice);
            transform(choice.begin(), choice.end(), choice.begin(), ::tolower);
            if (choice == "yes" || choice == "y") {
                return true;
            } else if (choice == "no" || choice == "n") {
                return false;
            } else {
                cout << "Invalid input, please enter 'yes' or 'no'." << endl;
            }
        }
    }

public:
    Table(vector<string> people) {
        customers = people;
        table_count++;
        cout << "Please be seated at Table no " << table_count << endl;
        customer_service();  // This will handle menu display and ordering
        
        cout << "Please ask for add-ons any time by writing 'add on'" << endl;
        cout << "When you are done, please be sure to call for the Bill by writing 'bill'" << endl;

        string s;
        while (true) {
            getline(cin, s);
            transform(s.begin(), s.end(), s.begin(), ::tolower);

            if (s == "add on") {
                add_on(); // Placeholder for add-ons feature
            } else if (s == "bill") {
                finalize_order(); // Finalize the order and print the bill
                break; // Exit the loop after showing the bill
            } else {
                cout << "Sorry, choice not recognized. Please re-enter." << endl;
            }
        }
    }

    ~Table() {
        cout << "Thank you for dining with us!" << endl;
    }

    static bool get_table_status(int table_number) {
        // Implementation needed
        return false;
    }

    static vector<string> get_order_dishes(int table_number) {
        // Implementation needed
        return vector<string>();
    }

    static int get_order_total(int table_number) {
        // Implementation needed
        return 0;
    }

    static string get_review_content(int table_number) {
        // Implementation needed
        return "";
    }

    static int get_review_rating(int table_number) {
        // Implementation needed
        return 0;
    }

    int getTableNumber() const {
        // Implementation needed
        return 0;
    }

    vector<string> getOrders() const {
        // Implementation needed
        return vector<string>();
    }

    bool hasAdditionalOrder() const {
        // Implementation needed
        return false;
    }

    void resetAdditionalOrder() {
        // Implementation needed
    }
};

int Table::table_count = 0;

class Admin {
public:
    void display_menu() {
        cout << "Admin Menu:\n";
        cout << "1. View all tables\n";
        cout << "2. View all orders\n";
        cout << "3. View all reviews\n";
        cout << "4. Update dishes\n";
        cout << "5. Update staff\n";
        cout << "6. Logout\n";
    }

    void view_all_tables() {
        for (int i = 1; i <= 10; ++i) {
            if (Table::get_table_status(i)) {
                cout << "Table " << i << ": Occupied\n";
            } else {
                cout << "Table " << i << ": Empty\n";
            }
        }
    }

    void view_all_orders() {
        for (int i = 1; i <= 10; ++i) {
            vector<string> orders = Table::get_order_dishes(i);
            int total_amount = Table::get_order_total(i);
            if (!orders.empty()) {
                cout << "Order for Table " << i << ":\n";
                for (const auto& dish : orders) {
                    cout << " - " << dish << endl;
                }
                cout << "Total Amount: $" << total_amount << endl;
            }
        }
    }

    void view_all_reviews() {
        for (int i = 1; i <= 10; ++i) {
            string content = Table::get_review_content(i);
            int rating = Table::get_review_rating(i);
            cout << "Review for Table " << i << ":\n";
            cout << "Content: " << content << endl;
            cout << "Rating: " << rating << "/5\n";
        }
    }

    void update_dishes() {
        while (true) {
            cout << "Update Dishes Menu:\n";
            cout << "1. Add dish\n";
            cout << "2. Remove dish\n";
            cout << "3. Modify dish\n";
            cout << "4. Back\n";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    string dish;
                    int price;
                    cout << "Enter dish name: ";
                    getline(cin, dish);
                    cout << "Enter dish price: ";
                    cin >> price;
                    dishes.push_back({dish, price});
                    cout << "Dish added successfully.\n";
                    break;
                }
                case 2: {
                    cout << "Enter dish name to remove: ";
                    string dish_to_remove;
                    getline(cin, dish_to_remove);
                    auto it = find_if(dishes.begin(), dishes.end(),
                                      [&](const pair<string, int>& p) { return p.first == dish_to_remove; });
                    if (it != dishes.end()) {
                        dishes.erase(it);
                        cout << "Dish removed successfully.\n";
                    } else {
                        cout << "Dish not found.\n";
                    }
                    break;
                }
                case 3: {
                    cout << "Enter dish name to modify: ";
                    string dish_to_modify;
                    getline(cin, dish_to_modify);
                    auto it = find_if(dishes.begin(), dishes.end(),
                                      [&](const pair<string, int>& p) { return p.first == dish_to_modify; });
                    if (it != dishes.end()) {
                        int new_price;
                        cout << "Enter new dish price: ";
                        cin >> new_price;
                        it->second = new_price;
                        cout << "Dish updated successfully.\n";
                    } else {
                        cout << "Dish not found.\n";
                    }
                    break;
                }
                case 4:
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

   /* void update_staff() {
        cout << "Updating staff...\n";
        int staff_id;
        cout << "Enter Staff ID to update: ";
        cin >> staff_id;
        cin.ignore();

        Staff* staff_member = Staff::get_staff_by_id(staff_id);
        if (staff_member) {
            string new_name;
            cout << "Enter new staff member's name: ";
            getline(cin, new_name);
            staff_member->set_name(new_name);
            cout << "Staff member's information updated successfully.\n";
        } else {
            cout << "Staff member not found.\n";
        }
    }*/

    void logout() {
        cout << "Logged out successfully.\n";
    }

    void manage_restaurant() {
        while (true) {
            display_menu();

            int choice;
            cin >> choice;

            switch (choice) {
                case 1:
                    view_all_tables();
                    break;
                case 2:
                    view_all_orders();
                    break;
                case 3:
                    view_all_reviews();
                    break;
                case 4:
                    update_dishes();
                    break;
              /*  case 5:
                    update_staff();*/
                    break;
                case 6:
                    logout();
                    return;
                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    void login() {
        cout << "Admin Login\n";
        cout << "Enter your username: ";
        string username;
        cin >> username;
        cout << "Enter your password: ";
        string password;
        cin >> password;

        if (username == "admin" && password == "password") {
            cout << "Logged in successfully.\n";
            manage_restaurant();
        } else {
            cout << "Invalid username or password.\n";
        }
    }
};

class Staff {
protected:
    string name;
    int id;

public:
    // Constructor
    Staff(string name, int id) : name(name), id(id) {}

    virtual void displayInfo() const {
        cout << "Staff: " << name << " | ID: " << id << endl;
    }

    virtual void fetchOrder(Table &table) = 0;

    void set_name(const string& new_name) {
        name = new_name;
    }

    // Getter for ID
    int get_id() const {
        return id;
    }

    static Staff* get_staff_by_id(int staff_id, const vector<Staff*>& staff_list) {
        for (Staff* staff_member : staff_list) {
            if (staff_member->get_id() == staff_id) {
                return staff_member; // Return pointer to staff member if ID matches
            }
        }
        return nullptr; // Return nullptr if not found
    }

    virtual ~Staff() {}
};

class Waiter : public Staff {
public:
    Waiter(string name, int id) : Staff(name, id) {}

    void fetchOrder(Table &table) override {
        vector<string> orders = table.getOrders();
        if (orders.empty()) {
            cout << "No orders placed at the table yet." << endl;
        } else {
            cout << "Fetching orders from Table " << table.getTableNumber() << "...\n";
            for (const auto& order : orders) {
                cout << "Order: " << order << endl;
            }
        }
    }

    void checkForAddOn(Table &table) {
        if (table.hasAdditionalOrder()) {
            cout << "Fetching additional orders from Table " << table.getTableNumber() << "...\n";
            fetchOrder(table);
            table.resetAdditionalOrder();
        } else {
            cout << "No additional orders from Table " << table.getTableNumber() << ".\n";
        }
    }

    void deliverOrder(const vector<string>& preparedDishes) {
        cout << "Delivering the following dishes:\n";
        for (const auto& dish : preparedDishes) {
            cout << "- " << dish << endl;
        }
    }

    void reportUnavailableDish(const string &dish) {
        cout << "Waiter reporting to customer: Sorry, the dish \"" << dish << "\" is unavailable due to ingredient shortage.\n";
        cout << "Please consider ordering something else." << endl;
    }
};

class Chef : public Staff {
private:
    vector<string> ingredientStock;

public:
    Chef(string name, int id, const vector<string>& stock)
        : Staff(name, id), ingredientStock(stock) {}

    void fetchOrder(Table &table) override {
        cout << "Chef " << name << " is reviewing the order from Table " << table.getTableNumber() << endl;
        vector<string> orders = table.getOrders();
        if (orders.empty()) {
            cout << "No orders to prepare for this table." << endl;
        } else {
            cout << "Orders to prepare:" << endl;
            for (const auto& dish : orders) {
                cout << "- " << dish << endl;
            }
        }
    }

    bool checkIngredients(const string& dish) {
        return find(ingredientStock.begin(), ingredientStock.end(), dish) != ingredientStock.end();
    }

    vector<string> prepareDishes(const vector<string>& orders, Waiter &waiter) {
        vector<string> preparedDishes;
        for (const auto& dish : orders) {
            try {
                if (checkIngredients(dish)) {
                    cout << "Preparing " << dish << endl;
                    preparedDishes.push_back(dish);
                } else {
                    throw runtime_error("Insufficient ingredients for " + dish);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
                waiter.reportUnavailableDish(dish);
            }
        }
        return preparedDishes;
    }

    void notifyWaiter(Waiter &waiter, const vector<string>& dishes) {
        cout << "Chef notifying Waiter...\n";
        waiter.deliverOrder(dishes);
    }
};
class Menu : public Staff {
public:
    Menu(string name, int id) : Staff(name, id) {}

    void displayMenu(const vector<pair<string, int>>& dishes) {
        cout << "Current Menu:\n";
        cout << "Dish\tPrice\n";
        for (const auto& dish : dishes) {
            cout << dish.first << "\t" << dish.second << endl;
        }
    }

    void addDish(vector<pair<string, int>>& dishes, const string& dish, int price) {
        dishes.emplace_back(dish, price);
        cout << "Dish " << dish << " added successfully.\n";
    }
};


int main() {
    Admin admin;
    vector<Table*> tables;
    vector<Staff*> staff;

    // Initialize some staff members
    staff.push_back(new Waiter("John", 1));
    staff.push_back(new Chef("Alice", 2, {"Pasta", "Pizza", "Burger", "Salad"}));

    while (true) {
        cout << "\nWelcome to Restaurant Management System\n";
        cout << "1. Login as Admin\n";
        cout << "2. Enter as Customer\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                admin.login();
                break;
            }
            case 2: {
                cout << "Enter number of customers: ";
                int num_customers;
                cin >> num_customers;
                vector<string> customers(num_customers);
                cin.ignore();
                for (int i = 0; i < num_customers; ++i) {
                    cout << "Enter name of customer " << i+1 << ": ";
                    getline(cin, customers[i]);
                }
                tables.push_back(new Table(customers));
                break;
            }
            case 3: {
                cout << "Thank you for using our system. Goodbye!\n";
                // Clean up
                for (auto table : tables) delete table;
                for (auto s : staff) delete s;
                return 0;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}
