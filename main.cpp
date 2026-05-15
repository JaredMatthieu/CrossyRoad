
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif

using namespace std;

// color codes
#define RESET            "\033[0m"
#define BOLD             "\033[1m"
#define FG_RED           "\033[31m"
#define FG_GREEN         "\033[32m"
#define FG_YELLOW        "\033[33m"
#define FG_BLUE          "\033[34m"
#define FG_CYAN          "\033[36m"
#define FG_WHITE         "\033[37m"
#define FG_BRIGHT_WHITE  "\033[97m"
#define FG_BRIGHT_GREEN  "\033[92m"
#define FG_BRIGHT_CYAN   "\033[96m"
#define FG_BRIGHT_RED    "\033[91m"
#define FG_BRIGHT_YELLOW "\033[93m"
#define BG_GREEN         "\033[42m"
#define BG_RED           "\033[41m"
#define FG_BLACK         "\033[30m"

// node
struct Node {
    string data;
    Node* next;
};
typedef Node* NodePtr;

struct Inputs {
    int CPPUp;
    int CPPDown;
    int CPPLeft;
    int CPPRight;
};

void setCursorStart() { //Flores: Uses cursor placement instead of clearing to try and improve performance
    #ifdef _WIN32
        COORD coord = {0, 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    #else
        cout << "\033[H";  // moves cursor to top-left without clearing
    #endif
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");        
    #else
        system("clear");
    #endif
}

void sleepMs(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

// colored line ---
void println(const string& text, const string& color = FG_WHITE, bool bold = false) {
    if (bold) cout << BOLD;
    cout << color << text << RESET << "\n";
}

// UI functions

void showTitleScreen() {
    clearScreen();
    cout << "\n";

    cout << BOLD << FG_BRIGHT_GREEN << " _____ _____ _____ ____     _____ _____ _____ _____ _____ _____ _____ _____ " << RESET << "\n";
    cout << BOLD << FG_BRIGHT_GREEN << "| __  |     |  _  |    \\   |     | __  |     |   __|   __|     |   | |   __|" << RESET << "\n";
    cout << BOLD << FG_BRIGHT_GREEN << "|    -|  |  |     |  |  |  |   --|    -|  |  |__   |__   |-   -| | | |  |  |" << RESET << "\n";
    cout << BOLD << FG_BRIGHT_GREEN << "|__|__|_____|__|__|____/   |_____|__|__|_____|_____|_____|_____|_|___|_____|" << RESET << "\n";

    cout << "\n";
    println("                        ~ C H A L L E N G E ~", FG_BRIGHT_YELLOW, true);
    println("                      CSDC102  |  Language: C++", FG_BRIGHT_CYAN);
    cout << "\n";
    println("              HOW TO PLAY", FG_BRIGHT_WHITE, true);
    cout << "\n";
    println("              [ < ^ v > ]   Move with Arrow Keys",                  FG_BRIGHT_CYAN);
    println("              [  #####  ]   Dodge trucks in the ROAD ZONE",         FG_BRIGHT_RED);
    println("              [  =====  ]   Hop on logs in the RIVER ZONE",         FG_BLUE);
    println("              [    5x   ]   Reach the finish line 5 times to win!", FG_BRIGHT_YELLOW);
    cout << "\n";
    println("                Press ENTER to start...", FG_BRIGHT_GREEN, true);
    cout << "\n";

    cin.get();
}
    
// get player name
string getPlayerName() {
    clearScreen();
    cout << "\n";
    println("  PLAYER SETUP", FG_BRIGHT_YELLOW, true);
    cout << "\n";
    cout << FG_CYAN << "  ---------------------------------------------------------------------" << RESET << "\n";
    cout << "\n";
    println("  What is your name?", FG_WHITE);
    cout << "\n";
    cout << "  " << FG_BRIGHT_GREEN << BOLD << ">> " << RESET;

    string name;
    getline(cin, name);
    if (name.empty()) name = "Player";

    cout << "\n";
    println("  Welcome,  " + name + "!", FG_BRIGHT_CYAN, true);
    cout << "\n";

    return name;
}

// difficulty
int chooseDifficulty() {
    clearScreen();
    cout << "\n";
    println("  SELECT DIFFICULTY", FG_BRIGHT_YELLOW, true);
    cout << "\n";
    cout << FG_CYAN << "  ---------------------------------------------------------------------" << RESET << "\n";
    cout << "\n";

    cout << "  " << BG_GREEN << FG_BLACK << BOLD << " 1 " << RESET;
    cout << "  " << FG_BRIGHT_GREEN << BOLD << "EASY  " << RESET;
    cout << FG_WHITE << "Speed: 180ms  |  2 trucks/logs per lane" << RESET << "\n";
    cout << "\n";
    cout << "  " << BG_RED << FG_BLACK << BOLD << " 2 " << RESET;
    cout << "  " << FG_BRIGHT_RED << BOLD << "HARD  " << RESET;
    cout << FG_WHITE << "Speed: 100ms  |  3 trucks/logs per lane" << RESET << "\n";
    cout << "\n";
    cout << FG_CYAN << "  ---------------------------------------------------------------------" << RESET << "\n";
    cout << "\n";
    cout << "  " << FG_BRIGHT_GREEN << BOLD << ">> Enter choice (1 or 2): " << RESET;

    int diff = 1;
    cin >> diff;
    cin.ignore();
    if (diff != 1 && diff != 2) diff = 1;

    cout << "\n";
    string label  = (diff == 1) ? "EASY" : "HARD";
    string lcolor = (diff == 1) ? FG_BRIGHT_GREEN : FG_BRIGHT_RED;
    println("  Selected: " + label, lcolor, true);
    cout << "\n";

    return diff;
}

// lane generator (static for now)
string generateTerrain(char laneType) {
    string lane(42, ' ');

    lane[0] = '|';
    lane[41] = '|';

    if(laneType == 'F') {
        for(int i = 1; i <= 40; i++){
            lane[i] = '=';
        }
    }

    else if(laneType == 'B'){

        for(int i = 1; i <= 40; i++){
            lane[i] = '.';
        }
    }

    else if(laneType == 'R'){

        for(int i = 1; i <= 40; i++){
            lane[i] = '.';
        }

        int trucks = 2 + rand() % 2;

        int currentPos = 1 + rand() % 40;
        int posEnd = currentPos + 4;

        for(int t = 0; t < trucks; t++){ //loops for the amount of truck
            int truckCount = 0; //tracks which part of the truck it is

            for(int j = 0; j < 5; j++){
                if(currentPos + j <= 40){
                    lane[currentPos + truckCount] = '#';
                }
                else{
                    currentPos = 1, truckCount = 0;
                    lane[currentPos + truckCount] = '#';
                }
                truckCount++;
            }

            posEnd = currentPos + truckCount;
            currentPos = posEnd + (3 + rand() % 5);
        }
    }

    else if(laneType == 'V'){

        for(int i = 1; i <= 40; i++){
            lane[i] = '~';
        }

        int logs = 2;

        int currentPos = 1 + rand() % 40;
        int posEnd = currentPos + 4;

        for(int t = 0; t < logs; t++){ //loops for the amount of log
            int logCount = 0; //tracks which part of the log it is
            
            for(int j = 0; j < 4; j++){
                if(currentPos + j <= 40){
                    lane[currentPos + logCount] = '=';
                }
                else{
                    currentPos = 1, logCount = 0;
                    lane[currentPos + logCount] = '=';
                }
                logCount++;
            }

            posEnd = currentPos + logCount;
            currentPos = posEnd + (10 + rand() % 10);
        }
    }

    return lane;
}

///////////////////////////////////// core game functions


//shifting function

//shift inner 40 characters left, wrap the first character to the end
string shiftLeft(const string& lane) {
    string result = lane;
    char warp = result[1]; // store the first character for wrapping
    for (int i = 1; i < 40; i++) result[i] = result[i + 1];
    result[40] = warp; // wrap the first character to the end
    return result;
}

//shift inner 40 characters right, wrap the last character to the front
string shiftRight(const string& lane) {
    string result = lane;
    char warp = result[40]; // store the last character for wrapping
    for (int i = 40; i > 1; i--) result[i] = result[i - 1];
    result[1] = warp; // wrap the last character to the front
    return result;
}

//shifting function end


NodePtr buildRoad() {
    NodePtr head = NULL;
    NodePtr tail = NULL;

    char zoneMap[20] = {

            'F',

            'R','R','R','R','R',
            'B',
            'V','V',
            'B',

            'R','R','R','R','R',
            'B',
            'V','V',
            'B',

            'S'
        };

    for (int i = 0; i < 20; i++) {
        NodePtr newNode = new Node;
        newNode->next = NULL;

        newNode->data = generateTerrain(zoneMap[i]);

        // linked list
        if (head == NULL) {
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    return head;
}

// traverse linked list, print the board, overlay the player
void displayRoad(NodePtr head, string playerName, int playerX, int playerY, int lives, int score) {
    NodePtr curr = head;
    int row = 0;
    
    cout << "---------- Road Crossing Challenge ----------\n";
    cout << "Player: " << playerName << " | Lives: " << lives << " | Crossings: " << score << "\n";
    while (curr != NULL) {
        string rowData = curr->data;
        if (row == playerY) rowData[playerX] = 'P';
        cout << rowData << "\n";
        curr = curr->next;
        row++;
    }
    cout.flush(); // flush ONCE at the end
}

// pevent memory leaks
void freeList(NodePtr& head) {
    NodePtr temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        delete temp;
    }
    head = NULL;   // now this actually sets the original to NULL
}

//zone map - mirrors the same order as the buildRoad function for easy reference
//used by collision and shift function to know what the row contains 

const char 
ZONE_MAP[20] = {
    'F', // finish line

    'R','R','R','R','R', // river zone
    'B', // safe zone
    'V','V', // river zone with logs
    'B', // safe zone

    'R','R','R','R','R', // river zone
    'B', // safe zone
    'V','V', // river zone with logs
    'B', // safe zone

    'S'  // starting line
};

// returns pointer to node at position 'index'
NodePtr getLane(NodePtr head, int index) {
    NodePtr curr = head;
    for (int i = 0; i < index && curr != NULL; i++) {
        curr = curr->next;
    }
    return curr;
}

//shift obstacle function
void shiftObstacles(NodePtr head) {
    int roadNum = 0;
    int riverNum = 0;
    NodePtr curr = head;

    for (int i = 0; i < 20 && curr != NULL; i++, curr = curr->next) {

        if (ZONE_MAP[i] == 'R') {
            roadNum++;
            curr->data = (roadNum % 2 == 1)
            ? shiftRight(curr->data)
            : shiftLeft(curr->data);
        }

        else if (ZONE_MAP[i] == 'V') {
            riverNum++;
            curr->data = (riverNum % 2 == 1)
            ? shiftRight(curr->data)
            : shiftLeft(curr->data);
        }
    }
}

//check collision and drowned function

bool checkCollision(int playerX, int playerY, NodePtr laneCache[20]) {
    if (ZONE_MAP[playerY] != 'R') return false; // only check for trucks in road zones
    NodePtr lane = laneCache[playerY];
    if (lane == NULL) return false; // safety check
    return (lane->data[playerX] == '#'); // collision if player is on a truck
}

bool checkDrowned(int playerX, int playerY, NodePtr laneCache[20]) {
    if (ZONE_MAP[playerY] != 'V') return false; // only check for logs in river zones
    NodePtr lane = laneCache[playerY];
    if (lane == NULL) return false; // safety check
    return (lane->data[playerX] == '~'); //return true if player is in water without a log (drowned)
}


//Leaderboard

struct LeaderboardEntry {
    string name;
    int score;
};

//Save one result to the leaderboard after every game
void saveScore(const string& playerName, int score) {
    ofstream outFile("leaderboard.txt", ios::app);
    if (outFile.is_open()) {
        outFile << playerName << "|" << score << endl;
        outFile.close();
    } else {
        cerr << "Unable to open leaderboard file." << endl;
    }
}

void showLeaderboard() {
    ifstream file("leaderboard.txt");
    LeaderboardEntry entries[100];
    int count = 0;

    if (file.is_open()) {
        string line;
        while (getline(file, line) && count < 100) {
            int sep = line.find('|');
            if (sep == (int)string::npos) continue;  // skip malformed lines
            entries[count].name  = line.substr(0, sep);
            entries[count].score = stoi(line.substr(sep + 1));
            count++;
        }
        file.close();
    }

    // bubble sort and display — rest stays the same
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - 1 - i; j++) {
            if (entries[j].score < entries[j + 1].score) {
                LeaderboardEntry temp = entries[j];
                entries[j]           = entries[j + 1];
                entries[j + 1]       = temp;
            }
        }
    }

    cout << endl;
    cout << " ===== LEADERBOARD (Top 5) =====" << endl;
    int display = (count < 5) ? count : 5;
    for (int i = 0; i < display; i++) {
        cout << "  " << (i + 1) << ". "
            << entries[i].name << " — "
            << entries[i].score << " crossing(s)" << endl;
    }
    if (count == 0) cout << "  (No scores yet.)" << endl;
    cout << " ================================" << endl;
}

// leaderboard end below on how and where to call
// Call saveScore() when the game ends — win or lose
// Then immediately call showLeaderboard() so player sees rankings

//gameover/win function 

void showGameOver(bool won, const string& playerName, int score) {
    clearScreen();
    if (won) {
        cout << "Congratulations " << playerName << "! You crossed 5 times!" << endl;
    } else {
        cout << "Game Over, " << playerName << "! You were hit too many times." << endl;
    }

    //saves to the leaderboard and shows it
    saveScore(playerName, score);

    showLeaderboard();

    cout << "Press ENTER to return to the title screen..." << endl; 
    cin.ignore();
    cin.get();
}



//gameover/win endd


//Player Movement and Control : Flores
int keyboardInput(int input, int& playerX, int& playerY) {
    if (_kbhit()) {
        input = _getch();
        if (input == 0xE0 || input == 0) { 
            input = _getch();              
        }
        switch (input) {
            case 72: if (playerY > 0)  playerY--; break;  // UP
            case 80: if (playerY < 19) playerY++; break;  // DOWN
            case 75: if (playerX > 1)  playerX--; break;  // LEFT
            case 77: if (playerX < 40) playerX++; break;  // RIGHT
        }
        return 1;
    }
    return -1;
}

void shiftLogPlayer(int& playerX, int& playerY, NodePtr laneCache[20]) { //Flores
    if (ZONE_MAP[playerY] != 'V') return;  //Checks if the current zone is a river zone, if not, no need to check for log movement
    NodePtr lane = laneCache[playerY]; //accesses the current lane using the cache for efficiency
    if (lane == NULL) return; 
    
    if (lane->data[playerX] == '=') {
        int riverNum = 0;
        for (int i = 0; i <= playerY; i++) { // counts how many river zones have been passed to determine log movement direction
            if (ZONE_MAP[i] == 'V') riverNum++;
        }
        if (riverNum % 2 == 1) {
            if (playerX < 40) playerX++;
        } else {
            if (playerX > 1) playerX--;
        }
    }
}


/////////////////////////////////// MAIN LOOP

int main() {
    showTitleScreen();
    
    string name = getPlayerName();
    
    int diff = chooseDifficulty();

    clearScreen();
    cout << "\n";
    println("  PLAYER MENU", FG_BRIGHT_GREEN, true);
    cout << "\n";
    println("  Player    : " + name, FG_WHITE);
    println("  Difficulty: " + string(diff == 1 ? "Easy (180ms, 2 obstacles)" : "Hard (100ms, 3 obstacles)"), FG_WHITE);
    cout << "\n";
    println("  Press ENTER to start the game!", FG_BRIGHT_GREEN, true);
    cout << "\n";

    cin.get();
    
    // player pos
    int playerX = 20; 
    int playerY = 19; 
    int lives = 3; // added to track player lives
    int score = 0; // added to track successful crossings
    int input;

    // generate linked list map
    NodePtr roadList = buildRoad();
    NodePtr laneCache[20];
    
    

    bool isPlaying = true;
    bool playerWon = false; //added to track win condition

    int gameSpeed = (diff == 2) ? 100 : 180; // set game speed based on difficulty
    // main game loops
    while (isPlaying) {
        NodePtr curr = roadList;
        //shift obstacles every tick
        shiftObstacles(roadList);
        for (int i = 0; i < 20; i++, curr = curr->next) laneCache[i] = curr; //Caches the lane to avoid traversing the linked list multiple times for collision checks and display
        shiftLogPlayer(playerX, playerY, laneCache); 
        
        setCursorStart();
        displayRoad(roadList, pName, playerX, playerY, lives, score);
        
            //check win condition before starting the game loop
    if (score >= 5) {
        playerWon = true;
        isPlaying = false;
        break;
    }

    //check lose condition before starting the game loop
    if (lives <= 0) {
        playerWon = false;
        isPlaying = false;
        break;
    }

    //check if player reached the finish line (row 0)
    if (playerY == 0) {
        score++; //increment score for successful crossing
        playerX = 20, playerY = 19; //reset player to starting position
        freeList(roadList); //free old road
        roadList = buildRoad(); //generate new road
    }

    //collision: truck hit
    if (checkCollision(playerX, playerY, laneCache)) {
        lives--; // lose a life if hit by a truck
        playerX = 20; // reset player to starting position
        playerY = 19; // reset player to starting position  
    }

    //collision: drowned
    if (checkDrowned(playerX, playerY, laneCache)) { 
        lives--; // lose a life if drowned
        playerX = 20; // reset player to starting position  
        playerY = 19; // reset player to starting position
    }

    int elapsed = 0;
    while (elapsed < gameSpeed) { //Flores:
        if (_kbhit()) keyboardInput(input, playerX, playerY);
        Sleep(10);
        elapsed += 10;
        }
    }   

    //show game over/win screen
    showGameOver(playerWon, name, score);

    freeList(roadList);
    roadList = NULL;

    return 0;
    
}
