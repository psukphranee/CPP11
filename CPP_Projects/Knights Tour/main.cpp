/*
Two game board arrays are created. This first is the heuristic and the second is the
is the board we traverse using the heuristic as a guide for its movements. The starting
position is randomly selected. Starting from this random position, we traverse the board
using the heuristic grid values as a guide to determine what next move to make. We continue
traversing the board until we land at a position where there are no more possible moves.
Each move is labeled with a counter. The starting position is labeled 0.
*/

#include <iostream>
#include <iomanip>
#include <array>
#include <random>
#include <ctime>
using namespace std;

//global Variables
const int gridSize = 8;
const int maxMoves = 8;
array<array<int, gridSize>, gridSize> gameBoard = {}; //game board
array<array<int, gridSize>, gridSize> heuristicGrid = {}; //heuristic board
array<array<int, gridSize>, 3> coordPossibleMovements = {}; //possible movements tracker

//movement arrays
const array<int, 8> horizontal = {2,1, -1,-2,-2,-1, 1,2};
const array<int, 8> vertical = {-1,-2,-2,-1,1,2,2,1};

int currRow = 0;
int currCol = 0;
int nextRow = 0;
int nextCol = 0;
int movementCounter = 1;
int possibleMovements = 0;

//Functions
void populateHeuristicGrid();
int inBounds(int, int);
int determinePossibleMovements();
int smallestAccessibilityIndex();
void printPossibleMovements();

//Random engine
default_random_engine engine(static_cast<unsigned int>(time(0)));

int main()
{
  populateHeuristicGrid();
  //print heuristicGrid
  /*
  cout << "Heuristic Grid:\n";
  for(int i = 0; i < gridSize; ++i){
    for(int j = 0; j < gridSize; ++j){
      cout << setw(3) << heuristicGrid[i][j];
    }
    cout << endl;
  }
  cout << "--------------------------\n";
  */

  cout << "Enter initial row and column [0,7]: ";
  cin >> currRow >> currCol;

  //set initial row and column (start @ 0)
  //currRow = 3;
  //currCol = 4;
  cout << "Current row: " << currRow << "\nCurrent col: " << currCol << "\nMovement counter: " << movementCounter << endl;
  gameBoard[currRow][currCol] = movementCounter;
  possibleMovements = determinePossibleMovements();
  //cout << "Number of possible movements: " << possibleMovements << endl << endl;
  //printPossibleMovements();

  while(possibleMovements != 0){
    int min_index = smallestAccessibilityIndex();
    currRow = coordPossibleMovements[1][min_index];
    currCol = coordPossibleMovements[2][min_index];
    //cout << "Updating current coodinates to row index " << currRow << " col index " << currCol << endl;
    ++movementCounter;
    gameBoard[currRow][currCol] = movementCounter;
    //cout << "Movement counter incremented to " << movementCounter << " and assigned to the game board at coords [" << currRow << "," << currCol << "]" << endl;
    //cout << "[" << currRow << "," << currCol << "] = " << movementCounter << endl;
    possibleMovements = determinePossibleMovements();
    //printPossibleMovements();
    //cout <<  endl;
  }

  //print gameBoard
  cout << "Gameboard:\n";
  for(int i = 0; i < gridSize; ++i){
    for(int j = 0; j < gridSize; ++j){
      cout << setw(3) << gameBoard[i][j];
    }
    cout << endl;
  }
  cout << movementCounter << " moves." << endl;
}

void printPossibleMovements(){
  cout << "Possible Movements Table:\n";
  for(auto const &row: coordPossibleMovements){
    for(auto const &element: row){
      cout << setw(3) << element;
    }
    cout << endl;
  }
  cout << "--------------------------\n";
}

int determinePossibleMovements()
{
  //cout << "\tDetermining possible movements..." << endl;

  int initRow = currRow;
  int initCol = currCol;
  int possibleMovements = 0;

  //calculate coordinates of next moves
  //cout << "\tCalculating coordinates of surrounding 8 moves." << endl;
  for(int i = 0; i < maxMoves; ++i)
  {
    coordPossibleMovements[1][i] = initRow + vertical[i];
    coordPossibleMovements[2][i] = initCol + horizontal[i];
  }
  //mark with access index, the coordinates in bounds
  //cout << "\tMarking coordinates that are in bounds." << endl;
  for(int i = 0; i < maxMoves; ++i)
  {
    int n = coordPossibleMovements[1][i];
    int m = coordPossibleMovements[2][i];

    if(inBounds(n,m)){
      coordPossibleMovements[0][i] = heuristicGrid[n][m];
    }else{
      coordPossibleMovements[0][i] = 0; //explicitly mark 0
    }
  }
  //loop through coodinates in bounds, mark with a 0 the colmns that have been traversed
  //cout << "\tUnmarking coordinates that have been traversed." << endl;
  for(int i = 0; i < maxMoves; ++i)
  {
    int n = coordPossibleMovements[1][i];
    int m = coordPossibleMovements[2][i];
    int marker = coordPossibleMovements[0][i];

    //skip the columns marked 0
    if(marker == 0)
      continue;
    if(gameBoard[n][m] > 0) //traversed coordinates have value > 0
      coordPossibleMovements[0][i] = 0;
  }

  //return the number of possible movements
  for(int i = 0; i < maxMoves; ++i)
    if(coordPossibleMovements[0][i] != 0)
      possibleMovements++;

  //cout << "\tReturning with value " << possibleMovements << endl;
  return possibleMovements;
}

/*
int smallestAccessibilityIndex(){
  //cout << "Determining index with smallest accessibility number...\n";
  //loop through row 0 of coordPossibleMovements
  int min = 8;
  int min_index = 0;
  for(int i = 0; i < maxMoves; ++i){
    if((coordPossibleMovements[0][i] <= min) && (coordPossibleMovements[0][i] > 0)){
      min = coordPossibleMovements[0][i];
      min_index = i;
    }
  }
  //cout << "...lowest accessibility number of " << min << " at index " << min_index << endl;
  return min_index;
}
*/
//Commenting out smallestAccessibilityIndex() above and replacing it with a dynamic one which randomly selects a move when there's more than one move with the same lowest accessibility index

int smallestAccessibilityIndex()
{
  //create 3 integer vectors corresponding to the 3 rows of coordPossibleMovements
  vector<int> indices = {};
  vector<int> row = {};
  vector<int> col = {};

  int min_val = 8;
  int min_index = 0;

  //first find the minimum accessibility values
  for(int i = 0; i < maxMoves; ++i){
    if((coordPossibleMovements[0][i] <= min_val) && (coordPossibleMovements[0][i] > 0)){
      min_val = coordPossibleMovements[0][i];
    }
  }

  //loop througgh the coordPossibleMovements to find all accessibility values equal to min_val
  for(int i = 0; i < maxMoves; ++i){
    if(coordPossibleMovements[0][i] == min_val){
      indices.push_back(i);
      row.push_back(coordPossibleMovements[1][i]);
      col.push_back(coordPossibleMovements[2][i]);
    }
  }

  const int upper_limit = indices.size() - 1;
  uniform_int_distribution<unsigned int> randInt(0, upper_limit);

  return indices[randInt(engine)];
}

int inBounds(int row, int col)
{
  if((row < gridSize) && (row >= 0)){
    if((col >= 0) && (col < gridSize)){
      return 1;
    }
  }
  return 0;
}

//function defs
void populateHeuristicGrid()
{
  int movementCounter = 0;

  for(int i = 0; i < gridSize; ++i){
    for(int j = 0; j < gridSize; ++j){
      movementCounter = 0;
      for(int k = 0; k < gridSize; ++k){
        nextRow = i + vertical[k];
        nextCol = j + horizontal[k];
        //cout << "nextRow: " << nextRow << "\nnextCol: " << nextCol << endl;
        if(inBounds(nextRow, nextCol)){
          ++movementCounter;
        }
      }
      heuristicGrid[i][j] = movementCounter;
    }
  }

}
