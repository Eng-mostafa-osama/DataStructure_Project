#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
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

// Queue Class with CSV logging
class customQueue
{
private:
    int queue_Front, queue_Tail;
    string *queueArray;
    int size;
    int current_Size;

public:
    customQueue(int size)
    {
        this->size = size;
        queueArray = new string[size];
        queue_Front = 0;
        queue_Tail = 0;
        current_Size = 0;
    }
    ~customQueue()
    {
        delete[] queueArray; // Free dynamically allocated memory
    }
    bool isEmpty()
    {
        if (current_Size == 0)
        {
            cout << "the Queue is empty" << endl;
            return true;
        }
        else
            return false;
    }
    bool isFull()
    {
        return current_Size == size;
    }

    void enqueue(string item)

    {
        if (isFull() == true)
        {
            cout << "Queue Overflow" << endl;
        }
        else
        {
            queueArray[queue_Tail] = item;
            queue_Tail = (queue_Tail + 1) % size; // Circular behavior
            current_Size++;
        }
    }
    string dequeue()
    {
        if (isEmpty() == true)
        {
            cout << "Queue Underflow" << endl;
            return ""; // Return an empty string if the queue is empty
        }
        else
        {
            string value = queueArray[queue_Front];
            queue_Front = (queue_Front + 1) % size; // Circular behavior
            current_Size--;
            return value; // Return the dequeued item
        }
    }

    void peek()
    {
        if (isEmpty())
        {
            cout << "Queue is empty. No peek value." << endl;
        }
        cout << queueArray[queue_Front] << endl;
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Queue Contents:" << endl;

        int count = 0;
        int index = queue_Front;
        while (count < current_Size)
        {
            cout << queueArray[index] << endl;
            index = (index + 1) % size;
            count++;
        }
    }
};

// Stack Class
class customStack
{
private:
    string *stackArray; // Array to store stack elements
    int size;           // Maximum size
    int top;            // Index of the top element

public:
    customStack(int size) // constructor
    {
        this->size = size;
        stackArray = new string[size];
        top = -1;
    }

    ~customStack() // deconstructor
    {
        delete[] stackArray;
    }

    bool isEmpty()
    {
        return (top == -1);
    }

    bool isFull()
    {
        return (top == (size - 1));
    }

    void push(string newItem)
    {
        if (isFull() == true)
        {
            cout << "Stack Overflow";
        }
        else
        {
            stackArray[++top] = newItem;
            cout << "pushed " << newItem << endl;
        }
    }

    string pop()
    {
        if (isEmpty() == true)
        {
            cout << "Stack Underflow";
            return "";
        }
        else
        {
            string item = stackArray[top--];
            cout << "popped item: " << item << endl;
            return item;
        }
    }

    string getTop()
    {
        if (isEmpty() == true)
        {
            cout << "stack is empty" << endl;
            return "";
        }
        else
        {
            return stackArray[top];
        }
    }

    void display()
    {
        if (isEmpty())
        {
            cout << "Stack is empty!" << endl;
        }
        else
        {
            cout << "Stack elements (top to bottom): ";
            for (int i = top; i >= 0; i--)
            {
                cout << stackArray[i] << endl;
            }
            cout << endl;
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
    customQueue departure_Conveyer_Belt(stack_Queue_Size);
    customQueue arrival_Conveyer_Belt(stack_Queue_Size);
    customStack plane_Cargo(stack_Queue_Size);
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

            while (!to_Main_Menu)
            {
                clearScreen();
                cout << "1. Enter passengers luggage\t2. Show luggage in conveyer belt\n3. show luggage in plane's cargo\t4.Show the luggage History\n 5.Exit" << endl;
                cin >> luggage_Menu_Choice;
                switch (luggage_Menu_Choice)
                {
                case 1:
                    clearScreen();
                    while (add_Passenger_Valid)
                    {
                        cout << "Enter Passenger Name\n";
                        cin.ignore();
                        getline(cin, passenger_Name);
                        cout << "Enter the passenger class (Business -> 1     Economy -> 2): " << endl;
                        cin >> passenger_Class;
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
                        while (true)
                        {
                            cout << "Number of Bags:";
                            cin >> luggage_Number;
                            if (luggage_Number < 0 || luggage_Number > 2)
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

                                if (passenger_Class == 1)
                                {
                                    for (int i = 0; i < luggage_Number; i++)
                                    {
                                        while (true)
                                        {
                                            cout << "Enter luggage weight:";
                                            cin >> luggage_Weight;
                                            if ((luggage_Weight > 15) || (luggage_Weight < 0))
                                            {
                                                cout << "Error luggage weight exceeds the limit" << endl;
                                            }
                                            else
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
                                            cin >> luggage_Weight;
                                            if ((luggage_Weight > 10) || (luggage_Weight < 0))
                                            {
                                                cout << "Error luggage weight exceeds the limit" << endl;
                                                continue;
                                            }
                                            else
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
                    cin >> luggage_Menu_Choice;
                    while (true)
                    {
                        if (luggage_Menu_Choice != 1 && luggage_Menu_Choice != 2 && luggage_Menu_Choice != 3)
                        {
                            cout << "Invalid Choice Please Try Again" << endl;
                            cout << "1.Show luggage in the departure conveyer belt\t2. Show luggage in the arrival conveyer belt" << endl;
                            cin >> luggage_Menu_Choice;
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
                        cin.ignore();
                        cin.get();
                        clearScreen();
                        cout << "\n1. To move luggage to the plane's cargo\t2. To exit" << endl;
                        cin >> luggage_Menu_Choice;
                        while (luggage_Menu_Choice != 1 && luggage_Menu_Choice != 2)
                        {
                            cout << "Invalid Choice Please Try Again" << endl;
                            cout << "\n1. To move luggage to the plane's cargo\t2. To exit" << endl;
                            cin >> luggage_Menu_Choice;
                        }
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
                        cin.ignore();
                        cin.get();
                        clearScreen();
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
                    cin.ignore();
                    cin.get();
                    cout << "1. To offload the luggage into the conveyer belt\t 2.to Exit" << endl;
                    cin >> luggage_Menu_Choice;
                    while (luggage_Menu_Choice != 1 && luggage_Menu_Choice != 2)
                    {
                        cout << "Invalid Choice Please Try Again" << endl;
                        cout << "1. To offload the luggage into the conveyer belt\t 2.to Exit" << endl;
                        cin >> luggage_Menu_Choice;
                    }
                    if (luggage_Menu_Choice == 1)
                    {
                        while (!plane_Cargo.isEmpty())
                        {
                            arrival_Conveyer_Belt.enqueue(plane_Cargo.pop());
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
                    cin.ignore();
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