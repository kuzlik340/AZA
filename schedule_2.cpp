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


// Find the root of the set containing `x`, with path compression
void compress_path(int x) {
    int root = x;

    // Traverse up the tree to find the root
    while (U[root].parent != root) {
        root = U[root].parent;
    }

    // Compress the path: update the parent of each node visited to point directly to the root
    while (x != root) {
        int temp = U[x].parent; // Store the current parent
        U[x].parent = root;     // Update the parent to the root
        x = temp;               // Move up the tree
    }
}


// Modify the `small()` function to use `findRoot`
int small(int p) {
    return U[(U[p].parent)].smallest;
}


// Initialize a single set for element `i`
void makeset(int i) {
    U[i].parent = i;       // Initially, the node is its own parent
    U[i].depth = 0;        // Initial depth is 0
    U[i].smallest = i;     // The only element in the set is the smallest
}

void merge(int p, int q) {
    // Find the roots of both sets
    int rootP = U[p].parent; 
    int rootQ = U[q].parent;

    // If the roots are the same, no need to merge
    if (rootP == rootQ) return;

    if (U[rootP].depth == U[rootQ].depth) {
        U[rootP].depth += 1;     // Increase the depth of the tree
        U[rootQ].parent = p;     // Make `p` the parent of `q`
        if (U[rootQ].smallest < U[rootP].smallest) {
            U[rootP].smallest = U[rootQ].smallest; // Update the smallest element
        }
    }
    // If the depth of `p` is less than the depth of `q`
    else if (U[rootP].depth < U[rootQ].depth) {
        U[rootP].parent = q;     // Make `q` the parent of `p`
        if (U[rootP].smallest < U[rootQ].smallest) {
            U[rootQ].smallest = U[rootP].smallest; // Update the smallest element
        }
    }
    // Otherwise, `p`'s depth is greater than `q`'s depth
    else {
        U[rootQ].parent = p;     // Make `p` the parent of `q`
        if (U[rootQ].smallest < U[rootP].smallest) {
            U[rootP].smallest = U[rootQ].smallest; // Update the smallest element
        }
    }
    compress_path(p);
    compress_path(q);
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
        if (availableSlot != 0) {
            // Reserve the slot by merging it with the previous slot
            merge(availableSlot, availableSlot - 1);

            // Add the job to the schedule
            J[availableSlot-1] = job;
        }
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
        {7, 1, 55}
    };

    vector<Job> J{};
    cout << "\nInput table\n" << endl;
    for (Job& job : jobs) {
        cout << "Job ID: " << job.id << " |  Deadline: " << job.deadline << " |  Profit: " << job.profit << endl;
    }
    cout << "\nSorted table\n" << endl;

    sort(jobs.begin(), jobs.end(), [](const Job& a, const Job& b) {
        return a.profit > b.profit;
    });

    for (Job& job : jobs) {
        cout << "Job ID: " << job.id << " |  Deadline: " << job.deadline << " |  Profit: " << job.profit << endl;
    }

    schedule(jobs, J);

    cout << "\nAfter schedulling\n" << endl;

    int profit_total = 0;
    int i = 1;

    for (Job& job : J) {
        cout << "\033[34mDAY " << i++ << ": \033[0m" << "Job ID: " << job.id << " |  Deadline: " << job.deadline << " |  Profit: " << job.profit << endl;
        profit_total += job.profit;
    }

    cout << "Total profit: " << profit_total << "\n" << endl;


    return 0;
}