#include<stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <time.h>

#define MAX_COLS 10
#define MAX_ROWS 10

struct Cell {
    int row;
    int col;
    int position;
};

struct SnakesAndLadder {
    int start;
    int end;
};

struct Pair {
    int val;
    int step;
};

struct Player {
    int position;
    char name[3];
};

// Function to create a new pair of Struct Pair type for BFS
struct Pair* newPair(int val, int step) {
    struct Pair* pair = (struct Pair*)malloc(sizeof(struct Pair));
    pair->val = val;
    pair->step = step;
    return pair;
}

struct Queue {
    int front, rear, size;
    unsigned capacity;
    struct Pair** array;
};

// Function to create a queue
struct Queue* createQueue(unsigned capacity) {
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
    queue->rear = capacity - 1;
    queue->array = (struct Pair**)malloc(queue->capacity * sizeof(struct Pair*));
    return queue;
}

// Check if the queue is empty
bool isEmpty(struct Queue* queue) {
    return (queue->size == 0);
}

// Check if the queue is full
bool isFull(struct Queue* queue) {
    return (queue->size == queue->capacity);
}

// Function to add an item to the queue
void enqueue(struct Queue* queue, struct Pair* item) {
    if (isFull(queue)) {
        return;
    }
    queue->rear = (queue->rear + 1) % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
}

// Function to dequeue a pair
struct Pair* dequeue(struct Queue* queue) {
    if (isEmpty(queue)) {
        return NULL;
    }
    struct Pair* item = queue->array[queue->front];
    queue->front = (queue->front + 1) % queue->capacity;
    queue->size = queue->size - 1;
    return item;
}

// Mapping function
void map(struct Cell board[][MAX_COLS], int rows, int cols, int position, int *row, int *col) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (board[i][j].position == position) {
                *row = i;
                *col = j;
                return; // Found the position
            }
        }
    }
    // Row and col not found
    *row = -1;
    *col = -1;
}

void shortestPath(struct Cell board[][MAX_COLS], int rows, int cols, int matrix[][MAX_COLS], int start, int end) {
    int distance[rows][cols];
    int parent[rows][cols];
    int diceNumbers[rows][cols]; // Array to store dice numbers
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            distance[i][j] = INT_MAX;
            parent[i][j] = -1;
            diceNumbers[i][j] = -1;
        }
    }
    // Set distance to zero for starting position
    int startRow, startCol;
    map(board, rows, cols, start, &startRow, &startCol);
    distance[startRow][startCol] = 0;
    // Create a queue for BFS
    struct Queue* queue = createQueue(rows * cols);
    struct Pair* startPair = newPair(start, 0);
    enqueue(queue, startPair);
    while (!isEmpty(queue)) {
        struct Pair* current = dequeue(queue);
        int num = current->val;
        int d = current->step;
        for (int i = 1; i <= 6; i++) {
            if (num + i <= rows * cols) {
                int newNum = num + i;
                int newRow, newCol;
                map(board, rows, cols, newNum, &newRow, &newCol);
                if (d + 1 <= distance[newRow][newCol]) {
                    parent[newRow][newCol] = num;
                    distance[newRow][newCol] = d + 1;
                    diceNumbers[newRow][newCol] = i;
                    if (matrix[newRow][newCol] == -1) {
                        struct Pair* pair1 = newPair(newNum, d + 1);
                        enqueue(queue, pair1);
                    } else {
                        int newVal = matrix[newRow][newCol];
                        int newR, newC;
                        map(board, rows, cols, newVal, &newR, &newC);
                        parent[newR][newC] = newNum;
                        distance[newR][newC] = d + 1;
                        struct Pair* newPair2 = newPair(newVal, d + 1);
                        enqueue(queue, newPair2);
                    }
                }
            }
        }
        free(current); // Free the current pair
    }
    int r, c;
    map(board, rows, cols, rows * cols, &r, &c);
    printf("Shortest Path from 1 to %d\n", rows * cols);
    // Create an array to store the dice numbers
    int dicePath[rows * cols];
    int pathLength = 0;
    while (parent[r][c] != -1) {
        printf("%d <- ", r * cols + c + 1);
        dicePath[pathLength++] = diceNumbers[r][c];
        // Record the dice number
        map(board, rows, cols, parent[r][c], &r, &c);
    }
    printf("1\n");
    printf("Minimum number of dice throws: %d\n", distance[startRow][startCol]);
    printf("Dice Numbers Used: ");
    // Print dice numbers in reverse order to show the path from start to end
    for (int i = pathLength - 1; i >= 0; i--) {
        if (dicePath[i] != -1) {
            printf("%d ", dicePath[i]);
        }
    }
    printf("\n");
}

void printBoardWithPlayers(struct Cell board[][MAX_COLS], int rows, int cols, struct Player player1, struct Player player2) {
    for (int i = rows - 1; i >= 0; i--) {
        for (int j = 0; j < cols; j++) {
            if (player1.position == board[i][j].position) {
                printf("(%d)P1\t", board[i][j].position);
            } else if (player2.position == board[i][j].position) {
                printf("(%d)P2\t", board[i][j].position);
            } else {
                printf("(%d)\t", board[i][j].position);
            }
        }
        printf("\n");
    }
}

// Function to create board
// The board follows a zigzag pattern, start remains the same
void createBoard(struct Cell board[][MAX_COLS], int rows, int cols) {
    int num = 1;
    int direction = -1; // Zigzag
    // Initialize the board and store the cell positions
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].row = i;
            board[i][j].col = j;
            board[i][j].position = 0;
        }
    }
    // Fill the board with numbers in a zigzag pattern and update position
    for (int i = rows - 1; i >= 0; i--) {
        if (direction == 1) {
            for (int j = 0; j < cols; j++) {
                board[i][j].position = num++;
            }
        } else {
            for (int j = cols - 1; j >= 0; j--) {
                board[i][j].position = num++;
            }
        }
        direction = -direction; // Change direction after each row
    }
}

void createMatrix(struct Cell board[][MAX_COLS], int matrix[][MAX_COLS], int rows, int cols,
                  struct SnakesAndLadder* snakesAndLadders, int numSnakesAndLadders) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            matrix[i][j] = -1;
        }
    }
    for (int i = 0; i < numSnakesAndLadders; i++) {
        int position = snakesAndLadders[i].start;
        int value = snakesAndLadders[i].end;
        int row, col;
        map(board, rows, cols, position, &row, &col);
        matrix[row][col] = value;
    }
}

// Function to print the board
void printBoard(struct Cell board[][MAX_COLS], int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d,%d-%d\t", board[i][j].row, board[i][j].col, board[i][j].position);
        }
        printf("\n");
    }
}

// Function to create random snakes and ladders
void createRandomSnakesAndLadders(struct SnakesAndLadder* snakesAndLadders, int numSnakesAndLadders, int totalCells) {
    srand(time(NULL));
    for (int i = 0; i < numSnakesAndLadders; i++) {
        int start, end;
        start = rand() % (totalCells - 1) + 1;
        end = rand() % (totalCells - start) + (start + 1);
        // Create snakes and ladder entry
        snakesAndLadders[i].start = start;
        snakesAndLadders[i].end = end;
    }
}

void playGame(struct Cell board[][MAX_COLS], int rows, int cols, int matrix[][MAX_COLS]) {
    struct Player player1 = {1, "P1"};
    struct Player player2 = {1, "P2"};
    int goal = rows * cols;
    int player1Turn = 1; // Use this to switch turns
    while (player1.position != goal && player2.position != goal) {
        int dice = rand() % 6 + 1;
        if (player1Turn) {
            // Player 1's turn
            printf("Player1's turn\n");
            printf("Current Player: %s\n", player1.name);
            printf("From position: %d\n", player1.position);
            player1.position += dice;
            if (player1.position > goal) {
                player1.position = goal;
            }
            int row1, col1;
            map(board, rows, cols, player1.position, &row1, &col1);
            if (matrix[row1][col1] != -1) {
                int newPosition = matrix[row1][col1];
                map(board, rows, cols, newPosition, &row1, &col1);
                player1.position = board[row1][col1].position;
            }
            printf("Dice Move: %d\n", dice);
            printf("To Position: %d\n", player1.position);
        } else {
            // Player 2's turn
            printf("Player2's turn\n");
            printf("Current Player: %s\n", player2.name);
            printf("From position: %d\n", player2.position);
            player2.position += dice;
            if (player2.position > goal) {
                player2.position = goal;
            }
            int row2, col2;
            map(board, rows, cols, player2.position, &row2, &col2);
            if (matrix[row2][col2] != -1) {
                int newPosition = matrix[row2][col2];
                map(board, rows, cols, newPosition, &row2, &col2);
                player2.position = board[row2][col2].position;
            }
            printf("Dice Move: %d\n", dice);
            printf("To Position: %d\n", player2.position);
        }
        // Print the board with players' positions
        printBoardWithPlayers(board, rows, cols, player1, player2);
        // Switch to the other player's turn
        player1Turn = !player1Turn;
    }
    if (player1.position == goal) {
        printf("Player1 is the winner\n");
    } else {
        printf("Player2 is the winner\n");
    }
}

int main() {
    int rows = 10;
    int cols = 10;
    struct Cell board[rows][cols];
    // Create the board
    createBoard(board, rows, cols);
    // Create an array to store the snakes and ladders
    int numSnakesAndLadders = 10;
    struct SnakesAndLadder snakesAndLadders[numSnakesAndLadders];
    // Create random snakes and ladders
    createRandomSnakesAndLadders(snakesAndLadders, numSnakesAndLadders, rows * cols);
    // Print the board
    printBoard(board, rows, cols);
    // Map position 1 from (1,4)
    int row, col;
    int positionToFind = 25;
    map(board, rows, cols, positionToFind, &row, &col);
    // Print the snakes and ladders position
    for (int i = 0; i < numSnakesAndLadders; i++) {
        printf("(%d -> %d) ", snakesAndLadders[i].start, snakesAndLadders[i].end);
    }
    printf("\n");

    int matrix[rows][cols];
    createMatrix(board, matrix, rows, cols, snakesAndLadders, numSnakesAndLadders);
    // Print the matrix
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }

    // Two-player game loop
    while (1) {
        printf("\nChoose an option:\n");
        printf("1. Find the shortest path\n");
        printf("2. Play the game\n");
        printf("3. Generate random position of snakes and ladders\n");
        printf("4. Quit\n");
        int choice;
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                printf("\nFinding the shortest path\n");
                int start = 1;
                int end = rows * cols;
                shortestPath(board, rows, cols, matrix, start, end);
                break;
            case 2:
                printf("\nPlaying the game:\n");
                playGame(board, rows, cols, matrix);
                break;
            case 3:
                createRandomSnakesAndLadders(snakesAndLadders, numSnakesAndLadders, rows * cols);
                createMatrix(board, matrix, rows, cols, snakesAndLadders, numSnakesAndLadders);
                printf("\nMatrix with snakes and ladders values:\n");
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        printf("%d\t", matrix[i][j]);
                    }
                    printf("\n");
                }
                break;
            case 4:
                return 0;
            default:
                printf("Invalid choice. Please select a valid option.\n");
                break;
        }
    }
}







