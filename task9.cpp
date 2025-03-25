#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};

// Function to calculate FCFS scheduling
void FCFS(vector<Process> processes) {
    int n = processes.size();
    int completion_time = 0;
    int total_waiting_time = 0;
    
    cout << "\nFCFS Scheduling:\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    
    for (int i = 0; i < n; i++) {
        if (completion_time < processes[i].arrival_time)
            completion_time = processes[i].arrival_time;
        
        processes[i].waiting_time = completion_time - processes[i].arrival_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
        
        completion_time += processes[i].burst_time;
        total_waiting_time += processes[i].waiting_time;
        
        cout << processes[i].id << "\t\t" << processes[i].arrival_time << "\t\t"
             << processes[i].burst_time << "\t\t" << processes[i].waiting_time << "\t\t"
             << processes[i].turnaround_time << "\n";
    }

    cout << "Average Waiting Time: " << fixed << setprecision(2) 
         << (float)total_waiting_time / n << "\n";
}

// Function to calculate SJF (Non-Preemptive)
void SJF(vector<Process> processes) {
    int n = processes.size();
    vector<bool> completed(n, false);
    int completed_count = 0, current_time = 0, total_waiting_time = 0;
    
    cout << "\nSJF Scheduling:\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";

    while (completed_count < n) {
        int min_burst = 1e9, min_index = -1;
        
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].burst_time < min_burst) {
                    min_burst = processes[i].burst_time;
                    min_index = i;
                }
            }
        }

        if (min_index == -1) {
            current_time++;
            continue;
        }

        completed[min_index] = true;
        processes[min_index].waiting_time = current_time - processes[min_index].arrival_time;
        processes[min_index].turnaround_time = processes[min_index].waiting_time + processes[min_index].burst_time;
        total_waiting_time += processes[min_index].waiting_time;
        
        current_time += processes[min_index].burst_time;
        completed_count++;

        cout << processes[min_index].id << "\t\t" << processes[min_index].arrival_time << "\t\t"
             << processes[min_index].burst_time << "\t\t" << processes[min_index].waiting_time << "\t\t"
             << processes[min_index].turnaround_time << "\n";
    }

    cout << "Average Waiting Time: " << fixed << setprecision(2) 
         << (float)total_waiting_time / n << "\n";
}

// Function to calculate Round Robin scheduling
void RoundRobin(vector<Process> processes, int quantum) {
    int n = processes.size();
    vector<int> remaining_burst(n);
    int total_waiting_time = 0, current_time = 0;
    
    for (int i = 0; i < n; i++)
        remaining_burst[i] = processes[i].burst_time;

    cout << "\nRound Robin Scheduling (Quantum = " << quantum << "):\n";
    cout << "Process\tArrival Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    
    vector<int> waiting_time(n, 0);
    vector<int> turnaround_time(n, 0);
    
    bool done;
    do {
        done = true;
        for (int i = 0; i < n; i++) {
            if (remaining_burst[i] > 0) {
                done = false;
                
                if (remaining_burst[i] > quantum) {
                    current_time += quantum;
                    remaining_burst[i] -= quantum;
                } else {
                    current_time += remaining_burst[i];
                    waiting_time[i] = current_time - processes[i].arrival_time - processes[i].burst_time;
                    turnaround_time[i] = waiting_time[i] + processes[i].burst_time;
                    remaining_burst[i] = 0;
                }
            }
        }
    } while (!done);
    
    for (int i = 0; i < n; i++) {
        total_waiting_time += waiting_time[i];
        cout << processes[i].id << "\t\t" << processes[i].arrival_time << "\t\t"
             << processes[i].burst_time << "\t\t" << waiting_time[i] << "\t\t"
             << turnaround_time[i] << "\n";
    }

    cout << "Average Waiting Time: " << fixed << setprecision(2) 
         << (float)total_waiting_time / n << "\n";
}

int main() {
    vector<Process> processes = {
        {1, 0, 5},
        {2, 1, 3},
        {3, 2, 8},
        {4, 3, 6}
    };
    
    FCFS(processes);
    SJF(processes);
    RoundRobin(processes, 2);

    return 0;
}