///
/// author: Owen Gruss
/// email: omg6429@rit.edu
///
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "queueADT.h"
#define OPEN 0
#define WALL 1

/**
* Struct Coords holds the x and y coordinates of a node
* 
* @field x coordinate
* @filed y coordinate
*/
typedef struct coords_s {
        size_t x;
        size_t y;
} Coords;

/**
* Struct QNode is a node for the queue representing a place in the maze
*
* @field pathSize holds the size of the current path
* @field capacity of the path array
* @field current place in the maze
* @field path array holding the current path
*/
typedef struct qnode{
	int pathSize;
	int capacity;
	Coords current;
	Coords* path;
} QNode;

/**
* breadth first search that finds the length of the shorest path and returns
* an array of the coordinate in the path.
* 
* @param rows, number of rows in maze
* @param cols, number of cols in maze
* @param maze, matrix representation of the maze
* @param visited, matrix of visited nodes to avoid loops
* @param pathSize, return param for shortest path size
*/
Coords* breadthFirst(int rows, int cols, int maze[][cols], int visited[][cols],
	int* pathSizeRet){
	//mallocs the queue
	QueueADT que = que_create(NULL);
	//creates the starting point of the maze and adds it to the queue
	QNode* start = malloc(sizeof(QNode));
	start->current.x = 0;
	start->current.y = 0;
	start->path = malloc(sizeof(Coords) * 1);
	start->path[0].x = 0;
	start->path[0].y = 0;
	start->pathSize = 0;
	start->capacity = 1;
	visited[0][0] = 1;
	que_insert(que, start);
	//declares the necessary temp variables necessary fot he breath first
	//search
	QNode* temp;
	int x;
	int y;
	int pS;
	int capacity;
	QNode* add;
	Coords* path;
	Coords tempCoord;
	Coords* tempPath;
	//main search loop
	while(x < cols - 1 ||  y < rows - 1){
		if(!que_empty(que))
			temp = que_remove(que);
		else{
			return NULL;
		}
		x = (int)temp->current.x;
		y = (int)temp->current.y;
		pS = temp->pathSize;
		capacity = temp->capacity;
		path = temp->path;
		free(temp);
		//checks if the position to the lect the current node is a 
		//valid move and adds it to the que
		if(x != 0 && maze[y][x - 1] == OPEN && 
		visited[y][x - 1] != 1){
			visited[y][x - 1] = 1;
			add = malloc(sizeof(QNode));
			add->current.x = x - 1;
       			add->current.y = y;
        		add->pathSize = pS + 1;
			add->capacity = capacity + 1;
			tempCoord.x = x - 1;
			tempCoord.y = y;
			tempPath = malloc(sizeof(Coords) * add->capacity);
			for(int a = 0; a < capacity; a++){
				tempPath[a].x = path[a].x;
				tempPath[a].y = path[a].y;
			}
			tempPath[pS] = tempCoord;
			add->path = tempPath;
			que_insert(que, add);
		}
		//checks if the position to the right of the current node is a 
		//valid move and adds it to the que
		if(x != cols - 1 && maze[y][x + 1] == OPEN && 
		visited[y][x + 1] != 1){
			visited[y][x + 1] = 1;
			add = malloc(sizeof(QNode));
                        add->current.x = x + 1;
                        add->current.y = y;
			tempCoord.x = x + 1;
                        tempCoord.y = y;
			add->capacity = capacity + 1;
                        tempPath = malloc(sizeof(Coords) * add->capacity);
                        for(int a = 0; a < capacity; a++){
                                tempPath[a].x = path[a].x;
                                tempPath[a].y = path[a].y;
                        }
			tempPath[pS] = tempCoord;
                        add->path = tempPath;
                        add->pathSize = pS + 1;
                        que_insert(que, add);
 		}
		//checks if the position above the current node is a valid 
                //move and adds it to the que
		if(y != 0 && maze[y - 1][x] == OPEN &&
                visited[y - 1][x] != 1){
                        visited[y - 1][x] = 1;
			add = malloc(sizeof(QNode));
                        add->current.x = x;
                        add->current.y = y - 1;
                        add->pathSize = pS + 1;
			tempCoord.x = x;
                        tempCoord.y = y - 1;
			add->capacity = capacity + 1;
                       	tempPath = malloc(sizeof(Coords) * add->capacity);
                        for(int a = 0; a < capacity; a++){
                                tempPath[a].x = path[a].x;
                                tempPath[a].y = path[a].y;
                        }
			tempPath[pS] = tempCoord;
                        add->path = tempPath;
                        que_insert(que, add);
                }
		//checks if the position below the current node is a valid 
		//move and adds it to the que
		if(y != rows - 1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
			add = malloc(sizeof(QNode));
                        add->current.x = x;
                        add->current.y = y + 1;
                        add->pathSize = pS + 1;
			tempCoord.x = x;
                        tempCoord.y = y + 1;
			add->capacity = capacity + 1;
                        tempPath = malloc(sizeof(Coords) * add->capacity);
                        for(int a = 0; a < capacity; a++){
                                tempPath[a].x = path[a].x;
                                tempPath[a].y = path[a].y;
                        }
			tempPath[pS] = tempCoord;
                        add->path = tempPath;
                        que_insert(que, add);
                }
		//frees the current path unless it is the solution
		if(x < cols - 1 ||  y < rows - 1)
			free(path);
	}
	//cleans up the leftovers in the que when the solution is found.
	while(!que_empty(que)){
		temp = que_remove(que);
		free(temp->path);
		free(temp);
	}
	*pathSizeRet = pS + 1;
	que_destroy(que);
	return path;
}

/**
* Recursively searches the maze depth first in order to find out if the maze is solvable
*
* @param rows, number of rows in the maze
* @param cols, number of cols in the maze
* @paras maze, matrix representation of the maze
* @param visited, matrix of already visited places
* @param x, current col
* @param y, current row
* @param done, indicates if the end was found or not
*/
void depthSearch(int rows, int cols, int maze[][cols], int visited[][cols],
	 int x, int y, int *done){
	if(x == cols - 1 && y == rows - 1){
		*done = 1;
	}
	else if(*done == 0){
		visited[x][y] = 1;
		if(x > 0 && visited[x - 1][y] != 1 && maze[x-1][y] == 0)
			depthSearch(rows, cols, maze, visited, x - 1, y, done);
		if(y > 0 && visited[x][y - 1] != 1 && maze[x][y-1] == 0)
		        depthSearch(rows, cols, maze, visited, x, y - 1, done);
		if(x < (rows -1) && visited[x + 1][y] != 1 && maze[x + 1][y] == 0)
			depthSearch(rows, cols, maze, visited, x + 1, y, done);
		if(y < (cols -1) && visited[x][y + 1] != 1 && maze[x][y+1] == 0)
			depthSearch(rows, cols, maze, visited, x, y + 1, done);
		return;
	}	
}

/**
* takes in a string representation of the input and finds the rows and cols in
* maze but does not create it.
*
* @param input, string of chars form the input file
* @param size, length of string
* @param rows, return param for number of rows
* @param cols, return param for number of cols
*/
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

/**
* takes in a string with all of the input data and creates a matrix that 
* represents the maze.
*
* @param input, string of chars from the input file
* @param rows, num rows in the maze
* @param cols, num cols in the maze
* @param ret, return matrix to have the maze saved to.
*/
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
}

/**
* prints out the correctly formatted solution in the maze
* 
* @patam list of Coords in maze
* @param rows rows of the maze
* @param cols cols of the maze
* @param pathSize length of the
*/
void printPrettySolution(FILE* out, Coords* trail, int rows, int cols, int maze[][cols], int pathSize){
	fprintf(out, "\n");
        for(int a = 0; a <= cols; a ++)
                fprintf(out, "oo");
        fprintf(out, "o\n");
	for(size_t row = 0; row < (size_t)rows; row++){
                if(row != 0)
                        fprintf(out, "o");
                else
                        fprintf(out, " ");
                for(size_t col = 0; col < (size_t)cols; col++){
			if(row == 0 && col == 0)
				fprintf(out, " +");
			else
                        for(int a = 0; a < pathSize; a++){ 
				if(trail[a].x == col && trail[a].y == row){
					fprintf(out, " +");
					break;
				}
				if(a == pathSize - 1){
					if( maze[row][col] == 1)
						fprintf(out, " o");
					else
						fprintf(out, "  ");
				}
			}
                }
                if(row != (size_t)rows - 1)
                        fprintf(out, " o");
                fprintf(out,"\n");
        }
	for(int a = 0; a <= cols; a ++)
                fprintf(out, "oo");
        fprintf(out, "o\n");
}

/**
* prints out the correctly formatted maze
* 
* @param rows rows of the matrix
* @param cols cols of the matrix
* @param maze matrix representation of the maze
*/
void printPrettyMaze(FILE* out, int rows, int cols, int maze[][cols]){
	fprintf(out, "\n");
	for(int a = 0; a <= cols; a ++)
		fprintf(out, "oo");
	fprintf(out, "o\n");
	for(int row = 0; row < rows; row++){
		if(row != 0)
			fprintf(out, "o");
		else
			fprintf(out, " ");
		for(int col = 0; col < cols; col++){
			if( maze[row][col] == 1)
				fprintf(out, " %d", maze[row][col]);
			else
				fprintf(out, "  ");
		}	
		if(row != rows - 1)
			fprintf(out, " o");
		fprintf(out, "\n");
	}
	for(int a = 0; a <= cols; a ++)
                fprintf(out, "oo");
	fprintf(out, "o\n");
}

/**
* Creates an array from the input given and returns the array and its size
*
* @param size, return param for the size of the created array
* @param pfile file to be read in
* @return String with all data from file concatenated into it
*/
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
	}
	*size = count;
	return ret;
}

int main(int argc, char ** argv){
	
	char* usage = "USAGE: \nmopsolver [-bspmh] [-i INFILE] [-o OUTFILE] \n\nOptions: \n-h Print this helpful message to stdout. \n-b Add borders and pretty print.  (Default off.) \n-s Print shortest solution steps. (Default off.) \n-m Print matrix after reading.    (Default off.) \n-p Print solution with path.      (Default off.) \n-i INFILE Read maze from INFILE.  (Default stdin.) \n-o OUTFILE Write maze to OUTFILE. (Default stdout.)\n";
	char* in = NULL;
	char* out = NULL;
	int opt;
	extern char* optarg;
	int i = 0, o = 0, b = 0, s = 0, m = 0, p = 0, h = 0;
	while((opt = getopt ( argc, argv, "i:o:bspmh")) != -1)
		switch(opt){
			case 'i':
				in = optarg;
				i = 1;
				break;
			case 'o':
				out = optarg;
				o = 1;
				break;
			case 'b':
				b = 1;
				break;
			case 'h':
				h = 1;
				break;
			case 's':
				s = 1;
				break;
			case 'm':
				m = 1;
				break;
			case 'p': 
				p = 1;
				break;
	}
	if(h == 1)
		printf("%s", usage);
	int size;
	FILE* pfileIn;
	FILE* pfileOut;
	if( i == 1)
		pfileIn = fopen(in, "r");
	else
		pfileIn = stdin;
	if( o == 1)
		pfileOut = fopen(out, "r");
	else
		pfileOut = stdout;

	char* file = makeArray(pfileIn, &size);
	
	int rows;
	int cols;
	getSizes(file, size, &rows, &cols);
	int maze[rows][cols];
	convertArray(file, rows, cols, maze);
	free(file);
	int visited[rows][cols];
	for( int a = 0; a < rows; a++)
                for(int b = 0; b < cols; b++)
                        visited[a][b] = 0;
        int canComplete = 0;
	Coords* path;
	depthSearch(rows, cols, maze, visited, 0, 0, &canComplete);
	for( int a = 0; a < rows; a++)
		for(int b = 0; b < cols; b++)
			visited[a][b] = 0;
	
	int pathSize;	
	path = breadthFirst(rows, cols, maze, visited, &pathSize);
	if(m == 1)
		for(int a = 0; a < size; a++)
			fprintf(pfileOut, "%c", file[a]);
	if(s == 1)
		fprintf(pfileOut, "Solution in %d steps.", pathSize);
	if(b == 1)
		printPrettyMaze(pfileOut, rows, cols, maze);
	if(p == 1)
		printPrettySolution(pfileOut, path, rows, cols, maze, pathSize);
	free(path);
	return EXIT_SUCCESS;
}
