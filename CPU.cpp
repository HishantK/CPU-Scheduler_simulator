#include <bits/stdc++.h>
using namespace std;

struct Task {
    string id;
    int burstTime, remainingTime;
    vector<string> memBlocks;
    int memIndex = 0;
};

// Represents a cache level with FIFO eviction policy
struct CacheLevel {
    string name;
    int capacity, latency;
    deque<string> slots;

    CacheLevel(const string& n, int cap, int lat) : name(n), capacity(cap), latency(lat) {}

    bool contains(const string& block) const {
        return find(slots.begin(), slots.end(), block) != slots.end();
    }

    // Adds block to cache; if full, evicts oldest (front) and returns it
    string insert(const string& block) {
        string evicted = "";
        if ((int)slots.size() >= capacity) {
            evicted = slots.front();
            slots.pop_front();
        }
        slots.push_back(block);
        return evicted;
    }

    void print() const {
        cout << name << ": [";
        for (size_t i = 0; i < slots.size(); ++i) cout << (i ? ", " : "") << slots[i];
        cout << "]";
    }
};

CacheLevel L1("L1", 32, 4), L2("L2", 128, 12), L3("L3", 512, 40);
int ramAccesses = 0;

// Propagates evicted blocks from L1 -> L2 -> L3
void promoteToL1(const string& block) {
    string e1 = L1.insert(block);
    L1.print(); 
    if (!e1.empty()) cout << " (" << e1 << " evicted)";
    cout << "\n";

    if (!e1.empty()) {
        string e2 = L2.insert(e1);
        L2.print(); if (!e2.empty()) cout << " (" << e2 << " evicted from L2)";
        cout << "\n";
        if (!e2.empty()) {
            string e3 = L3.insert(e2);
            L3.print(); if (!e3.empty()) cout << " (" << e3 << " evicted from L3)";
            cout << "\n";
        } else { L3.print(); cout << "\n"; }
    } else { L2.print(); cout << "\n"; L3.print(); cout << "\n"; }
}

// Searches hierarchy L1->L2->L3->RAM
int accessMemory(const string& block) {
    if (L1.contains(block)) { L1.print(); cout << " -> HIT\n"; L2.print(); cout << "\n"; L3.print(); cout << "\n"; return L1.latency; }
    L1.print(); cout << " >> MISS\n";

    if (L2.contains(block)) { L2.print(); cout << " >> HIT (" << L2.latency << ")\n"; L3.print(); cout << "\n"; promoteToL1(block); return L2.latency; }
    L2.print(); cout << " >> MISS\n";

    if (L3.contains(block)) { L3.print(); cout << " >> HIT (" << L3.latency << ")\n"; promoteToL1(block); return L3.latency; }
    L3.print(); cout << " >> MISS\n";

    cout << "Fetching from RAM\n";
    ramAccesses++;
    promoteToL1(block);
    return 200;
}

void printCacheState() { L1.print(); cout << "\n"; L2.print(); cout << "\n"; L3.print(); cout << "\n"; }

vector<Task> loadTasks(const string& filename) {
    ifstream file(filename);
    vector<Task> tasks;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        istringstream ss(line);
        string token, id;
        int burst;
        ss >> token >> id >> token >> burst >> token;
        Task t = {id, burst, burst, {}};
        string block;
        while (ss >> block) t.memBlocks.push_back(block);
        tasks.push_back(t);
    }
    return tasks;
}

void runScheduler(vector<Task>& tasks) {
    const int QUANTUM = 3; // Round Robin time slice
    queue<Task*> q;
    for (auto& t : tasks) q.push(&t);

    Task* curr = nullptr;
    int qLeft = 0, cycle = 0;

    // Simulation loop: processes one CPU cycle at a time
    while (!q.empty() || curr) {
        if (!curr || qLeft == 0) {
            if (curr && curr->remainingTime > 0) q.push(curr); // Preempt
            if (q.empty()) break;
            curr = q.front(); q.pop();
            qLeft = QUANTUM;
        }

        cycle++;
        curr->remainingTime--;
        qLeft--;

        cout << "\nCycle " << cycle << " - Running: " << curr->id << "\n";

        // If task requests memory, calculate stall cycles
        if (curr->memIndex < (int)curr->memBlocks.size()) {
            string block = curr->memBlocks[curr->memIndex++];
            cout << "Requesting: " << block << "\n";
            int lat = accessMemory(block);
            if (lat > 1) { cycle += (lat - 1); cout << "Stall: +" << (lat - 1) << " cycles\n"; }
        } else printCacheState();

        if (curr->remainingTime == 0) curr = nullptr;
    }

    cout << "\n=== Final Results ===\nTotal Cycles : " << cycle << "\nTasks Completed: " << tasks.size() << "\nRAM Access: " << ramAccesses << "\nCPU Scheduling Algorithm: Round Robin \nMemory management method: FIFO\n";
}

int main() {
    vector<Task> tasks = loadTasks("input.txt");
    runScheduler(tasks);
    return 0;
}   