
#include <iostream>
#include <string>
#ifdef _WIN32
#include <conio.h>
#endif

using namespace std;

// node
struct Node {
    string data;
    Node* next;
};
typedef Node* NodePtr;

void clearScreen() {
    #ifdef _WIN32
        system("cls");   
    #else
        system("clear"); 
    #endif
}

// UI functions

void showTitleScreen() {
    cout << " _                 _         _                   _" << endl;
    cout << "|  _ \\ _   _ _  _| |  / ___| _ _  _ _() _   _ _" << endl;
    cout << "| |_) / _ \\ / _ |/ _ | | |   | '_/ _ \\/ __/ __| | ' \\ / _` |" << endl;
    cout << "|  _ < () | (| | (_| | | |___| | | () \\_ \\__ \\ | | | | (_| |" << endl;
    cout << "|_| \\_\\___/ \\__,_|\\__,_|  \\____|_|  \\___/|___/___/_|_| |_|\\__, |" << endl;
    cout << "              C h a l l e n g e                        |___/" << endl << endl;
    cout << "Written for CSDC102 | Language: C++" << endl << endl;
    cout << "HOW TO PLAY:" << endl;
    cout << "- Move with Arrow Keys" << endl;
    cout << "- Dodge trucks (#####) in the ROAD ZONE" << endl;
    cout << "- Hop on logs (====) in the RIVER ZONE" << endl;
    cout << "- Reach the finish line 5 times to win!" << endl << endl;
    cout << "Press ENTER to start..." << endl;
    
    cin.get();
}
    
// get player name
string getPlayerName() {
    string name;
    clearScreen();
    
    cout << "Enter player name: ";
    getline(cin, name);
    return name;
}

// difficulty
int chooseDifficulty() {
    int diff;
    clearScreen();
    
    cout << "SELECT DIFFICULTY" << endl;
    cout << "1. Easy  — speed: 180ms, 2 trucks/logs per lane" << endl;
    cout << "2. Hard  — speed: 100ms, 3 trucks/logs per lane" << endl << endl;
    cout << "Enter choice (1 or 2): ";
    cin >> diff;
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

    else if(laneType == 'B' || laneType == 'S'){

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
void displayRoad(NodePtr head, string playerName, int playerX, int playerY) {
    NodePtr curr = head;
    int row = 0;
    
    cout << "---------- Road Crossing Challenge ----------" << endl;
    cout << "Player: " << playerName << " | Lives: 3 | Crossings: 0" << endl;

    while (curr != NULL) {
        string rowData = curr->data;
        
        if (row == playerY) {
            rowData[playerX] = 'P';
        }
        
        cout << rowData << endl;
        curr = curr->next;
        row++;
    }
}

// pevent memory leaks
void freeList(NodePtr head) {
    NodePtr temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        delete temp;
    }
}


/////////////////////////////////// MAIN LOOP

int main() {
    showTitleScreen();
    string pName = getPlayerName();
    int difficulty = chooseDifficulty();

    // player pos
    int playerX = 20; 
    int playerY = 20; 

    // generate linked list map
    NodePtr roadList = buildRoad();

    bool isPlaying = true;

    // main game loop
    while (isPlaying) {
        clearScreen();
        displayRoad(roadList, pName, playerX, playerY);
        
        #ifdef _WIN32
        int ch = _getch();
        if (ch == 224) {
            ch = _getch();
            switch(ch) {
                case 72: // UP 
                    if (playerY > 0) playerY--;
                    break;
                case 80: // DOWN 
                    if (playerY < 19) playerY++;
                    break;
                case 75: // LEFT
                    if (playerX > 1) playerX--;
                    break;
                case 77: // RIGHT
                    if (playerX < 40) playerX++;
                    break;
            }
        } else if (ch == 'q' || ch == 'Q') {
            isPlaying = false; 
            // q to quit
        }
        #endif
    }
    freeList(roadList);
    roadList = NULL;

    return 0;
}