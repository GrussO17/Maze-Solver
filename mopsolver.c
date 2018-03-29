///
/// author: Owen Gruss
/// email: omg6429@rit.edu
///
#include <stdio.h>
#include <stdlib.h>

#include "queueADT.h"
#define OPEN 0
#define WALL 1


typedef struct coords_s {
        size_t x;
        size_t y;
} Coords;

typedef struct qnode{
	int pathSize;
	int capacity;
	Coords current;
	Coords* path;
} QNode;


Coords* breadthFirst(int rows, int cols, int const maze[][cols], int visited[][cols], int* pathSizeRet){
	QueueADT que = que_create(NULL);
	QNode* start = malloc(sizeof(QNode));
	start->current.x = 0;
	start->current.y = 0;
	start->path = malloc(sizeof(Coords) * 5);
	start->pathSize = 0;
	visited[0][0] = 1;
	que_insert(que, start);
	QNode* temp;
	int x = start->current.x;
	int y = start->current.y;
	int pS;
	int capacity;
	Coords* path;
	Coords tempCoord;
	printf("COLS = %d, ROWS = %d\n", cols, rows);
	while(x < cols - 1 ||  y < rows - 1){
		printf("Before x = %d, y = %d pS = %d\n", x, y, pS);
		printf("Before maze = %d, visited = %d\n", maze[x][y], visited[x][y]);
		if(!que_empty(que))
			temp = que_remove(que);
		else{
			printf("no solution");
			return NULL;
		}
		x = (int)temp->current.x;
		y = (int)temp->current.y;
		pS = temp->pathSize;
		capacity = temp->capacity;
		path = temp->path;
		if(capacity == pS - 1){
			printf("Reallocating....");
			temp = realloc(path, sizeof(Coords)*(capacity + 5));
			capacity = capacity + 5;
		}
		if(x != 0 && maze[y][x - 1] == OPEN && 
		visited[y][x - 1] != 1){
			visited[y][x - 1] = 1;
			QNode* add = malloc(sizeof(QNode));
			add->current.x = x - 1;
       			add->current.y = y;
        		add->pathSize = pS + 1;
			tempCoord.x = x - 1;
			tempCoord.y = y;
			path[pS] = tempCoord;
			add->path = path;
			que_insert(que, add);
		}
		if(x != cols - 1 && maze[y][x + 1] == OPEN && 
		visited[y][x + 1] != 1){
			visited[y][x + 1] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->current.x = x + 1;
                        add->current.y = y;
			tempCoord.x = x + 1;
                        tempCoord.y = y;
                        path[pS] = tempCoord;
                        add->path = path;
                        add->pathSize = pS + 1;
                        que_insert(que, add); 
 		}
		if(y != 0 && maze[y - 1][x] == OPEN &&
                visited[y - 1][x] != 1){
                        visited[y - 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->current.x = x;
                        add->current.y = y - 1;
                        add->pathSize = pS + 1;
			tempCoord.x = x;
                        tempCoord.y = y - 1;
                        path[pS] = tempCoord;
                        add->path = path;
                        que_insert(que, add);
                }
		if(y != rows - 1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->current.x = x;
                        add->current.y = y + 1;
                        add->pathSize = pS + 1;
			tempCoord.x = x;
                        tempCoord.y = y + 1;
                        path[pS] = tempCoord;
                        add->path = path;
                        que_insert(que, add);
                }
	}
	*pathSizeRet = pS;
	printf(" x = %d, y = %d\n pS = %d, capacity = %d, testpath = %zu\n", x, y, pS, capacity, path[2].y);
	return path;
}

void depthSearch(int rows, int cols, int maze[][cols], int visited[][cols],
	 int x, int y, int *done){
	//printf(" x = %d, y = %d, done = %d\n", x, y, *done);
	//printf("maze = %d\n", maze[x][y]);
	printPrettyMaze(rows, cols, maze);
	if(x == cols - 1 && y == rows - 1){
		*done = 1;
	}
	else if(*done == 0){
		visited[x][y] = 1;
		if(x > 0 && visited[x - 1][y] != 1 && maze[x-1][y] != 1)
			depthSearch(rows, cols, maze, visited, x - 1, y, done);
		if(y > 0 && visited[x][y - 1] != 1 && maze[x][y-1] != 1)
		        depthSearch(rows, cols, maze, visited, x, y - 1, done);
		if(x < cols -1 && visited[x + 1][y] != 1 && maze[x + 1][y] != 1)
			depthSearch(rows, cols, maze, visited, x + 1, y, done);
		if(y < rows -1 && visited[x][y + 1] != 1 && maze[x][y+1] != 1)
			depthSearch(rows, cols, maze, visited, x, y + 1, done);
	}	
}
void getSizes(char* input, int size, int* rows, int* cols){
	for(int a = 0; a < size; a++){
                if(input[a] == '\n'){
                       *cols = a;
                       break;
                }
        }
        int counter = 0;
        for(int a = 0; a < size; a++)
                if(input[a] != '\n')
                        counter++;
        *rows = counter/(*cols);
	return;
}
void convertArray(char* input, int rows, int cols, int ret[][cols]){
	int counter = 0;
	for( int row = 0; row < rows; row++){
		for(int col = 0; col < cols; col++){
			if(input[counter] != '\n'){
				ret[row][col] = (int)input[counter] - '0';
			}
			else
				col--;
			counter++;
			
		}
	}
	printf("rows = %d, cols = %d\n", rows, cols);
	for(int a = 0; a < rows; a++){
               for(int b = 0; b < cols; b++){
                         printf("%d ", ret[a][b]);
                }
                printf("\n");
        }
}

void printPrettySolution(Coords* trail, int rows, int cols, int pathSize){
	printf("\n");
        for(int a = 0; a <= cols; a ++)
                printf("oo");
        printf("o\n");
	for(int row = 0; row < rows; row++){
                if(row != 0)
                        printf("o");
                else
                        printf(" ");
                for(int col = 0; col < cols; col++){
			if(row == 0 && col == 0)
				printf(" x");
			else
                        for(int a = 0; a < pathSize; a++){ 
				if(trail[a].x == col && trail[a].y == row){
					printf(" x");
					break;
				}
				if(a == pathSize - 1)
					printf("  ");
			}
                }
                if(row != rows - 1)
                        printf(" o");
                printf("\n");
        }
}

void printPrettyMaze(int rows, int cols, int maze[][cols]){
	printf("\n");
	for(int a = 0; a <= cols; a ++)
		printf("oo");
	printf("o\n");
	for(int row = 0; row < rows; row++){
		if(row != 0)
			printf("o");
		else
			printf(" ");
		for(int col = 0; col < cols; col++){
			if( maze[row][col] == 1)
				printf(" %d", maze[row][col]);
			else
				printf("  ");
		}	
		if(row != rows - 1)
			printf(" o");
		printf("\n");
	}
	for(int a = 0; a <= cols; a ++)
                printf("oo");
	printf("o\n");
}

char* makeArray(FILE* pfile, int* size){
	char character;
	char* ret = malloc(sizeof(char) * 10);
	int count = 0;
	int space = 10;
	while(fscanf(pfile, "%c", &character) != EOF){
		if(character != ' '){
			ret[count] = character;
			count++;
		}
		if(count == space){
			ret = realloc(ret, sizeof(char)* space + 10);
			space = space + 10;
		}
		printf("%d   %c\n", count, character);
	}
	*size = count;
	return ret;
}

int main(int argc, char ** argv){
	/**
	char* usage = "USAGE: \nmopsolver [-bspmh] [-i INFILE] [-o OUTFILE] \n\n\Options: \n-h Print this helpful message to stdout. \n-b Add borders and pretty print.  (Default off.) \n-s Print shortest solution steps. (Default off.) \n-m Print matrix after reading.    (Default off.) \n-p Print solution with path.      (Default off.) \n-i INFILE Read maze from INFILE.  (Default stdin.) \n-o OUTFILE Write maze to OUTFILE. (Default stdout.)\n";
	FILE* in = stdin;
	FILE* out = stdout;
	int opt;
	int b, s, m, p = 0;
	while((opt = getopt ( argc, argv, "i:o:bspmh")) != -1)
		switch(opt){
			case 'i':
				in = optarg;
			case 'o':
				out = optarg;
			case 'b':
				b = 1;
			case 'h':
				printf("%s" usage);
				return EXIT_SUCCESS;
			case 's':
				s = 1;
			case 'm':
				m = 1;
			case 'p': 
				p = 1;
	}
	*/
	int size;
	printf("%s", argv[2]);
	FILE* pfile = fopen(argv[1], "r");
	char* file = makeArray(pfile, &size);
	for(int a = 0; a < size; a++)
		printf("%c", file[a]);
	int rows;
	int cols;
	getSizes(file, size, &rows, &cols);
	int maze[rows][cols];
	convertArray(file, rows, cols, maze);
	int visited[rows][cols];
	printPrettyMaze(rows, cols, maze);
	for( int a = 0; a < rows; a++)
                for(int b = 0; b < cols; b++)
                        visited[a][b] = 0;
        int canComplete = 0;
	Coords* path;
	depthSearch(rows, cols, maze, visited, 0, 0, &canComplete);
	printPrettyMaze(rows, cols, maze);
	printf("depth Check %d\n", canComplete);
	for( int a = 0; a < rows; a++)
		for(int b = 0; b < cols; b++)
			visited[a][b] = 0;
	
	
	path = breadthFirst(rows, cols, maze, visited, &canComplete);	
	printf("%d", canComplete);
	printPrettyMaze(rows, cols, maze);
	printPrettySolution(path, rows, cols, canComplete);
	return EXIT_SUCCESS;
}
