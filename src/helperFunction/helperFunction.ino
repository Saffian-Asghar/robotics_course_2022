/**
  Name: Robotics Final Project
  Purpose: Creat a path finding + obstacle avoiding bot, using ultra sensor and A* algorythme
  @author Saffian Asghar, Emilio Espinossa, Alexis Culpin
  @version 0.01
*/

// definding the size of the grid
#define row 8
#define col 8



char getDirection(x,y, w, z)
{
  if (x == w && y < z)
  {
  return "f";
  }
  if (x == w && y > z)
  {
    return "b";
    }
    if (x < w && y == z)
  {
    return "l";
    }

    if (x < w && y == z)
  {
    return "r";
    }
    if ((x == w && y == z) || (x !=w && y != z)
  {
    return "e";
    }
  }
