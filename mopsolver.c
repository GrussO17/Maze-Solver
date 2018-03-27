///
/// author: Owen Gruss
/// email: omg6429@rit.edu
///
#include <stdio.h>
#include <stdlib.h>
#define OPEN 0
#define WALL 1



typedef struct qnode_s {
	struct qnode_s *next;
	int pathSize;
	size_t x;     // using separate x/y coordinates
	size_t y;
} QNode;

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
	que = que->next;
	return ret;
}

void destroy(QNode* que){
	if(que->next != NULL)
		destroy(que->next);
	free(que);
}

int breadthFirst(int size, int rows, int maze[][size], int visited[][size]){
	QNode* que = malloc(sizeof(QNode));
	que->x = 0;
	que->y = 0;
	que->pathSize = 0;
	que->next = NULL;
	QNode* temp;
	int x;
	int y;
	int pS;
	do{
		temp = dequeue(que);
		x = (int)temp->x;
		y = (int)temp->y;
		pS = temp->pathSize;
		free(temp);
		if(x != 0 && maze[y][x - 1] == OPEN && 
		visited[y][x - 1] != 1){
			visited[y][x - 1] = 1;
			enqueue(que, x - 1, y, pS++);
		}
		if(x != size-1 && maze[y][x + 1] == OPEN && 
		visited[y][x + 1] != 1){
			visited[y][x + 1] = 1;
                        enqueue(que, x + 1, y, pS++); 
 		}
		if(y != 0 && maze[y - 1][x] == OPEN &&
                visited[y - 1][x] != 1){
                        visited[y - 1][x] = 1;
                        enqueue(que, x, y - 1, pS++);
                }
		if(y != rows-1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
                        enqueue(que, x, y + 1, pS++);
                }
	}while(x !=  size - 1 && y != size-1);
	destroy(que);
	return pS;
}

int depthSearch(int size, int rows, int** maze, int visited[][size],
	 int x, int y, int done){
	
	if(x == size-1 && y == size-1){
		return 1;
		done = 1;
	}
	else if(done == 0){
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
	return 0;	
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
	int* returnCols = 0;
	int* returnRows = 0;
	if(argc != 2) 
		return EXIT_FAILURE;
	FILE* pFile = fopen(argv[1], "r");
	int** maze = readFile(pFile, returnCols, returnRows);
	int visited[*returnRows][*returnCols];
        for(int a = 0; a < *returnRows; a++)
                for(int b = 0; b < *returnCols; b++)
                        visited[a][b] = 0;
	int canComplete = depthSearch(*returnCols, *returnRows, maze, visited, 0, 0, 0);
	printf("can be complete? %d", canComplete);
}
