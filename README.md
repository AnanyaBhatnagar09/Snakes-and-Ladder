# Snakes-and-Ladder

Snakes and Ladders is a two-player game where players roll dice to move across a board, aiming to reach the final destination with the minimum number of dice throws. Additionally, the game provides the shortest path to reach the destination.

Data Structures Used:
1)Graph (BFS using Queue)
2)2D Array (Matrix)
3)Arrays (Lists)

Algorithms:

A)Board Creation:

->Constructed using a 2D array where each cell contains row, column, and position information.
->Board numbering starts from the bottom row in a zigzag pattern.

B)Snake and Ladder Generation:

->Random generation of snakes and ladders.
->Stored in an array using structs (snakes_and_ladder(start, end)).

C)Mapping Function:

->Maps position to corresponding row and column indices.
Example: In a 5x5 board, position 1 maps to (4,0).

Matrix:

D)Manages player movement:

-> -1 indicates no snake or ladder; otherwise, it denotes the endpoint of a snake or ladder.

Finding the Shortest Path
・Matrices Used:

1)Distance Matrix: Tracks minimum dice throws to each position (initialized to infinity).
2)Parent Matrix: Supports backtracking to find the shortest path (initialized with -1).
3)Dice Numbers Matrix: Indicates which dice number was used to achieve the minimum throw (initialized to -1).

Algorithm (Pseudo-code):

1)Begin from position 1 with distance marked as 0.
2)Utilize BFS traversal using a queue to explore all possible moves.
3)Update distances, parents, and dice numbers matrix based on each move and the presence of snakes/ladders.

Usage
1)Players take turns rolling dice and moving accordingly.
2)The game outputs the shortest path and the minimum number of dice throws required.

Example Flow
If a ladder exists at position 3 leading to position 22:
Update parent matrix accordingly.

3)BFS continues until reaching the destination.
4)Display the path using the parent matrix and the shortest path using the distance matrix.

Conclusion
This project implements the classic Snakes and Ladders game using efficient data structures and algorithms to provide gameplay and pathfinding functionalities.
