#include <iostream>
#include <string>
using namespace std;

//Global Variables
const static short COLS = 20; const static short ROWS = 10;
string aliveChar = "■"; string deadChar = "□";

struct cell{
    cell* neighbors[8]; // up left, up, up right, left, right, down left, down, down right
    bool alive = false;
};

void initializeBoard(cell* board[][COLS]){
    for(size_t i = 0; i < ROWS; i++){
        for(size_t j = 0; j < COLS; j++){
            board[i][j] = new cell();
        }
    }
    for(size_t i = 0; i < ROWS; i++){
        for(size_t j = 0; j < COLS; j++){
            board[i][j]->neighbors[0] = ((i > 0 && j > 0) ? board[i-1][j-1] : nullptr);           // up left
            board[i][j]->neighbors[1] = ((i > 0) ? board[i-1][j] : nullptr);                      // up
            board[i][j]->neighbors[2] = ((i > 0 && j < COLS-1) ? board[i-1][j+1] : nullptr);      // up right *
            board[i][j]->neighbors[3] = ((j > 0) ? board[i][j-1] : nullptr);                      // left
            board[i][j]->neighbors[4] = ((j < COLS-1) ? board[i][j+1] : nullptr);                 // right *
            board[i][j]->neighbors[5] = ((i < ROWS-1 && j > 0) ? board[i+1][j-1] : nullptr);      // down left
            board[i][j]->neighbors[6] = ((i < ROWS-1) ? board[i+1][j] : nullptr);                 // down *
            board[i][j]->neighbors[7] = ((i < ROWS-1 && j < COLS-1) ? board[i+1][j+1] : nullptr); // down right *
        }
    }
}

unsigned short neighborsAlive(cell* currCell){
    unsigned short total = 0;
    for(size_t i = 0; i < 8; i++){
        cell* neighbor = currCell->neighbors[i];
        total += ((neighbor != nullptr && neighbor->alive) ? 1 : 0);
    }
    return total;
};

void update(cell* board[][COLS]){
    //rules: dead cell with 3 neighbors = alive
    //live cells without 2 or 3 neighbors = death

    //NOTE: I NEED TO MAKE A NEW BOARD; OTHERWISE THE INITIALIZATION OF NEW CELL STATES WILL AFFECT THE CURRENT INITIALIZATION OF OTHER CELLS
    bool newBoard[ROWS][COLS];
    for(size_t i = 0; i < ROWS; i++){
        for(size_t j = 0; j < COLS; j++){
            cell* currCell = board[i][j];
            unsigned short numNeighbors = neighborsAlive(currCell);
            newBoard[i][j] = (currCell->alive ? ((numNeighbors == 2 || numNeighbors == 3) ? true : false) : ((numNeighbors == 3) ? true : false));
        }
    }
    for(size_t i = 0; i < ROWS; i++){
        for(size_t j = 0; j < COLS; j++){
            board[i][j]->alive = newBoard[i][j];
        }
    }
}

void render(cell* board[][COLS]){
    cell* currCell = nullptr;
    cout << "\n  ";
    for(size_t i = 0; i < COLS; i++){
        cout << static_cast<char>('A' + i);
    }
    cout << endl;
    for(size_t i = 0; i < ROWS; i++){
        cout << i << " ";
        for(size_t j = 0; j < COLS; j++){
            currCell = board[i][j];
            cout << ((currCell == nullptr || !currCell->alive) ? deadChar : aliveChar);
        }
        cout << "\n";
    }
}

int main(){
    cout << "WELCOME TO CONWAY'S GAME OF LIFE!\n\n";
    cell* board[ROWS][COLS] = {{nullptr}};
    initializeBoard(board);
    render(board);
    int sentinel = 0;
    string input = "";
    do{
        cout << "\nPLEASE CHOOSE AN OPTION!\n0) Quit\n1) Run Simulation\n2) Board Visual Settings\n3) Game Rules\n\nUSER INPUT: ";
        cin >> input;
        string colInput = "", rowInput = "";
        char COL = '\0'; int ROW = 0;
        if(isdigit(input[0])){
            sentinel = input[0] - '0';
            switch(sentinel){
                case 0:
                    break;
                case 1:{
                    cout << "THE SIMULATION IS CURRENTLY USING " << ROWS << " ROWS AND " << COLS << " COLUMNS FOR ITS BOARD.\n";
                    cout << "THE SIMULATION IS CURRENTLY USING " << aliveChar << " TO REPRESENT AN ALIVE CELL AND " << deadChar << " TO REPRESENT A DEAD CELL.\n";
                    do{
                        cout << "INPUT THE CELL(S) THAT YOU WOULD LIKE TO CHANGE THE ALIVE STATE OF. INPUT THE COLUMN, A SPACE, THEN THE ROW (EX: B 2). TO QUIT, TYPE -1 -1\nUSER INPUT: ";
                        cin >> colInput >> rowInput;
                        bool validRow = true;
                        for(size_t i = 0; i < rowInput.length(); i++){
                            validRow &= isdigit(rowInput[i]);
                        }
                        if(colInput.length() == 1 && rowInput.length() > 0 && validRow){
                            COL = static_cast<int>(colInput[0] - 'A'); ROW = stoi(rowInput);
                            if((COL < COLS) && (ROW < ROWS)){
                                board[ROW][COL]->alive = !board[ROW][COL]->alive;
                                render(board);
                            }else{
                                cout << "\nROW OR COLUMN IS NOT WITHIN SIZE!\n";
                                cout << "\nTHE SIMULATION IS CURRENTLY USING " << ROWS << " ROWS AND " << COLS << " COLUMNS FOR ITS BOARD.\n";
                            }
                        }else{
                            if(colInput != "-1"){
                                cout << "\nINVALID INPUT!\n";
                            }
                        }
                    }while(colInput != "-1" && rowInput != "-1");
                    cout << "\nBOARD INITALIZED!\n";
                    bool validInput = false;
                    bool manual = false;
                    do{
                        cout << "WOULD YOU LIKE TO RUN EACH GENERATION CONTINUOUSLY OR WOULD YOU LIKE TO STEP MANUALLY THROUGH EACH GENERATION?\n0) AUTO\n1) MANUAL\nUSER INPUT: ";
                        cin >> input;
                        if(isdigit(input[0])){
                            switch(input[0] - '0'){
                                case 0:
                                    cout << "\nAUTO-STEP ENABLED!\n";
                                    validInput = true;
                                    break;
                                case 1:
                                    cout << "\nMANUAL-STEP ENABLED!\n";
                                    manual = true;
                                    validInput = true;
                                    break;
                                default:
                                    cout << "\nINVALID INPUT!\n";
                                    break;
                            }
                        }else{
                            cout << "\nINVALID INPUT!\n";
                        }
                    }while(!validInput);
                    if(manual){
                        int n = 0;
                        render(board);
                        cout << "GENERATION 0\n";
                        do{
                            cout << "TO GO TO THE NEXT GENERATION, PRESS ENTER; INPUT -1 TO EXIT.\nUSER INPUT: ";
                            input = "";
                            getline(cin, input);
                            update(board);
                            render(board);
                            cout << "GENERATION " << ++n << "\n";
                        }while(input != "-1");
                    }else{
                        bool validGen = true;
                        do{
                            cout << "HOW MANY GENERATIONS WOULD YOU LIKE TO RUN FOR? PLEASE INSERT A NUMBER.\nNUMBER OF GENERATIONS: ";
                            cin >> input;
                            validGen = true;
                            for(size_t j = 0; j < input.length(); j++){
                                validGen &= isdigit(input[j]);
                            }
                            if(validGen){
                                int numGenerations = stoi(input);
                                cout << "RUNNING SIMULATION FOR " << input << " GENERATIONS!\n\n";
                                for(size_t n = 0; n < numGenerations + 1; n++){
                                    render(board);
                                    cout << "GENERATION " << n << "\n";
                                    update(board);
                                }
                            }else{
                                cout << "\nINVALID INPUT!\n";
                            }
                        }while(!validGen);
                    }
                    break;
                }
                case 2:{
                    cout << "PLEASE INPUT THE CHARACTER THAT YOU WANT TO REPRESENT THE ALIVE STATE FOR A CELL:\nALIVE CHAR: ";
                    cin >> input;
                    aliveChar = input[0];
                    cout << "PLEASE INPUT THE CHARACTER THAT YOU WANT TO REPRESENT THE DEAD STATE FOR A CELL:\nDEAD CHAR: ";
                    cin >> input;
                    deadChar = input[0];
                    render(board);
                    break;
                }
                case 3:{
                    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~RULES FOR CONWAY'S GAME OF LIFE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n";
                    cout << "|                         A cell is defined by one square on the board.                         |\n";
                    cout << "|          A neighbor is any cell adjacent (directly or diagonally) to a current cell.          |\n";
                    cout << "|      1) Any live cell with fewer than two live neighbors dies, as if by underpopulation.      |\n";
                    cout << "|       2) Any live cell with two or three live neighbors lives on to the next generation.      |\n";
                    cout << "|      3) Any live cell with more than three live neighbors dies, as if by overpopulation.      |\n";
                    cout << "| 4) Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.|\n";
                    cout << "+~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+\n";
                    break;
                }
                default:{
                    cout << "\nINCORRECT INPUT. PLEASE CHOOSE ONE OF THE MENU OPTIONS!";
                    break;
                }
            }
        }else{
            cout << "\nINCORRECT INPUT. PLEASE CHOOSE ONE OF THE MENU OPTIONS!";
            sentinel = 4;
        }
    }while(sentinel);
    //DELETE AND FREE MEMORY
    for (size_t i = 0; i < ROWS; ++i) {
        for (size_t j = 0; j < COLS; ++j) {
            delete board[i][j];
        }
    }
    return 0;
}