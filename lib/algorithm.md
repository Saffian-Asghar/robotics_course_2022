# Algorithm for Path Finding Robot
The algorithm utilizes A star algorithm to find the best optimal path from point a to b using manhattan distance.

### A* Algorithm

![Alt text](images/aStar.png)

This finds the shortest path using heuristics on the given grid.


### Overall Path Finding Algorithm
Once we have the path in an array of coordinates, we need to use the path to move the robot around, and detect obstacle and recalculate the path

![Alt text](images/algorithm.png)