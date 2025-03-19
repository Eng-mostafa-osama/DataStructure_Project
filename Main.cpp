#include <iostream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
using namespace std;

// Function to get current timestamp as a string
string getCurrentTimestamp() {
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&now_time);
    stringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %X");
    return ss.str();
}
// Find the first non-whitespace character 
string trim(const string &str) { 
    size_t first = str.find_first_not_of(" \t\n\r"); 
    if (first == string::npos) return ""; // String contains only whitespace
    // Find the last non-whitespace character
size_t last = str.find_last_not_of(" \t\n\r");
string trimmed = str.substr(first, last - first + 1);

// If more than one character remains, throw an error
if (trimmed.length() > 1)
    throw runtime_error("Multiple characters entered!");

return trimmed;
}

// Logging function to CSV
void logLuggageID(const string &luggageID) {
             ofstream file("luggage_log.csv", ios::app); if(file.is_open()) { if(file.tellp() == 0) file << "LuggageID" << "\n"; file << luggageID << "\n"; file.close(); } else { cerr << "Error: Unable to open the CSV file for writing." << "\n"; } }

void printDepartureLog() { 
            ifstream file("luggage_log.csv"); 
            if(file.is_open()) { string line; 
                while(getline(file, line)) { cout << line << "\n"; } file.close(); } 
            else { cerr << "Error: Unable to open the CSV file for reading." << "\n"; } }

class Luggage {
    private:
        string passengerName;
        string ticketClass;
        double weight;
        int bagNumber;
    public:
        Luggage(string passengerName, string ticketClass, double weight, int bagNumber) 
            : passengerName(passengerName), ticketClass(ticketClass), weight(weight), bagNumber(bagNumber) {}
        Luggage() : passengerName(""), ticketClass(""), weight(0.0), bagNumber(0) {}
    
        // Getters
        string getPassengerName() const { return passengerName; }
        string getTicketClass() const { return ticketClass; }
        double getWeight() const { return weight; }
        int getBagNumber() const { return bagNumber; }
    };

class Stack{
    private:
    Luggage* stackArray;
    int size;
    int top;
public:
    Stack(int size) : size(size), top(-1) {
        stackArray = new Luggage[size];
    }
    ~Stack() {
        delete[] stackArray;
    }

    bool isEmpty() { return top == -1; }
    bool isFull() { return top == size - 1; }

    void push(const Luggage& luggage) {
        if (isFull()) {
            cout << "Stack Overflow" << endl;
        } else {
            stackArray[++top] = luggage;
            logLuggageID(to_string(luggage.getBagNumber()));
        }
    }

    void pop() {
        if (isEmpty()) {
            cout << "Stack Underflow" << endl;
        } else {
            Luggage item = stackArray[top--];
            logLuggageID(to_string(item.getBagNumber()));
        }
    }
};

class Queue {
    private:
        Luggage* queueArray;
        int size;
        int front, rear, currentSize;
    public:
        Queue(int size) : size(size), front(0), rear(0), currentSize(0) {
            queueArray = new Luggage[size];
        }
        ~Queue() {
            delete[] queueArray;
        }
    
        bool isEmpty() { return currentSize == 0; }
        bool isFull() { return currentSize == size; }
    
        void enqueue(const Luggage& luggage) {
            if (isFull()) {
                cout << "Queue Overflow" << endl;
            } else {
                queueArray[rear] = luggage;
                rear = (rear + 1) % size;
                currentSize++;
                logLuggageID(to_string(luggage.getBagNumber()));
            }
        }
    
        Luggage dequeue() {
            if (isEmpty()) {
                cout << "Queue Underflow" << endl;
                return Luggage();
            } else {
                Luggage item = queueArray[front];
                front = (front + 1) % size;
                currentSize--;
                logLuggageID(to_string(item.getBagNumber()));
                return item;
            }
        }
    };
    
int main()
{
    int stack_Queue_Size = 200;
    string passenger_Class,passengerName,luggageIdPrefix;
    int mainMenuChoice,LuggageMenuChoice,passengerCount;
    int luggageId, ticketId = 0;
    int luggageNumber;
    double luggageWeight;
    bool toMainMenu,addPassenger,switchLoops = false;
    
    while(true)
    {
        cout << "1. Start new System" << "\n";
        cout << "2. Exit application" << "\n";
        cout << "Enter your choice: ";
        cin >> mainMenuChoice;
        
        if(mainMenuChoice == 2)
        {
            cout << "Exiting application." << "\n";
            break;
        }
        else if(mainMenuChoice == 1)
        {
            bool to_Main_Menu = false;
            while(!to_Main_Menu)
            {
                cout << "\nSub Menu" << "\n";
                cout << "1. Enter passenger's luggage" << "\n";
                cout << "2. Show luggage in departure conveyor belt" << "\n";
                cout << "3. Show luggage in plane's cargo" << "\n";
                cout << "4. Exit to Main Menu" << "\n";
                cout << "Enter your choice: ";
                cin >> LuggageMenuChoice;
                
                switch(!toMainMenu)
                {
                    case 1:
                    {
                        if(passengerCount >= 100)
                        {
                            cout << "Passenger amount exceeded the limit." << "\n";
                            break;
                        }
                        bool add_Passenger_Valid = false;
                        while(!add_Passenger_Valid)
                        {
                            cout << "Enter the passenger name: ";
                            cin.ignore();
                            getline(cin, passengerName);
                            
                            cout << "Enter the passenger class (b/e): ";
                            cin >> passenger_Class;
                           char passenger_Class_Choice = trim(passenger_Class);

                            while(passenger_Class != 'b' && passenger_Class != 'e')
                            {
                                cout << "Error. Enter a valid class (b/e): ";
                                cin >> passenger_Class;
                            }
                            
                            cout << "Enter the ticket ID: ";
                            cin >> ticketId;
                            
                            cout << "Enter number of bags (should be 2): ";
                            cin >> luggageNumber;
                            if(luggageNumber != 2)
                            {
                                cout << "Error. Luggage number is incorrect. Try again." << "\n";
                                continue;
                            }
                            
                            for(int i = 0; i < luggageNumber; i++)
                            {
                                bool validWeight = false;
                                while(!validWeight)
                                {
                                    cout << "Enter weight for bag " << (i+1) << ": ";
                                    cin >> luggageWeight;
                                    if(luggageWeight < 0)
                                    {
                                        cout << "Error. Weight cannot be negative." << "\n";
                                        continue;
                                    }
                                    if(passenger_Class == 'b')
                                    {
                                        if(luggageWeight > 15)
                                            cout << "Error. For Business class, bag weight cannot exceed 15KG." << "\n";
                                        else
                                            validWeight = true;
                                    }
                                    else if(passenger_Class == 'e')
                                    {
                                        if(luggageWeight > 10)
                                            cout << "Error. For Economy class, bag weight cannot exceed 10KG." << "\n";
                                        else
                                            validWeight = true;
                                    }
                                }
                            }
                            
                            luggageId++;
                            string result_Luggage_Id = luggageIdPrefix + to_string(luggageId);
                            departure_Conveyer_Belt(result_Luggage_Id);
                            cout << "Luggage logged with ID: " << result_Luggage_Id << "\n";
                            
                            add_Passenger_Valid = true;
                            passengerCount++;
                        }
                        break;
                    }
                    case 2:
                    {
                        cout << "\nDeparture Conveyor Belt Log:" << "\n";
                        printDepartureLog();
                        break;
                    }
                    case 3:
                    {
                        cout << "Plane Cargo details not implemented." << "\n";
                        break;
                    }
                    case 4:
                    {
                        to_Main_Menu = true;
                        break;
                    }
                    default:
                        cout << "Error. Wrong choice. Try again." << "\n";
                        break;
                }
            }
        }
        else
            cout << "Invalid choice. Try again." << "\n";
    }
    return 0;
    
}