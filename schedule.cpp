#include <iostream>
#include <vector>
#include <algorithm>

using std::vector;
using std::cout;
using std::endl;
using std::sort;


struct Job{
    int id;
    int deadline;
    int profit;
};

void schedule(const vector<Job>& jobs, vector<Job>& J) {
    /* Find out the maxdeadline to create 
     * an array for time slots */
    int maxDeadline = 0;
    for (const Job& job : jobs) {
        if (job.deadline > maxDeadline) {
            maxDeadline = job.deadline;
        }
    }

    /* Create an array with time slots */
    /* -1 means that time slot is free */
    vector<int> time_slots(maxDeadline, -1);
    J.resize(maxDeadline);
    /* main cycle for jobs */
    for (const Job& job : jobs) {
        /* nested loop to check for each day 
         * before deadline to insert the job*/
        for (int i = job.deadline - 1; i >= 0; i--){
            if(time_slots[i] == -1){
                time_slots[i] = job.id;
                J[i] = job;
                break;
            }
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