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
	size_t x;     // using separate x/y coordinates
	size_t y;
} QNode;
/**
void enqueue(QNode* que, size_t x, size_t y, int pS ){
	QNode* add = malloc(sizeof(QNode));
	QNode* current = que;
	while(current->next != NULL){
		current = current->next;
	}
	current->next = add;
	add->x = x;
	add->y = y;
	add->pathSize = pS;
	add->next = NULL;
}

QNode* dequeue(QNode* que){
	QNode* ret = que;
	que = ret->next;
	return ret;
}

void destroy(QNode* que){
	if(que->next != NULL)
		destroy(que->next);
	free(que);
}
*/
int breadthFirst(int size, int rows, int maze[][size], int visited[][size]){
	QueueADT que = que_create(NULL);
	QNode* start = malloc(sizeof(QNode));
	start->x = 0;
	start->y = 0;
	start->pathSize = 1;
	que_insert(que, start);
	QNode* temp;
	int x;
	int y;
	int pS;
	do{
		printf(" x = %d, y = %d pS = %d\n", x, y, pS);
		temp = que_remove(que);
		x = (int)temp->x;
		y = (int)temp->y;
		pS = temp->pathSize;
		//free(temp);
		if(x != 0 && maze[y][x - 1] == OPEN && 
		visited[y][x - 1] != 1){
			visited[y][x - 1] = 1;
			QNode* add = malloc(sizeof(QNode));
			add->x = x - 1;
       			add->y = y;
        		add->pathSize = pS++;
			que_insert(que, add);
		}
		if(x != size-1 && maze[y][x + 1] == OPEN && 
		visited[y][x + 1] != 1){
			visited[y][x + 1] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x + 1;
                        add->y = y;
                        add->pathSize = pS++;
                        que_insert(que, add); 
 		}
		if(y != 0 && maze[y - 1][x] == OPEN &&
                visited[y - 1][x] != 1){
                        visited[y - 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x;
                        add->y = y - 1;
                        add->pathSize = pS++;
                        que_insert(que, add);
                }
		if(y != rows-1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
			QNode* add = malloc(sizeof(QNode));
                        add->x = x;
                        add->y = y + 1;
                        add->pathSize = pS++;
                        que_insert(que, add);
                }
	}while(!(x ==  size - 1) || !(y == rows - 1));
	printf(" x = %d, y = %d\n pS = %d\n", x, y, pS);
	return pS;
}

void depthSearch(int size, int rows, int maze[][size], int visited[][size],
	 int x, int y, int *done){
	printf(" x = %d, y = %d\n", x, y);
	
	if(x == size-1 && y == rows - 1){
		*done = 1;
	}
	else if(*done == 0){
		visited[x][y] = 1;
		if(x != 0 && visited[x - 1][y] != 1)
			depthSearch(size, rows, maze, visited, x - 1, y, done);
		if(y != 0 && visited[x][y - 1] != 1)
		        depthSearch(size, rows, maze, visited, x, y - 1, done);
		if(x != size -1  && visited[x + 1][y] != 1)
			depthSearch(size, rows, maze, visited, x + 1, y, done);
		if(y != size -1  && visited[x][y + 1] != 1)
			depthSearch(size, rows, maze, visited, x, y + 1, done);
	}	
}
int** convertArray(char* input, int size){
	int width;
	for(int a = 0; a < size; a++){
		if(input[a] == '\n'){
			width = a;
			break;
		}
	}
	


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

int** readFile(FILE* pfile, int* returnCols, int* returnRows){
	FILE* temp = pfile;
	int counter = -1;
	char character = 'a';
	while( character != '\n'){
		fscanf(temp, "%c", &character);
		counter++;
	}
	printf(" width %d", counter);
	int size = counter;
	int** ret;
	ret = (int**)malloc(sizeof(int) * size);
	char next;
	next = 'a';
	int numrows = 1;
	int num = 0;
	for( int j = 0; next != EOF; j++){
		numrows ++;
		if( j != 0 ){
		       ret = (int**)realloc(ret, sizeof(int) 
			* (size) * numrows); 
		}
		for( int i = 0; i < size; i++){
			fscanf( pfile, "%i", &num);
			ret[j][i] = num;
			printf("%d\n", num);
		}
		fscanf(pfile, "%c", &next);
	}
	*returnCols = size;
	*returnRows = numrows;
	return ret;
}

int main(int argc, char ** argv){
	
	int* returnCols = 1;
	int* returnRows = 1;
	if(argc != 2) 
		return EXIT_FAILURE;
	FILE* pFile = fopen(argv[1], "r");
	/**
	int** maze = readFile(pFile, returnCols, returnRows);
	char* file = makeArray( );
	int visited[*returnRows][*returnCols];
	for( int a = 0; a < returnRows; a++)
		for(int b = 0; b < returnCols; b++)
			visited[a][b] = 0;
	int canComplete;
	depthSearch(*returnCols, *returnRows, maze, visited, 0, 0, &canComplete);
	printf("can be complete? %d", canComplete);
	*/
	int* size;
	char* file = makeArray(pFile, size);
	for(int a = 0; a < *size; a++)
		printf("%c", file[a]);
}
