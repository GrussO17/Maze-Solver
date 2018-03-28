///
/// author: Owen Gruss
/// email: omg6429@rit.edu
///
#include <stdio.h>
#include <stdlib.h>

#include "queueADT.h"
#define OPEN 0
#define WALL 1



typedef struct qnode{
	int pathSize;
	int** visited;
	size_t x;     // using separate x/y coordinates
	size_t y;
} QNode;

int breadthFirst(int size, int rows, int maze[][size], int visited[][size]){
	QueueADT que = que_create(NULL);
	QNode* start = malloc(sizeof(QNode));
	start->x = 0;
	start->y = 0;
	start->pathSize = 0;
	visited[0][0] = 1;
	que_insert(que, start);
	QNode* temp;
	int x = start->x;
	int y;
	int pS;
	while(x !=  size - 1 ||  y != rows - 1){
		printf("Before x = %d, y = %d pS = %d\n", x, y, pS);
		printf("Before maze = %d, visited = %d\n", maze[x][y], visited[x][y]);
		if(!que_empty(que))
			temp = que_remove(que);
		else{
			printf("no solution");
			break;
		}
		x = (int)temp->x;
		y = (int)temp->y;
		pS = temp->pathSize;
		//free(temp);
		printf("After x = %d, y = %d pS = %d\n", x, y, pS);
		printf("After maze = %d, visited = %d\n", maze[x][y], visited[x][y]);
		if(x != 0 && maze[y][x - 1] == OPEN && 
		visited[y][x - 1] != 1){
			visited[y][x - 1] = 1;
			QNode* add = malloc(sizeof(QNode));
			add->x = x - 1;
       			add->y = y;
        		add->pathSize = pS + 1;
			que_insert(que, add);
		}
		if(x != size-1 && maze[y][x + 1] == OPEN && 
		visited[y][x + 1] != 1){
			visited[y][x + 1] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x + 1;
                        add->y = y;
                        add->pathSize = pS + 1;
                        que_insert(que, add); 
 		}
		if(y != 0 && maze[y - 1][x] == OPEN &&
                visited[y - 1][x] != 1){
                        visited[y - 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x;
                        add->y = y - 1;
                        add->pathSize = pS + 1;
                        que_insert(que, add);
                }
		if(y != rows-1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x;
                        add->y = y + 1;
                        add->pathSize = pS + 1;
                        que_insert(que, add);
                }
	}
	printf(" x = %d, y = %d\n pS = %d\n", x, y, pS);
	return pS;
}

void depthSearch(int cols, int rows, int maze[][cols], int visited[][cols],
	 int x, int y, int *done){
	printf(" x = %d, y = %d, done = %d\n", x, y, *done);
	printf("maze = %d\n", maze[x][y]);	
	if(x == cols-1 && y == rows - 1){
		*done = 1;
	}
	else if(*done == 0){
		visited[x][y] = 1;
		if(x != 0 && visited[x - 1][y] != 1 && maze[x-1][y] != 1)
			depthSearch(cols, rows, maze, visited, x - 1, y, done);
		if(y != 0 && visited[x][y - 1] != 1 && maze[x][y-1] != 1)
		        depthSearch(cols, rows, maze, visited, x, y - 1, done);
		if(x != cols -1 && visited[x + 1][y] != 1 && maze[x + 1][y] != 1)
			depthSearch(cols, rows, maze, visited, x + 1, y, done);
		if(y != rows -1 && visited[x][y + 1] != 1 && maze[x][y+1] != 1)
			depthSearch(cols, rows, maze, visited, x, y + 1, done);
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

	return ret;
}

void printPrettyMaze(int rows, int cols, int maze[][cols]){
	printf("\n");
	for(int a = 0; a <= cols; a ++)
		printf("oo");
	printf("\n");
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
	
	if(argc != 2) 
		return EXIT_FAILURE;
	FILE* pFile = fopen(argv[1], "r");
	int size;
	char* file = makeArray(pFile, &size);
	for(int a = 0; a < size; a++)
		printf("%c", file[a]);
	int rows;
	int cols;
	getSizes(file, size, &rows, &cols);
	int maze[rows][cols];
	convertArray(file, rows, cols, maze);
	int visited[rows][cols];
	for( int a = 0; a < rows; a++)
                for(int b = 0; b < cols; b++)
                        visited[a][b] = 0;
        int canComplete = 0;
	depthSearch(cols, rows, maze, visited, 0, 0, &canComplete);
	printf("%d", canComplete);
	for( int a = 0; a < rows; a++)
		for(int b = 0; b < cols; b++)
			visited[a][b] = 0;
	canComplete = breadthFirst(cols, rows, maze, visited);
        printf("%d", canComplete);
	printPrettyMaze(rows, cols, maze);
	return EXIT_SUCCESS;
}
