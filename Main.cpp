#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <limits>
using namespace std;

void logToCSV(const string &bagId, const string &ticketId, const string &passengerName)
{
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&now_time);
    stringstream ss;
    ss << put_time(&local_tm, "%Y-%m-%d %X");

    ofstream csvFile("departure_log.csv", ios::app);
    if (csvFile.is_open())
    {
        // Write header if the file is empty
        if (csvFile.tellp() == 0)
        {
            csvFile << "BagID,TicketID,PassengerName,Timestamp\n";
        }
        // Write data row
        csvFile << bagId << "," << ticketId << "," << passengerName << "," << ss.str() << "\n";
        csvFile.close();
    }
    else
    {
        cerr << "Unable to open CSV file." << endl;
    }
}

/* char trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
    {
        throw "no input detected";
    }
    size_t last = str.find_last_not_of(" \t\n\r");
    string trimmed = str.substr(first, last - first + 1);
    return trimmed[0];
} */

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

int getValidatedInt(int min, int max) {
    int x;
    while (true) {
        if (cin >> x) {
            if (x >= min && x <= max) {
                // Clear buffer AFTER validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            // Clear buffer for out-of-range numbers
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter between " << min << "-" << max << ": ";
        } else {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer: ";
        }
    }
}

double getValidatedDouble(double min, double max) {
    double x;
    while (true) {
        if (cin >> x) {
            if (x >= min && x <= max) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            cout << "Error luggage weight exceeds the limit" << endl;
        } else {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Validate name contains only letters and spaces
bool isValidName(const string &name) {
    if (name.empty()) return false;
    return all_of(name.begin(), name.end(), [](char c) {
        return isalpha(c) || c == ' ' || c == '\''; // Allow apostrophes
    });
}

class Node
{
public:
    string data;
    Node *next;
    Node()
    {
        data = "";
        next = nullptr;
    }
};

// Queue Class with CSV logging
class customQueue
{
private:
    Node *front;
    Node *rear;

public:
    customQueue()
    {
        front = nullptr;
        rear = nullptr;
    }
    ~customQueue()
    {
        while (front != nullptr)
        {
            Node *temp = front;
            front = front->next;
            delete temp;
        }
    }

    bool isEmpty()
    {
        return (front == nullptr);
    }

    void enqueue(string item)
    {
        Node *newNode = new Node();
        newNode->data = item;
        newNode->next = nullptr;
        if (rear == nullptr)
        {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    string dequeue()
    {
        if (isEmpty())
        {
            cout << "Queue Underflow" << endl;
            return ""; // Return an empty string if the queue is empty
        }
        else
        {
            Node *temp = front;
            string value = front->data;
            front = front->next;
            if (front == nullptr)
                rear = nullptr; // If the queue becomes empty, set rear to nullptr
            delete temp;
            return value; // Return the dequeued item
        }
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Queue Contents:" << endl;
        Node *current = front;
        while (current != nullptr)
        {
            cout << current->data << endl;
            current = current->next;
        }
    }
};

// Stack Class
class customStack
{
private:
    Node *top;

public:
    customStack()
    {
        top = nullptr;
    }
    ~customStack()
    {
        while (top != nullptr)
        {
            Node *temp = top;
            top = top->next;
            delete temp;
        }
    }
    bool isEmpty()
    {
        return (top == nullptr);
    }

    void push(string item)
    {
        Node *newNode = new Node();
        newNode->data = item;
        newNode->next = top;
        top = newNode;
    }

    string pop()
    {
        if (isEmpty())
        {
            cout << "Stack Underflow" << endl;
            return ""; // Return an empty string if the stack is empty
        }
        else
        {
            Node *temp = top;
            string value = top->data;
            top = top->next;
            delete temp;
            return value; // Return the popped item
        }
    }

    void peek()
    {
        if (isEmpty())
        {
            cout << "Stack is empty. No peek value." << endl;
        }
        cout << top->data << endl;
    }
    void display()
    {
        if (isEmpty())
        {
            cout << "Stack is empty." << endl;
            return;
        }
        cout << "Stack Contents:" << endl;
        Node *current = top;
        while (current != nullptr)
        {
            cout << current->data << endl;
            current = current->next;
        }
    }
};

// System Functions
void clearScreen()
{
    system("cls || clear");
}
int main()
{
    const int stack_Queue_Size = 200;
    int passenger_Class;
    int main_Menu_Choice, luggage_Menu_Choice, passengers_count = 0;
    int luggage_Id = 0;

    string passenger_Name, luggage_Id_Prefix, result_Luggage_Id, result_Ticket_Id;
    int luggage_Number, ticket_Id;
    double luggage_Weight;
    bool to_Main_Menu = false;
    bool add_Passenger_Valid = true;
    bool switch_Loops = false;
    customQueue departure_Conveyer_Belt = customQueue();
    customQueue arrival_Conveyer_Belt = customQueue();
    customStack plane_Cargo = customStack();
    while (true)
    {
        cout << "1.Start System\t" << "2. Exit application\n";
        
        int main_Menu_Choice = getValidatedInt(1, 2);

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

            while (!to_Main_Menu)
            {
                clearScreen();
                cout << "1. Enter passengers luggage\t2. Show luggage in conveyer belt\n3. show luggage in plane's cargo\t4.Show the luggage History\n 5.Exit" << endl;
                int luggage_Menu_Choice = getValidatedInt(1, 5);
                switch (luggage_Menu_Choice)
                {
                case 1:
                    clearScreen();
                    while (add_Passenger_Valid)
                    {
                        cout << "Enter Passenger Name\n";
                        cin.ignore();
                        
                        while (true)
                        {
                           getline(cin, passenger_Name);
                           passenger_Name = trim(passenger_Name);
                           if(passenger_Name.empty())
                           {
                               cout << "Invalid name. Please enter a valid name: ";
                               continue;
                           }
                           else if (isValidName(passenger_Name))
                            {
                                 break;
                            }
                            else
                            {
                                 cout << "Invalid name. Please enter a valid name: ";
                            } 
                        }
                        
                        cout << "Enter the passenger class (Business -> 1     Economy -> 2): " << endl;
                        int passenger_Class = getValidatedInt(1, 2);
                        if ((passenger_Class != 1) && (passenger_Class != 2))
                        {
                            while ((passenger_Class != 1) || (passenger_Class != 2))
                            {
                                cout << "Invalid Class Please Try Again" << endl;
                                cout << "Enter the passenger class ((Business -> 1     Economy -> 2): " << endl;

                                while (true)
                                {
                                    cin >> passenger_Class;
                                    if ((passenger_Class != 1) || (passenger_Class != 2))
                                    {
                                        cout << "Invalid Class Please Try Again" << endl;
                                        continue;
                                    }
                                    else if (passenger_Class == 1 || passenger_Class == 2)
                                    {
                                        break;
                                    }
                                }
                            }
                        }

                        cout << "Enter Ticket ID:";
                        cin >> ticket_Id;
                        cout << "Enter luggage Weight and number of bags\t( BE -> 2   bags each 15KG\t EC -> 2 bags each 10KG" << endl;
                        int luggage_Number = getValidatedInt(0, 2);

                        

                        while (true)
                        {
                            if (luggage_Number == 0)
                            {
                                break;
                            }

                            else
                            {

                                if (passenger_Class == 1)
                                {
                                    for (int i = 0; i < luggage_Number; i++)
                                    {
                                        while (true)
                                        {
                                            cout << "Enter luggage weight:";
                                            double luggage_Weight = getValidatedDouble(0, 15);
                                            if ((luggage_Weight <= 15) || (luggage_Weight >= 0))
                                            {
                                                luggage_Id++;
                                                luggage_Id_Prefix = "BE";
                                                result_Luggage_Id = luggage_Id_Prefix + to_string(luggage_Id);
                                                result_Ticket_Id = luggage_Id_Prefix + to_string(ticket_Id);
                                                departure_Conveyer_Belt.enqueue(result_Luggage_Id);

                                                break;
                                            }
                                        }
                                    }
                                }
                                else if (passenger_Class == 2)
                                {
                                    for (int i = 0; i < luggage_Number; i++)
                                    {
                                        while (true)
                                        {
                                            cout << "Enter luggage weight:";
                                            double luggage_Weight = getValidatedDouble(0, 15);
                                            if ((luggage_Weight <= 10) || (luggage_Weight >= 0))
                                            {
                                                luggage_Id++;
                                                luggage_Id_Prefix = "EC";
                                                result_Luggage_Id = luggage_Id_Prefix + to_string(luggage_Id);
                                                result_Ticket_Id = luggage_Id_Prefix + to_string(ticket_Id);
                                                departure_Conveyer_Belt.enqueue(result_Luggage_Id);
                                                break;
                                            }
                                        }
                                    }
                                }
                            }
                            break;
                        }
                        logToCSV(result_Luggage_Id, result_Ticket_Id, passenger_Name);
                        passengers_count++;
                        if (passengers_count > 100)
                        {
                            cout << "plane limit exceeded" << endl;
                            add_Passenger_Valid = false;
                            break;
                        }
                        else
                        {
                            cout << "1. To Enter A new Passenger\t 2.exit Passenger Luggage Menu" << endl;
                            cin >> luggage_Menu_Choice;
                            if (luggage_Menu_Choice == 1)
                            {
                                clearScreen();
                                continue;
                            }
                            else if (luggage_Menu_Choice == 2)
                            {
                                clearScreen();
                                break;
                            }
                        }
                    }

                    break;
                case 2:
                    clearScreen();

                    cout << "1.Show luggage in the departure conveyer belt\t2. Show luggage in the arrival conveyer belt \n3.Exit" << endl;
                    luggage_Menu_Choice = getValidatedInt(1, 3);
                    while (true)
                    {
                        if (luggage_Menu_Choice != 1 && luggage_Menu_Choice != 2 && luggage_Menu_Choice != 3)
                        {
                            cout << "Invalid Choice Please Try Again" << endl;
                            cout << "1.Show luggage in the departure conveyer belt\t2. Show luggage in the arrival conveyer belt" << endl;
                            luggage_Menu_Choice = getValidatedInt(1, 3);
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (luggage_Menu_Choice == 1)
                    {
                        departure_Conveyer_Belt.display();
                        cout << "Press any key to continue" << endl;
                        cin.get();
                        cout << "\n1. To move luggage to the plane's cargo\t2. To exit" << endl;
                        luggage_Menu_Choice = getValidatedInt(1, 2);
                        if (luggage_Menu_Choice == 1)
                        {
                            while (!departure_Conveyer_Belt.isEmpty())
                            {

                                plane_Cargo.push(departure_Conveyer_Belt.dequeue());
                                if (departure_Conveyer_Belt.isEmpty())
                                {
                                    cout << "All luggage has been moved to the plane's cargo" << endl;
                                    break;
                                }
                            }
                        }
                        else if (luggage_Menu_Choice == 2)
                        {
                            break;
                        }
                    }
                    else if (luggage_Menu_Choice == 2)
                    {

                        arrival_Conveyer_Belt.display();
                        cout << "Press any key to continue" << endl;
                        cin.get();
                    }
                    else if (luggage_Menu_Choice == 3)
                    {
                        break;
                    }

                    break;
                case 3:
                    clearScreen();
                    plane_Cargo.display();
                    cout << "Press any key to continue" << endl;
                    cin.get();
                    cout << "1. To offload the luggage into the conveyer belt\t 2.to Exit" << endl;
                    luggage_Menu_Choice = getValidatedInt(1, 2);
                    if (luggage_Menu_Choice == 1)
                    {
                        while (!plane_Cargo.isEmpty())
                        {
                            string tmpluggage = plane_Cargo.pop();
                            cout << "Offloading luggage: " << tmpluggage << endl;
                            arrival_Conveyer_Belt.enqueue(tmpluggage);
                            if (plane_Cargo.isEmpty())
                            {
                                cout << "All luggage has been offloaded into the conveyer belt" << endl;
                                break;
                            }
                        }
                    }
                    else if (luggage_Menu_Choice == 2)
                    {
                        break;
                    }

                    break;
                case 4:
                    clearScreen();
                    printCSVContents();
                    cout << "Press any key to continue" << endl;
                    cin.get();
                    break;

                case 5:
                    clearScreen();
                    return 1;
                    break;

                default:
                    cout << "Invalid Choice Please Try Again" << endl;
                    break;
                }
            }
        }
    }

    return 0;
}
