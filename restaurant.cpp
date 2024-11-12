#include <bits/stdc++.h>
using namespace std;

// Forward declarations
class Waiter;
class Chef;
class Order;

class Bill {
private:
    const Order* order;
    const vector<pair<string, double>>& dishes;

public:
    Bill(const Order* o, const vector<pair<string, double>>& d) 
        : order(o), dishes(d) {}

 void Bill::show_bill() {
    double total = 0.0;
    cout << "\nBill Summary:\n";
    cout << "Order Name\tQuantity\tTotal Price\n";

    for (const auto& item : order->get_order()) {
        double price = 0.0;

        for (const auto& dish : dishes) {
            if (dish.first == item.first) {
                price = dish.second;
                break;
            }
        }

        double item_total = price * item.second;
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

class Order {
protected:
    unordered_map<string, int> orderItems; // Stores ordered items and their quantities
    vector<pair<string, double>> dishes; // List of dishes available
    string review;
    const string MENU_FILE = "menu.txt";

public:
    Order() {
        loadMenuFromFile();
        cout << "Order object created." << endl;
    }

    virtual ~Order() {
        cout << "Order object destroyed." << endl;
    }

    void loadMenuFromFile() {
        ifstream file(MENU_FILE);
        if (!file) {
            cout << "Error opening file. Creating a new menu.\n";
            return;
        }
        dishes.clear();
        string line, name;
        double price;
        while (getline(file, line)) {
            istringstream iss(line);
            if (getline(iss, name, ',') && iss >> price) {
                dishes.push_back({name, price});
            }
        }
        file.close();
    }

    void displayMenu() {
        cout << "Current Menu:\n";
        cout << "No.\tDishes\t\tPrice\n";
        for (size_t i = 0; i < dishes.size(); ++i) {
            cout << i + 1 << ".\t" << dishes[i].first << "\t\t" << dishes[i].second << endl;
        }
    }

    void saveMenuToFile() {
        ofstream file(MENU_FILE);
        if (!file) {
            cout << "Error opening file for writing.\n";
            return;
        }
        for (const auto& dish : dishes) {
            file << dish.first << "," << dish.second << "\n";
        }
        file.close();
        cout << "Menu saved to file successfully.\n";
    }

    void updateMenu() {
        displayMenu();
        int choice;
        cout << "Enter the number of the dish you want to update: ";
        cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(dishes.size())) {
            string newName;
            double newPrice;
            cout << "Enter new name for the dish (or press enter to keep current): ";
            cin.ignore();
            getline(cin, newName);
            if (!newName.empty()) {
                dishes[choice - 1].first = newName;
            }
            cout << "Enter new price for the dish: ";
            cin >> newPrice;
            dishes[choice - 1].second = newPrice;
            cout << "Dish updated successfully.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void removeDishFromMenu() {
        displayMenu();
        int choice;
        cout << "Enter the number of the dish you want to remove: ";
        cin >> choice;
        if (choice > 0 && choice <= static_cast<int>(dishes.size())) {
            dishes.erase(dishes.begin() + choice - 1);
            cout << "Dish removed successfully.\n";
        } else {
            cout << "Invalid choice.\n";
        }
    }

    void addDishToMenu() {
        string name;
        double price;
        cout << "Enter the name of the new dish: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter the price of the new dish: ";
        cin >> price;
        dishes.push_back({name, price});
        cout << "Dish added successfully.\n";
    }

    virtual void get_order() {
        int n;
        do {
            cout << "Please enter the choice number of the dishes you want to eat (enter 0 to confirm your order):" << endl;
            while (true) {
                if (!(cin >> n)) {
                    cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cout << "Invalid input. Please enter a number." << endl;
                    continue;
                }
                if (n == 0) {
                    break;
                }
                if (n >= 1 && n <= static_cast<int>(dishes.size())) {
                    add_dish(dishes[n - 1].first);
                } else {
                    cout << "Invalid choice. Please select a valid dish number." << endl;
                }
            }
            if (orderItems.empty()) {
                cout << "No dishes selected. Please select at least one dish." << endl;
                continue;
            }
            cout << "Your current order is:" << endl;
            for (const auto& item : orderItems) {
                cout << item.first << ": " << item.second << endl;
            }
            cout << "Do you want to confirm this order? (yes/no): ";
            bool confirmed = get_user_choice();
            if (confirmed) {
                return;
            }
        } while (true);
    }

    void checkForAddOn() {
        while (true) {
            cout << "Would you like to add more dishes? (yes/no): ";
            string response;
            cin >> response;
            if (response == "no") {
                cout << "Type 'bill' to generate bill.\n";
                break;
            }
            cout << "Please select from the menu again:\n";
            displayMenu();
            int selection;
            cout << "Enter dish number: ";
            cin >> selection;
            if (selection > 0 && selection <= static_cast<int>(dishes.size())) {
                add_dish(dishes[selection - 1].first);
                cout << "Added " << dishes[selection - 1].first << " to the order.\n";
            } else {
                cout << "Invalid selection.\n";
            }
        }
    }

    void add_on() {
        checkForAddOn();
    }

  void finalize_order() {
    Bill bill(this, dishes);
    bill.show_bill();
    confirm_payment();
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
            getline(cin, choice);
            transform(choice.begin(), choice.end(), choice.begin(), ::tolower);  // Convert to lowercase
            if (choice == "yes" || choice == "y") {
                return true;
            } else if (choice == "no" || choice == "n") {
                return false;
            } else {
                cout << "Invalid input. Please enter 'yes' or 'no': ";
            }
        }
    }

    void add_dish(const string& dish_name) {
        orderItems[dish_name]++;
    }

    const unordered_map<string, int>& get_order() const {
        return orderItems;
    }
};

class Table : public Order {
private:
    static const int tableCount = 10; // Total number of tables
    static int tableCapacities[10]; // Array to store capacities of each table
    static int availableTables[10]; // Array to track available tables
    vector<string> customers; // List of customers at the table
    static int table_count; // Current count of tables
    vector<string> orders; // Store orders placed at the table

public:
    Table(vector<string> people) : customers(people) {
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
            }
        }
    }

    ~Table() {
        cout << "Thank you for dining with us!" << endl;
    }

    friend class Chef ;  
    friend class Waiter;

    // Get the table number (1-based index)
    int getTableNumber() const {
        return table_count; // Returning the table number based on the current count
    }

    // Get orders placed at the table
    vector<string> getOrders() const {
        return orders; // Return a copy of the orders
    }

    // Function to add an order
    void addOrder(const string& order) {
        orders.push_back(order); // Add an order to the list
    }

    void assignTables(int customerCount) {
        int tablesAssigned = 0;
        vector<int> assignedTables;

        for (int i = 0; i < tableCount; i++) {
            if (availableTables[i] && customerCount > 0) {
                if (customerCount >= tableCapacities [i]) {
                    cout << "Assigned Table " << i + 1 << " with capacity " << tableCapacities[i] << endl;
                    assignedTables.push_back(i + 1);
                    customerCount -= tableCapacities[i];
                    availableTables[i] = 0; // Mark table as unavailable
                    tablesAssigned++;
                } else if (customerCount > 0) {
                    cout << "Assigned Table " << i + 1 << " with capacity " << tableCapacities[i] << endl;
                    assignedTables.push_back(i + 1);
                    availableTables[i] = 0; // Mark table as unavailable
                    customerCount = 0; // All customers seated
                    tablesAssigned++;
                }
            }
        }

        if (customerCount > 0) {
            cout << "Not enough tables available for all customers." << endl;
        } else {
            cout << "Total tables assigned: " << tablesAssigned << endl;
        }
    }

    void customer_service() {
        cout << "Performing customer service for table " << table_count << endl;
        // Implement the customer service logic here
    }

    void add_on() {
        cout << "Adding on to the order for table " << table_count << endl;
        // Implement the add-on logic here
    }

    void finalize_order() {
        cout << "Finalizing order for table " << table_count << endl;
        // Implement the order finalization logic here
    }
};

// Static member initialization
int Table::tableCapacities[10] = {4, 4, 4, 6, 6, 6, 8, 8, 10, 10};
int Table::availableTables[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int Table::table_count = 0;



class Staff : public Order {
protected:
    string name;
    int id;

public:
    // Constructor
    Staff(string name, int id) : name(name), id(id) {}

    virtual void displayInfo() const {
        cout << "Staff: " << name << " | ID: " << id << endl;
    }

    // Declare a pure virtual function for fetching orders
    virtual void fetchOrder(Table& table) = 0;  // Abstract function to fetch orders

    void set_name(const string& new_name) {
        name = new_name;
    }

    virtual void login() = 0; // Example of a pure virtual function

    // Getter for ID
    int get_id() const {
        return id;
    }

    virtual ~Staff() {}
};

// Inner class for Chef as a child of Staff
class Chef : public Staff {
private:
    vector<string> ingredientStock;

public:
    Chef(string name, int id, const vector<string>& stock)
        : Staff(name, id), ingredientStock(stock) {}

    void fetchOrder(Table &table) override {
        cout << "Chef " << name << " is reviewing the order from Table " << table.getTableNumber() << endl;
        const auto& orders = table.getOrders();
        if (orders.empty()) {
            cout << "No orders to prepare for this table." << endl;
        } else {
            cout << "Orders to prepare:" << endl;
        }
    }

    // Prepare dishes and return them to the waiter
    vector<string> prepareDishes(const unordered_map<string, int>& orders) {
        vector<string> preparedDishes;
        for (const auto& order : orders) {
            try {
                if (checkIngredients(order.first)) {
                    cout << "Preparing " << order.first << " (Quantity: " << order.second << ")" << endl;
                    for (int i = 0; i < order.second; ++i) {
                        preparedDishes.push_back(order.first);
                    }
                } else {
                    throw runtime_error("Insufficient ingredients for " + order.first);
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
        return preparedDishes;
    }

    void confirmDishReady(Waiter &waiter, const vector<string>& preparedDishes);

private:
    bool checkIngredients(const string& dish) {
        // Check if ingredients are available (dummy implementation)
        return true; // Assume ingredients are always available for simplicity
    }
};

class Waiter : public Staff {
private:
    Table& associated_table;  // Reference to the associated Table

public:
    // Constructor that takes a reference to a Table instance
    Waiter(Table &table, const string& name, int id) : Staff(name, id), associated_table(table) {
        cout << "Waiter created for Table " << associated_table.getTableNumber() << "." << endl;
    }

    // Method to fetch order from the specific table and pass it to the chef
    void fetchOrder(Chef &chef) {
        cout << "Fetching order from Table " << associated_table.getTableNumber() << "...\n";

        // Get orders from the associated table
        vector<string> orders = associated_table.getOrders();

        if (orders.empty()) {
            cout << "No orders placed at the table yet." << endl;
        } else {
            chef.fetchOrder(associated_table);  // Pass the order to the chef for review
        }
    }

    // Method to deliver prepared dishes to the table
    void deliverOrder(const vector<string>& preparedDishes) {
        cout << "Delivering the following dishes to the  specific Table " << associated_table.getTableNumber() << ":\n";
        for (const auto& dish : preparedDishes) {
            cout << "- " << dish << endl;
        }
    }

    // Method to confirm payment
    void confirm_payment() {
        cout << "Has the payment been made? ";
        if (get_user_choice()) {
            cout << "Payment confirmed. Thank you!\n";
        } else {
            cout << "Payment pending. Please process the payment.\n";
        }
    }

    // Method to collect customer review after the meal
    void collect_review() {
        cout << "Please provide your review for the dining experience: ";
        string review;
        cin.ignore();  // Clear the input buffer before getting the review
        getline(cin, review);

        // Assuming that reviews are being stored elsewhere, you might want to
        // pass this review to the Admin class or some central repository.
        cout << "Thank you for your review!" << endl;
    }

    // Helper function to get user choice for yes/no input
    bool get_user_choice() {
        string choice;
        while (true) {
            cout << "(yes/no): ";
            getline(cin, choice);
            transform(choice.begin(), choice.end(), choice.begin(), ::tolower);  // Convert to lowercase
            if (choice == "yes" || choice == "y") {
                return true;
            } else if (choice == "no" || choice == "n") {
                return false;
            } else {
                cout << "Invalid input, please enter 'yes' or 'no'." << endl;
            }
        }
    }

    // Destructor
    ~Waiter() {
        cout << "Waiter destructor called." << endl;
    }
};

// Define the confirmDishReady method after the Waiter class definition
void Chef::confirmDishReady(Waiter &waiter, const vector<string>& preparedDishes) {
    cout << "Chef " << name << " confirms that the following dishes are ready to be served:\n";
    for (const auto& dish : preparedDishes) {
        cout << "- " << dish << endl;
    }
    waiter.deliverOrder(preparedDishes); // Notify waiter to deliver the prepared dishes
}

class Admin{
    vector<string>& review;
    vector<pair<string, int>> dishes; // Assuming dishes is a vector of dish name and price

public:
    // Constructor to initialize review vector
    Admin(const string& name, int id, vector<string>& k) :  review(k) {}

    // Function to display the Admin menu
    void display_menu() {
        cout << "Admin Menu:\n";
        cout << "1. View all reviews\n";
        cout << "2. Update dishes\n";
        cout << "3. Logout\n";
    }

    // Function to view all reviews
    void view_all_reviews() {
        if (review.empty()) {
            cout << "No reviews to view" << endl;
            return;
        }
        for (int i = 0; i < review.size(); ++i) {
            cout << "Review number " << i + 1 << " is:\n";
            cout << review[i] << endl;
            cout << endl;
        }
    }

    // Function to update dishes
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

    // Function to log out the admin
    void logout() {
        cout << "Logged out successfully.\n";
    }

    // Function to manage the restaurant
    void manage_restaurant() {
        while (true) {
            display_menu();
            int choice;
            cin >> choice;
            switch (choice) {
                case 1:
                    view_all_reviews();
                    break;

                case 2:
                    update_dishes();
                    break;

                case 3:
                    logout();
                    return;

                default:
                    cout << "Invalid choice. Please try again.\n";
            }
        }
    }

    // Function to log in as the admin
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

int main(){
    vector<string> reviews;
    vector<Table*> tables;
    vector<Staff*> staff;

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
                Admin admin("Admin Name", 1, reviews);
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
