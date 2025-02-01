#include <iostream>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>

#define RESET "\033[0m"
#define GREEN "\033[32m"
#define BLUE "\033[34m"

using namespace std;

void shellLoop(FILE *input);
void processCommand(string command);
void executeExternalCommand(vector<string> &args, bool background, string inputFile, string outputFile, bool append);
void changeDirectory(vector<string> &args);
void listDirectory(vector<string> &args);
void printEnvironment();
void setVariable(vector<string> &args);
void echoCommand(vector<string> &args);
void showHelp();
void pauseShell();
void displayPrompt();

int main(int argc, char *argv[]) {
    if (argc > 1) {
        FILE *batchFile = fopen(argv[1], "r");
        if (!batchFile) {
            perror("Error opening batch file");
            return 1;
        }
        shellLoop(batchFile);
        fclose(batchFile);
    } else {
        shellLoop(stdin);
    }
    return 0;
}

void shellLoop(FILE *input) {
    string command;
    while (true) {
        if (input == stdin) {
            displayPrompt();
        }
        
        if (!getline(cin, command)) {
            break; // Exit on EOF
        }

        processCommand(command);
    }
}

void displayPrompt() {
    cout << BLUE << "myshell" << RESET << GREEN << " > " << RESET;
}

void processCommand(string command) {
    vector<string> args;
    stringstream ss(command);
    string token;
    bool background = false;
    string inputFile, outputFile;
    bool append = false;

    while (ss >> token) {
        if (token == "&") {
            background = true;
        } else if (token == "<") {
            ss >> inputFile;
        } else if (token == ">") {
            ss >> outputFile;
            append = false;
        } else if (token == ">>") {
            ss >> outputFile;
            append = true;
        } else {
            args.push_back(token);
        }
    }

    if (args.empty()) return;

    string commandName = args[0];

    if (commandName == "cd") {
        changeDirectory(args);
    } else if (commandName == "dir") {
        listDirectory(args);
    } else if (commandName == "environ") {
        printEnvironment();
    } else if (commandName == "set") {
        setVariable(args);
    } else if (commandName == "echo") {
        echoCommand(args);
    } else if (commandName == "help") {
        showHelp();
    } else if (commandName == "pause") {
        pauseShell();
    } else if (commandName == "quit") {
        cout << "Exiting shell..." << endl;
        exit(0);
    } else {
        executeExternalCommand(args, background, inputFile, outputFile, append);
    }
}

void changeDirectory(vector<string> &args) {
    if (args.size() < 2) {
        cout << "Current Directory: " << getenv("PWD") << endl;
    } else {
        if (chdir(args[1].c_str()) == 0) {
            setenv("PWD", args[1].c_str(), 1);
        } else {
            perror("cd failed");
        }
    }
}

void listDirectory(vector<string> &args) {
    string dirPath = (args.size() > 1) ? args[1] : ".";
    DIR *dir = opendir(dirPath.c_str());
    if (!dir) {
        perror("dir failed");
        return;
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != nullptr) {
        cout << entry->d_name << "  ";
    }
    cout << endl;
    closedir(dir);
}

void printEnvironment() {
    extern char **environ;
    for (char **env = environ; *env; ++env) {
        cout << *env << endl;
    }
}

void setVariable(vector<string> &args) {
    if (args.size() < 3) {
        cerr << "Usage: set VARIABLE VALUE" << endl;
    } else {
        setenv(args[1].c_str(), args[2].c_str(), 1);
    }
}

void echoCommand(vector<string> &args) {
    for (size_t i = 1; i < args.size(); ++i) {
        cout << args[i] << " ";
    }
    cout << endl;
}

void showHelp() {
    cout << "myshell - A custom interactive shell\n";
    cout << "Commands:\n";
    cout << "  cd [dir]      - Change directory\n";
    cout << "  dir [dir]     - List directory contents\n";
    cout << "  environ       - Show environment variables\n";
    cout << "  set VAR VALUE - Set an environment variable\n";
    cout << "  echo [text]   - Print text\n";
    cout << "  help          - Show this help message\n";
    cout << "  pause         - Wait for Enter key\n";
    cout << "  quit          - Exit shell\n";
}

void pauseShell() {
    cout << "Press Enter to continue...";
    cin.ignore();
}

void executeExternalCommand(vector<string> &args, bool background, string inputFile, string outputFile, bool append) {
    pid_t pid = fork();
    if (pid == 0) {
        // Handle input redirection
        if (!inputFile.empty()) {
            int fd = open(inputFile.c_str(), O_RDONLY);
            if (fd == -1) {
                perror("Input redirection failed");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }

        // Handle output redirection
        if (!outputFile.empty()) {
            int fd = open(outputFile.c_str(), append ? O_WRONLY | O_CREAT | O_APPEND : O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd == -1) {
                perror("Output redirection failed");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        vector<char *> execArgs;
        for (string &arg : args) {
            execArgs.push_back(&arg[0]);
        }
        execArgs.push_back(nullptr);

        execvp(execArgs[0], execArgs.data());
        perror("Execution failed");
        exit(1);
    } else if (pid > 0) {
        if (!background) {
            waitpid(pid, nullptr, 0);
        }
    } else {
        perror("Fork failed");
    }
}