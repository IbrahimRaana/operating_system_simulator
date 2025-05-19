#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

using namespace std;

// Global Variables
long int RAM = 8192;  // 8 GB RAM
long int HDD = 512000;  // 500 GB Storage
long int CORES = 4;
int choice = 0;
bool exitFlag = false;

string process_str[] = {
    "CALCULATOR", "TICTACTOE", "CLOCK", "NOTEPAD", "TODO", 
    "CREATE", "DELETE", "RENAME", "MUSIC", "CALENDAR",
    "RANDOM", "UNITCONVERT"
};

const int maxprocess = 12;
bool kernel_mode = false;
int threads_per_core = 2;
vector<int> runningProcessesTimes(maxprocess, 0);
vector<vector<int>> runningCores;

// Function Declarations
void displayWelcome();
void displayMessageCentered(string message, int windowWidth);
void MainMenu();
void displayTime();
void Calculator();
void TicTacToe();
void Clock();
void Notepad();
void TodoList();
void CreateFile();
void DeleteFile();
void RenameFile();
void PlayMusic();
void Calendar();
void RandomGenerator();
void UnitConverter();

// Function to emulate Windows getch()
char getch() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return buf;
}

// Function to clear screen in Linux
void clearScreen() {
    system("clear");  // Using system clear for better WSL compatibility
}

// Function to make a beep sound in Linux
void makeBeep() {
    cout << "\a" << flush;
    usleep(500000);  // Sleep for 500ms
}

// Sleep function for Linux (in milliseconds)
void sleepMs(int milliseconds) {
    usleep(milliseconds * 1000);
}

// Welcome Screen
void displayWelcome() {
    clearScreen();
    cout << "\n\n\n";
    displayMessageCentered("*", 80);
    displayMessageCentered("Welcome to Our Operating System Simulation", 80);
    displayMessageCentered("Developed by:", 80);
    displayMessageCentered("Bilawal and Ibrahim Zahid", 80);
    displayMessageCentered("*", 80);
    cout << "\n\n";
    sleepMs(2000);  // Display for 2 seconds
}

void displayMessageCentered(string message, int windowWidth) {
    int messageWidth = message.length();
    int leftMargin = (windowWidth - messageWidth) / 2;
    cout << string(leftMargin, ' ') << message << endl;
}

void MainMenu() {
    clearScreen();
    cout << "\n\n";
    displayMessageCentered("******* MAIN MENU *******", 80);
    cout << "\n\n";
    displayMessageCentered("1. Calculator", 80);
    displayMessageCentered("2. Tic Tac Toe", 80);
    displayMessageCentered("3. Clock", 80);
    displayMessageCentered("4. Notepad", 80);
    displayMessageCentered("5. TO-DO List", 80);
    displayMessageCentered("6. Create File", 80);
    displayMessageCentered("7. Delete File", 80);
    displayMessageCentered("8. Rename File", 80);
    displayMessageCentered("9. Play Music", 80);
    displayMessageCentered("10. Calendar", 80);
    displayMessageCentered("11. Random Number Generator", 80);
    displayMessageCentered("12. Unit Converter", 80);
    displayMessageCentered("0. Exit", 80);
    cout << "\n";
    displayMessageCentered("Enter your choice: ", 80);
}

void displayTime() {
    time_t now = time(nullptr);
    tm* current_time = localtime(&now);
    string time_str = "Current Time: " + 
                     to_string(current_time->tm_hour) + ":" + 
                     to_string(current_time->tm_min);
    displayMessageCentered(time_str, 80);
}

// Calculator Implementation
void Calculator() {
    clearScreen();
    double num1, num2;
    char op;
    displayMessageCentered("*** Calculator ***", 80);
    cout << "\nEnter first number: ";
    cin >> num1;
    cout << "Enter operator (+, -, *, /): ";
    cin >> op;
    cout << "Enter second number: ";
    cin >> num2;

    switch(op) {
        case '+': cout << "\nResult: " << num1 + num2; break;
        case '-': cout << "\nResult: " << num1 - num2; break;
        case '*': cout << "\nResult: " << num1 * num2; break;
        case '/': 
            if(num2 != 0) cout << "\nResult: " << num1 / num2;
            else cout << "\nError: Division by zero!";
            break;
        default: cout << "\nInvalid operator!";
    }
    cout << "\n\nPress any key to continue...";
    getch();
}

// Clock Implementation
void Clock() {
    clearScreen();
    displayMessageCentered("*** Digital Clock ***", 80);
    for(int i = 0; i < 10 && !exitFlag; i++) {
        clearScreen();
        displayTime();
        sleepMs(1000);
    }
}

// Notepad Implementation
void Notepad() {
    clearScreen();
    displayMessageCentered("*** Simple Notepad ***", 80);
    string filename, content;
    cout << "\nEnter filename to create/open: ";
    cin >> filename;
    cin.ignore();
    cout << "Enter text (press Ctrl+Z and Enter to save):\n";
    
    ofstream file(filename);
    while(getline(cin, content)) {
        file << content << endl;
    }
    file.close();
    cout << "\nFile saved successfully!\n";
    cout << "Press any key to continue...";
    getch();
}

// TicTacToe Implementation
void TicTacToe() {
    clearScreen();
    displayMessageCentered("*** Tic Tac Toe ***", 80);
    char board[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};
    int row, col;
    char currentPlayer = 'X';
    bool gameWon = false;
    int moves = 0;

    auto displayBoard = [&]() {
        clearScreen();
        displayMessageCentered("*** Tic Tac Toe ***", 80);
        cout << "\n\n";
        for(int i = 0; i < 3; i++) {
            cout << "\t\t\t     " << board[i][0] << " | " << board[i][1] << " | " << board[i][2] << endl;
            if(i != 2) cout << "\t\t\t    ---|---|---" << endl;
        }
    };

    auto checkWin = [&]() {
        for(int i = 0; i < 3; i++) {
            if(board[i][0] == currentPlayer && board[i][1] == currentPlayer && board[i][2] == currentPlayer) return true;
            if(board[0][i] == currentPlayer && board[1][i] == currentPlayer && board[2][i] == currentPlayer) return true;
        }
        if(board[0][0] == currentPlayer && board[1][1] == currentPlayer && board[2][2] == currentPlayer) return true;
        if(board[0][2] == currentPlayer && board[1][1] == currentPlayer && board[2][0] == currentPlayer) return true;
        return false;
    };

    while(!gameWon && moves < 9) {
        displayBoard();
        cout << "\nPlayer " << currentPlayer << "'s turn." << endl;
        cout << "Enter row (1-3) and column (1-3): ";
        cin >> row >> col;
        row--; col--;

        if(row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == ' ') {
            board[row][col] = currentPlayer;
            moves++;
            if(checkWin()) {
                displayBoard();
                cout << "\nPlayer " << currentPlayer << " wins!" << endl;
                gameWon = true;
            } else {
                currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            }
        } else {
            cout << "Invalid move! Press any key to try again...";
            getch();
        }
    }

    if(!gameWon) {
        displayBoard();
        cout << "\nGame Draw!" << endl;
    }
    cout << "\nPress any key to continue...";
    getch();
}

// TodoList Implementation
void TodoList() {
    clearScreen();
    vector<string> todos;
    string task;
    int choice;
    
    while(true) {
        clearScreen();
        displayMessageCentered("*** TODO List ***", 80);
        cout << "\n1. Add Task";
        cout << "\n2. View Tasks";
        cout << "\n3. Delete Task";
        cout << "\n4. Back to Main Menu";
        cout << "\n\nEnter choice: ";
        cin >> choice;
        cin.ignore();

        switch(choice) {
            case 1:
                cout << "Enter task: ";
                getline(cin, task);
                todos.push_back(task);
                cout << "Task added!";
                break;
            case 2:
                if(todos.empty()) {
                    cout << "\nNo tasks in the list!";
                } else {
                    cout << "\nYour Tasks:\n";
                    for(size_t i = 0; i < todos.size(); i++) {
                        cout << i+1 << ". " << todos[i] << endl;
                    }
                }
                break;
            case 3:
                if(todos.empty()) {
                    cout << "\nNo tasks to delete!";
                } else {
                    int index;
                    cout << "Enter task number to delete: ";
                    cin >> index;
                    if(index > 0 && index <= static_cast<int>(todos.size())) {
                        todos.erase(todos.begin() + index - 1);
                        cout << "Task deleted!";
                    } else {
                        cout << "Invalid task number!";
                    }
                }
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice!";
        }
        cout << "\nPress any key to continue...";
        getch();
    }
}

// File Operations
void CreateFile() {
    clearScreen();
    displayMessageCentered("*** Create File ***", 80);
    string filename, content;
    cout << "\nEnter filename to create: ";
    cin >> filename;
    cin.ignore();
    
    ofstream file(filename);
    if(file.is_open()) {
        cout << "Enter file content (press Ctrl+Z and Enter when done):\n";
        while(getline(cin, content)) {
            file << content << endl;
        }
        file.close();
        cout << "\nFile created successfully!";
    } else {
        cout << "\nError creating file!";
    }
    cout << "\nPress any key to continue...";
    getch();
}

void DeleteFile() {
    clearScreen();
    displayMessageCentered("*** Delete File ***", 80);
    string filename;
    cout << "\nEnter filename to delete: ";
    cin >> filename;
    
    if(remove(filename.c_str()) == 0) {
        cout << "\nFile deleted successfully!";
    } else {
        cout << "\nError deleting file!";
    }
    cout << "\nPress any key to continue...";
    getch();
}

void RenameFile() {
    clearScreen();
    displayMessageCentered("*** Rename File ***", 80);
    string oldname, newname;
    cout << "\nEnter current filename: ";
    cin >> oldname;
    cout << "Enter new filename: ";
    cin >> newname;
    
    if(rename(oldname.c_str(), newname.c_str()) == 0) {
        cout << "\nFile renamed successfully!";
    } else {
        cout << "\nError renaming file!";
    }
    cout << "\nPress any key to continue...";
    getch();
}

// Music Player Implementation
void PlayMusic() {
    clearScreen();
    displayMessageCentered("*** Simple Music Player ***", 80);
    cout << "\nPlaying a simple tune...\n";
    
    makeBeep();
    makeBeep();
    makeBeep();
    makeBeep();
    makeBeep();
    
    cout << "\nDone playing!";
    cout << "\nPress any key to continue...";
    getch();
}

// Calendar Implementation
void Calendar() {
    clearScreen();
    displayMessageCentered("*** Calendar ***", 80);
    
    time_t now = time(nullptr);
    tm* ltm = localtime(&now);
    
    int year = 1900 + ltm->tm_year;
    int month = 1 + ltm->tm_mon;
    
    cout << "\nCurrent Date: " << ltm->tm_mday << "/" << month << "/" << year << endl;
    
    const string months[] = {"January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};
    
    cout << "\n      " << months[month-1] << " " << year << endl;
    cout << " Su Mo Tu We Th Fr Sa" << endl;
    
    tm first = *ltm;
    first.tm_mday = 1;
    mktime(&first);
    int firstDay = first.tm_wday;
    
    int days = 31;
    if(month == 4 || month == 6 || month == 9 || month == 11) days = 30;
    else if(month == 2) {
        days = 28;
        if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) days = 29;
    }
    
    for(int i = 0; i < firstDay; i++) cout << "   ";
    for(int i = 1; i <= days; i++) {
        printf("%3d", i);
        if((i + firstDay) % 7 == 0) cout << endl;
    }
    
    cout << "\n\nPress any key to continue...";
    getch();
}

// Random Number Generator
void RandomGenerator() {
    clearScreen();
    displayMessageCentered("*** Random Number Generator ***", 80);
    int min, max;
    cout << "\nEnter range (min max): ";
    cin >> min >> max;
    
    if(min > max) swap(min, max);
    
    srand(time(nullptr));
    int random = min + rand() % (max - min + 1);
    
    cout << "\nRandom number between " << min << " and " << max << " is: " << random;
    cout << "\n\nPress any key to continue...";
    getch();
}

// Unit Converter
void UnitConverter() {
    clearScreen();
    displayMessageCentered("*** Unit Converter ***", 80);
    cout << "\n1. Length (m to ft)";
    cout << "\n2. Weight (kg to lb)";
    cout << "\n3. Temperature (C to F)";
    cout << "\n\nEnter choice: ";
    
    int choice;
    double value;
    cin >> choice;
    
    cout << "Enter value: ";
    cin >> value;
    
    switch(choice) {
        case 1:
            cout << value << " meters = " << (value * 3.28084) << " feet";
            break;
        case 2:
            cout << value << " kilograms = " << (value * 2.20462) << " pounds";
            break;
        case 3:
            cout << value << " Celsius = " << (value * 9/5 + 32) << " Fahrenheit";
            break;
        default:
            cout << "Invalid choice!";
    }
    
    cout << "\n\nPress any key to continue...";
    getch();
}

// Main Process Scheduler
void processScheduler() {
    while(!exitFlag) {
        clearScreen();
        MainMenu();
        cin >> choice;

        if(choice == 0) {
            exitFlag = true;
            kernel_mode = true;
            break;
        }

        switch(choice) {
            case 1: Calculator(); break;
            case 2: TicTacToe(); break;
            case 3: Clock(); break;
            case 4: Notepad(); break;
            case 5: TodoList(); break;
            case 6: CreateFile(); break;
            case 7: DeleteFile(); break;
            case 8: RenameFile(); break;
            case 9: PlayMusic(); break;
            case 10: Calendar(); break;
            case 11: RandomGenerator(); break;
            case 12: UnitConverter(); break;
            default: 
                displayMessageCentered("Invalid choice! Press any key to continue...", 80);
                getch();
        }
    }
}

int main() {
    // Initialize cores
    runningCores.resize(CORES, vector<int>(threads_per_core, -1));
    
    // Display welcome screen
    displayWelcome();
    
    // Start process scheduler
    processScheduler();
    
    // Exit message
    clearScreen();
    displayMessageCentered("Thank you for using our OS Simulation!", 80);
    displayMessageCentered("Developed by Bilawal and Ibrahim Zahid", 80);
    sleepMs(2000);
    
    return 0;
}
