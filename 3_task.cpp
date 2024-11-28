#include <iostream>
#include <queue>
#include <vector>


using std::cout;
using std::endl;
using std::priority_queue;
using std::vector;
using std::greater;
using std::cin;


void merging(priority_queue<int, vector<int>, greater<int>>& minHeap){
    int total = 0;
    int a = 0;
    int b = 0;
    int res = 0;
    while(!minHeap.empty()){
        a = minHeap.top();
        minHeap.pop();
        if(!minHeap.empty()){
            b = minHeap.top();
            minHeap.pop();
            cout << a << " will be merged with " << b << endl;
            res = a + b;
            total += res;
            minHeap.push(res);   
        }
    }
    cout << "\nTotal record moves: " << total << endl;
}

int main(){
    priority_queue<int, vector<int>, greater<int>> minHeap;
    int num_of_files{};
    cout << "Please input the number of files (to work with default input just put -1): ";
    cin >> num_of_files;
    int tmp{};
    if (num_of_files < 0) {
        minHeap.push(3);
        minHeap.push(1);
        minHeap.push(4);
        minHeap.push(7);
        minHeap.push(2);
        minHeap.push(9);
    } 
    else{
        for(int i = 0; i < num_of_files; i++){
            cin >> tmp;
            minHeap.push(tmp);
        }
    }
    cout << "\nStarting merging operation\n";
    merging(minHeap);
    return 0;
}