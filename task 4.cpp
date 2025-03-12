#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <queue>

using namespace std;

// Function to generate a page reference sequence based on a probability model
vector<int> generatePageSequence(int numPages, int sequenceLength, double probability) {
    vector<int> sequence;
    int currentPage = rand() % numPages; // Start with a random page
    
    for (int i = 0; i < sequenceLength; i++) {
        sequence.push_back(currentPage);
        if ((static_cast<double>(rand()) / RAND_MAX) > probability) { // Switch pages based on probability
            currentPage = rand() % numPages;
        }
    }
    return sequence;
}

// FIFO Page Replacement Algorithm
int fifoPageReplacement(const vector<int>& pageSequence, int frameCount) {
    unordered_set<int> memoryFrames;
    queue<int> frameQueue;
    int pageFaults = 0;

    for (int page : pageSequence) {
        if (memoryFrames.find(page) == memoryFrames.end()) { // Page fault occurs
            if (memoryFrames.size() >= frameCount) {
                int evictedPage = frameQueue.front();
                frameQueue.pop();
                memoryFrames.erase(evictedPage);
            }
            frameQueue.push(page);
            memoryFrames.insert(page);
            pageFaults++;
        }
    }
    return pageFaults;
}

// Simulate local and global page replacement policies for two processes
void simulateReplacement(int numPages, int sequenceLength, double probability, int framesPerProcess) {
    vector<int> process1Sequence = generatePageSequence(numPages, sequenceLength, probability);
    vector<int> process2Sequence = generatePageSequence(numPages, sequenceLength, probability);
    
    cout << "Process 1 Page Sequence: ";
    for (int page : process1Sequence) cout << page << " ";
    cout << endl;
    
    cout << "Process 2 Page Sequence: ";
    for (int page : process2Sequence) cout << page << " ";
    cout << endl;

    // Local Page Replacement: Each process has a fixed number of frames
    int localFaults1 = fifoPageReplacement(process1Sequence, framesPerProcess);
    int localFaults2 = fifoPageReplacement(process2Sequence, framesPerProcess);
    cout << "Local Replacement - Process 1 Faults: " << localFaults1 << endl;
    cout << "Local Replacement - Process 2 Faults: " << localFaults2 << endl;

    // Global Page Replacement: Both processes share memory dynamically
    vector<int> combinedSequence;
    combinedSequence.insert(combinedSequence.end(), process1Sequence.begin(), process1Sequence.end());
    combinedSequence.insert(combinedSequence.end(), process2Sequence.begin(), process2Sequence.end());
    int globalFaults = fifoPageReplacement(combinedSequence, framesPerProcess * 2);
    cout << "Global Replacement - Total Faults: " << globalFaults << endl;
}

int main() {
    srand(static_cast<unsigned>(time(0)));
    int numPages = 5; // Total available pages
    int sequenceLength = 20; // Number of references
    double probability = 0.7; // Probability of repeating the same page
    int framesPerProcess = 3; // Frames allocated per process in local policy

    simulateReplacement(numPages, sequenceLength, probability, framesPerProcess);
    return 0;
}