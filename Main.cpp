#include <iostream>
using namespace std;
class Luggage{
    private:
    string passengerName; // Passenger name
    string ticketClass;  // Ticket class
    double weight;      // Weight of the luggage
    int bagNumber;     // Bag number
public:
Luggage(string passengerName, string ticketClass, double weight, int bagNumber) {
        this->passengerName = passengerName;
        this->ticketClass = ticketClass;
        this->weight = weight;
        this->bagNumber = bagNumber;
    } // Constructor to initialize luggage details
    Luggage() : passengerName(""), ticketClass(""), weight(0.0), bagNumber(0) {} // Default constructor

};

class stack{
    private:
    int *stackArray; // Array to store stack elements
    int size;   // Maximum size
    int top;    // Index of the top element

    public:
    stack(int size) //constructor
    {
        this -> size = size;
        stackArray = new int[size];
        top = -1;
    }
   
    ~stack() //deconstructor
    {
        delete[] stackArray;
    }
   
    bool isEmpty()
    {
        return (top == -1);
    }

    bool isFull()
    {
        return (top == (size-1));
    }

    void push(int newItem)
    {
        if(isFull() == true){
            cout << "Stack Overflow";
        }
        else{
            stackArray[++top] = newItem;
            cout << "pushed " <<newItem << endl; 
        }
    }

    void pop()
    {
        if(isEmpty() == true){
            cout << "Stack Underflow";
           
        }
        else{
            int item = stackArray[top--];
            cout << "popped item: "<< item << endl;
            
        }
    }

    int getTop()
    {
        if (isEmpty()== true)
        {
            cout << "stack is empty" << endl;
            return -1;
        }
        else{
            return stackArray[top];
        }
        
    }

    void display() {
        if (isEmpty()) {
            cout << "Stack is empty!" << endl;
        } else {
            cout << "Stack elements (top to bottom): ";
            for (int i = top; i >= 0; i--) {
                cout << stackArray[i] << endl;
            }
            cout << endl;
        }
    }
};

class queue{
    private:
    int queue_Front,queue_Tail;
    string* queueArray;
    int size;
    int current_Size;

    public:
    queue(int size){
        this -> size = size;
        queueArray = new string[size];
        queue_Front = 0;
        queue_Tail = 0;
        current_Size = 0;

    }
    ~queue() {
        delete[] queueArray; // Free dynamically allocated memory
    }
    bool isEmpty(){
        if(current_Size == 0)
        {
            cout <<"the Queue is empty"<<endl ;
            return true;
        }
        else
        return false;
        
        
    }
    bool isFull(){
        return current_Size == size;
          
    }
   
    void enqueue(string item)
    {
        if(isFull() == true){
            cout << "Queue Overflow" << endl;
        }
        else{
        queueArray[queue_Tail] = item;
        queue_Tail = (queue_Tail + 1) % size; // Circular behavior
        current_Size++;}
        
    }

    void dequeue(){
        if(isEmpty() == true){
            cout << "Queue Underflow" << endl;
        }
        else{
        string value = queueArray[queue_Front];
        queue_Front = (queue_Front + 1) % size; // Circular behavior
        current_Size--;
    }
    }

    void peek() {
        if (isEmpty()) {
            cout << "Queue is empty. No peek value." << endl;
            
        }
         cout << queueArray[queue_Front] << endl;
    }

    void display() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return;
        }
        cout << "Queue Contents:" << endl;

        int count = 0;
        int index = queue_Front;
        while (count < current_Size) {
            cout << queueArray[index] << endl;
            index = (index + 1) % size;
            count++;
        }
        
        
    }
};
int main()
{
    queue q(4);
    q.enqueue("num 1");
    q.enqueue("num 2");
    q.enqueue("num 3");
    q.enqueue("num 4");
    q.peek();
    q.display();
    return 0;
}