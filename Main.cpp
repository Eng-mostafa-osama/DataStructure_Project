#include <iostream>
using namespace std;
class stack{
    private:
    int* stackArray; // Array to store stack elements
    int size;   // Maximum size
    int top;    // Index of the top element

    public:
    stack(int size) //constructor
    {
        this -> size = size;
        stackArray = new int(size);
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

    int pop()
    {
        if(isEmpty() == true){
            cout << "Stack Underflow";
            return -1;
        }
        else{
            int item = stackArray[top--];
            cout << "popped item: "<< item << endl;
            return item;
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
int main()
{
    stack mystack(5);
    for (int i = 0; i < 4; i++)
    {
        mystack.push(i);
    }
    mystack.display();
    mystack.getTop();
    mystack.isFull();
    for (int i = 0; i < 5; i++)
    {
        mystack.pop();
    }
    mystack.display();
    mystack.getTop();
    mystack.isEmpty();
    return 0;
}