#include <mpi.h>

#include <stdio.h>

#include <stdlib.h>

#include <math.h>

#include <limits.h>

#include <string.h>

int counter=0;

double startWorkers=0,totalTimeWorkers=0;
double startTotal=0,totalTime=0;
double startRoot=0,totalTimeRoot=0;
typedef struct {
    int x, y;
}
Position;
Position newPosition(int x, int y) {
    Position p;
    p.x = x;
    p.y = y;
    return p;
}
typedef struct Cell Cell;
struct Cell {
    Position position;
    Cell * parent;
    double f, g, h;
    int traversable; //0->non traversable, 1 traversable
    int visited;
};
Cell newCell(Position p, int traversable) {
    Cell c;
    c.position = p;
    c.parent = NULL;
    c.f = INT_MAX;
    c.g = INT_MAX;
    c.h = INT_MAX;
    c.traversable = traversable;
    c.visited = 0;
    return c;
}
void printCell(FILE* f,Cell * c) {
    if(f){
        if(c->parent)
            fprintf(f,"%d %p (%d,%d): f=%f, g=%f, h=%f, traversable=%d, visited=%d p=%p\n", counter, c,c -> position.x, c -> position.y, c -> f, c -> g, c -> h, c -> traversable, c -> visited,(c->parent));
        else
            fprintf(f,"%d %p (%d,%d): f=%f, g=%f, h=%f, traversable=%d, visited=%d p=NULL\n", counter, c, c -> position.x, c -> position.y, c -> f, c -> g, c -> h, c -> traversable, c -> visited);
    }else{
        if(c->parent)
            printf("%d %p (%d,%d): f=%f, g=%f, h=%f, traversable=%d, visited=%d p=%p\n", counter, c,c -> position.x, c -> position.y, c -> f, c -> g, c -> h, c -> traversable, c -> visited,(c->parent));
        else
            printf("%d %p (%d,%d): f=%f, g=%f, h=%f, traversable=%d, visited=%d p=NULL\n", counter, c, c -> position.x, c -> position.y, c -> f, c -> g, c -> h, c -> traversable, c -> visited);
    }  
}
Cell copyCell(Cell * cell) {
    Cell c = newCell(cell -> position, cell -> traversable);
    return c;

}

typedef struct {
    int top;
    int currentSize;
    int maxSize;
    Cell ** items;
}
Stack;
Stack newStack(int maxSize) {
    Stack s;
    s.maxSize = maxSize;
    s.top = -1;
    s.currentSize = 1;
    s.items = (Cell ** ) malloc(sizeof(Cell * ));
    return s;
}
// Utility function to return the size of the stack
int getStackSize(Stack * pt) {
    return pt -> top + 1;
}
// Utility function to check if the stack is full or not
int isStackFull(Stack * pt) {
    return pt -> top == pt -> currentSize - 1; // or return size(pt) == pt->maxsize;
}
// Utility function to check if the stack is empty or not
int isStackEmpty(Stack * pt) {
    return pt -> top == -1; // or return size(pt) == 0;
}
// Utility function to add an element `x` to the stack
void push(Stack * pt, Cell * c) {
    // check if the stack is already full. Then inserting an element would
    // lead to stack overflow
    if (isStackFull(pt)) {
        if (pt -> currentSize * 2 > pt -> maxSize) {
            printf("Overflow\nProgram Terminated\n");
            exit(EXIT_FAILURE);
        } else {
            Cell ** newptr = (Cell ** ) realloc(pt -> items, pt -> currentSize * sizeof(Cell * ) * 2);

            if (newptr) {
                //printf("Reallocating stack\n");
                pt -> items = newptr;
                pt -> currentSize *= 2;
            } else {
                printf("Unable to allocate stack space\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    //printf("Inserting %d\n", x);

    // add an element and increment the top's index
    pt -> items[++pt -> top] = c;
}
void emptyStack(Stack * s) {
    if (s != NULL) {
        //this doesn't free the items, only the container
        if (s -> items != NULL) {
            free(s -> items);
        }
        s -> items = (Cell ** ) malloc(sizeof(Cell * ));
        s -> currentSize = 1;
        s -> top = -1;
    }
}
// Utility function to return the top element of the stack
Cell * peek(Stack * pt) {
    // check for an empty stack
    if (!isStackEmpty(pt)) {
        return pt -> items[pt -> top];
    } else {
        return NULL;
    }
}
// Utility function to pop a top element from the stack
Cell * pop(Stack * pt) {
    // check for stack underflow
    if (isStackEmpty(pt)) {
        printf("Underflow\nProgram Terminated\n");
        return NULL;
    }

    //printf("Removing %d\n", peek(pt));

    // decrement stack size by 1 and (optionally) return the popped element
    return pt -> items[pt -> top--];
}
void printStack(Stack * s) {
    printf("maxsize=%d, top= %d, currentsize=%d\n", s -> maxSize, s -> top, s -> currentSize);
    if (!isStackEmpty(s)) {
        int i = 0;
        for (i = 0; i < getStackSize(s); i++) {
            printCell(NULL,s -> items[i]);
        }
    } else {
        printf("Stack is empty\n");
    }
}

typedef struct {
    Cell ** items;
    int itemsCount;
    int currentSize;
    int maxSize;
}
Set;
Set newSet(int capacity) {
    Set s;
    s.items = (Cell ** ) malloc(sizeof(Cell * ));
    s.itemsCount = 0;
    s.currentSize = 1;
    s.maxSize = capacity;
    return s;
}
void emptySet(Set * s) {
    if (s != NULL) {
        int i;
        for (i = 0; i < s -> currentSize; i++) {
            free(s -> items[i]);
        }
        free(s -> items);
        s -> items = (Cell ** ) malloc(sizeof(Cell * ));
        s -> currentSize = 1;
        s -> itemsCount = 0;
    }
}
int isSetFull(Set * s) {
    return s -> itemsCount >= s -> currentSize;
}
int isSetEmpty(Set * s) {
    return s -> itemsCount == 0;
}
void insert(Set * s, Cell * c) {
    if (isSetFull(s)) {
        if (s -> currentSize * 2 > s -> maxSize) {
            printf("Overflow\nProgram Terminated\n");
            exit(EXIT_FAILURE);
        } else {
            Cell ** newptr = (Cell ** ) realloc(s -> items, s -> currentSize * sizeof(Cell * ) * 2);
            if (newptr) {
                //printf("Reallocating set\n");
                s -> items = newptr;
                s -> currentSize *= 2;
            } else {
                printf("Unable to allocate stack space\n");
                exit(EXIT_FAILURE);
            }
        }

    }

    int insertPos = 0;
    while (insertPos < s -> itemsCount && s -> items[insertPos] -> f <= c -> f) {
        insertPos++;
    }
    int i;
    for (i = s -> itemsCount; i > insertPos; i--) {
        s -> items[i] = s -> items[i - 1];
    }
    s -> items[insertPos] = c;
    s -> itemsCount++;
}
int removeItemInPosition(Set * s, int position) {
    if (position >= s -> itemsCount) {
        return 0;
    } else if (s -> itemsCount > 1) {
        int i;
        for (i = position; i < s -> itemsCount - 1; i++) {
            s -> items[i] = s -> items[i + 1];
        }
    }
    s -> itemsCount--;
    s -> items[s -> itemsCount] = NULL;
    return 1;
}
Cell * getItemInPosition(Set * s, int position) {
    if (position >= s -> itemsCount) {
        return NULL;
    } else {
        return s -> items[position];
    }
}
void printSet(Set * s) {
    printf("%d itemscount=%d, currentsize= %d, maxsize=%d\n",counter, s -> itemsCount, s -> currentSize, s -> maxSize);
    if (!isSetEmpty(s)) {
        int i = 0;
        for (i = 0; i < s -> itemsCount; i++) {
            printCell(NULL,s -> items[i]);
        }
    } else {
        printf("Set is empty\n");
    }
}

typedef struct {
    Cell ** grid;
    int rows, cols;
    Cell * globalSrc;
    Cell * globalDest;
}
Grid;

void emptyGrid(Grid * g) {
    if (g -> grid) {
        int i;
        for (i = 0; i < g -> rows; i++) {
            free(g -> grid[i]);
        }
        free(g -> grid);
    }
    g -> grid = NULL;
    g -> globalSrc = NULL;
    g -> globalDest = NULL;
}
void printGrid(Grid * g) {
    printf("rows=%d, columns= %d, src=(%d,%d), dst=(%d,%d)\n", g -> rows, g -> cols, g -> globalSrc -> position.x, g -> globalSrc -> position.y, g -> globalDest -> position.x, g -> globalDest -> position.y);
    if (g -> grid) {
        int r, c;
        for (r = 0; r < g -> rows; r++) {
            for (c = 0; c < g -> cols; c++) {
                printCell(NULL, & (g -> grid[r][c]));
            }
        }
    } else {
        printf("Grid is empty\n");
    }
}

typedef struct {
    Cell * gravity;
    Cell ** * paths;
    Cell ** grid;
    int rank;
    int maxPathsNumber;
    int pathsCount;
    int rows, cols;
    int active;
    Cell ** startsList;
}
LocalGrid;
LocalGrid newLocalGrid(Cell ** grid, int rank, int rows, int cols, int active) {
    LocalGrid lg;
    lg.gravity = NULL;
    lg.rank = rank;
    lg.maxPathsNumber = 300000;
    lg.pathsCount = 0;
    lg.rows = rows;
    lg.cols = cols;
    lg.paths = (Cell ** * ) malloc(sizeof(Cell ** ) * lg.maxPathsNumber);
    lg.startsList = (Cell ** ) malloc(sizeof(Cell * ) * lg.maxPathsNumber);
    lg.grid = grid;
    lg.active = active;
    //printf("Allocating %d at %p, paths at %p\n", lg.rank, &(lg.grid[0]),lg.paths);
    return lg;
}
void emptyLocalGrid(LocalGrid * lg) {
    //printf("calling empty method on %p\n", &(lg->grid[0]));
    int i, j;
    if (lg -> grid) {
        for (i = 0; i < lg -> rows; i++) {
            free(lg -> grid[i]);
        }
        free(lg -> grid);
    }
    if (lg -> paths) {
        for (i = 0; i < lg -> pathsCount; i++) {
            for (j = 0; j < lg -> rows; j++) {
                free(lg -> paths[i][j]);
            }
            //printf("removing local path in %p\n", &(lg->paths[i][0]));
            free(lg -> paths[i]);
            //printf("removing local path in %p\n", &(lg->paths[i]));
        }

    }
    free(lg -> paths);
    free(lg -> startsList);
    lg -> grid = NULL;
    lg -> gravity = NULL;
    lg -> paths = NULL;
}
LocalGrid copyGridInit(LocalGrid localGrid) {

    int r, c;
    Cell * grav = NULL;
    Cell ** grid = (Cell ** ) malloc(sizeof(Cell * ) * localGrid.rows);
    for (r = 0; r < localGrid.rows; r++) {
        grid[r] = (Cell * ) malloc(sizeof(Cell) * localGrid.cols);
        for (c = 0; c < localGrid.cols; c++) {
            Cell newCell = copyCell( & (localGrid.grid[r][c]));
            if (localGrid.gravity && localGrid.gravity == & (localGrid.grid[r][c]))
                grav = & grid[r][c];
            grid[r][c] = newCell;
        }
    }
    LocalGrid lg = newLocalGrid(grid, localGrid.rank, localGrid.rows, localGrid.cols, localGrid.active);
    lg.gravity = grav;
    /*for (r = 0; r < localGrid.rows; r++) {
    	for (c = 0; c < localGrid.cols; c++) {
    		Cell newCell = copyCell(&(localGrid.grid[r][c]));
    		lg.grid[r][c] = newCell;
    	}
    }*/
    return lg;
}
void printLocalGrid(LocalGrid * lg) {
    printf("rank=%d\n", lg -> rank);
    if (lg -> grid) {
        int r, c;
        for (r = 0; r < lg -> rows; r++) {
            for (c = 0; c < lg -> cols; c++) {
                printCell(NULL, & (lg -> grid[r][c]));
            }
        }
    } else {
        printf("Grid is empty\n");
    }
}

Grid createGrid(const char * filepath) {
    Grid g;
    FILE * file = fopen(filepath, "r");
    Position src, dst;
    g.globalDest = NULL;
    g.globalSrc = NULL;
    g.grid = NULL;

    if (file) {
        int read = fscanf(file, "dim=(%d,%d)\n", & g.rows, & g.cols);
        if (read == 0) {
            printf("File format exception\n");
            exit(1);
        }

        read = fscanf(file, "src=(%d,%d)\n", & src.x, & src.y);
        if (read == 0) {
            printf("File format exception\n");
            exit(2);
        }

        read = fscanf(file, "dst=(%d,%d)\n", & dst.x, & dst.y);
        if (read == 0) {
            printf("File format exception\n");
            exit(3);
        }

        g.grid = (Cell ** ) malloc(sizeof(Cell * ) * g.rows);
        if (g.grid) {
            int r, c;
            for (r = 0; r < g.rows; r++) {
                g.grid[r] = (Cell * ) malloc(sizeof(Cell) * g.cols);
                if (g.grid[r]) {
                    for (c = 0; c < g.cols; c++) {
                        int sup;
                        read = fscanf(file, "%d,", & sup);
                        if (read) {
                            g.grid[r][c] = newCell(newPosition(r, c), sup);
                            if (!g.globalSrc && r == src.x && c == src.y) {
                                g.globalSrc = & g.grid[r][c];
                                g.globalSrc -> f = 0;
                                g.globalSrc -> g = 0;
                                g.globalSrc -> h = 0;
                            } else if (!g.globalDest && r == dst.x && c == dst.y) {
                                g.globalDest = & g.grid[r][c];
                            }
                        } else {
                            printf("Can't read from file\n");
                            exit(4);
                        }
                    }
                } else {
                    printf("Can't allocate row\n");
                    exit(5);
                }
            }
            if (!g.grid || !g.globalSrc || !g.globalDest) {
                printf("Source or destination not set\n");
                exit(5);
            }
        } else {
            printf("Can'tallocate grid\n");
            exit(6);
        }
    } else {
        printf("Can't open file  %s\n", filepath);
        exit(7);
    }
    return g;
}

int isValidInLG(LocalGrid * lg, int r, int c) {
    // Returns true if row number and column number
    // is in range
    return (r >= 0) && (r < lg -> rows) && (c >= 0) && (c < lg -> cols) && lg -> grid[r][c].traversable;
}
int isValidInG(Grid * g, int r, int c) {
    // Returns true if row number and column number
    // is in range
    return (r >= 0) && (r < g -> rows) && (c >= 0) && (c < g -> cols) && g -> grid[r][c].traversable;
}
int isDestination(Cell * c, Position goal) {
    return (c -> position.x == goal.x && c -> position.y == goal.y);
}
double calculateHValue(Position * pos1, Position * pos2, double( * func)(Position * , Position * )) {
    // Return using the distance formula
    return func(pos1, pos2);
}
double euclideanDistance(Position * pos1, Position * pos2) {
    return ((double) sqrt((pos1 -> x - pos2 -> x) * (pos1 -> x - pos2 -> x) + (pos1 -> y - pos2 -> y) * (pos1 -> y - pos2 -> y)));
}
double manhattanDistance(Position * pos1, Position * pos2) {
    return abs(pos1 -> x - pos2 -> x) + abs(pos1 -> y - pos2 -> y);
}
int calculateCellValues(Cell * currentCell, Cell * newCell, Set * openList, Position goal) {
    int foundDest = 0;
    if (isDestination(currentCell, goal)) {
        //newCell -> parent = currentCell;
        foundDest = 1;
    }
    if (!newCell -> visited) {
        double gnew = currentCell -> g + 1.0;
        double hnew = calculateHValue( & (newCell -> position), & goal, euclideanDistance);
        double fnew = gnew + hnew;
        if (newCell -> f > fnew) {
            newCell -> g = gnew;
            newCell -> h = hnew;
            newCell -> f = fnew;
            newCell -> parent = currentCell;
            //printf("adding (%d%d) to openlist\n", newCell->position.x, newCell->position.y);
            insert(openList, newCell);
        }

    }
    //printf("Addedd new cell into openList (%d,%d) (->parent(%d,%d) \n", newCell -> position.x, newCell -> position.y, newCell -> parent -> position.x, newCell -> parent -> position.y);
    return foundDest;
}
Cell * calculateClosestCell(LocalGrid * lg, Position goal, Cell ** grid) {
    /*Position gridStart = lg->grid[0][0].position;
    int posx=0, posy=0;

    if (goal.x >= gridStart.x + lg->cols) {
    	posx = 4;
    }
    else if (goal.x > gridStart.x) {
    	posx = goal.x % lg->cols;
    }

    if (goal.y >= gridStart.y + lg->rows) {
    	posy = 4;
    }
    else if (goal.y > gridStart.y) {
    	posy = goal.y%lg->rows;
    }
    //this doesn't work when the closest cell is not traversable
    while (!lg->grid[posx][posy].traversable) {
    	if (abs(goal.y - posy) > abs(goal.x - posx) && posy>0 && posy<lg->rows) {
    		posy += (int)((goal.y > posy) - 0.5) * 2;
    	}
    	else if (posx > 0 && posx < lg->cols) {
    		posx += (int)((goal.x > posx) - 0.5) * 2;
    	}
    	else { return NULL; }
    }*/

    //dumb version works like this, implement version with set, visiting neighbors
    Cell * cell = NULL;
    double distance = INT_MAX;
    int r, c;
    for (r = 0; r < lg -> rows; r++) {
        for (c = 0; c < lg -> cols; c++) {
            if (grid[r][c].traversable) {
                double newdistance = calculateHValue( & (grid[r][c].position), & goal, euclideanDistance);
                if (newdistance < distance) {
                    distance = newdistance;
                    cell = & (grid[r][c]);
                }
            }
        }
    }
    return cell;
}
void SendPosition(Position p, int receiver_rank) {
    int sup[2] = {
        p.x,
        p.y
    };
    MPI_Send(sup, 2, MPI_INT, receiver_rank, 0, MPI_COMM_WORLD);//TODO:use Isend?
}

Position ReceivePosition(int sender_rank) {
    int sup[2];
    if (sender_rank == -1) {
        MPI_Recv(sup, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {

        MPI_Recv(sup, 2, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    Position p;
    p.x = sup[0];
    p.y = sup[1];
    return p;
}
void SendGrid(Cell ** grid, int receiver_rank, int rows_per_block, int cols_per_block) {
    Cell * flattenedPath = NULL;
    int size = 0;
    int * parents;
    if (grid) {
        int r, c;
        size = sizeof(Cell) * (rows_per_block * cols_per_block);
        MPI_Request request;
        MPI_Isend( & size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,&request);
        flattenedPath = (Cell * ) malloc(size);
        parents = (int * ) malloc(sizeof(int) * (rows_per_block * cols_per_block * 2));
        for (r = 0; r < rows_per_block; r++) {
            for (c = 0; c < cols_per_block; c++) {
                flattenedPath[(r) * rows_per_block + c] = grid[r][c];
                int px = -1, py = -1;
                if (grid[r][c].parent) {
                    px = grid[r][c].parent -> position.x;
                    py = grid[r][c].parent -> position.y;
                }
                parents[((r) * rows_per_block + c) * 2] = px;
                parents[((r) * rows_per_block + c) * 2 + 1] = py;
                //printf("parent of (%d,%d) is in (%d,%d), [%d,%d]", grid[r][c].position.x, grid[r][c].position.y, px, py, ((r) * rows_per_block + c) * 2, ((r) * rows_per_block + c) * 2 + 1);
                //if (grid[r][c].parent)
                    //printf("(%d,%d)\n", grid[r][c].parent -> position.x, grid[r][c].parent -> position.y);
                //else
                   // printf("\n");
            }
        }
        MPI_Wait(&request, MPI_STATUS_IGNORE);
        MPI_Send(parents, rows_per_block * cols_per_block * 2, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(flattenedPath, size, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    }else{
        
    MPI_Send( & size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    
    //if(grid)
       // printf("(%d,%d) sending grid to root, length:%d\n",grid[0][0].position.x,grid[0][0].position.y, (int)(size/sizeof(Cell*)));
}
Cell ** ReceiveGrid(int sender_rank, int rows_per_block, int cols_per_block) {
    int size;
    Cell ** grid = NULL;
    if (sender_rank == -1) {
        MPI_Recv( & size, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } else {

        MPI_Recv( & size, 1, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    totalTimeWorkers+=MPI_Wtime()-startWorkers;
    if (size) {
        Cell * flattenedGrid = (Cell * ) malloc(size);
        int * parents = (int * ) malloc(sizeof(int) * (rows_per_block * cols_per_block * 2));
        if (sender_rank == -1) {
            MPI_Recv(parents, rows_per_block * cols_per_block * 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(flattenedGrid, size, MPI_BYTE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(parents, rows_per_block * cols_per_block * 2, MPI_INT, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv(flattenedGrid, size, MPI_BYTE, sender_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        grid = (Cell ** ) malloc(sizeof(Cell * ) * rows_per_block);
        int r, c;
        for (r = 0; r < rows_per_block; r++) {
            grid[r] = (Cell * ) malloc(sizeof(Cell) * cols_per_block);
            for (c = 0; c < cols_per_block; c++) {
                grid[r][c] = flattenedGrid[r * rows_per_block + c];
            }
        }
        for (r = 0; r < rows_per_block; r++) {
            for (c = 0; c < cols_per_block; c++) {
                if (parents[(r * rows_per_block + c) * 2] != -1) {
                    //printf("root: (%d,%d) has parent in (%d,%d)\n", grid[r][c].position.x, grid[r][c].position.y, parents[(r * rows_per_block + c) * 2], parents[((r * rows_per_block + c) * 2 + 1)]);
                    grid[r][c].parent = & (grid[parents[((r * rows_per_block + c) * 2)] % rows_per_block][parents[((r * rows_per_block + c) * 2 + 1)] % cols_per_block]);
                } else {
                    //printf("root: (%d,%d) has no parent\n", grid[r][c].position.x, grid[r][c].position.y);
                    grid[r][c].parent = NULL;
                }
            }
        }
    }
    return grid;
}
void notifyWorkers(Position cellpos, int process_number) {
    int worker_id;
    for (worker_id = 1; worker_id < process_number + 1; worker_id++) {
        SendPosition(cellpos, worker_id);
        //printf("%d pos (%d,%d) sent to %d\n",counter++,cellpos.x,cellpos.y,worker_id);
    }
    startWorkers=MPI_Wtime();
    
}

void printToFile(Cell* dest,const char outpath[]){
    
    FILE * file = fopen(outpath, "w");
    fprintf(file,"root time %f\n", totalTimeRoot);
    fprintf(file,"workers time %f\n", totalTimeWorkers);
    fprintf(file,"total time %f\n", totalTime);
    while (dest -> parent != dest) {
        printCell(file,dest);
        dest = dest -> parent;
    }
    printCell(file,dest);

}
void controllerSearch(Grid g, int process_number,const char outpath[]) {
    if (isDestination(g.globalSrc, g.globalDest -> position)) {
        printf("We are already at the destination\n");
        return;
    }
    startRoot=MPI_Wtime();
    Set openList = newSet(g.rows * g.cols*50);
    //Set closedList = newSet(g.rows * g.cols*50);
    insert( & openList, g.globalSrc);
    int found = 0;
    int rows_per_block = g.rows / sqrt(process_number);
    int cols_per_block = g.cols / sqrt(process_number);
    while (!isSetEmpty( & openList)) {
        Cell * firstElement = getItemInPosition( & openList, 0);
        removeItemInPosition( & openList, 0);
        int i = firstElement -> position.x;
        int j = firstElement -> position.y;
        if(!firstElement->visited){
            firstElement -> visited = 1;
            totalTimeRoot+=MPI_Wtime()-startRoot;
            notifyWorkers(newPosition(i, j), process_number);
           // printf("%d analyzing %d,%d\n",counter,i,j);
           // printCell(&(g.grid[i][j]));
            Cell ** path = ReceiveGrid(-1, rows_per_block, cols_per_block);
            if (path) {
                //printf("%d analyzing grid\n",counter);
                ///printf("%d root: received grid starting in (%d,%d)\n", counter++ ,path[0][0].position.x, path[0][0].position.y);
                int r, c;
                for (r = 0; r < rows_per_block; r++) {
                    for (c = 0; c < cols_per_block; c++) {
                        //printf("analyzing\n");
                        Cell * localCell = & (path[r][c]);
                        //printCell(localCell);
                        Cell * globalCell = & (g.grid[localCell -> position.x][localCell -> position.y]);
                        if (localCell -> traversable) {
                            
                            //printf("qui %f\n",localCell->g);
                            double newG=localCell->g+firstElement->g;
                            //printf("qua %f\n",newG);
                            if(newG < globalCell->g){
                                globalCell->g=newG;
                                globalCell->h=calculateHValue(&(globalCell->position),&(g.globalDest->position),euclideanDistance);
                                globalCell->f=globalCell->g+globalCell->h;
                                globalCell->visited=globalCell->visited||localCell->visited;
                                globalCell->parent=&(g.grid[localCell->parent->position.x][localCell->parent->position.y]);
                                if(globalCell->parent==globalCell){
                                    globalCell->parent=firstElement;
                                }
                            }
                            
                            startRoot=MPI_Wtime();
                            if ((globalCell -> visited && r == 0) && isValidInG( & g, globalCell -> position.x - 1, globalCell -> position.y)) 
                            {
                                Cell * neighbor = & (g.grid[globalCell -> position.x - 1][globalCell -> position.y]);
                                if (neighbor->g> globalCell->g+1) {
                                    neighbor -> g = globalCell -> g + 1;
                                    neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                    neighbor -> f = neighbor -> g + neighbor -> h;
                                    if(!neighbor -> parent){
                                        //printf("%d a added (%d,%d) to openList\n",counter,neighbor->position.x,neighbor->position.y);
                                        insert( & openList, neighbor);
                                    }
                                    neighbor -> parent = globalCell;
                                }
                            }
                            else if ((globalCell -> visited && r == rows_per_block - 1) && isValidInG( & g, globalCell -> position.x + 1, globalCell -> position.y)) {
                                Cell * neighbor = & (g.grid[globalCell -> position.x + 1][globalCell -> position.y]);
                                if (neighbor->g> globalCell->g+1) {
                                        neighbor -> g = globalCell -> g + 1;
                                        neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                        neighbor -> f = neighbor -> g + neighbor -> h;
                                        if(!neighbor -> parent){
                                            //printf("%d b added (%d,%d) to openList\n",counter,neighbor->position.x,neighbor->position.y);
                                            insert( & openList, neighbor);
                                        }
                                        neighbor -> parent = globalCell;
                                } 
                            }

                            if ((globalCell -> visited && c == 0)&& isValidInG( & g, globalCell -> position.x, globalCell -> position.y - 1)) {
                                Cell * neighbor = & (g.grid[globalCell -> position.x][globalCell -> position.y - 1]);
                                if (neighbor->g> globalCell->g+1) {
                                    neighbor -> g = globalCell -> g + 1;
                                    neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                    neighbor -> f = neighbor -> g + neighbor -> h;
                                    if(!neighbor -> parent){
                                       // printf("%d c added (%d,%d) to openList\n",counter,neighbor->position.x,neighbor->position.y);
                                        insert( & openList, neighbor);
                                    }
                                    neighbor -> parent = globalCell;
                                }
                            } 
                            else if ((globalCell -> visited && c == cols_per_block - 1) && isValidInG( & g, globalCell -> position.x, globalCell -> position.y + 1)) {
                                Cell * neighbor = & (g.grid[globalCell -> position.x][globalCell -> position.y + 1]);
                                if (neighbor->g> globalCell->g+1) {
                                    neighbor -> g = globalCell -> g + 1;
                                    neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                    neighbor -> f = neighbor -> g + neighbor -> h;
                                    if(!neighbor -> parent){
                                      //  printf("%d d added (%d,%d) to openList\n",counter,neighbor->position.x,neighbor->position.y);
                                        insert( & openList, neighbor);
                                    }
                                    neighbor -> parent = globalCell;
                                }
                            }                                
                            if (globalCell -> parent && !globalCell -> visited && !localCell->visited) {
                               // printf("%d e added (%d,%d) to openList\n",counter,globalCell->position.x,globalCell->position.y);
                                insert( & openList, globalCell);
                            }
                            if (globalCell == g.globalDest) {
                                found = 1;
                            }
                           // printCell(globalCell);
                        }
                        totalTimeRoot+=MPI_Wtime()-startRoot;
                        //printf("end \n");
                    }
                }
                /*if (localGravity) {
                    int x, y;
                    int lastCell_i = localGravity -> position.x;
                    int lastCell_j = localGravity -> position.y;
                    for (x = -1; x < 2; x++) {
                        for (y = -1; y < 2; y++) {
                            if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
                                Cell * neighbor = & (g.grid[lastCell_i + x][lastCell_j + y]);
                                if (isValidInG( & g, lastCell_i + x, lastCell_j + y) && !neighbor -> parent) {
                                    if (!neighbor -> visited) {
                                        neighbor -> g = g.grid[lastCell_i][lastCell_j].g + 1;
                                        neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                        neighbor -> f = neighbor -> g + neighbor -> h;
                                        if (!neighbor -> parent || (neighbor -> parent && neighbor -> parent -> g > g.grid[lastCell_i][lastCell_j].g)) {
                                            neighbor -> parent = & (g.grid[lastCell_i][lastCell_j]);
                                        }
                                        insert( & openList, neighbor);
                                        //printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
                                    }
                                }
                            }
                        }
                    }

                    lastCell_i = firstElement -> position.x;
                    lastCell_j = firstElement -> position.y;
                    for (x = -1; x < 2; x++) {
                        for (y = -1; y < 2; y++) {
                            if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
                                Cell * neighbor = & (g.grid[lastCell_i + x][lastCell_j + y]);
                                if (isValidInG( & g, lastCell_i + x, lastCell_j + y) && !neighbor -> parent) {
                                    if (!neighbor -> visited) {
                                        neighbor -> g = firstElement -> g + 1;
                                        neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                        neighbor -> f = neighbor -> g + neighbor -> h;
                                        if (!neighbor -> parent || (neighbor -> parent && neighbor -> parent -> g > firstElement -> g)) {
                                            neighbor -> parent = firstElement;
                                        }
                                        insert( & openList, neighbor);
                                        //printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
                                    }
                                }
                            }
                        }
                    }
                }*/
            } else {
                startRoot=MPI_Wtime();
                int x, y;
                int lastCell_i = firstElement -> position.x;
                int lastCell_j = firstElement -> position.y;
                for (x = -1; x < 2; x++) {
                    for (y = -1; y < 2; y++) {
                        if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
                            Cell * neighbor = & (g.grid[lastCell_i + x][lastCell_j + y]);
                            if (isValidInG( & g, lastCell_i + x, lastCell_j + y)) {
                                if (neighbor->g> firstElement->g+1) {
                                    neighbor -> g = firstElement -> g + 1;
                                    neighbor -> h = calculateHValue( & (neighbor -> position), & (g.globalDest -> position), euclideanDistance);
                                    neighbor -> f = neighbor -> g + neighbor -> h;
                                    if (!neighbor -> parent /*|| (neighbor -> parent && neighbor -> parent -> g > firstElement -> g)*/) {
                                        //printf("%d f added (%d,%d) to openList\n",counter,neighbor->position.x,neighbor->position.y);
                                        insert( & openList, neighbor);
                                    }
                                    neighbor -> parent = firstElement;
                                    //printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
                                }
                                ///printf("%d   eneighbor of (%d,%d) -> (%d,%d) f=%f g=%f h=%f\n",counter++,firstElement->position.x,firstElement->position.y,neighbor->position.x,neighbor->position.y, neighbor->f,neighbor->g, neighbor->h);
                                ///if(neighbor->parent)
                                //  printf("%d   (%d,%d) has parent (%d,%d)\n",counter++,neighbor->position.x,neighbor->position.y,neighbor->parent->position.x,neighbor->parent->position.y);
                            
                            }
                            
                        }
                    }
                }
                totalTimeRoot+=MPI_Wtime()-startRoot;
            }
           // printf("%d openlist:\n",counter);
          //  printSet(&openList);
          //  printf("\n");
          //  printf("%d closedlist:\n",counter);
          //  printSet(&closedList);
          //  printf("\n");
            counter++;
            if (found) {
                totalTime=MPI_Wtime()-startTotal;
                Cell * cell = g.globalDest;
                printToFile(cell,outpath);
                //printGridContent(g.grid, 30, 30);
                printf("in fondo\n");
                
                return;
            }
        }
    }
    return;
}
Cell ** calculateLocalAStar(LocalGrid lg, Cell * localSrc, Position globalDest) {
    int i;
    Cell ** path = (Cell ** ) malloc(sizeof(Cell * ) * lg.rows);
    for (i = 0; i < lg.rows; i++) {
        path[i] = (Cell * ) malloc(sizeof(Cell) * lg.cols);
        memcpy(path[i], lg.grid[i], sizeof(Cell) * lg.cols);
    }
    localSrc = & path[localSrc -> position.x % lg.rows][localSrc -> position.y % lg.cols];
    Cell * localDest = & (path[lg.gravity -> position.x % lg.rows][lg.gravity -> position.y % lg.cols]);
    if (localDest && isDestination(localSrc, localDest -> position)) {
        int i;
        for (i = 0; i < lg.rows; i++) {
            free(path[i]);
        }
        free(path);
        return NULL;
    }
    localSrc -> f = 0;
    localSrc -> g = 0;
    localSrc -> h = 0;

    localSrc -> parent = localSrc;
    Set openList = newSet(lg.cols * lg.rows);

    insert( & openList, localSrc);


    while ((!isSetEmpty( & openList))) {
        Cell * firstElement = getItemInPosition( & openList, 0);
        int i = firstElement -> position.x % lg.rows;
        int j = firstElement -> position.y % lg.cols;
        firstElement -> visited = 1;
        int x, y;
        int found=0;
        for (x = -1; x < 2; x++) {
            for (y = -1; y < 2; y++) {
                if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {

                    if (isValidInLG( & lg, i + x, j + y)) {

                        if (localDest) {
                            found=calculateCellValues( & (path[i][j]), & (path[i + x][j + y]), & openList, localDest -> position);
                        }

                    }

                }
            }

        }
        if(found){
            Cell * cell = firstElement;
            cell -> visited = 1;
            while (cell -> parent != cell) {
                //printCell(cell);
                cell = cell -> parent;
            }
            return path;
        }
        removeItemInPosition( & openList, 0);
    }
    for (i = 0; i < lg.rows; i++) {
        free(path[i]);
    }
    free(path);
    return NULL;
}

int main(int argc, char const * argv[]) {
    MPI_Init(NULL, NULL);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, & world_size);
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, & world_rank);
    if (argc == 4) {
        const char * path = argv[1];
        const char * outpath = argv[3];
        char * endpoint;
        int processNumber = strtol(argv[2], & endpoint, 10);
        int rows_per_block, cols_per_block;
        Cell * localGridCells = NULL;
        Cell * rcvGridCells = NULL;
        int globalDestCoord[2];
        if (world_rank == 0) {
            printf("Loading %s\n", path);
            printf("Number of processes to create %d\n", processNumber);
            Grid g = createGrid(path);
            globalDestCoord[0] = g.globalDest -> position.x;
            globalDestCoord[1] = g.globalDest -> position.y;
            //split grid 
            int i, j, k;
            rows_per_block = g.rows / sqrt(processNumber);
            cols_per_block = g.cols / sqrt(processNumber);
            int offset = rows_per_block * cols_per_block;
            localGridCells = (Cell * ) malloc(sizeof(Cell) * (g.rows * g.cols + offset));
            for (k = 0; k < processNumber; k++) {
                int start_index_x = (rows_per_block) * (k / (int) sqrt(processNumber));
                int start_index_y = (cols_per_block) * (k % (int) sqrt(processNumber));
                for (i = 0; i < rows_per_block; i++) {
                    for (j = 0; j < cols_per_block; j++) {
                        localGridCells[offset + k * (rows_per_block * cols_per_block) + (i) * rows_per_block + j] = g.grid[start_index_x + i][start_index_y + j];
                        //printf("Adding cell (%d,%d) in %d\n", start_index_x + i, start_index_y + j, k * (rows_per_block * cols_per_block) + (i) * rows_per_block + j);
                    }
                }

                MPI_Send(globalDestCoord, 2, MPI_INT, k + 1, 0, MPI_COMM_WORLD);
                MPI_Send( & rows_per_block, 1, MPI_INT, k + 1, 0, MPI_COMM_WORLD);
                MPI_Send( & cols_per_block, 1, MPI_INT, k + 1, 0, MPI_COMM_WORLD);
            }
            rcvGridCells = (Cell * ) malloc(sizeof(Cell) * rows_per_block * cols_per_block);

            if (MPI_Scatter(localGridCells, (rows_per_block * cols_per_block) * sizeof(Cell), MPI_BYTE,
                    rcvGridCells, (rows_per_block * cols_per_block) * sizeof(Cell), MPI_BYTE,
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {

                printf("Scatter error\n");
                exit(1);
            }
            MPI_Barrier(MPI_COMM_WORLD);
            free(rcvGridCells);
            free(localGridCells);
            g.globalSrc -> parent = g.globalSrc;
            startTotal=MPI_Wtime();
            controllerSearch(g, processNumber,outpath);
            notifyWorkers(newPosition(-1,-1),processNumber);
            printf("waiting for all processes to finish\n");
            fflush(NULL);
        } else {
            MPI_Recv(globalDestCoord, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv( & rows_per_block, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Recv( & cols_per_block, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            rcvGridCells = (Cell * ) malloc(sizeof(Cell) * rows_per_block * cols_per_block);

            if (MPI_Scatter(localGridCells, (rows_per_block * cols_per_block) * sizeof(Cell), MPI_BYTE,
                    rcvGridCells, (rows_per_block * cols_per_block) * sizeof(Cell), MPI_BYTE,
                    0, MPI_COMM_WORLD) != MPI_SUCCESS) {

                printf("Scatter error\n");
                exit(1);
            }
          //  printf("%d allocated %d bytes\n", world_rank, rows_per_block * cols_per_block);
            //printf("%d = %d\n", world_rank, rcvGridCells[0].position.x);
            Cell ** localGridCells = (Cell ** ) malloc(sizeof(Cell * ) * rows_per_block);
            int r, c;
            for (r = 0; r < rows_per_block; r++) {
                localGridCells[r] = (Cell * ) malloc(sizeof(Cell) * cols_per_block);
                for (c = 0; c < cols_per_block; c++) {
                    localGridCells[r][c] = rcvGridCells[r * rows_per_block + c];
                }
            }
            MPI_Barrier(MPI_COMM_WORLD);

            LocalGrid lg = newLocalGrid(localGridCells, world_rank - 1, rows_per_block, cols_per_block, 1);
            Position globalDestPos;
            globalDestPos.x = globalDestCoord[0];
            globalDestPos.y = globalDestCoord[1];

            //printf("%d: global dest received (%d,%d)\n",world_rank,globalDestPos.x,globalDestPos.y);
            lg.gravity = calculateClosestCell( & lg, globalDestPos, lg.grid);
            //printf("%d: Gravity calculated to be in (%d,%d)\n", lg.rank, lg.gravity -> position.x, lg.gravity -> position.y);
            //printf("%d %p\n",lg.maxPathsNumber,&(lg.grid[0][0]));
            Position cellpos= ReceivePosition(0);
			do{
				
				int path_c, found = -1;
				for (path_c = 0; path_c < lg.pathsCount; path_c++) {
					if (lg.startsList[path_c] -> position.x % lg.rows == cellpos.x % lg.rows && lg.startsList[path_c] -> position.y % lg.cols == cellpos.y % lg.cols) {
						found = path_c;
					}
				}
                int assigned_worker = (cellpos.x / (lg.rows) * ((int) sqrt(processNumber))) + (cellpos.y / lg.cols); //first part is simplified from i/(g.rows/(int)sqrt(process_number))* ((int)sqrt(process_number))

					
				if (found < 0) {
					LocalGrid lg_copy = copyGridInit(lg);

					Cell * localSrc = calculateClosestCell( & lg_copy, cellpos, lg_copy.grid);
					//printf("%d: startpos=(%d,%d) localSrc=%p gridCell=%p\n", lg.rank, localSrc -> position.x, localSrc -> position.y, & localSrc, & lg_copy.grid[localSrc -> position.x][localSrc -> position.y]);
					Cell ** gridState = calculateLocalAStar(lg_copy, localSrc, lg.gravity -> position);
					//printf("%d: received starting point (%d,%d), not present in grid, creating copy (gravity:(%d,%d))\n", lg.rank, cellpos.x, cellpos.y, lg_copy.gravity -> position.x, lg_copy.gravity -> position.y);
					//emptyLocalGrid(&lg);
					if (gridState) {
						if (lg.pathsCount >= lg.maxPathsNumber) {
							printf("maxpathcount reached\n");
							exit(9);
						}
						found = lg.pathsCount;
						lg.startsList[lg.pathsCount] = & (gridState[localSrc -> position.x % lg.rows][localSrc -> position.y % lg.cols]);
						lg.paths[lg.pathsCount++] = gridState;
						//printf("%d: first path starting from (%d,%d)\n", lg.rank, lg.startsList[lg.pathsCount - 1] -> position.x, lg.startsList[lg.pathsCount - 1] -> position.y);

					}
					//printf("rank %d: assigned worker: %d, sending grid from (%d,%d)\n", lg.rank, assigned_worker, lg.grid[0][0].position.x, lg.grid[0][0].position.y);
					if (assigned_worker == lg.rank) {
						if (lg.active && found>=0) {
							Cell ** path = lg.paths[found];
							SendGrid(path, 0, lg.rows, lg.cols);
							lg.active = 0;
						} else {
							//printf("%d: ignoring send\n",lg.rank);
							SendGrid(NULL, 0, lg.rows, lg.cols);
						}
					} else {
						lg.active = 1;
					}
				}else if (assigned_worker == lg.rank) {
                   // printf("rank %d: assigned worker: %d, sending grid from (%d,%d)\n", lg.rank, assigned_worker, lg.grid[0][0].position.x, lg.grid[0][0].position.y);
					
                    SendGrid(lg.paths[found], 0, lg.rows, lg.cols);
                }
                cellpos= ReceivePosition(0);
               // printf("received %d\n",cellpos.x);
			}while(cellpos.x!=-1);
            //printf("aa received %d\n",cellpos.x);
            //printf("%d _____\n",world_rank);
        }

        //printf("%d localgrid pointer %p\n",world_rank,localGridCells);

        //printf("%d OK\n",world_rank);
    } else {
        printf("Bad arguments");

        MPI_Finalize();
        return 1;
    }
    MPI_Finalize();
    return 0;

}