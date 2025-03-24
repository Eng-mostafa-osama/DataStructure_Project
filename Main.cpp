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

char trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
    {
        throw "no input detected";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    string trimmed = str.substr(first, last - first + 1);
    return trimmed[0];
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
    int luggage_Number, ticket_Id;
    double luggage_Weight;
    bool to_Main_Menu = false;
    bool add_Passenger_Valid = false;
    bool switch_Loops = false;

    Queue departure_Conveyer_Belt(stack_Queue_Size);
    Queue arrival_Conveyer_Belt(stack_Queue_Size);
    Stack plane_Cargo(stack_Queue_Size);
   /*  try
    {
        string name, result;
        getline(cin, name);
        cout << name << endl;
        try{
        result = trim(name);
        }
        
        cout << result << endl;
    }
    catch (const char *error)
    {
       
    } */

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
                cout << "1. Enter passengers luggage\t2. Show luggage in conveyer belt\n	3. show luggage in plane’s cargo\t4.Exit application" << endl;
                cin >> luggage_Menu_Choice;
                switch (luggage_Menu_Choice)
                {
                case 1:
                    clearScreen();
                    cin >> add_Passenger_Valid;
                    while (!add_Passenger_Valid)
                    {
                        cout << "Enter Passenger Name\n";
                        getline(cin, passenger_Name);
                        cout << "Enter the passenger class (b/e): " << endl;
                        getline(cin, passenger_Class);
                        
                        try{passenger_Class = trim(passenger_Class);}
                        catch(const char* error){ cout << "Error: " << error << endl;}


                        while (!(passenger_Class.compare("b")) || !(passenger_Class.compare("e")))
                        {
                            cout << "Invalid Class Please Try Again" << endl;
                            cout << "Enter the passenger class (b/e): " << endl;
                            getline(cin, passenger_Class);
                            while(true){ try
                            {
                            passenger_Class = trim(passenger_Class);
                            }
                            catch(const char* error){ cout << "Error: " << error << endl;}

                            
                        }
                           
                            
                            if (!(passenger_Class.compare("b")) || !(passenger_Class.compare("e")))
                            {
                                continue;
                            }
                            else
                            {
                                break;
                            }
                        }

                        cout << "Enter Ticket ID:";
                        cin >> ticket_Id;
                        cout << "Enter luggage Weight and number of bags\t( BE -> 2   bags each 15KG\t EC -> 2 bags each 10KG" << endl;
                        while (true)
                        {
                            cout << "Number of Bags:";
                            cin >> luggage_Number;
                            if ((luggage_Number != 2 || luggage_Number < 0) && luggage_Number > 3)
                            {
                                cout << "error luggage number is incorrect try again" << endl;
                            }

                            else
                            {
                                break;
                            }
                        }

                        while (true)
                        {
                            if (luggage_Number == 0)
                            {
                                break;
                            }

                            else
                            {
                                cout << "Enter luggage Weight:";
                                cin >> luggage_Weight;
                                if (passenger_Class == "b")
                                {
                                    while (true)
                                    {
                                        if (!(luggage_Weight > 15) || !(luggage_Weight < 0))
                                        {
                                            cout << "Enter bags weight:";
                                            cin >> luggage_Weight;
                                            luggage_Id ++; 
                                            break;
                                        }
                                        else
                                        {
                                            cout << "Error luggage weight exceeds the limit Continue" << endl;
                                        }
                                    }
                                }
                                else if (passenger_Class == "e")
                                {
                                    while (true)
                                    {

                                        if (!(luggage_Weight > 10) || !(luggage_Weight < 0))
                                        {
                                            cout << "Enter bags weight:";
                                            cin >> luggage_Weight;
                                            luggage_Id ++; 
                                            break;
                                        }
                                        else
                                        {
                                            cout << "Error luggage weight exceeds the limit" << endl;

                                            continue;
                                        }
                                    }
                                }
                                
                            }

                            break;


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
}