#include <iostream>
#include <string>
#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include <stdio.h>
#include <math.h>
#include<windows.h>

using namespace cv;
using namespace std;

template <typename Type>

// My own implementation of a Queue
class ScottQueue
{
private:
    Type* array;
    int front;
    int rear;
    int capacity;
public:
    ScottQueue(int size)
    {
        capacity = size;
        array = new Type[capacity];
        front = -1;
        rear = -1;
    }

    bool isFull()
    {
        if (rear == -1 && front == -1)
            return false;
        if (abs(rear - front) + 1 == capacity)
            return true;
        return false;
    }
    bool isEmpty()
    {
        if (rear == -1 && front == -1)
            return true;
        return false;
    }

    void queue(Type value)
    {
        if (!isFull())
        {
            if (front == -1)
                front++;
            rear = (rear + 1) % capacity;
            array[rear] = value;
        }
        else
            cout << "\nThe queue is full!\n";
    }

    void dequeue()
    {
        if (!isEmpty())
        {
            if (front == rear)
            {
                front = -1;
                rear = -1;
            }
            else
                front = (front + 1) % capacity;

        }
        else
            cout << "\nThe queue is empty!\n";
    }

    Type first()
    {
        if (!isEmpty())
            return array[front];
        cout << "\nThe queue is empty!\n";
        return NULL;
    }
};

// Constants used for window and drawing function
#define M_PI 3.14159265358979323846264338327950288
#define degToRad(angleInDegrees) ((angleInDegrees) * M_PI / 180.0)
char wndname[] = "TurtleWindow";
Mat image = Mat::zeros(700, 700, CV_8UC3);
const Scalar WHITE(255, 255, 255);
const Scalar BLACK(0, 0, 0);
const int DELAY = 1;

void init()
{
    imshow(wndname, image);
    waitKey(DELAY);
}

// Used to represent "nodes" in the maze
class Square
{
private:
    int _x;
    int _y;
    Point init;
    int _direction;
    int _length;
    bool _visited;
    bool sides[4];
    /*
       0 = bottom
       1 = right
       2 = top
       3 = left
    */

    // Draws a line from current position to (x2, y2) in _direction by nPixels (steps)
    void moveForward(int nPixels, Scalar color = WHITE, bool repaint = true)
    {
        int x = static_cast<int>(round(nPixels * cos(degToRad(_direction))));
        int y = static_cast<int>(round(nPixels * sin(degToRad(_direction))));

        Point newPoint = Point(x + _x, y + _y);
        line(image, Point(_x, _y), newPoint, color);
        _x = newPoint.x;
        _y = newPoint.y;
        if (repaint)
        {
            imshow(wndname, image);
            waitKey(DELAY);
        }
    }

    void changeDirection(int direction)
    {
        _direction = direction;
    }

public:
    Point prev;

    Square(int x = 0, int y = 0, int length_size = 25)
    {
        _x = x;
        _y = y;
        init.x = x;
        init.y = y;
        _visited = false;
        _direction = 0;
        _length = length_size;
        for (int i = 0; i < 4; i++)
            sides[i] = true;
    }

    // Draws a square at current position
    void draw()
    {
        // Move right
        changeDirection(0);
        moveForward(_length);

        // Move down
        changeDirection(90);
        moveForward(_length);

        // Move left
        changeDirection(180);
        moveForward(_length);

        // Move up
        changeDirection(270);
        moveForward(_length);

    }

    void RemoveTop()
    {
        changeDirection(0);
        moveForward(_length, BLACK, true);
        _x = init.x;
        _y = init.y;
        sides[2] = false;
    }
    void RemoveBottom()
    {
        _y += _length;
        changeDirection(0);
        moveForward(_length, BLACK, true);
        _x = init.x;
        _y = init.y;
        sides[0] = false;
    }
    void RemoveLeft()
    {
        changeDirection(90);
        moveForward(_length, BLACK, true);
        _x = init.x;
        _y = init.y;
        sides[3] = false;
    }
    void RemoveRight()
    {
        _x += _length;
        changeDirection(90);
        moveForward(_length, BLACK, true);
        _x = init.x;
        _y = init.y;
        sides[1] = false;
    }

    // Returns the position of the square where Point.x = row # and Point.y = column #
    // Useful in conjunction with a 2D array representing squares
    Point getGridPos()
    {
        return Point(_y / _length, _x / _length);
    }

    // Returns position of square on window
    Point getPos()
    {
        return Point(_x, _y);
    }

    bool checkIfVisited()
    {
        return _visited;
    }

    void setVisited(bool set)
    {
        _visited = set;
    }

    // Checks if the side of a square still exists or is "valid"
    bool sideCheck(int side)
    {
        if (side > 3 || side < 0)
            return false;
        if (sides[side])
            return true;
        return false;
    }

    // Invalidates/ locks the side of a square
    void lockSide(int side, bool set = false)
    {
        if (side > -1 && side < 4)
        {
            sides[side] = set;
        }
    }

    // Draws a square grid given the rows and columns
    void drawGrid(int rows, int columns)
    {
        Point starting(0, 0);


        changeDirection(0);
        moveForward(_length * columns);
        _x = starting.x;
        _y = starting.y;
        for (int i = 0; i < rows; i++)
        {
            Point init(_x, _y);
            changeDirection(90);
            moveForward(_length);
            changeDirection(0);
            moveForward(_length * columns);
            changeDirection(270);
            moveForward(_length);
            _x = starting.x;
            _y = starting.y + init.y + _length;
        }

        _x = starting.x + _length;
        _y = starting.y;
        changeDirection(90);
        for (int i = 1; i <= columns; i++)
        {
            moveForward(_length * rows);
            _x = starting.x + i * _length;
            _y = starting.y;
        }
    }

    // Draws a filled circle at the current position of the square
    void drawCircle(Scalar color = WHITE)
    {
        Point init(_x, _y);

        circle(image, Point(_x + _length / 2, _y + _length / 2), _length / 5, color, FILLED);
        _x = init.x;
        _y = init.y;
        imshow(wndname, image);
        waitKey(DELAY);
    }
};

#define rows 30
#define columns 30
#define length_size 20 // square side length
#define BOTTOM 0
#define RIGHT 1
#define TOP 2
#define LEFT 3
#define length 20

int main()
{
    init();
    srand(time(NULL));
    while (true)
    {
        image.release();
        image = Mat::zeros(rows * (length_size + 3), columns * (length_size + 3), CV_8UC3);

        // 2D Array to store each square of the grid
        Square arr[rows][columns];
        // Queue used to store Squares and create the maze
        ScottQueue<Square> q(rows * columns);

        Point starting(0, 0);

        // Draws grid as specified by rows and columns
        Square grid(0, 0, length);
        grid.drawGrid(rows, columns);

        // Initialize array with all the squares of the grid
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                Square square(starting.x + (j * length), starting.y + (i * length), length);
                arr[i][j] = square;
            }
        }

        // Beginning square
        arr[0][0].setVisited(true);
        arr[0][0].RemoveLeft();

        // End square (assuming end is bottom right corner)
        arr[rows - 1][columns - 1].RemoveRight();

        //
        // While loop will queue up possible squares and go through them,
        // randomly removing X side of wall until there is no more square to modify
        //

        q.queue(arr[0][0]);
        Square* s = &arr[0][0];
        while (!q.isEmpty())
        {
            int x = q.first().getGridPos().x;
            int y = q.first().getGridPos().y;
            Square s = arr[x][y];

            int chosen_side;

            // While there is supposedly a valid side
            while (s.sideCheck(BOTTOM) || s.sideCheck(RIGHT) || s.sideCheck(TOP) || s.sideCheck(LEFT))
            {
                chosen_side = (rand() % 4);

                // Checks if its a valid move (excludes bound checking)
                if (s.sideCheck(chosen_side))
                {
                    switch (chosen_side)
                    {
                    case BOTTOM:
                        // Checks for out of bounds, if not continue
                        if (x + 1 < rows)
                            
                            // Move to X side, removing the appropriate side and making sure
                            // loops are prevented by "locking" (invalidating) the side
                            if (!arr[x + 1][y].checkIfVisited())
                            {
                                arr[x + 1][y].RemoveTop();
                                arr[x + 1][y].setVisited(true);
                                s.lockSide(BOTTOM);
                                q.queue(arr[x + 1][y]);
                                arr[x][y] = s;
                                s = arr[x + 1][y];
                            }
                        break;
                    case RIGHT:
                        if (y + 1 < columns)
                            if (!arr[x][y + 1].checkIfVisited())
                            {
                                arr[x][y + 1].RemoveLeft();
                                arr[x][y + 1].setVisited(true);
                                s.lockSide(RIGHT);
                                q.queue(arr[x][y + 1]);
                                arr[x][y] = s;
                                s = arr[x][y + 1];
                            }
                        break;
                    case 2:
                        if (x - 1 >= 0)
                            if (!arr[x - 1][y].checkIfVisited())
                            {
                                arr[x - 1][y].RemoveBottom();
                                arr[x - 1][y].setVisited(true);
                                s.lockSide(TOP);
                                q.queue(arr[x - 1][y]);
                                arr[x][y] = s;
                                s = arr[x - 1][y];
                            }
                        break;
                    case 3:
                        if (y - 1 >= 0)
                            if (!arr[x][y - 1].checkIfVisited())
                            {
                                arr[x][y - 1].RemoveRight();
                                arr[x][y - 1].setVisited(true);
                                s.lockSide(LEFT);
                                q.queue(arr[x][y - 1]);
                                arr[x][y] = s;
                                s = arr[x][y - 1];
                            }
                        break;
                    }
                }
                x = s.getGridPos().x;
                y = s.getGridPos().y;

                // Dequeues once the current square meets a dead end (all 4 sides are visited/ locked (invalid))
                int counter = 0;
                for (int i = 0; i < 4 && !(x == 0 && y == 0); i++)
                {
                    if (i == 0 && x + 1 >= rows || i == 1 && y + 1 >= columns || i == 2 && x - 1 < 0 || i == 3 && y - 1 < 0)
                        counter++;
                    else if (i == BOTTOM && arr[x + 1][y].checkIfVisited())
                        counter++;
                    else if (i == RIGHT && arr[x][y + 1].checkIfVisited())
                        counter++;
                    else if (i == TOP && arr[x - 1][y].checkIfVisited())
                        counter++;
                    else if (i == LEFT && arr[x][y - 1].checkIfVisited())
                        counter++;
                }
                if (counter == 4)
                    break;

            }
            q.dequeue();
        }
        q.~ScottQueue();

        ScottQueue<Square> bfs(rows * columns);
        ScottQueue<Square> solution(rows * columns);

        // Set all squares to not visited so BFS can utilize
        // the _visited member field of Square
        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < columns; j++)
            {
                arr[i][j].setVisited(false);
            }
        }

        arr[0][0].setVisited(true);
        bfs.queue(arr[0][0]);

        // Applies a slight fade for every square visited (goes from white to grey)
        double intensity = 1;

        Point exit(rows - 1, columns - 1);

        // Puts text "EXIT" where EXIT is located
        cv::putText(image, "EXIT", Point(rows * length_size + 10, columns * length_size), FONT_HERSHEY_COMPLEX, 0.5 * (double(columns) / double(20)), WHITE);


        //
        // Breadth first search implementation
        //
        while (bfs.first().getGridPos() != exit)
        {
            Square cur = bfs.first();
            int x = cur.getGridPos().x;
            int y = cur.getGridPos().y;

            // Draws a circle with a slight fade every iteration to somewhat illustrate the pathway of BFS
            intensity -=  1.0 / ((rows*columns) + 80);
            Scalar fade(255 * intensity, 255 * intensity, 255 * intensity);
            cur.drawCircle(fade);

            // Checks all four sides to see if they are possible
            for (int i = 0; i < 4; i++)
            {
                if (!cur.sideCheck(i))
                    switch (i)
                    {
                    case 0:
                        // Out of bounds check
                        if (x + 1 < rows)
                            if (!arr[x + 1][y].checkIfVisited())
                            {
                                arr[x + 1][y].prev.x = x;
                                arr[x + 1][y].prev.y = y;
                                arr[x + 1][y].setVisited(true);
                                bfs.queue(arr[x + 1][y]);
                            }
                        break;
                    case 1:
                        if (y + 1 < columns)
                            if (!arr[x][y + 1].checkIfVisited())
                            {
                                arr[x][y + 1].prev.x = x;
                                arr[x][y + 1].prev.y = y;
                                arr[x][y + 1].setVisited(true);
                                bfs.queue(arr[x][y + 1]);
                            }
                        break;
                    case 2:
                        if (x - 1 >= 0)
                            if (!arr[x - 1][y].checkIfVisited())
                            {
                                arr[x - 1][y].prev.x = x;
                                arr[x - 1][y].prev.y = y;
                                arr[x - 1][y].setVisited(true);
                                bfs.queue(arr[x - 1][y]);
                            }
                        break;
                    case 3:
                        if (y - 1 >= 0)
                            if (!arr[x][y - 1].checkIfVisited())
                            {
                                arr[x][y - 1].prev.x = x;
                                arr[x][y - 1].prev.y = y;
                                arr[x][y - 1].setVisited(true);
                                bfs.queue(arr[x][y - 1]);
                            }
                        break;
                    }
            }
            // Dequeues once all possible sides are visited (queued)
            bfs.dequeue();
        }
        // Draws last dot at the end
        bfs.first().drawCircle(length_size);

        Square cur = arr[exit.x][exit.y];

        // Each square has a unique square that led to it
        // By giving each square a Point member field that refers to its predecessor
        // We can back track from the end point to the beginning
        // Since BFS goes through all possibilities in the worst case, the solution path outputted
        // from this loop will be the most optimal given that the paths are unweighted
        while (cur.getGridPos() != Point(0, 0))
        {
            cur.drawCircle((255, 0, 143));
            int x = cur.prev.x;
            int y = cur.prev.y;
            cur = arr[x][y];
        }
        cur.drawCircle((255, 0, 143));
        Sleep(1500);

    } 
}

