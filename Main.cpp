#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <chrono>
#include <iomanip>
using namespace std;

// CSV Logging Functions
void logToCSV(const string& bagId) {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&now_time);
    stringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %X");

    ofstream csvFile("departure_log.csv", ios::app);
    if (csvFile.is_open()) {
        if (csvFile.tellp() == 0) {
            csvFile << "BagID,Timestamp\n";
        }
        csvFile << bagId << "," << ss.str() << "\n";
        csvFile.close();
    } else {
        cerr << "Unable to open CSV file." << endl;
    }
}

void printCSVContents() {
    ifstream csvFile("departure_log.csv");
    if (csvFile.is_open()) {
        string line;
        while (getline(csvFile, line)) {
            cout << line << endl;
        }
        csvFile.close();
    } else {
        cerr << "No log file found." << endl;
    }
}

// Luggage Class
class Luggage {
private:
    string passengerName;
    string ticketClass;
    double weight;
    int bagNumber;

public:
    Luggage(string name, string cls, double w, int num)
        : passengerName(name), ticketClass(cls), weight(w), bagNumber(num) {}

    string getTicketClass() const { return ticketClass; }
    int getBagNumber() const { return bagNumber; }
    string getPassengerName() const { return passengerName; }
    double getWeight() const { return weight; }
};

// Queue Class with CSV logging
class Queue {
private:
    Luggage** queueArray;
    int capacity;
    int front;
    int rear;
    int count;

public:
    Queue(int size) : capacity(size), front(0), rear(-1), count(0) {
        queueArray = new Luggage*[capacity];
    }

    ~Queue() {
        delete[] queueArray;
    }

    void enqueue(Luggage* luggage) {
        if (isFull()) {
            cout << "Queue overflow!" << endl;
            return;
        }
        rear = (rear + 1) % capacity;
        queueArray[rear] = luggage;
        count++;
        
        // Log to CSV
        string prefix = string(1, toupper(luggage->getTicketClass()[0]));
        string bagId = prefix + "-" + to_string(luggage->getBagNumber());
        logToCSV(bagId);
    }

    Luggage* dequeue() {
        if (isEmpty()) {
            cout << "Queue underflow!" << endl;
            return nullptr;
        }
        Luggage* item = queueArray[front];
        front = (front + 1) % capacity;
        count--;
        return item;
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
};

// Stack Class
class Stack {
private:
    Luggage** stackArray;
    int capacity;
    int top;

public:
    Stack(int size) : capacity(size), top(-1) {
        stackArray = new Luggage*[capacity];
    }

    ~Stack() {
        delete[] stackArray;
    }

    void push(Luggage* luggage) {
        if (isFull()) {
            cout << "Stack overflow!" << endl;
            return;
        }
        stackArray[++top] = luggage;
    }

    Luggage* pop() {
        if (isEmpty()) {
            cout << "Stack underflow!" << endl;
            return nullptr;
        }
        return stackArray[top--];
    }

    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == capacity - 1; }
};

// System Functions
void clearScreen() {
    system("cls || clear");
}

int main() {
    const int MAX_SIZE = 200;
    int luggage_id = 0;
    Queue departure_belt(MAX_SIZE);
    Stack plane_cargo(MAX_SIZE);

    while(true) {
        clearScreen();
        cout << "Airport Luggage System\n";
        cout << "1. Add Luggage\n2. Process to Plane\n";
        cout << "3. Show Departure Belt\n4. Show Plane Cargo\n";
        cout << "5. Display Log File\n6. Exit\nChoice: ";
        
        int choice;
        cin >> choice;

        switch(choice) {
            case 1: {
                cin.ignore();
                cout << "Passenger Name: ";
                string name;
                getline(cin, name);

                string cls;
                while(true) {
                    cout << "Class (Business/Economy): ";
                    getline(cin, cls);
                    transform(cls.begin(), cls.end(), cls.begin(), ::tolower);
                    if(cls == "business" || cls == "economy") break;
                    cout << "Invalid class! Please try again.\n";
                }

                Luggage* newLuggage = new Luggage(name, cls, 0.0, ++luggage_id);
                departure_belt.enqueue(newLuggage);
                cout << "Luggage added to departure belt!\n";
                break;
            }
            
            case 2: {
                while(!departure_belt.isEmpty()) {
                    Luggage* luggage = departure_belt.dequeue();
                    plane_cargo.push(luggage);
                }
                cout << "All luggage moved to plane cargo!\n";
                break;
            }
            
            case 3: {
                cout << "Departure Belt Contents:\n";
                // Implementation to show queue contents
                break;
            }
            
            case 4: {
                cout << "Plane Cargo Contents:\n";
                // Implementation to show stack contents
                break;
            }
            
            case 5: {
                clearScreen();
                cout << "Luggage Log File Contents:\n";
                printCSVContents();
                break;
            }
            
            case 6:
                return 0;
            
            default:
                cout << "Invalid choice! Please try again.\n";
        }
        cout << "\nPress Enter to continue...";
        cin.ignore();
        cin.get();
    }
}