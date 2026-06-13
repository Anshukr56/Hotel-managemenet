#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <algorithm>
using namespace std;

// ─────────────────────────────────────────────
//  Base class: Person
// ─────────────────────────────────────────────
class Person {
protected:
    string name;
    string phone;
public:
    Person() {}
    Person(string n, string p) : name(n), phone(p) {}
    string getName()  const { return name;  }
    string getPhone() const { return phone; }
};

// ─────────────────────────────────────────────
//  FoodItem
// ─────────────────────────────────────────────
struct FoodItem {
    string itemName;
    int    quantity;
    double price;

    FoodItem(string n, int q, double p) : itemName(n), quantity(q), price(p) {}
    double total() const { return quantity * price; }
};

// ─────────────────────────────────────────────
//  Guest  (inherits Person)
// ─────────────────────────────────────────────
class Guest : public Person {
    int    roomNumber;
    int    daysStayed;
    vector<FoodItem> foodOrders;

public:
    Guest() : roomNumber(0), daysStayed(0) {}
    Guest(string n, string p, int room, int days)
        : Person(n, p), roomNumber(room), daysStayed(days) {}

    int getRoomNumber() const { return roomNumber; }
    int getDaysStayed() const { return daysStayed; }

    void addFoodOrder(const FoodItem& item) {
        foodOrders.push_back(item);
    }

    const vector<FoodItem>& getFoodOrders() const { return foodOrders; }

    double getFoodTotal() const {
        double total = 0;
        for (const auto& f : foodOrders) total += f.total();
        return total;
    }
};

// ─────────────────────────────────────────────
//  Room
// ─────────────────────────────────────────────
class Room {
    int    roomNumber;
    string type;        // Single / Double / Suite
    double pricePerDay;
    bool   isBooked;

public:
    Room() : roomNumber(0), pricePerDay(0), isBooked(false) {}
    Room(int num, string t, double price)
        : roomNumber(num), type(t), pricePerDay(price), isBooked(false) {}

    int    getRoomNumber() const { return roomNumber;  }
    string getType()       const { return type;        }
    double getPricePerDay() const { return pricePerDay; }
    bool   getIsBooked()   const { return isBooked;    }

    void book()    { isBooked = true;  }
    void vacate()  { isBooked = false; }

    void display() const {
        cout << "  Room " << setw(3) << roomNumber
             << "  |  " << setw(7) << left << type
             << "  |  Rs." << setw(7) << right << fixed << setprecision(0) << pricePerDay << "/day"
             << "  |  " << (isBooked ? "BOOKED  " : "AVAILABLE") << "\n";
    }
};

// ─────────────────────────────────────────────
//  Hotel  (main management class)
// ─────────────────────────────────────────────
class Hotel {
    string         hotelName;
    vector<Room>   rooms;
    vector<Guest>  guests;

    // Food menu
    struct MenuItem { string name; double price; };
    vector<MenuItem> menu = {
        {"Masala Dosa",     80 },
        {"Idli Sambar",     60 },
        {"Veg Thali",       150},
        {"Chicken Biryani", 220},
        {"Paneer Butter Masala", 180},
        {"Fried Rice",      130},
        {"Cold Coffee",     90 },
        {"Fresh Juice",     70 }
    };

    // ── helpers ──────────────────────────────
    void printHeader(const string& title) const {
        cout << "\n" << string(55, '=') << "\n";
        cout << "  " << title << "\n";
        cout << string(55, '=') << "\n";
    }

    Room* findRoom(int num) {
        for (auto& r : rooms)
            if (r.getRoomNumber() == num) return &r;
        return nullptr;
    }

    Guest* findGuest(int roomNum) {
        for (auto& g : guests)
            if (g.getRoomNumber() == roomNum) return &g;
        return nullptr;
    }

public:
    Hotel(string name) : hotelName(name) {
        // Seed rooms
        rooms.push_back(Room(101, "Single", 1500));
        rooms.push_back(Room(102, "Single", 1500));
        rooms.push_back(Room(103, "Single", 1500));
        rooms.push_back(Room(201, "Double", 2500));
        rooms.push_back(Room(202, "Double", 2500));
        rooms.push_back(Room(301, "Suite",  5000));
        rooms.push_back(Room(302, "Suite",  5000));
    }

    // ── 1. View Rooms ─────────────────────────
    void viewRooms() {
        printHeader("Room Availability — " + hotelName);
        cout << "  " << string(51, '-') << "\n";
        for (const auto& r : rooms) r.display();
        cout << "  " << string(51, '-') << "\n";
    }

    // ── 2. Book Room ──────────────────────────
    void bookRoom() {
        printHeader("Book a Room");
        viewRooms();

        int roomNum;
        cout << "\nEnter Room Number to book: ";
        cin >> roomNum;

        Room* room = findRoom(roomNum);
        if (!room) { cout << "  [!] Room not found.\n"; return; }
        if (room->getIsBooked()) { cout << "  [!] Room is already booked.\n"; return; }

        string name, phone;
        int days;
        cin.ignore();
        cout << "Guest Name  : "; getline(cin, name);
        cout << "Phone No.   : "; getline(cin, phone);
        cout << "No. of Days : "; cin >> days;

        if (days <= 0) { cout << "  [!] Invalid number of days.\n"; return; }

        room->book();
        guests.push_back(Guest(name, phone, roomNum, days));

        cout << "\n  [✓] Room " << roomNum << " booked for " << name
             << " for " << days << " day(s).\n";
    }

    // ── 3. Order Food ─────────────────────────
    void orderFood() {
        printHeader("Food Ordering");

        int roomNum;
        cout << "Enter your Room Number: ";
        cin >> roomNum;

        Guest* guest = findGuest(roomNum);
        if (!guest) { cout << "  [!] No guest found for this room. Please book first.\n"; return; }

        cout << "\n  Welcome, " << guest->getName() << "!\n\n";
        cout << "  " << string(35, '-') << "\n";
        cout << "  #   Item                    Price\n";
        cout << "  " << string(35, '-') << "\n";
        for (int i = 0; i < (int)menu.size(); i++) {
            cout << "  " << setw(2) << (i + 1) << ". "
                 << setw(22) << left << menu[i].name
                 << "Rs." << fixed << setprecision(0) << menu[i].price << "\n";
        }
        cout << "  " << string(35, '-') << "\n";

        char more = 'y';
        while (more == 'y' || more == 'Y') {
            int choice, qty;
            cout << "\nSelect item no. (1-" << menu.size() << "): ";
            cin >> choice;
            if (choice < 1 || choice > (int)menu.size()) {
                cout << "  [!] Invalid choice.\n";
            } else {
                cout << "Quantity: ";
                cin >> qty;
                if (qty > 0) {
                    guest->addFoodOrder(FoodItem(menu[choice-1].name, qty, menu[choice-1].price));
                    cout << "  [✓] Added " << qty << "x " << menu[choice-1].name << "\n";
                }
            }
            cout << "Order more? (y/n): ";
            cin >> more;
        }
    }

    // ── 4. View Food Orders ───────────────────
    void viewFoodOrders() {
        printHeader("View Food Orders");
        int roomNum;
        cout << "Enter Room Number: ";
        cin >> roomNum;

        Guest* guest = findGuest(roomNum);
        if (!guest) { cout << "  [!] No guest for this room.\n"; return; }

        const auto& orders = guest->getFoodOrders();
        if (orders.empty()) { cout << "  No food orders yet.\n"; return; }

        cout << "\n  Guest: " << guest->getName() << "  (Room " << roomNum << ")\n";
        cout << "  " << string(45, '-') << "\n";
        cout << "  Item                    Qty    Amount\n";
        cout << "  " << string(45, '-') << "\n";
        for (const auto& f : orders) {
            cout << "  " << setw(22) << left  << f.itemName
                 << setw(5)  << right << f.quantity
                 << "   Rs." << setw(7) << fixed << setprecision(0) << f.total() << "\n";
        }
        cout << "  " << string(45, '-') << "\n";
        cout << "  Food Total: Rs." << fixed << setprecision(0) << guest->getFoodTotal() << "\n";
    }

    // ── 5. Generate Bill ──────────────────────
    void generateBill() {
        printHeader("Generate Bill / Checkout");

        int roomNum;
        cout << "Enter Room Number: ";
        cin >> roomNum;

        Guest* guest = findGuest(roomNum);
        Room*  room  = findRoom(roomNum);

        if (!guest || !room) { cout << "  [!] No active booking for this room.\n"; return; }

        double roomCharge = room->getPricePerDay() * guest->getDaysStayed();
        double foodCharge = guest->getFoodTotal();
        double subtotal   = roomCharge + foodCharge;
        double gst        = subtotal * 0.18;
        double total      = subtotal + gst;

        // ── Bill printout ──
        cout << "\n" << string(50, '*') << "\n";
        cout << "          " << hotelName << "\n";
        cout << "              INVOICE\n";
        cout << string(50, '*') << "\n";
        cout << "  Guest    : " << guest->getName()   << "\n";
        cout << "  Phone    : " << guest->getPhone()  << "\n";
        cout << "  Room     : " << roomNum << "  (" << room->getType() << ")\n";
        cout << "  Days     : " << guest->getDaysStayed() << "\n";
        cout << string(50, '-') << "\n";

        // Room charge
        cout << "  Room Charges\n";
        cout << "    Rs." << room->getPricePerDay() << "/day x "
             << guest->getDaysStayed() << " days"
             << "  =  Rs." << fixed << setprecision(2) << roomCharge << "\n";

        // Food
        cout << "\n  Food Orders\n";
        const auto& orders = guest->getFoodOrders();
        if (orders.empty()) {
            cout << "    (No food ordered)\n";
        } else {
            for (const auto& f : orders) {
                cout << "    " << setw(22) << left << f.itemName
                     << setw(3) << right << f.quantity << "x "
                     << "Rs." << f.price
                     << "  =  Rs." << fixed << setprecision(2) << f.total() << "\n";
            }
        }

        cout << string(50, '-') << "\n";
        cout << "  Sub-total          :  Rs." << fixed << setprecision(2) << subtotal   << "\n";
        cout << "  GST (18%)          :  Rs." << fixed << setprecision(2) << gst        << "\n";
        cout << string(50, '-') << "\n";
        cout << "  TOTAL AMOUNT       :  Rs." << fixed << setprecision(2) << total      << "\n";
        cout << string(50, '*') << "\n";
        cout << "      Thank you for staying at " << hotelName << "!\n";
        cout << string(50, '*') << "\n";

        // Checkout
        char ch;
        cout << "\nCheckout and free the room? (y/n): ";
        cin >> ch;
        if (ch == 'y' || ch == 'Y') {
            room->vacate();
            // Remove guest from list
            guests.erase(
                remove_if(guests.begin(), guests.end(),
                    [roomNum](const Guest& g){ return g.getRoomNumber() == roomNum; }),
                guests.end()
            );
            cout << "  [✓] Room " << roomNum << " is now available.\n";
        }
    }

    // ── 6. Display All Guests ─────────────────
    void displayGuests() {
        printHeader("Current Guests");
        if (guests.empty()) { cout << "  No guests currently checked in.\n"; return; }
        cout << "  " << string(55, '-') << "\n";
        cout << "  " << setw(6)  << left << "Room"
             << setw(20) << "Name"
             << setw(14) << "Phone"
             << setw(6)  << "Days" << "\n";
        cout << "  " << string(55, '-') << "\n";
        for (const auto& g : guests) {
            cout << "  " << setw(6)  << left << g.getRoomNumber()
                 << setw(20) << g.getName()
                 << setw(14) << g.getPhone()
                 << setw(6)  << g.getDaysStayed() << "\n";
        }
        cout << "  " << string(55, '-') << "\n";
    }

    // ── Main Menu ─────────────────────────────
    void run() {
        int choice;
        do {
            cout << "\n" << string(55, '=') << "\n";
            cout << "        " << hotelName << " — Management System\n";
            cout << string(55, '=') << "\n";
            cout << "  1. View All Rooms\n";
            cout << "  2. Book a Room\n";
            cout << "  3. Order Food\n";
            cout << "  4. View Food Orders\n";
            cout << "  5. Generate Bill / Checkout\n";
            cout << "  6. View All Guests\n";
            cout << "  0. Exit\n";
            cout << string(55, '-') << "\n";
            cout << "  Enter choice: ";
            cin >> choice;

            switch (choice) {
                case 1: viewRooms();       break;
                case 2: bookRoom();        break;
                case 3: orderFood();       break;
                case 4: viewFoodOrders();  break;
                case 5: generateBill();    break;
                case 6: displayGuests();   break;
                case 0: cout << "\n  Goodbye! Have a great day.\n\n"; break;
                default: cout << "  [!] Invalid option. Try again.\n";
            }
        } while (choice != 0);
    }
};

// ─────────────────────────────────────────────
//  main
// ─────────────────────────────────────────────
int main() {
    Hotel hotel("Grand Palace Hotel");
    hotel.run();
    return 0;
}