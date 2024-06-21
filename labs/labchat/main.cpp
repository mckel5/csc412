#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <iomanip>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int remainingTime; // For RR algorithm
    int startTime;
    int finishTime;
    int waitingTime;
};

std::mutex mtx; // Mutex for synchronization
std::condition_variable cv; // Condition variable for synchronization

std::queue<Process> readyQueue; // Queue for ready processes
std::vector<Process> processes; // Vector to hold all processes

// Simulate the execution of a process
void executeProcess(Process& p) {
    p.startTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    // Simulating process execution
    std::this_thread::sleep_for(std::chrono::seconds(p.burstTime)); // Dummy execution time

    p.finishTime = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    p.waitingTime = p.startTime - p.arrivalTime;
}

// Implement FCFS scheduling here
void firstComeFirstServed(const std::vector<Process>& processes) {
    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int completedProcesses = 0;

    for (const auto& p : processes) {
        std::cout << "Executing process " << p.pid << " (FCFS)\n";
        executeProcess(const_cast<Process&>(p));

        int turnaroundTime = p.finishTime - p.arrivalTime;
        totalTurnaroundTime += turnaroundTime;
        totalWaitingTime += p.waitingTime;

        completedProcesses++;
    }

    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / completedProcesses;
    double avgWaitingTime = static_cast<double>(totalWaitingTime) / completedProcesses;
    double throughput = static_cast<double>(completedProcesses) / (processes.back().finishTime - processes.front().arrivalTime);

    std::cout << "FCFS Metrics:\n";
    std::cout << "Average Turnaround Time: " << std::fixed << std::setprecision(2) << avgTurnaroundTime << " seconds\n";
    std::cout << "Average Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitingTime << " seconds\n";
    std::cout << "Throughput: " << std::fixed << std::setprecision(2) << throughput << " processes per second\n";
}

// Implement Round Robin scheduling here
void roundRobin(const std::vector<Process>& processes, int timeQuantum) {
    std::queue<Process> readyQueue; // Local queue for ready processes

    for (const auto& p : processes) {
        readyQueue.push(p);
    }

    int totalTurnaroundTime = 0;
    int totalWaitingTime = 0;
    int completedProcesses = 0;

    while (!readyQueue.empty()) {
        std::unique_lock<std::mutex> lock(mtx);
        if (!readyQueue.empty()) {
            Process currentProcess = readyQueue.front();
            readyQueue.pop();
            lock.unlock();

            if (currentProcess.remainingTime > timeQuantum) {
                std::this_thread::sleep_for(std::chrono::seconds(timeQuantum));
                currentProcess.remainingTime -= timeQuantum;
                // Put the process back into the ready queue
                lock.lock();
                readyQueue.push(currentProcess);
                lock.unlock();
            } else {
                std::this_thread::sleep_for(std::chrono::seconds(currentProcess.remainingTime));
                currentProcess.remainingTime = 0;

                int turnaroundTime = std::chrono::duration_cast<std::chrono::seconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count() - currentProcess.arrivalTime;

                totalTurnaroundTime += turnaroundTime;
                totalWaitingTime += currentProcess.waitingTime;

                completedProcesses++;

                // Process completed, print the PID
                std::cout << "Process " << currentProcess.pid << " completed.\n";
            }

            // Simulate context switch
            std::this_thread::sleep_for(std::chrono::seconds(1));
        } else {
            cv.wait(lock);
        }
    }

    double avgTurnaroundTime = static_cast<double>(totalTurnaroundTime) / completedProcesses;
    double avgWaitingTime = static_cast<double>(totalWaitingTime) / completedProcesses;
    double throughput = static_cast<double>(completedProcesses) / (processes.back().finishTime - processes.front().arrivalTime);

    std::cout << "Round Robin (RR) Metrics:\n";
    std::cout << "Average Turnaround Time: " << std::fixed << std::setprecision(2) << avgTurnaroundTime << " seconds\n";
    std::cout << "Average Waiting Time: " << std::fixed << std::setprecision(2) << avgWaitingTime << " seconds\n";
    std::cout << "Throughput: " << std::fixed << std::setprecision(2) << throughput << " processes per second\n";
}

int main() {
    // Initialize process details
    processes = {
        {1, 0, 5, 5, 0, 0, 0},  // Process ID, Arrival Time, Burst Time, Remaining Time, StartTime, FinishTime, WaitingTime
        {2, 1, 3, 3, 0, 0, 0},
        {3, 2, 8, 8, 0, 0, 0},
        {4, 3, 6, 6, 0, 0, 0},
        {5, 4, 4, 4, 0, 0, 0}
    };

    // Start FCFS Scheduler
    std::cout << "First-Come, First-Served (FCFS) Scheduling\n";
    firstComeFirstServed(processes);

    // Reset the ready state for the next scheduling algorithm, if necessary
    // (Not needed for RR as it uses its own local queue)
    for (auto& p : processes) {
        p.remainingTime = p.burstTime; // Reset remaining time for RR algorithm
        p.startTime = 0;
        p.finishTime = 0;
        p.waitingTime = 0;
    }

    // Start Round Robin Scheduler
    std::cout << "\nRound Robin (RR) Scheduling\n";
    int timeQuantum = 2; // Example time quantum
    roundRobin(processes, timeQuantum);

    return 0;
}

