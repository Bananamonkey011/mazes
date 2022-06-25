/* Your code here! */
#include "maze.h"

SquareMaze::SquareMaze()
{
}
bool SquareMaze::canTravel(int x, int y, int dir) const
{
    switch (dir)
    {
    // right
    case 0:
        return (x + 1) < width_ && !cells_[y][x].right_;
    // down
    case 1:
        return (y + 1) < height_ && !cells_[y][x].down_;
    // left
    case 2:
        return x > 0 && !cells_[y][x - 1].right_;
    // up
    case 3:
        return y > 0 && !cells_[y - 1][x].down_;
    // idk how u got here
    default:
        return false;
    }
}

PNG *SquareMaze::drawMaze() const
{
    PNG *canvas = new PNG(width_ * 10 + 1, height_ * 10 + 1);
    // blacken top row except entrance
    for (unsigned i = 10; i < canvas->width(); i++)
    {
        canvas->getPixel(i, 0) = HSLAPixel(0, 0, 0);
    }
    // blacken left column
    for (unsigned i = 0; i < canvas->height(); i++)
    {
        canvas->getPixel(0, i) = HSLAPixel(0, 0, 0);
    }

    // draw rest of maze
    for (int x = 0; x < width_; x++)
    {
        for (int y = 0; y < height_; y++)
        {
            // if cant travel right draw wall right
            if (!canTravel(x, y, 0))
            {
                for (unsigned i = 0; i <= 10; i++)
                {
                    canvas->getPixel((x + 1) * 10, y * 10 + i) = HSLAPixel(0, 0, 0);
                }
            }

            // if cant travel down draw wall right
            if (!canTravel(x, y, 1))
            {
                for (unsigned i = 0; i <= 10; i++)
                {
                    canvas->getPixel(x * 10 + i, (y + 1) * 10) = HSLAPixel(0, 0, 0);
                }
            }
        }
    }

    return canvas;
}
PNG *SquareMaze::drawMazeWithSolution()
{
    PNG *canvas = drawMaze();
    HSLAPixel red = HSLAPixel(0, 1, 0.5, 1);
    vector<int> dir = solveMaze();

    int x = 5;
    int y = 5;
    // canvas->getPixel(x, y) = red;
    for (unsigned i = 0; i < dir.size(); i++)
    {
        switch (dir[i])
        {
        case 0:
            for (unsigned i = 0; i < 10; i++)
            {
                canvas->getPixel(x, y) = red;
                x++;
            }
            break;

        case 1:
            for (unsigned i = 0; i < 10; i++)
            {
                canvas->getPixel(x, y) = red;
                y++;
            }
            break;

        case 2:
            for (unsigned i = 0; i < 10; i++)
            {
                canvas->getPixel(x, y) = red;
                x--;
            }
            break;

        case 3:
            for (unsigned i = 0; i < 10; i++)
            {
                canvas->getPixel(x, y) = red;
                y--;
            }
            break;
        }
    }
    canvas->getPixel(x, y) = red;
    x /= 10;
    y /= 10;
    for (int k = 1; k < 10; k++)
    {
        canvas->getPixel(x * 10 + k, (y + 1) * 10) = HSLAPixel();
    }

    return canvas;
}
void SquareMaze::makeMaze(int width, int height)
{
    // init maze
    cells_.clear();
    width_ = width;
    height_ = height;
    // cout << width_ << " " << height_ << " " << width * height << endl;

    for (unsigned y = 0; int(y) < height_; y++)
    {
        vector<Cell> row;
        for (unsigned x = 0; int(x) < width_; x++)
        {
            row.push_back(Cell(true, true, (y * width_) + x));
        }
        cells_.push_back(row);
        row.clear();
    }

    // cycle detection
    DisjointSets dset = DisjointSets();
    dset.addelements(width * height);
    // remove walls
    random_device rng;
    mt19937 urng(rng());
    int randCell = urng() % (width_ * height_);
    int randSide = urng() % 2;
    while (dset.size(0) < width_ * height_)
    {
        randCell = urng() % (width_ * height_);
        randSide = urng() % 2;
        // removing a right wall
        if (randSide == 0 && dset.find(randCell) != dset.find(getRight(randCell).cellNumber_))
        {
            getCell(randCell).right_ = false;
            dset.setunion(randCell, getRight(randCell).cellNumber_);
        }
        // removing a bottom wall
        else if (randSide == 1 &&
                 dset.find(randCell) !=
                     dset.find(getDown(randCell).cellNumber_))
        {
            getCell(randCell).down_ = false;
            dset.setunion(randCell, getDown(randCell).cellNumber_);
        }
    }
}
void SquareMaze::setWall(int x, int y, int dir, bool exists)
{
    if (dir)
    {
        cells_[y][x].down_ = exists;
    }
    else
    {
        cells_[y][x].right_ = exists;
    }
}
vector<int> SquareMaze::solveMaze()
{
    //BFS queue of coords to travel to
    queue<pair<int, int>> q;
    //visited map stores (dist from start, direction traveled to get to cell)
    vector<vector<pair<int, int>>> visited;
    for (int i = 0; i < height_; i++)
    {
        vector<pair<int, int>> row;
        for (int j = 0; j < width_; j++)
        {
            row.push_back(pair<int, int>(-1, -1));
        }
        visited.push_back(row);
        row.clear();
    }

    q.push(pair<int, int>(0, 0));
    visited[0][0] = pair<int, int>(1, 1);
    while (!q.empty())
    {
        pair<int, int> xy = q.front();
        q.pop();
        size_t initSize = q.size();

        //if not at bottom then add neightbors/continue traversing

        //if can travel and already havent treaveled
        //right
        if (canTravel(xy.first, xy.second, 0) && visited[xy.second][xy.first + 1].first == -1)
        {
            q.push(pair<int, int>(xy.first + 1, xy.second));
            visited[xy.second][xy.first + 1] = pair<int, int>(visited[xy.second][xy.first].first + 1, 0);
        }
        //down
        if (canTravel(xy.first, xy.second, 1) && visited[xy.second + 1][xy.first].first == -1)
        {
            q.push(pair<int, int>(xy.first, xy.second + 1));
            visited[xy.second + 1][xy.first] = pair<int, int>(visited[xy.second][xy.first].first + 1, 1);
        }
        //left
        if (canTravel(xy.first, xy.second, 2) && visited[xy.second][xy.first - 1].first == -1)
        {
            q.push(pair<int, int>(xy.first - 1, xy.second));
            visited[xy.second][xy.first - 1] = pair<int, int>(visited[xy.second][xy.first].first + 1, 2);
        }
        //up
        if (canTravel(xy.first, xy.second, 3) && visited[xy.second - 1][xy.first].first == -1)
        {
            q.push(pair<int, int>(xy.first, xy.second - 1));
            visited[xy.second - 1][xy.first] = pair<int, int>(visited[xy.second][xy.first].first + 1, 3);
        }

        // if (xy.second == height_ - 1 && initSize == q.size())
        // {
        //     break;
        // }
    }
    // PNG* test = drawMaze();
    // test->writeToFile("testSolve.png");
    // for(unsigned i = 0; i<height_; i++){
    //     for(int j =0 ;j<width_; j++) {
    //         cout<<visited[i][j].first<< "\t";
    //     }
    //     cout<<endl;
    // }
    // cout<<endl;
    // cout<<endl;
    // for(unsigned i = 0; i<height_; i++){
    //     for(int j =0 ;j<width_; j++) {
    //         cout<<visited[i][j].second<< "\t";
    //     }
    //     cout<<endl;
    // }

    int maxPathCol = 0;
    for (int i = 1; i < width_; i++)
    {

        if (visited[height_ - 1][i].first > visited[height_ - 1][maxPathCol].first)
        {
            maxPathCol = i;
        }
    }

    vector<int> output;
    int y = height_ - 1;
    int x = maxPathCol;

    while (x != 0 || y != 0)
    {
        output.push_back(visited[y][x].second);
        switch (visited[y][x].second)
        {
        case 0:
            x -= 1;
            break;
        case 1:
            y -= 1;
            break;
        case 2:
            x += 1;
            break;
        case 3:
            y += 1;
            break;
        }
    }

    reverse(output.begin(), output.end());
    return output;
}

SquareMaze::Cell &SquareMaze::getCell(int x, int y)
{
    return cells_[y][x];
}

SquareMaze::Cell &SquareMaze::getCell(int cellNum)
{
    // cout << "cellNums: " << cellNum << "\t";
    return cells_[cellNum / width_][cellNum % width_];
}

SquareMaze::Cell &SquareMaze::getRight(int cell)
{
    if (cell % width_ == width_ - 1)
    {
        // if cell on rightmost side return itself
        return getCell(cell);
    }
    return getCell(cell + 1);
}

SquareMaze::Cell &SquareMaze::getDown(int cell)
{
    if (cell >= width_ * (height_ - 1))
    {
        // if cell on bottom return itself
        return getCell(cell);
    }
    return getCell(cell + width_);
}