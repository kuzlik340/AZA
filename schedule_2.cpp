#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <map>
#include <queue> // For BFS to visualize the tree




using std::vector;
using std::cout;
using std::endl;
using std::sort;

// Define the nodetype structure
struct NodeType {
    int parent;   // The parent of the node
    int depth;    // The depth of the node in the tree
    int smallest; // The smallest element in the set
    NodeType(){
        parent = 0;  
        depth = 0;   
        smallest = 0;
    }
};


struct Job{
    int id;
    int deadline;
    int profit;
    Job(){
        id = 0;
        deadline = 0;
        profit = 0;
    }
    Job(int id, int deadline, int profit){
        this->id = id;
        this->deadline = deadline;
        this->profit = profit;
    }
};

const int n = 100; // Maximum number of elements in the universe
vector<NodeType> U(n + 1); // Universe array indexed from 1 to n

// Print sets as graphical trees
void printSets(int maxIndex) {
    cout << "\nDisjoint Sets (as Trees):\n";

    // Iterate over all nodes to find roots and print their trees
    for (int i = 1; i <= maxIndex; ++i) {
        // Find the root of the current set
        if (U[i].parent == i) { // Root node
            cout << "Tree rooted at " << i << ":\n";

            // Use a queue for BFS to print the tree structure
            std::queue<std::pair<int, int>> q; // Pair: (node, depth)
            q.push({i, 0});
            while (!q.empty()) {
                auto [current, depth] = q.front();
                q.pop();

                // Indentation for visualization
                for (int j = 0; j < depth; ++j) {
                    cout << "  ";
                }
                cout << current << " (smallest: " << U[current].smallest << ")\n";

                // Add children to the queue
                for (int j = 1; j <= maxIndex; ++j) {
                    if (U[j].parent == current && j != current) {
                        q.push({j, depth + 1});
                    }
                }
            }
            cout << endl;
        }
    }
}


// Initialize a single set for element `i`
void makeset(int i) {
    U[i].parent = i;       // Initially, the node is its own parent
    U[i].depth = 0;        // Initial depth is 0
    U[i].smallest = i;     // The only element in the set is the smallest
}

// Merge two sets represented by `p` and `q`
void merge(int p, int q) {
    // If the depths are equal
    if (U[p].depth == U[q].depth) {
        U[p].depth += 1;     // Increase the depth of the tree
        U[q].parent = p;     // Make `p` the parent of `q`
        if (U[q].smallest < U[p].smallest) {
            U[p].smallest = U[q].smallest; // Update the smallest element
        }
    }
    // If the depth of `p` is less than the depth of `q`
    else if (U[p].depth < U[q].depth) {
        U[p].parent = q;     // Make `q` the parent of `p`
        if (U[p].smallest < U[q].smallest) {
            U[q].smallest = U[p].smallest; // Update the smallest element
        }
    }
    // Otherwise, `p`'s depth is greater than `q`'s depth
    else {
        U[q].parent = p;     // Make `p` the parent of `q`
        if (U[q].smallest < U[p].smallest) {
            U[p].smallest = U[q].smallest; // Update the smallest element
        }
    }
}

// Find the smallest element in the set containing `p`
int small(int p) {
    if (U[p].parent != p) {
        U[p].parent = small(U[p].parent);
    }
    return U[p].smallest;
}


void initial(int d){
    d++;
    for(int i = 0; i < d; i++){
        makeset(i);
    }
}



void schedule(const vector<Job>& jobs, vector<Job>& J) {
    /* Find out the maxdeadline to create 
     * an array for time slots */
    int maxDeadline = 0;
    for (const Job& job : jobs) {
        if (job.deadline > maxDeadline) {
            maxDeadline = job.deadline;
        }
    }

    /* init d+1 joint sets */
    initial(maxDeadline);
    J.resize(maxDeadline);
    /* Create an array with time slots */
    /* -1 means that time slot is free */

    /* main cycle for jobs */
    for (const Job& job : jobs) {
        int availableSlot = small(job.deadline);
        cout << "Job ID : " << job.id << "Has this small() " << availableSlot << endl;
        if (availableSlot != 0) {
            // Reserve the slot by merging it with the previous slot
            merge(availableSlot, availableSlot - 1);

            // Add the job to the schedule
            J[availableSlot-1] = job;
        }
        printSets(maxDeadline);
    }
}    



int main(){
    vector<Job> jobs{ 
        {1, 2, 40}, 
        {2, 4, 15}, 
        {3, 3, 60}, 
        {4, 2, 20}, 
        {5, 3, 10}, 
        {6, 1, 45}, 
        {7, 1, 55}};

    vector<Job> J{};


    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.profit > b.profit;
    });


    for (const Job& job : jobs) {
        cout << "\tJob ID: " << job.id << " |  Deadline: " << job.deadline << " |  Profit: " << job.profit << endl;
    }

    schedule(jobs, J);

    cout << "\nAfter schedulling" << endl;

    int profit_total = 0;
    int i = 1;
    for (const Job& job : J) {
        if (job.id != 0){
            cout << "\033[34mDAY " << i++ << ": \033[0m" << "Job ID: " << job.id << " |  Deadline: " << job.deadline << " |  Profit: " << job.profit << endl;
            profit_total += job.profit;
        }
    }

    cout << "Total profit: " << profit_total << endl;


    return 0;
}