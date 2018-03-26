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

int breadthFirst(int size, int maze[][size]){
	int shortest;
	int visited[size][size] = {{0}};
	QNode* que = malloc(sizeof(QNode));
	que->x = 0;
	que->y = 0;
	que->pathSize = 0;
	que->next = NULL;
	QNode* temp;
	size_t x;
	size_t y;
	int pS;
	do{
		temp = dequeue(que);
		x = temp->x;
		y = temp->y;
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
		if(y != size-1 && maze[y + 1][x] == OPEN &&
                visited[y + 1][x] != 1){
                        visited[y + 1][x] = 1;
                        enqueue(que, x, y + 1, pS++);
                }
	}while(x !=  size - 1 && y != size-1)
	destoy(que);
	return pS;




}

/**
int** createTrail(int size){
	int* trail = malloc(sizeof(int)*size*size);
	for(int i = 0; i < size; i++){
		for(int j = 0; j < size; j++){
			trail[i][j] = 0;
		}
	}
}
*/
int depthSearch(int size, int maze[][size], int visited[][size],
	 int x, int y, int done){
	
	if(x == size-1 && y == size-1){
		return 1;
		done = 1;
	}
	else if(done == 0){
		visited[x][y] = 1;
		if(x != 0 && !visited[x - 1][y])
			depthSearch(size, maze, visited, x - 1, y, done);
		if(y != 0 && !visited[x][y - 1])
		        depthSearch(size, maze, visited, x, y - 1, done);
		if(x != size -1  && !visited[x + 1][y])
			depthSearch(size, maze, visited, x + 1, y, done);
		if(y != size -1  && !visited[x][y + 1])
			depthSearch(size, maze, visited, x, y + 1, done);
	}
}


int** readFile(FILE* pfile, int* returnSize){
	FILE* temp = pfile;
	int counter = 0;
	char ret;
	while(fscanf(temp, "%c", &ret) != '\n')
		counter++;
	int size = counter;
	int ret** = malloc(sizeof(int) * size);
	char* next = 'a';
	int numrows = 1;
	for( int j = 0; *next != EOF; j++){
		numrows ++;
		if( j != 0 )
			ret = realloc(ret, sizeof(int) * (size) * numrows); 
		for( int i = 0; i < size; i++){
			fscanf( pfile, "%i", ret[j][i]); 
		}
		fscanf(pfile, "%c", next);
	}
	returnSize = size;
	return ret;
}

int main(int argc, char ** argv){
	if(argc != 2) 
		return EXIT_FAILURE;
	FILE* pFile = fopen(argv[1], "r");
	int returnSize;
	int** maze = readFile(pFile, returnSize);
	fprintf("%i", returnSize);

}
