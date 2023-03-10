/*#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
//#include <mpi.h>
// Creating a shortcut for int, int pair type
//typedef pair < int, int >  Pair;
typedef struct {
    int first;
    int second;
}Pair;
Pair make_pair(int first, int second) {
    Pair p;
    p.first = first;
    p.second = second;
    return p;
}
// Creating a shortcut for pair<int, pair<int, int>> type
//typedef pair < double, pair < int, int > > pPair;
typedef struct {
    double first;
    Pair pair;
}pPair;

pPair make_p_pair(double first, Pair pair) {
    pPair p;
    p.first = first;
    p.pair = pair;
    return p;
}
// A structure to hold the necessary parameters
typedef struct {
    // Row and Column index of its parent
    // Note that 0 <= i <= ROW-1 & 0 <= j <= COL-1
    int parent_i, parent_j;
    // f = g + h
    double f, g, h;
}cell;

typedef struct {
    pPair** items;
    int itemsCount;
    pPair* minimum;
}set;
set* newSet(int capacity) {
    set* s = (set*)malloc(sizeof(set));
    s->items = (pPair**)malloc(sizeof(Pair*) * capacity);
    s->itemsCount = 0;
    s->minimum = NULL;
    return s;
}
void deallocateSet(set* s) {
    if (s != NULL) {
        if (s->items) {
            int i;
            for (i = 0; i < s->itemsCount; i++) {
                free(s->items[i]);
            }
        }
        free(s->items);
        //free(s->minimum);
        free(s);
    }

}
//TODO: Inefficient
int insert(set* s, pPair p, cell** cellDetails) {
    int insertPos = s->itemsCount;
    pPair* el = (pPair*)malloc(sizeof(pPair));
    el->first = p.first;
    el->pair.first = p.pair.first;
    el->pair.second = p.pair.second;
    if (s->minimum == NULL) {
        s->minimum = el;
    }
    else {
        //TODO: check outofbound
        if (cellDetails[p.pair.first][p.pair.second].f < cellDetails[s->items[0]->pair.first][s->items[0]->pair.second].f) {
            insertPos = 0;
            int i;
            for (i = s->itemsCount; i > 0; i--) {
                s->items[i] = s->items[i - 1];
            }
        }
    }/*
    for(int i=0;i<s->itemsCount;i++){
      if(p.first==s->items[i].first && p.pair.first==s->items[i].pair.first && p.pair.second==s->items[i].pair.second){
        return 0;
      }
    }*/

    s->items[insertPos] = el;
    s->itemsCount++;
    return 1;
}
int emptySet(set* s) {
    return s->itemsCount == 0;
}
int RemoveItemInPosition(set* s, int position) {
    if (position >= s->itemsCount) {
        return 0;
    }
    else if (s->itemsCount > 1)
    {
        s->minimum = s->items[position + 1];
        free(s->items[position]);
        int i;
        for (i = position; i < s->itemsCount - 1; i++) {
            s->items[i] = s->items[i + 1];
        }

    }
    else {
        free(s->items[position]);
        s->minimum = NULL;
    }
    s->itemsCount--;
    return 1;

}




// Data structure to represent a stack
typedef struct
{
    int maxsize;    // define max capacity of the stack
    int top;
    Pair* items;
}stack;

// Utility function to initialize the stack
stack* newStack(int capacity)
{
    stack* pt = (stack*)malloc(sizeof(stack));

    pt->maxsize = capacity;
    pt->top = -1;
    pt->items = (Pair*)malloc(sizeof(Pair) * capacity);

    return pt;
}

void deallocateStack(stack* s) {
    if (s != NULL) {
        if (s->items != NULL) {
            free(s->items);
        }
        free(s);
    }

}
// Utility function to return the size of the stack
int size(stack* pt) {
    return pt->top + 1;
}

// Utility function to check if the stack is empty or not
int isEmpty(stack* pt) {
    return pt->top == -1;                   // or return size(pt) == 0;
}

// Utility function to check if the stack is full or not
int isFull(stack* pt) {
    return pt->top == pt->maxsize - 1;      // or return size(pt) == pt->maxsize;
}

// Utility function to add an element `x` to the stack
void push(stack* pt, Pair x)
{
    // check if the stack is already full. Then inserting an element would
    // lead to stack overflow
    if (isFull(pt))
    {
        printf("Overflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }

    //printf("Inserting %d\n", x);

    // add an element and increment the top's index
    pt->items[++pt->top] = x;
}

// Utility function to return the top element of the stack
Pair peek(stack* pt)
{
    // check for an empty stack
    if (!isEmpty(pt)) {
        return pt->items[pt->top];
    }
    else {
        exit(EXIT_FAILURE);
    }
}

// Utility function to pop a top element from the stack
Pair pop(stack* pt)
{
    // check for stack underflow
    if (isEmpty(pt))
    {
        printf("Underflow\nProgram Terminated\n");
        exit(EXIT_FAILURE);
    }

    //printf("Removing %d\n", peek(pt));

    // decrement stack size by 1 and (optionally) return the popped element
    return pt->items[pt->top--];
}











int isValid(int** grid, int row, int col, int row_max, int col_max) {
    // Returns true if row number and column number
    // is in range
    return (row >= 0) && (row < row_max) && (col >= 0) && (col < col_max) && grid[row][col] == 1;
}
int isDestination(int row, int col, Pair dest) {
    if (row == dest.first && col == dest.second)
        return 1;
    else
        return 0;
}

double calculateHValue(int row, int col, Pair dest, double(*func)(int, int, Pair)) {
    // Return using the distance formula
    return func(row, col, dest);
}
double euclideanDistance(int row, int col, Pair dest) {
    return ((double)sqrt(
        (row - dest.first) * (row - dest.first) +
        (col - dest.second) * (col - dest.second)));
}
double manhattanDistance(int row, int col, Pair dest) {
    return abs(row - dest.first) + abs(col - dest.second);
}

// A Utility Function to trace the path from the source
// to destination
int c = 0;
void tracePath(cell** cellDetails, Pair dest, int row_max, int col_max) {
    printf("\nThe Path is ");
    int row = dest.first;
    int col = dest.second;

    stack* Path = newStack(row_max * col_max);

    while (!(cellDetails[row][col].parent_i == row &&
        cellDetails[row][col].parent_j == col)) {
        push(Path, make_pair(row, col));
        int temp_row = cellDetails[row][col].parent_i;
        int temp_col = cellDetails[row][col].parent_j;
        row = temp_row;
        col = temp_col;
    }
    int** board = (int**)malloc(row_max * sizeof(int*));

    // for each row allocate Cols ints
    int rr, cc;
    for (rr = 0; rr < row_max; rr++) {
        board[rr] = (int*)malloc(col_max * sizeof(int));
        for (cc = 0; cc < col_max; cc++) {
            board[rr][cc] = 0;
        }
    }
    push(Path, make_pair(row, col));
    while (!isEmpty(Path)) {
        Pair p = peek(Path);
        pop(Path);
        printf("-> (%d,%d) ", p.first, p.second);
        board[p.first][p.second] = 1;
    }
    printf("\n");

    char path[100] = "D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\outputs\\";
    char* buf = (char*)malloc(10);
    if (buf != NULL) {
        sprintf(buf, "%d", c);
        strcat(path,buf );
    }
    c++;
    free(buf);
    strcat(path, ".txt");
    FILE* pFile = fopen(path, "a");
    int i, j;
    for (i = 0; i < row_max; i++) {
        fprintf(pFile, "|");
        for (j = 0; j < col_max; j++) {
            if (i == 0 && j == 0) {
                fprintf(pFile, "D");
            }
            else if (i == 44 && j == 49) {
                fprintf(pFile, "S");
            }
            else if (board[i][j] == 1) {
                fprintf(pFile, "O");
            }
            else {
                fprintf(pFile, " ");
            }
        }
        fprintf(pFile, "|\n");
    }
    fclose(pFile);
    for (rr = 0; rr < row_max; rr++) {
        free(board[rr]);
    }
    free(board);
    deallocateStack(Path);
    return;
}
int calculateCellValues(int** grid, cell** cellDetails, int** closedList, set* openList, int i, int j, Pair dest, int curr_i, int curr_j, int row_max, int col_max) {
    int foundDest = 0;
    //printf("calculating values for (%d,%d)\n", i, j);
    if (isValid(grid, i, j, row_max, col_max) == 1) {
        // If the destination cell is the same as the
        // current successor
        if (isDestination(i, j, dest) == 1) {
            // Set the Parent of the destination cell
            cellDetails[i][j].parent_i = curr_i;
            cellDetails[i][j].parent_j = curr_j;
            printf("The destination cell is found\n");
            tracePath(cellDetails, dest, row_max, col_max);
            foundDest = 1;
        }

        // If the successor is already on the closed
        // list or if it is blocked, then ignore it.
        // Else do the following
        else if (closedList[i][j] == 0) {
            double gNew = cellDetails[curr_i][curr_j].g + 1.0;
            double hNew = calculateHValue(i, j, dest, euclideanDistance);
            double fNew = gNew + hNew;

            // If it isn???t on the open list, add it to
            // the open list. Make the current square
            // the parent of this square. Record the
            // f, g, and h costs of the square cell
            //			 OR
            // If it is on the open list already, check
            // to see if this path to that square is
            // better, using 'f' cost as the measure.
            if (cellDetails[i][j].f == INT_MAX ||
                cellDetails[i][j].f > fNew) {

                // Update the details of this cell
                cellDetails[i][j].f = fNew;
                cellDetails[i][j].g = gNew;
                cellDetails[i][j].h = hNew;
                cellDetails[i][j].parent_i = curr_i;
                cellDetails[i][j].parent_j = curr_j;

                insert(openList, make_p_pair(fNew, make_pair(i, j)), cellDetails);
                printf("addedd %d-%d to openlist\n", i, j);
            }
        }
    }
    return foundDest;
}

void aStarSearch(int** grid, Pair src, Pair dest, int row_max, int col_max) {
    // If the source is out of range
    if (isValid(grid, src.first, src.second, row_max, col_max) == 0) {
        printf("Source is invalid\n");
        return;
    }

    // If the destination is out of range
    if (isValid(grid, dest.first, dest.second, row_max, col_max) == 0) {
        printf("Destination is invalid\n");
        return;
    }
    if (isDestination(src.first, src.second, dest) == 1) {
        printf("We are already at the destination\n");
        return;
    }

    int** closedList = (int**)malloc(row_max * sizeof(int*));
    // for each row allocate Cols ints
    int row;
    for (row = 0; row < row_max; row++) {
        closedList[row] = (int*)malloc(col_max * sizeof(int));
        memset(closedList[row], 0, col_max * sizeof(int));
    }
    // Declare a 2D array of structure to hold the details
    // of that cell
    cell** cellDetails = (cell**)malloc(row_max * sizeof(cell*));
    // for each row allocate Cols ints
    for (row = 0; row < row_max; row++) {
        cellDetails[row] = (cell*)malloc(col_max * sizeof(cell));
    }
    int i, j;

    for (i = 0; i < row_max; i++) {
        for (j = 0; j < col_max; j++) {
            cellDetails[i][j].f = INT_MAX;
            cellDetails[i][j].g = INT_MAX;
            cellDetails[i][j].h = INT_MAX;
            cellDetails[i][j].parent_i = -1;
            cellDetails[i][j].parent_j = -1;
            if (i == src.first && j == src.second) {
                cellDetails[i][j].f = 0.0;
                cellDetails[i][j].g = 0.0;
                cellDetails[i][j].h = 0.0;
                cellDetails[i][j].parent_i = i;
                cellDetails[i][j].parent_j = j;
            }
        }
    }

    set* openList = newSet(90);
    insert(openList, make_p_pair(0.0, make_pair(src.first, src.second)), cellDetails);
    // We set this boolean value as false as initially
    // the destination is not reached.
    int foundDest = 0;

    while (emptySet(openList) == 0) {
        printf("%d ", openList->itemsCount);
        pPair* p = openList->items[0];

        i = p->pair.first;
        j = p->pair.second;
        // Remove this vertex from the open list
        //openList.erase(openList.begin());
        // Add this vertex to the closed list
        closedList[i][j] = 1;
        int x, y;
        printf("calculate [%d,%d]\n", i, j);
        for (x = -1; x < 2; x++) {
            for (y = -1; y < 2; y++) {
                if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
                    if (calculateCellValues(grid, cellDetails, closedList, openList, i + x, j + y, dest, i, j, row_max, col_max)) {
                        foundDest = 1;
                        // for each row allocate Cols ints
                        for (row = 0; row < row_max; row++) {
                            free(cellDetails[row]);
                            free(closedList[row]);
                        }
                        free(cellDetails);
                        free(closedList);
                        deallocateSet(openList);
                        return;
                    }

                }
            }

        }
        RemoveItemInPosition(openList, 0);

    }
    for (row = 0; row < row_max; row++) {
        free(cellDetails[row]);
        free(closedList[row]);
    }
    free(cellDetails);
    free(closedList);
    deallocateSet(openList);
    if (foundDest == 0)
    {
        printf("Failed to find the Destination Cell\n");

        return;
    }
}



int** load_matrix(char* filepath, int* rows, int* columns, Pair* src, Pair* dst) {
    //char* buffer;
    //size_t bufsize = 32;
    //size_t characters;
    FILE* file = fopen(filepath, "r");
    /*buffer = (char *)malloc(bufsize * sizeof(char));
    if( buffer == NULL)
    {
        perror("Unable to allocate buffer");
        exit(1);
    }
    characters = getline(&buffer,&bufsize,file);
    rows=atoi(buffer);
    characters = getline(&buffer,&bufsize,file);
    columns=atoi(buffer);*/
    int read = fscanf(file, "dim=(%d,%d)\n", rows, columns);
    if (read == 0) {
        exit(1);
    }

    read = fscanf(file, "src=(%d,%d)\n", &src->first, &src->second);
    if (read == 0) {
        exit(1);
    }

    read = fscanf(file, "dst=(%d,%d)\n", &dst->first, &dst->second);
    if (read == 0) {
        exit(1);
    }
    //printf("rows: %d\ncolumns: %d\n",*rows,*columns);
    // allocate Rows rows, each row is a pointer to int
    int** board = (int**)malloc(*rows * sizeof(int*));
    if (board) {
        int row, col;
        // for each row allocate Cols ints
        for (row = 0; row < *rows; row++) {
            int* p = (int*)malloc(*columns * sizeof(int));
            if (p)
            {
                board[row] = p;
                for (col = 0; col < *columns; col++) {

                    fscanf(file, "%d,", &board[row][col]);
                    printf("putting %d in board[%d][%d]\n", board[row][col], row, col);

                }
            }
            else {
                return NULL;
            }
        }
    }
    else {
        return NULL;
    }
    fclose(file);
    return board;
}
int main(int argc, char const* argv[]) {
    /*MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);*/
    int test_id;
    for (test_id = 0; test_id < 1; test_id++) {
        char path[100] = "D:\\Archivio\\universita\\HighPerformanceComputing\\A-star-parallel\\inputs\\";
        char* buf = (char*)malloc(10);
        int rows, columns;
        if (buf != NULL) {
            sprintf(buf, "%d", 1);
            strcat(path, buf);
        }
        free(buf);
        strcat(path, ".txt");
        printf("loading %s", path);

        Pair src = make_pair(0, 0);

        Pair dst = make_pair(0, 0);
        int** grid = load_matrix(path, &rows, &columns, &src, &dst);
        /* Description of the Grid-
        1--> The cell is not blocked
        0--> The cell is blocked */
        int k;
        for (k = 0; k < 1; k++) {
            printf("calculating path from %d,%d to 0,0\n", src.first, src.second);
            fflush(stdout);
            int i, j;
            for (i = 0; i < rows; i++) {
                printf("|");
                for (j = 0; j < columns; j++) {
                    if (i == dst.first && j == dst.second) {
                        printf("D");
                    }
                    else if (i == src.first && j == src.second) {
                        printf("S");
                    }
                    else if (grid[i][j] == 1) {
                        printf("O");
                    }
                    else {
                        printf("X");
                    }
                }
                printf("|\n");
            }
            printf("\n");


            aStarSearch(grid, src, dst, rows, columns);
        }
        for (k = 0; k < rows; k++) {
            free(grid[k]);
        }
        free(grid);

    }
    //system("pause");  
    //MPI_Finalize();
    return (0);
}