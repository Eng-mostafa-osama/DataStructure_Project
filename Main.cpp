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

int getValidatedInt(int min, int max)
{
    int x;
    while (true)
    {
        if (cin >> x)
        {
            if (x >= min && x <= max)
            {
                // Clear buffer AFTER validation
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            // Clear buffer for out-of-range numbers
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Please enter between " << min << "-" << max << ": ";
        }
        else
        {
            // Handle non-integer input
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter an integer: ";
        }
    }
}

double getValidatedDouble(double min, double max)
{
    double x;
    while (true)
    {
        if (cin >> x)
        {
            if (x >= min && x <= max)
            {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                return x;
            }
            cout << "Error luggage weight exceeds the limit" << endl;
        }
        else
        {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
}

string trim(const string &str)
{
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos)
        return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, (last - first + 1));
}

// Validate name contains only letters and spaces
bool isValidName(const string &name)
{
    if (name.empty())
        return false;
    return all_of(name.begin(), name.end(), [](char c)
                  {
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

class LuggageNode
{
public:
    string luggageID;
    string status;
    string ticketID;
    string passengerName;
    int height;
    LuggageNode *left;
    LuggageNode *right;

    LuggageNode(string id, string tkt, string name, string sts)
        : luggageID(id), ticketID(tkt), passengerName(name), status(sts),
          height(1), left(nullptr), right(nullptr) {}
};

class AVLTree
{
private:
    LuggageNode *root;

    int height(LuggageNode *node)
    {
        return node ? node->height : 0;
    }

    int balanceFactor(LuggageNode *node)
    {
        return height(node->right) - height(node->left);
    }

    void updateHeight(LuggageNode *node)
    {
        node->height = 1 + max(height(node->left), height(node->right));
    }

    LuggageNode *rotateRight(LuggageNode *y)
    {
        LuggageNode *x = y->left;
        y->left = x->right;
        x->right = y;
        updateHeight(y);
        updateHeight(x);
        return x;
    }

    LuggageNode *rotateLeft(LuggageNode *x)
    {
        LuggageNode *y = x->right;
        x->right = y->left;
        y->left = x;
        updateHeight(x);
        updateHeight(y);
        return y;
    }

    LuggageNode *balance(LuggageNode *node)
    {
        updateHeight(node);
        if (balanceFactor(node) == 2)
        {
            if (balanceFactor(node->right) < 0)
                node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        if (balanceFactor(node) == -2)
        {
            if (balanceFactor(node->left) > 0)
                node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        return node;
    }

    LuggageNode *insertHelper(LuggageNode *node, string id, string tkt, string name, string sts)
    {
        if (!node)
            return new LuggageNode(id, tkt, name, sts);

        if (id < node->luggageID)
            node->left = insertHelper(node->left, id, tkt, name, sts);
        else
            node->right = insertHelper(node->right, id, tkt, name, sts);

        return balance(node);
    }

    LuggageNode *searchHelper(LuggageNode *node, string id)
    {
        if (!node || node->luggageID == id)
            return node;
        return searchHelper(id < node->luggageID ? node->left : node->right, id);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(string id, string tkt, string name, string sts)
    {
        root = insertHelper(root, id, tkt, name, sts);
    }

    LuggageNode *search(string id)
    {
        return searchHelper(root, id);
    }

    void updateStatus(string id, string newStatus)
    {
        LuggageNode *node = search(id);
        if (node)
            node->status = newStatus;
    }

    void trackLuggage(string id)
    {
        LuggageNode *found = search(id);
        if (found)
        {
            cout << "\nLuggage Details:\n"
                 << "ID: " << found->luggageID << "\n"
                 << "Status: " << found->status << "\n"
                 << "Passenger: " << found->passengerName << "\n"
                 << "Ticket: " << found->ticketID << endl;
        }
        else
        {
            cout << "Luggage not found!" << endl;
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
    AVLTree luggageTracker;
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
        cout << "1.Start System\t2. Exit application\n";
        int main_Menu_Choice = getValidatedInt(1, 2);

        if (main_Menu_Choice == 2)
        {
            cout << "Goodbye";
            return 0;
        }
        else if (main_Menu_Choice == 1)
        {
            while (!to_Main_Menu)
            {
                clearScreen();
                cout << "1. Enter passengers luggage\n2. Show luggage in conveyer belt\n"
                     << "3. Show luggage in plane's cargo\n4. Show luggage History\n"
                     << "5. Track Luggage\n6. Exit" << endl;
                
                int luggage_Menu_Choice = getValidatedInt(1, 6);
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
                            if (passenger_Name.empty())
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
                                                  luggageTracker.insert(result_Luggage_Id, result_Ticket_Id, 
                                                passenger_Name, "departure");

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
                                                luggageTracker.insert(result_Luggage_Id, result_Ticket_Id,
                                            passenger_Name, "departure");
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

                               string tmpluggage = departure_Conveyer_Belt.dequeue();
                               plane_Cargo.push(tmpluggage);
                                luggageTracker.updateStatus(tmpluggage, "cargo");
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
                            luggageTracker.updateStatus(tmpluggage, "arrival");
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


                     case 5: // New tracking feature
                {
                    clearScreen();
                    cout << "Enter Luggage ID to track (e.g., BE101): ";
                    string searchID;
                    cin >> searchID;
                    luggageTracker.trackLuggage(searchID);
                    cout << "Press any key to continue...";
                    cin.ignore();
                    cin.get();
                    break;
                }
                case 6:
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
