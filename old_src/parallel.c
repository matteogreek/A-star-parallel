#define _CRT_SECURE_NO_WARNINGS
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

typedef struct {
	int x, y;
}Position;
Position newPosition(int x, int y) {
	Position p;
	p.x = x;
	p.y = y;
	return p;
}

typedef struct Cell Cell;
struct Cell {
	Position position;
	Cell* parent;
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
void printCell(Cell* c) {
	printf("(%d,%d): f=%f, g=%f, h=%f, traversable=%d, visited=%d\n", c->position.x, c->position.y, c->f, c->g, c->h, c->traversable, c->visited);
}
Cell copyCell(Cell* cell) {
	Cell c = newCell(cell->position, cell->traversable);
	return c;

}

typedef struct {
	int top;
	int currentSize;
	int maxSize;
	Cell** items;
}Stack;
Stack newStack(int maxSize)
{
	Stack s;
	s.maxSize = maxSize;
	s.top = -1;
	s.currentSize = 1;
	s.items = (Cell**)malloc(sizeof(Cell*));
	return s;
}
// Utility function to return the size of the stack
int getStackSize(Stack* pt) {
	return pt->top + 1;
}
// Utility function to check if the stack is full or not
int isStackFull(Stack* pt) {
	return pt->top == pt->currentSize - 1;      // or return size(pt) == pt->maxsize;
}
// Utility function to check if the stack is empty or not
int isStackEmpty(Stack* pt) {
	return pt->top == -1;                   // or return size(pt) == 0;
}
// Utility function to add an element `x` to the stack
void push(Stack* pt, Cell* c)
{
	// check if the stack is already full. Then inserting an element would
	// lead to stack overflow
	if (isStackFull(pt))
	{
		if (pt->currentSize * 2 > pt->maxSize) {
			printf("Overflow\nProgram Terminated\n");
			exit(EXIT_FAILURE);
		}
		else {
			Cell** newptr = (Cell**)realloc(pt->items, pt->currentSize * sizeof(Cell*) * 2);

			if (newptr) {
				//printf("Reallocating stack\n");
				pt->items = newptr;
				pt->currentSize *= 2;
			}
			else {
				printf("Unable to allocate stack space\n");
				exit(EXIT_FAILURE);
			}
		}
	}

	//printf("Inserting %d\n", x);

	// add an element and increment the top's index
	pt->items[++pt->top] = c;
}
void emptyStack(Stack* s) {
	if (s != NULL) {
		//this doesn't free the items, only the container
		if (s->items != NULL) {
			free(s->items);
		}
		s->items = (Cell**)malloc(sizeof(Cell*));
		s->currentSize = 1;
		s->top = -1;
	}
}
// Utility function to return the top element of the stack
Cell* peek(Stack* pt)
{
	// check for an empty stack
	if (!isStackEmpty(pt)) {
		return pt->items[pt->top];
	}
	else {
		return NULL;
	}
}
// Utility function to pop a top element from the stack
Cell* pop(Stack* pt)
{
	// check for stack underflow
	if (isStackEmpty(pt))
	{
		printf("Underflow\nProgram Terminated\n");
		return NULL;
	}

	//printf("Removing %d\n", peek(pt));

	// decrement stack size by 1 and (optionally) return the popped element
	return pt->items[pt->top--];
}
void printStack(Stack* s) {
	printf("maxsize=%d, top= %d, currentsize=%d\n", s->maxSize, s->top, s->currentSize);
	if (!isStackEmpty(s)) {
		int i = 0;
		for (i = 0; i < getStackSize(s); i++) {
			printCell(s->items[i]);
		}
	}
	else {
		printf("Stack is empty\n");
	}
}

typedef struct {
	Cell** items;
	int itemsCount;
	int currentSize;
	int maxSize;
}Set;
Set newSet(int capacity) {
	Set s;
	s.items = (Cell**)malloc(sizeof(Cell*));
	s.itemsCount = 0;
	s.currentSize = 1;
	s.maxSize = capacity;
	return s;
}
void emptySet(Set* s) {
	if (s != NULL) {
		if (s->items != NULL)
			free(s->items);
		s->items = (Cell**)malloc(sizeof(Cell*));
		s->currentSize = 1;
		s->itemsCount = 0;
	}
}
int isSetFull(Set* s) {
	return s->itemsCount >= s->currentSize;
}
int isSetEmpty(Set* s) {
	return s->itemsCount == 0;
}
void insert(Set* s, Cell* c) {
	if (isSetFull(s))
	{
		if (s->currentSize * 2 > s->maxSize) {
			printf("Overflow\nProgram Terminated\n");
			exit(EXIT_FAILURE);
		}
		else {
			Cell** newptr = (Cell**)realloc(s->items, s->currentSize * sizeof(Cell*) * 2);
			if (newptr) {
				//printf("Reallocating set\n");
				s->items = newptr;
				s->currentSize *= 2;
			}
			else {
				printf("Unable to allocate stack space\n");
				exit(EXIT_FAILURE);
			}
		}

	}

	int insertPos = 0;
	while (insertPos < s->itemsCount&& s->items[insertPos]->f <= c->f) {
		insertPos++;
	}
	int i;
	for (i = s->itemsCount; i > insertPos; i--) {
		s->items[i] = s->items[i - 1];
	}
	s->items[insertPos] = c;
	s->itemsCount++;
}
int removeItemInPosition(Set* s, int position) {
	if (position >= s->itemsCount) {
		return 0;
	}
	else if (s->itemsCount > 1) {
		int i;
		for (i = position; i < s->itemsCount - 1; i++) {
			s->items[i] = s->items[i + 1];
		}
	}
	s->itemsCount--;
	s->items[s->itemsCount] = NULL;
	return 1;
}
Cell* getItemInPosition(Set* s, int position) {
	if (position >= s->itemsCount) {
		return NULL;
	}
	else {
		return s->items[position];
	}
}
void printSet(Set* s) {
	printf("itemscount=%d, currentsize= %d, maxsize=%d\n", s->itemsCount, s->currentSize, s->maxSize);
	if (!isSetEmpty(s)) {
		int i = 0;
		for (i = 0; i < s->itemsCount; i++) {
			printCell(s->items[i]);
		}
	}
	else {
		printf("Set is empty\n");
	}
}

typedef struct {
	Cell** grid;
	int rows, cols;
	Cell* globalSrc;
	Cell* globalDest;
}Grid;
Grid createGrid(const char* filepath) {
	Grid g;
	FILE* file = fopen(filepath, "r");
	Position src, dst;
	g.globalDest = NULL;
	g.globalSrc = NULL;
	g.grid = NULL;
	if (file) {
		int read = fscanf(file, "dim=(%d,%d)\n", &g.rows, &g.cols);
		if (read == 0) { exit(1); }

		read = fscanf(file, "src=(%d,%d)\n", &src.x, &src.y);
		if (read == 0) { exit(1); }

		read = fscanf(file, "dst=(%d,%d)\n", &dst.x, &dst.y);
		if (read == 0) { exit(1); }

		g.grid = (Cell**)malloc(g.rows * sizeof(Cell*));
		if (g.grid) {
			int r, c;
			for (r = 0; r < g.rows; r++) {
				Cell* gridRow = (Cell*)malloc(g.cols * sizeof(Cell));
				if (gridRow) {
					g.grid[r] = gridRow;
					for (c = 0; c < g.cols; c++) {
						int sup;
						read = fscanf(file, "%d,", &sup);
						if (read) {
							g.grid[r][c] = newCell(newPosition(r, c), sup);
							if (!g.globalSrc && r == src.x && c == src.y) {
								g.globalSrc = &g.grid[r][c];
								g.globalSrc->f = 0;
								g.globalSrc->g = 0;
								g.globalSrc->h = 0;
							}
							else if (!g.globalDest && r == dst.x && c == dst.y) {
								g.globalDest = &g.grid[r][c];
							}
						}
						else { exit(1); }
					}
				}
				else { exit(1); }
			}
			if (!g.grid || !g.globalSrc || !g.globalDest) { exit(1); }
		}
		else { exit(1); }
	}
	else {
		printf("Errore durante l'apertura di %s", filepath);
		exit(1);
	}
	return g;
}
void emptyGrid(Grid* g) {
	if (g->grid) {
		int i;
		for (i = 0; i < g->rows; i++) {
			free(g->grid[i]);
		}
		free(g->grid);
	}
	g->grid = NULL;
	g->globalSrc = NULL;
	g->globalDest = NULL;
}
void printGrid(Grid* g) {
	printf("rows=%d, columns= %d, src=(%d,%d), dst=(%d,%d)\n", g->rows, g->cols, g->globalSrc->position.x, g->globalSrc->position.y, g->globalDest->position.x, g->globalDest->position.y);
	if (g->grid) {
		int r, c;
		for (r = 0; r < g->rows; r++) {
			for (c = 0; c < g->cols; c++) {
				printCell(&(g->grid[r][c]));
			}
		}
	}
	else {
		printf("Grid is empty\n");
	}
}

typedef struct {
	Cell* gravity;
	Cell*** paths;
	Cell** grid;
	int rank;
	int maxPathsNumber;
	int pathsCount;
	int rows, cols;
	int active;
}LocalGrid;
LocalGrid newLocalGrid(Cell** grid, int rank, int rows, int cols,int active) {
	LocalGrid lg;
	lg.gravity = NULL;
	lg.rank = rank;
	lg.maxPathsNumber = 3000;
	lg.pathsCount = 0;
	lg.rows = rows;
	lg.cols = cols;
	lg.paths = (Cell***)malloc(sizeof(Cell**) * lg.maxPathsNumber);
	lg.grid = grid;
	lg.active = active;
	return lg;
}
void emptyLocalGrid(LocalGrid* lg) {
	if (lg->grid) {
		int i;
		for (i = 0; i < lg->rows; i++) {
			free(lg->grid[i]);
		}
		free(lg->grid);
	}
	free(lg->paths);
	lg->grid = NULL;
	lg->gravity = NULL;
	lg->paths = NULL;
}
LocalGrid copyGridInit(LocalGrid localGrid) {

	int r, c;
	Cell* grav=NULL;
	Cell** grid = (Cell**)malloc(sizeof(Cell*) * localGrid.rows);
	for (r = 0; r < localGrid.rows; r++) {
		grid[r] = (Cell*)malloc(sizeof(Cell) * localGrid.cols); 
		for (c = 0; c < localGrid.cols; c++) {
			Cell newCell = copyCell(&(localGrid.grid[r][c]));
			if(localGrid.gravity && localGrid.gravity== &(localGrid.grid[r][c]))
				grav = &grid[r][c];
			grid[r][c] = newCell;
		}
	}
	LocalGrid lg = newLocalGrid(grid, localGrid.rank, localGrid.rows, localGrid.cols,localGrid.active);
	lg.gravity = grav;
	/*for (r = 0; r < localGrid.rows; r++) {
		for (c = 0; c < localGrid.cols; c++) {
			Cell newCell = copyCell(&(localGrid.grid[r][c]));
			lg.grid[r][c] = newCell;
		}
	}*/
	return lg;
}
void printLocalGrid(LocalGrid* lg) {
	printf("rank=%d\n", lg->rank);
	if (lg->grid) {
		int r, c;
		for (r = 0; r < lg->rows; r++) {
			for (c = 0; c < lg->cols; c++) {
				printCell(&(lg->grid[r][c]));
			}
		}
	}
	else {
		printf("Grid is empty\n");
	}
}
double calculateHValue(Position* pos1, Position* pos2, double(*func)(Position*, Position*)) {
	// Return using the distance formula
	return func(pos1, pos2);
}
double euclideanDistance(Position* pos1, Position* pos2) {
	return ((double)sqrt((pos1->x - pos2->x) * (pos1->x - pos2->x) + (pos1->y - pos2->y) * (pos1->y - pos2->y)));
}
double manhattanDistance(Position* pos1, Position* pos2) {
	return abs(pos1->x - pos2->x) + abs(pos1->y - pos2->y);
}
Cell* calculateClosestCell(LocalGrid* lg, Position goal) {
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
	Cell* cell = NULL;
	double distance = INT_MAX;
	int r, c;
	for (r = 0; r < lg->rows; r++) {
		for (c = 0; c < lg->cols; c++) {
			if (lg->grid[r][c].traversable) {
				double newdistance = calculateHValue(&(lg->grid[r][c].position), &goal, euclideanDistance);
				if (newdistance < distance) {
					distance = newdistance;
					cell = &(lg->grid[r][c]);
				}
			}
		}
	}
	return cell;
}
int isValidInLG(LocalGrid* lg, int r, int c) {
	// Returns true if row number and column number
	// is in range
	return (r >= 0) && (r < lg->rows) && (c >= 0) && (c < lg->cols) && lg->grid[r][c].traversable;
}
int isValidInG(Grid* g, int r, int c) {
	// Returns true if row number and column number
	// is in range
	return (r >= 0) && (r < g->rows) && (c >= 0) && (c < g->cols) && g->grid[r][c].traversable;
}
int isDestination(Cell* c, Position goal) {
	return (c->position.x == goal.x && c->position.y == goal.y);
}
int calculateCellValues(Cell* currentCell, Cell* newCell, Set* openList, Position goal) {
	int foundDest = 0;
	if (isDestination(newCell, goal)) {
		newCell->parent = currentCell;
		foundDest = 1;
	}
	if (!newCell->visited) {
		double gnew = currentCell->g + 1.0;
		double hnew = calculateHValue(&(newCell->position), &goal, euclideanDistance);
		double fnew = gnew + hnew;
		if (newCell->f > fnew) {
			newCell->g = gnew;
			newCell->h = hnew;
			newCell->f = fnew;
			newCell->parent = currentCell;
			//printf("adding (%d%d) to openlist\n", newCell->position.x, newCell->position.y);
			insert(openList, newCell);
		}

	}
	return foundDest;
}

void printGridContent(Cell** grid, int cols, int rows) {
	int r, c;
	for (r = 0; r <rows; r++) {
		for (c = 0; c < cols; c++) {
			if (&(grid[r][c]))
				printCell(&(grid[r][c]));
			else
				printf("NULL\n");
		}
	}
}


Cell** calculateLocalAStar(LocalGrid localGrid, Position globalSource, Position globalDest) {

	LocalGrid lg = copyGridInit(localGrid);

	Cell* localSrc = calculateClosestCell(&lg, globalSource);
	Cell* localDest = lg.gravity; 
	//printf("rank %d:\nlocalSrc=", lg.rank);
	//printCell(localSrc);
	//printf("localDest=");
	//printCell(localDest);

	if (localDest && isDestination(localSrc, localDest->position)) {
		//printf("We are already at the destination\n");
		return NULL;
	}

	//modifica anche fuori, lg non è passato in copia
	localSrc->f = 0;
	localSrc->g = 0;
	localSrc->h = 0;
	localSrc->parent = localSrc;

	Set openList = newSet(100);

	insert(&openList, localSrc);

	int foundDest = 0;

	while (!isSetEmpty(&openList)) {

		Cell* firstElement = getItemInPosition(&openList, 0);
		int i = firstElement->position.x % lg.rows;
		int j = firstElement->position.y % lg.cols;
		firstElement->visited = 1;
		int x, y;

		//printf("calculate [%d,%d]\n", i, j);
		for (x = -1; x < 2; x++) {
			for (y = -1; y < 2; y++) {
				if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {

					if (isValidInLG(&lg, i + x, j + y)) {

						if (localDest && calculateCellValues(&(lg.grid[i][j]), &(lg.grid[i + x][j + y]), &openList, localDest->position)) {
							Cell* cell = &(lg.grid[i + x][j + y]);
							cell->visited = 1;
							while (cell->parent != cell) {
								//printCell(cell);
								cell = cell->parent;
							}
							return lg.grid;
						}

					}


				}
			}

		}

		removeItemInPosition(&openList, 0);
	}

	return NULL;
}
void notifyWorkers(Position cellpos,int process_number,LocalGrid* workers) {
	int worker_id;

	for (worker_id = 0; worker_id < process_number; worker_id++) {
		//printf("Position (%d,%d) received by worker %d\n",cellpos.x,cellpos.y,worker_id);

		Cell** gridState = calculateLocalAStar(workers[worker_id], cellpos, workers[worker_id].gravity->position);
		if (gridState) {
			workers[worker_id].paths[workers[worker_id].pathsCount++] = gridState;
		}
		//printGrid(&gridState);
	}
}
Cell** getBestPath(LocalGrid worker, Position p) {
	int i;
	for (i = 0; i < worker.pathsCount; i++) {
		if(worker.paths[i][p.x%worker.rows][p.y % worker.cols].g==0){
		//if (peek(&worker.paths[i])->position.x == p.x && peek(&worker.paths[i])->position.y == p.y) {
			return worker.paths[i];
		}
	}
	return NULL;
}
void controllerSearch(Grid g,int process_number, LocalGrid* workers) {

	if (isDestination(g.globalSrc, g.globalDest->position)) {
		printf("We are already at the destination\n");
		return;
	}
	Set openList = newSet(g.rows * g.cols);
	insert(&openList, g.globalSrc);
	int foundDest = 0;
	while (!isSetEmpty(&openList)) {
		Cell* firstElement = getItemInPosition(&openList, 0);
		int i = firstElement->position.x;
		int j = firstElement->position.y;

		firstElement->visited = 1;
		if (i == 6 && j == 1) {
			int z = 0;
		}
		notifyWorkers(newPosition(i , j ), process_number, workers);
		
		int assigned_worker = (i / (g.rows / (int)sqrt(process_number)) * ((int)sqrt(process_number))) + (j / (g.cols / (int)sqrt(process_number)));  //first part is simplified from i/(g.rows/(int)sqrt(process_number))* ((int)sqrt(process_number))
		int w_c;
		for (w_c = 0; w_c < process_number; w_c++) {
			if (w_c != assigned_worker) {
				workers[w_c].active = 1;
			}
		}
		if (workers[assigned_worker].active) {
			
			Cell** path=getBestPath(workers[assigned_worker], firstElement->position);
			if(path){
				int r, c;
				for (r = 0; r < workers[assigned_worker].rows; r++) {
					for (c = 0; c < workers[assigned_worker].cols; c++) {
						Cell* localCell = &(path[r][c]);
						Cell* globalCell = &(g.grid[localCell->position.x][localCell->position.y]);
						//check this
						double newG= localCell->g + firstElement->g;


						if (newG < globalCell->g) {
							globalCell->g = localCell->g + firstElement->g;
							globalCell->h = calculateHValue(&(globalCell->position), &(g.globalDest->position), euclideanDistance);
							globalCell->f = globalCell->g + globalCell->h;
						}
						if (!globalCell->visited && localCell->visited) {
							printf("Visited (%d,%d)\n", globalCell->position.x, globalCell->position.y);
						}
						if(!globalCell->parent)
							globalCell->visited = localCell->visited;
						if (localCell->parent && (!globalCell->parent /* || globalCell->parent->g>localCell->parent->g*/)) {
							globalCell->parent = &(g.grid[localCell->parent->position.x][localCell->parent->position.y]);
						}
						if (globalCell == g.globalDest) {
							foundDest = 1;
							
						}


						if (globalCell->visited && r == 0 && isValidInG(&g, globalCell->position.x - 1, globalCell->position.y) ) {
							Cell* neighbor = &(g.grid[globalCell->position.x -1][globalCell->position.y]);
							if (!neighbor->visited) {
								neighbor->g = globalCell->g + 1;
								neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
								neighbor->f = neighbor->g + neighbor->h;
								neighbor->parent = globalCell;
								insert(&openList, neighbor);
								printf("Added to open list: (%d,%d)\n",neighbor->position.x,neighbor->position.y);
							}
						}
						else if (globalCell->visited && r == workers[assigned_worker].rows-1 && isValidInG(&g, globalCell->position.x + 1, globalCell->position.y)) {
							Cell* neighbor = &(g.grid[globalCell->position.x + 1][globalCell->position.y]);
							if (!neighbor->visited) {
								neighbor->g = globalCell->g + 1;
								neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
								neighbor->f = neighbor->g + neighbor->h;
								neighbor->parent = globalCell;
								insert(&openList, neighbor);
								printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
							}
						}


						if (globalCell->visited && c == 0 && isValidInG(&g, globalCell->position.x, globalCell->position.y -1)) {
							Cell* neighbor = &(g.grid[globalCell->position.x][globalCell->position.y -1]);
							if (!neighbor->visited) {
								neighbor->g = globalCell->g + 1;
								neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
								neighbor->f = neighbor->g + neighbor->h;
								neighbor->parent = globalCell;
								insert(&openList, neighbor);
								printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
							}
						}
						else if (globalCell->visited && c == workers[assigned_worker].cols - 1 && isValidInG(&g, globalCell->position.x , globalCell->position.y+1)) {
							Cell* neighbor = &(g.grid[globalCell->position.x][globalCell->position.y +1]);
							if (!neighbor->visited) {
								neighbor->g = globalCell->g + 1;
								neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
								neighbor->f = neighbor->g + neighbor->h;
								neighbor->parent = globalCell;
								insert(&openList, neighbor);
								printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
							}
						}
						if (localCell->f<INT_MAX && !globalCell->visited) {
							insert(&openList, &(g.grid[localCell->position.x][localCell->position.y]));
						}


					}
				}
				int x, y;
				int lastCell_i = workers[assigned_worker].gravity->position.x;
				int lastCell_j = workers[assigned_worker].gravity->position.y;
				for (x = -1; x < 2; x++) {
					for (y = -1; y < 2; y++) {
						if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
							Cell* neighbor = &(g.grid[lastCell_i + x][lastCell_j + y]);
							if (isValidInG(&g, lastCell_i + x, lastCell_j + y) && !neighbor->parent) {
								if (!neighbor->visited) {
									neighbor->g = g.grid[lastCell_i][lastCell_j].g + 1;
									neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
									neighbor->f = neighbor->g + neighbor->h;
									if (!neighbor->parent || (neighbor->parent && neighbor->parent->g > g.grid[lastCell_i][lastCell_j].g)) {
										neighbor->parent = &(g.grid[lastCell_i][lastCell_j]);
									}
									insert(&openList, neighbor);
									printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
								}
							}
						}
					}
				}
				lastCell_i = firstElement->position.x;
				lastCell_j = firstElement->position.y;
				for (x = -1; x < 2; x++) {
					for (y = -1; y < 2; y++) {
						if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
							Cell* neighbor = &(g.grid[lastCell_i + x][lastCell_j + y]);
							if (isValidInG(&g, lastCell_i + x, lastCell_j + y) && neighbor->f == INT_MAX) {
								if (!neighbor->visited) {
									neighbor->g = firstElement->g + 1;
									neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
									neighbor->f = neighbor->g + neighbor->h;
									if (!neighbor->parent || (neighbor->parent && neighbor->parent->g > firstElement->g)) {
										neighbor->parent = firstElement;
									}
									insert(&openList, neighbor);
									printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
								}
							}
						}
					}
				}

				

			}
			else {
				int x, y;
				int lastCell_i = firstElement->position.x;
				int lastCell_j = firstElement->position.y;
				for (x = -1; x < 2; x++) {
					for (y = -1; y < 2; y++) {
						if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
							Cell* neighbor = &(g.grid[lastCell_i + x][lastCell_j + y]);
							if (isValidInG(&g, lastCell_i + x, lastCell_j + y) && neighbor->f == INT_MAX) {
								if (!neighbor->visited) {
									neighbor->g = firstElement->g + 1;
									neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
									neighbor->f = neighbor->g + neighbor->h;
									if (!neighbor->parent || (neighbor->parent && neighbor->parent->g > firstElement->g)) {
										neighbor->parent = firstElement;
									}
									insert(&openList, neighbor);
									printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
								}
							}
						}
					}
				}
			}
		}
		else {

			int x, y;
			int lastCell_i = firstElement->position.x;
			int lastCell_j = firstElement->position.y;
			for (x = -1; x < 2; x++) {
				for (y = -1; y < 2; y++) {
					if (!(x == 0 && y == 0) && (x == 0 || y == 0)) {
						Cell* neighbor = &(g.grid[lastCell_i + x][lastCell_j + y]);
						if (isValidInG(&g, lastCell_i + x, lastCell_j + y) && neighbor->f == INT_MAX) {

							if (!neighbor->visited) {
								neighbor->g = firstElement->g + 1;
								neighbor->h = calculateHValue(&(neighbor->position), &(g.globalDest->position), euclideanDistance);
								neighbor->f = neighbor->g + neighbor->h;
								if (!neighbor->parent || (neighbor->parent && neighbor->parent->g > firstElement->g)) {
									neighbor->parent = firstElement;
								}
								insert(&openList, neighbor);
								printf("Added to open list: (%d,%d)\n", neighbor->position.x, neighbor->position.y);
							}
						}
					}
				}
			}
		}

		removeItemInPosition(&openList, 0);

		//printf("New openlist calculated based on rank :%d\n",assigned_worker);
		//printSet(&openList);
		//printGrid(&g);
		workers[assigned_worker].active = 0;//TODO:riattivare worker all'uscita dal quadrato
		if (foundDest) {

			Cell* cell = g.globalDest;
			printGridContent(g.grid, 15, 15);
			printf("in fondo\n");
			while (cell->parent != cell) {
				printCell(cell);
				cell = cell->parent;
			}
			printCell(cell);
			return;
		}

	}
}
int main(int argc, char const* argv[]) {
	if (argc == 3) {
		const char* path = argv[1];
		char* endpoint;
		printf("loading %s\n", path);
		int process_number = strtol(argv[2], &endpoint, 10);
		printf("number of processes to create %d\n", process_number);
		Grid g = createGrid(path);
		printGrid(&g);
		int rank_id;
		LocalGrid* workers = (LocalGrid*)malloc(sizeof(LocalGrid) * process_number);
		for (rank_id = 0; rank_id < process_number; rank_id++) {
			Grid localGrid;
			localGrid.rows = g.rows / (int)sqrt(process_number);
			localGrid.cols = g.cols / (int)sqrt(process_number);
			localGrid.grid = (Cell**)malloc(sizeof(Cell*) * localGrid.rows);
			int r, c;
			for (r = 0; r < localGrid.rows; r++) {
				localGrid.grid[r] = (Cell*)malloc(sizeof(Cell) * localGrid.cols);
				for (c = 0; c < localGrid.cols; c++) {
					localGrid.grid[r][c] = g.grid[rank_id / (int)sqrt(process_number) * localGrid.rows + r][rank_id % (int)sqrt(process_number) * localGrid.cols + c];
				}

			}
			workers[rank_id] = newLocalGrid(localGrid.grid, rank_id, localGrid.rows, localGrid.cols,1);
			workers[rank_id].gravity=calculateClosestCell(&workers[rank_id], g.globalDest->position);
			//printLocalGrid(&workers[rank_id]);
		}
		printf("initialized all workers\n\n");
		g.globalSrc->parent = g.globalSrc;
		controllerSearch(g, process_number, workers);
		emptyGrid(&g);
	}
	else {
		printf("Bad arguments");
		return 0;
	}

	_CrtDumpMemoryLeaks();

}