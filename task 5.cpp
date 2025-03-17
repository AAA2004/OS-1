#include <iostream>
#include <filesystem>
#include <unordered_map>
#include <vector>
#include <algorithm>

using namespace std;
namespace fs = filesystem;

// Function to scan directory and collect file sizes into bins
void scan_directory(const fs::path& directory, unordered_map<size_t, int>& histogram, size_t bin_width) {
    for (const auto& entry : fs::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            size_t file_size = entry.file_size();
            size_t bin = file_size / bin_width;
            histogram[bin]++;
        }
    }
}

// Function to print histogram in sorted order
void print_histogram(const unordered_map<size_t, int>& histogram, size_t bin_width) {
    vector<pair<size_t, int>> sorted_hist(histogram.begin(), histogram.end());
    
    // Sort by bin range (ascending order)
    sort(sorted_hist.begin(), sorted_hist.end());

    cout << "File Size Histogram:\n";
    for (const auto& [bin, count] : sorted_hist) {
        cout << "[" << bin * bin_width << " - " << (bin + 1) * bin_width - 1 << "] : ";
        cout << string(count, '*') << " (" << count << " files)\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <directory> <bin_width>\n";
        return 1;
    }

    fs::path start_directory = argv[1];
    size_t bin_width = stoul(argv[2]);

    if (!fs::exists(start_directory) || !fs::is_directory(start_directory)) {
        cerr << "Error: Invalid directory path.\n";
        return 1;
    }

    unordered_map<size_t, int> histogram;
    scan_directory(start_directory, histogram, bin_width);
    print_histogram(histogram, bin_width);

    return 0;
}
