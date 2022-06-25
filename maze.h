/* Your code here! */
#pragma once
#include "dsets.h"
#include "cs225/PNG.h"
#include "cs225/HSLAPixel.h"

#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <queue>

using namespace std;
using cs225::HSLAPixel;
using cs225::PNG;

class SquareMaze
{
public:
    struct Cell
    {
        bool right_;
        bool down_;
        unsigned cellNumber_;

        Cell(bool r, bool d, unsigned num) : right_(r), down_(d), cellNumber_(num) {}
    };
    
    SquareMaze();
    bool canTravel(int x,
                   int y,
                   int dir) const;
    PNG *drawMaze() const;
    PNG *drawMazeWithSolution();
    void makeMaze(int width,
                  int height);
    void setWall(int x,
                 int y,
                 int dir,
                 bool exists);
    vector<int> solveMaze();

    Cell &getCell(int x, int y);
    Cell &getCell(int cellNum);
    Cell &getRight(int cell);
    Cell &getDown(int cell);

    private:
    //(rightExists, downExists)   =>   first bool is true if left wall exists, second bool is true if bottom wall exists
    vector<vector< Cell > > cells_;
    int width_;
    int height_;
};