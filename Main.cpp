#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
using namespace std;

// CSV Logging Functions
void logToCSV(const string &bagId)
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&now_time);
    stringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %X");

    ofstream csvFile("departure_log.csv", ios::app);
    if (csvFile.is_open())
    {
        if (csvFile.tellp() == 0)
        {
            csvFile << "BagID,Timestamp\n";
        }
        csvFile << bagId << "," << ss.str() << "\n";
        csvFile.close();
    }
    else
    {
        cerr << "Unable to open CSV file." << endl;
    }
}

void printCSVContents()
{
    ifstream csvFile("departure_log.csv");
    if (csvFile.is_open())
    {
        string line;
        while (getline(csvFile, line))
        {
            cout << line << endl;
        }
        csvFile.close();
    }
    else
    {
        cerr << "No log file found." << endl;
    }
}
// Luggage Class
class Luggage
{
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
class Queue
{
private:
    Luggage **queueArray;
    int capacity;
    int front;
    int rear;
    int count;

public:
    Queue(int size) : capacity(size), front(0), rear(-1), count(0)
    {
        queueArray = new Luggage *[capacity];
    }

    ~Queue()
    {
        delete[] queueArray;
    }

    void enqueue(Luggage *luggage)
    {
        if (isFull())
        {
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

    Luggage *dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue underflow!" << endl;
            return nullptr;
        }
        Luggage *item = queueArray[front];
        front = (front + 1) % capacity;
        count--;
        return item;
    }

    bool isEmpty() const { return count == 0; }
    bool isFull() const { return count == capacity; }
};

// Stack Class
class Stack
{
private:
    Luggage **stackArray;
    int capacity;
    int top;

public:
    Stack(int size) : capacity(size), top(-1)
    {
        stackArray = new Luggage *[capacity];
    }

    ~Stack()
    {
        delete[] stackArray;
    }

    void push(Luggage *luggage)
    {
        if (isFull())
        {
            cout << "Stack overflow!" << endl;
            return;
        }
        stackArray[++top] = luggage;
    }

    Luggage *pop()
    {
        if (isEmpty())
        {
            cout << "Stack underflow!" << endl;
            return nullptr;
        }
        return stackArray[top--];
    }

    bool isEmpty() const { return top == -1; }
    bool isFull() const { return top == capacity - 1; }
};

// System Functions
void clearScreen()
{
    system("cls || clear");
}

int main()
{
    const int stack_Queue_Size = 200;
    string passenger_Class;
    int main_Menu_Choice, luggage_Menu_Choice, passengers_count;
    int luggage_Id = 0;
    string passenger_Name, luggage_Id_Prefix;
    int luggage_Number;
    double luggage_Weight;
    bool to_Main_Menu = false;
    bool add_Passenger_Valid = false;
    bool switch_Loops = false;


    Queue departure_Conveyer_Belt(stack_Queue_Size);           
    Queue arrival_Conveyer_Belt(stack_Queue_Size);            
    Stack plane_Cargo(stack_Queue_Size);

    while (true)
    {
        cout << "1.Start System\t" << "2. Exit application\n";
        cin >> main_Menu_Choice;
        if (!(main_Menu_Choice == 1 || main_Menu_Choice == 2))
        {
            while (true)
            {
                cout << "incorrect Choice  Please Try Again" << endl;
                cin >> main_Menu_Choice;
                if (main_Menu_Choice == 1 || main_Menu_Choice == 2)
                {
                    break;
                }
                else
                {
                    continue;
                }
            }
        }
        else if (main_Menu_Choice == 2)
        {
            cout << "good Bye";
            return 1;
        }
        else if (main_Menu_Choice == 1)
        {
            clearScreen();
            while (!to_Main_Menu)
            {
                cout<< "1. Enter passengers luggage\t2. Show luggage in conveyer belt\n	3. show luggage in plane’s cargo\t4.Exit application"<< endl;
                cin >> luggage_Menu_Choice;
                switch (luggage_Menu_Choice)
                {
                case 1:
                    clearScreen();
                    cin >> add_Passenger_Valid;
                    while (!add_Passenger_Valid)
                    {
                        cout << "Enter Passenger Name\n";
                        cin >> passenger_Name;
                        cout << "Enter the passenger class (b/e): " << endl;
                        cin >> passenger_Class;
                        while (passenger_Class._Equal('b') || passenger_Class != 'c')
                        {
                            /* code */
                        }
                        

                    }
                    
                    break;

                case 2:
                clearScreen();
                    break;
                case 3:
                clearScreen();
                    break;
                case 4:
                clearScreen();
                return 1;
                    break;

                default:
                    break;
                }
            }

        }
    }

    return 0;
}