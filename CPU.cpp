#include<bits/stdc++.h>
using namespace std;

struct task{
    string id;
    int burstTime,remainTime;
    vector<string> memBlock;
    int ind = 0;
    
};

struct Cache { 
    int cycles;
    int numSlot,curr;
    queue<string> mem;
    
    Cache(int cycles, int numSlot){
       this->cycles = cycles;
        this->numSlot = numSlot;
        curr = 0;
    }
    bool full(){
        if(curr == numSlot){
            return true;
        }
        return false;
    }
    string insert(string m){
        if(curr == numSlot){
            string s = mem.front();
            mem.pop();
            mem.push(m);
            return s;
        }else{
            mem.push(m);
            return m;
        }
        
    }
    bool check(string m){
        if(curr == 0){
            return false;
        }
        queue<string> q = mem;
        while(!q.empty()){
            string s = q.front();

            if(s == m) return true;
            
            q.pop();
        }
        
        return false;

    }
    void print(){
        queue<string> q = mem;
        cout << '[';
        while(!q.empty()){
            string s = q.front();
            cout << s << ',';
            q.pop();
        }
        cout << ']';
    }
};

queue<task> TaskConversion(ifstream& file) {
    queue<task> q;
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue; 

        stringstream ss(line);
        string labelTask, labelBurst, labelMem;
        task T1;
        
        
        if (ss >> labelTask >> T1.id >> labelBurst >> T1.burstTime >> labelMem) {
            T1.remainTime = T1.burstTime; 
            
            string block;
            while (ss >> block) {
                T1.memBlock.push_back(block);
            }
            
            
            q.push(T1);
        }
    }
    return q;
}

Cache l1(4,32),l2(12,128),l3(40,512);
int access(string m){
    queue<string> q1 = l1.mem;
    while(!q1.empty()){
        string s = q1.front();
        if(s == m){
            cout << "L1: ";
            l1.print();
            cout << "->Hit";
            cout << endl;
            cout << "L2: ";
            l2.print();
            cout << endl;
            cout << "L3: ";
            l3.print();
            cout << endl;
            return 4;
        }
        q1.pop();
    }
    queue<string> q2 = l2.mem;
    while(!q2.empty()){
        string s = q2.front();
        if(s == m){
            cout << "L1: ";
            l1.print();
            cout << "->Miss";
            cout << endl;
            cout << "L2: ";
            l2.print();
            cout << "->Hit";
            cout << endl;
            cout << "L3: ";
            l3.print();
            cout << endl;
            return 12;
        }
        q2.pop();
    }


    queue<string> q3 = l3.mem;
    while(!q3.empty()){
        string s = q3.front();
        if(s == m){
            cout << "L1: ";
            l1.print();
            cout << "->Miss";
            cout << endl;
            cout << "L2: ";
            l2.print();
            cout << "->Miss";
            cout << endl;
            cout << "L3: ";
            l3.print();
            cout << "->Hit";
            cout << endl;
            return 40;
        }
        q3.pop();
    }
    cout << "L1: ";
    l1.print();
    cout << "->Miss";
    cout << endl;
    cout << "L2: ";
    l2.print();
    cout << "->Miss";
    cout << endl;
    cout << "L3: ";
    l3.print();
    cout << "->Miss";
    cout << endl;
    return 200;
    
}

void bringMemTOL1(string s){
    string a;
    string b;
    string c;
    if(l1.full()){
        a = l1.mem.front();
        l1.mem.pop();
        l1.mem.push(s);
        if(l2.full()){
            b = l2.mem.front();
            l2.mem.pop();
            l2.mem.push(a);
            if(l3.full()){
                c = l3.mem.front();
                l3.mem.pop();
                l3.mem.push(b);
                cout << a << " gets evicted from L1 and added in L2 and " << b << "gets evicted from l2 and is added in L3 and " << c << " gets evicted from l3 and is added in RAM" << endl;
            }else{
                l3.mem.push(b);
                cout << a << " gets evicted from L1 and added in L2 and " << b << "gets evicted from l2 and is added in L3" << endl;
                l3.curr++;
            }
        }else{
            l2.mem.push(a);
            l2.curr++;
            cout << a << " gets evicted from L1 and is added in L2" << endl;

        }
    }else{
        l1.mem.push(s);
        l1.curr++;

    }
}
void printAllCache(){
    cout << "L1 :";
    l1.print();
    cout << endl;
    cout << "L2 :";
    l2.print();
    cout << endl;
    cout << "L3 :";
    l3.print();
    cout << endl;
}
void scheduler(queue<task> q){
    int quant = 3;
    int totalCycles = 0;
    int totaltask = 0;
    int ramAccess = 0;
    while(!q.empty()){
        task T = q.front();
        int t = quant;
        q.pop();
        while(t > 0){
            if(T.remainTime == 0) break;
            if(T.ind < T.memBlock.size()){
                string memo = T.memBlock[T.ind];
                T.ind++;
                cout << "Task working on: " << T.id << endl;
                cout << "Memory required: " << memo << endl;
                int cy = access(memo);
                if(cy == 12){
                    int z = l2.curr;
                    queue<string> temp;
                    int i = 0;
                    while(i < z){
                        string op = l2.mem.front();
                        if(op == memo){
                            i++;
                            l2.mem.pop();
                            continue;
                        }else{
                            temp.push(op);
                        }
                        l2.mem.pop();
                        i++;
                    }
                    l2.mem = temp;
                    l2.curr--;
                    bringMemTOL1(memo);
                }else if(cy == 40){
                    int z = l3.curr;
                    queue<string> temp;
                    int i = 0;
                    while(i < z){
                        string op = l3.mem.front();
                        if(op == memo){
                            i++;
                            l3.mem.pop();
                            continue;
                        }else{
                            temp.push(op);
                        }
                        l3.mem.pop();
                        i++;
                    }
                    l3.curr--;
                    l3.mem = temp;
                    bringMemTOL1(memo);
                }else if(cy == 200){
                    bringMemTOL1(memo);
                    ramAccess++;

                }
                totalCycles += cy;
                t--;
                T.remainTime--;
                cout << "After changes if any.." << endl; 
                printAllCache();
            }else{
                totalCycles++;
                t--;
                T.remainTime--;
            }
        }
        if(T.remainTime > 0){
            q.push(T);
        }else{
            totaltask++;
        }
    }
    cout << "Results: " << endl;
    cout << "Total cycles: " << totalCycles << endl;
    cout << "Total tasks completed: " << totaltask << endl;
    cout << "Scheduler: Round Robin" << endl;
    cout << "Ram Acess: " << ramAccess << endl;
}
int main(){
    ifstream input("input.txt");
    queue<task> q = TaskConversion(input);
    scheduler(q);
}