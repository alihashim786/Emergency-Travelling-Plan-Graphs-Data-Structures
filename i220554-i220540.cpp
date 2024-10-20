#include<iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

class Queue {
    Node* front;
    Node* rear;
    int size;

public:
    Queue() {
        front = NULL; // Initializing front as NULL
        rear = NULL; // Initializing rear as NULL
        size = 0; // Initializing size as 0
    }
    bool isEmpty() {
        return front == NULL; // Checking if front is NULL
    }
    void push(int num) {
        size++;
        Node* temp = new Node; // Creating a new node
        temp->data = num;
        temp->next = NULL;
        if (isEmpty()) {
            front = temp; // If the queue is empty, set front and rear to the new node
            rear = temp;
        }
        else {
            rear->next = temp; // If not, add the new node after the current rear and update rear
            rear = temp;
        }
    }
    void pop() {
        size--;
        if (isEmpty()) {
            cout << "Queue is empty" << endl; // If the queue is empty, print a message
            return;
        }
        Node* temp = front; // Store the front node in a temporary variable
        front = front->next; // Move front to the next node
        delete temp; // Delete the previous front node
        temp = NULL;
        if (front == NULL) {
            rear = NULL; // If front is NULL, set rear to NULL as well
        }
    }
    int getFront() {
        if (isEmpty()) {
            cout << "Queue is empty" << endl; // If the queue is empty, print a message
            return -1;
        }
        return front->data; // Return the data of the front node
    }
    int getSize() {
        return size; // Return the size of the queue
    }
};

struct graphNode {
    int cities;
    int linkedCities;
    int days;
    int count;
    graphNode* next;
};

class graph {
    graphNode* city;
    int Gsize;
public:
    graph(int totalCities) {
        if (totalCities == 0) {
            cout << "There are no cities" << endl; // If there are no cities, print a message
        }
        Gsize = totalCities;
        city = new graphNode[totalCities]; // Creating an array of graphNodes
        // Initializing the properties of the first city
        city[0].days = 0;
        city[0].count = 0;
        city[0].linkedCities = 1;
        // Loop to initialize properties for the rest of the cities
        for (int i = 0; i < Gsize - 1; i++) {
            if (i != 0) {
                city[i].count = city[i - 1].count + 1;
                if (city[i].count % 6 == 0) {
                    city[i].days = city[i - 1].days + 1;
                }
                else
                    city[i].days = city[i - 1].days;
            }
            city[i].cities = i;
            city[i].linkedCities = 1;
            // Creating a new graphNode for the next city
            graphNode* temp = new graphNode;
            temp->cities = i + 1;
            temp->next = NULL;
            temp->linkedCities = -1;
            temp->count = city[i].count + 1;
            temp->days = Gsize * 10;
            city[i].next = temp;
        }
        // Initializing properties for the last city
        city[totalCities - 1].cities = totalCities - 1;
        city[totalCities - 1].linkedCities = 0;
        city[totalCities - 1].next = NULL;
        city[totalCities - 1].count = city[totalCities - 2].count + 1;
        if (city[totalCities - 1].count % 6 == 0) {
            city[totalCities - 1].days = city[totalCities - 2].days + 1;
        }
        else
            city[totalCities - 1].days = city[totalCities - 2].days;
    }

    // Function to perform BFS and calculate the minimum days required
    int BFS() {
        bool* visited = new bool[Gsize];
        int* tempArr = new int[Gsize];
        int max = -1;
        int index = 0;
        // Initializing arrays
        for (int i = 0; i < Gsize; i++) {
            visited[i] = false;
            tempArr[i] = 0;
        }
        Queue rasta;
        if (!visited[0]) {
            rasta.push(0);
            visited[0] = true;
            bool reached = false;
            while (!rasta.isEmpty()) {
                int front = rasta.getFront();
                rasta.pop();
                if (front == Gsize - 1) {
                    reached = true; // If the destination is reached, set reached flag to true
                }

                graphNode* temp = city[front].next;
                while (temp != NULL) {
                    if (!visited[temp->cities]) {
                        if (temp->cities > max) {
                            max = temp->cities; // Update max value
                        }
                        rasta.push(temp->cities); // Push the city to the queue
                        visited[temp->cities] = true; // Mark city as visited
                    }
                    temp = temp->next;
                }
                if (tempArr[index - 1] != max) {
                    tempArr[index++] = max; // Store max value in an array
                }
                if (reached) {
                    break; // Break the loop if destination is reached
                }
            }
        }
        delete[] visited; // Deallocate memory
        if (city[Gsize - 1].count % 6 != 0) {
            city[Gsize - 1].days++; // Increment days if count is not divisible by 6
        }
        return city[Gsize - 1].days; // Return the minimum days required
    }

    // Function to add aerial routes between cities
    void addAerialRoutes(int departure, int arrival) {
        graphNode* temp = new graphNode;
        temp->cities = arrival;
        temp->linkedCities = -1;
        temp->next = NULL;
        graphNode* temp2;
        if (departure == Gsize - 1) {
            temp2 = &city[departure];
        }
        else {
            temp2 = city[departure].next;
        }
        if (!temp2) {
            city[departure].next = temp; // If no route exists, add the route
        }
        else {
            while (temp2->next != NULL) {
                temp2 = temp2->next;
            }
            temp2->next = temp; // Traverse to the end and add the route
        }
        city[departure].linkedCities++; // Increment linkedCities count

        // Adjusting days based on conditions
        if (city[departure].linkedCities > 1) {
            if ((city[departure].count % 6 == 0) && city[departure].cities != 0 && city[departure].days > 0) {
                city[departure].days--; // Decrement days under certain conditions
            }
        }
        if (city[departure].days + 1 < city[arrival].days) {
            city[arrival].days = city[departure].days + 1;
            city[arrival].count = 0;

            // Adjusting count and days for subsequent cities
            for (int i = arrival + 1; i < Gsize; i++) {
                if (city[i - 1].days < city[i].days) {
                    city[i].count = city[i - 1].count + 1;
                    if (city[i].count % 6 == 0) {
                        city[i].days = city[i - 1].days + 1;
                    }
                    else
                        city[i].days = city[i - 1].days;
                }
                else
                    city[i].count = city[i - 1].count + 1;
            }
        }
    }

};

// Function to check if a string represents an integer
bool isInteger(const string& input) {
    for (char ch : input) {
        if (ch < '0' || ch > '9') {
            return false;
        }
    }
    return true;
}

// Function to convert string to integer
int stringToInt(const string& str) {
    int result = 0;
    bool isNegative = false;
    size_t i = 0;

    // Check for negative sign
    if (str[0] == '-') {
        isNegative = true;
        i = 1;
    }

    // Convert characters to integer
    for (; i < str.length(); ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            result = result * 10 + (str[i] - '0');
        } else {
            // Invalid input
            std::cout << "Invalid input string." << std::endl;
            return 0;
        }
    }

    return isNegative ? -result : result;
}

// Main function
int main() {
    string tempTests, tempCities, tempAir;
    int to, from, cities, aerial, tests;

    bool validInput = false;

    int i = 1;
    while (!validInput) {
        cout << "Enter the number of test cases: ";
        cin >> tempTests;
        validInput = isInteger(tempTests);

        if (validInput) {
            validInput = true;

            tests = stringToInt(tempTests);

            while (i <= tests) {
                cout << "---------------TEST CASE " << i << "----------------\n\n";
                bool a = true;
                while (a) {
                    cout << "Enter total number of Cities: ";
                    cin >> tempCities;
                    if (isInteger(tempCities)) {
                        a = false;
                        cities = stringToInt(tempCities);

                    }
                    else {
                        cout << "Invalid Input\n";
                    }
                }

                bool b = true;
                graph G(cities);
                while (b) {
                    cout << "Enter Aerial routes: ";
                    cin >> tempAir;
                    if (isInteger(tempAir)) {
                        b = false;
                        aerial = stringToInt(tempAir);
                        for (int i = 0; i < aerial; i++) {
                            cout << "Route " << i + 1 << ": ";
                            cout << "From: ";
                            cin >> from;
                            cout << "To: ";
                            cin >> to;
                            G.addAerialRoutes(from, to);
                        }
                        cout << "The minimum number of days required for test case: " << i << " are: " << G.BFS() << endl;
                        i++;
                    }

                    else {
                        cout << "Invalid Input\n";
                    }
                }

            }

        }
        else {
            cout << "you have entered invalid input\n";
        }
    }

    return 0;
}
