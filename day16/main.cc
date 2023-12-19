#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <set>
#include <queue>
#include <numeric>

using namespace std;


vector<string>
read_data(ifstream &f)
{
    vector<string> out;
    string ln;
    while (f >> ln)
        out.push_back(ln);

    return out;
}

enum {RIGHT, LEFT, UP, DOWN};

void
move_right(vector<string> &grid, queue<tuple<int, int, int>> &pending,
           int x, int y)
{
    int xmax = grid[0].size();
    if (x + 1 < xmax)
        pending.push({RIGHT, x + 1, y});
}

void
move_left(vector<string> &grid, queue<tuple<int, int, int>> &pending,
           int x, int y)
{
    if (x)
        pending.push({LEFT, x - 1, y});
}

void
move_up(vector<string> &grid, queue<tuple<int, int, int>> &pending,
           int x, int y)
{
    if (y)
        pending.push({UP, x, y - 1});
}

void
move_down(vector<string> &grid, queue<tuple<int, int, int>> &pending,
           int x, int y)
{
    int ymax = grid.size();
    if (y + 1 < ymax)
        pending.push({DOWN, x, y + 1});
}

string
get_dir_name(int dir)
{
    if (dir == RIGHT)
        return "RIGHT";
    if (dir == LEFT)
        return "LEFT";
    if (dir == UP)
        return "UP";
    if (dir == DOWN)
        return "DOWN";
    return "WTF";
}

int
energize_tiles(vector<string> grid, int dir, int x, int y)
{
    queue<tuple<int, int, int>> pending;
    set<tuple<int, int, int>> seen;
    pending.push({dir, x, y});

    while (!pending.empty()) {
        if (seen.contains(pending.front())) {
            pending.pop();
            continue;
        }
        seen.insert(pending.front());
        int dir = get<0>(pending.front());
        int x = get<1>(pending.front());
        int y = get<2>(pending.front());
        pending.pop();
        char tile = grid[y][x];

        switch (dir) {
        case RIGHT:
            if (tile == '.' || tile == 'S') {
                grid[y][x] = 'S';
                move_right(grid, pending, x, y);
            } else if (tile == '-' || tile == 'H') {
                grid[y][x] = 'H';
                move_right(grid, pending, x, y);
            } else if (tile == '|' || tile == 'V') {
                grid[y][x] = 'V';
                move_up(grid, pending, x, y);
                move_down(grid, pending, x, y);
            } else if (tile == '\\' || tile == 'B') {
                grid[y][x] = 'B';
                move_down(grid, pending, x, y);
            } else if (tile == '/' || tile == 'F') {
                grid[y][x] = 'F';
                move_up(grid, pending, x, y);
            }
            break;
        case LEFT:
            if (tile == '.' || tile == 'S') {
                grid[y][x] = 'S';
                move_left(grid, pending, x, y);
            } else if (tile == '-' || tile == 'H') {
                grid[y][x] = 'H';
                move_left(grid, pending, x, y);
            } else if (tile == '|' || tile == 'V') {
                grid[y][x] = 'V';
                move_up(grid, pending, x, y);
                move_down(grid, pending, x, y);
            } else if (tile == '\\' || tile == 'B') {
                grid[y][x] = 'B';
                move_up(grid, pending, x, y);
            } else if (tile == '/' || tile == 'F') {
                grid[y][x] = 'F';
                move_down(grid, pending, x, y);
            }
            break;
        case UP:
            if (tile == '.' || tile == 'S') {
                grid[y][x] = 'S';
                move_up(grid, pending, x, y);
            } else if (tile == '-' || tile == 'H') {
                grid[y][x] = 'H';
                move_left(grid, pending, x, y);
                move_right(grid, pending, x, y);
            } else if (tile == '|' || tile == 'V') {
                grid[y][x] = 'V';
                move_up(grid, pending, x, y);
            } else if (tile == '\\' || tile == 'B') {
                grid[y][x] = 'B';
                move_left(grid, pending, x, y);
            } else if (tile == '/' || tile == 'F') {
                grid[y][x] = 'F';
                move_right(grid, pending, x, y);
            }
            break;
        case DOWN:
            if (tile == '.' || tile == 'S') {
                grid[y][x] = 'S';
                move_down(grid, pending, x, y);
            } else if (tile == '-' || tile == 'H') {
                grid[y][x] = 'H';
                move_left(grid, pending, x, y);
                move_right(grid, pending, x, y);
            } else if (tile == '|' || tile == 'V') {
                grid[y][x] = 'V';
                move_down(grid, pending, x, y);
            } else if (tile == '\\' || tile == 'B') {
                grid[y][x] = 'B';
                move_right(grid, pending, x, y);
            } else if (tile == '/' || tile == 'F') {
                grid[y][x] = 'F';
                move_left(grid, pending, x, y);
            }
            break;
        }
    }

    long sum = 0;
    for (auto a : grid)
        for (auto b : a)
            if (b == 'S' || b == 'H' || b == 'V' || b == 'B' || b == 'F')
                sum++;

    return sum;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.\n";
        exit(-1);
    }

    vector<string> grid = read_data(f);

    
    int sum1 = energize_tiles(grid, RIGHT, 0, 0);
    cout << "part 1: " << sum1 << "\n";
    assert(sum1 == 6514);

    int sum2 = 0;
    for (int x = 0; x < grid[0].size(); x++) {
        int sum;
        sum = energize_tiles(grid, DOWN, x, 0);
        sum2 = max(sum, sum2);
        sum = energize_tiles(grid, UP, x, grid.size());
        sum2 = max(sum, sum2);
    }
    for (int y = 0; y < grid.size(); y++) {
        int sum;
        sum = energize_tiles(grid, RIGHT, 0, y);
        sum2 = max(sum, sum2);
        sum = energize_tiles(grid, LEFT, grid[0].size(), y);
        sum2 = max(sum, sum2);
    }
    cout << "part 2: " << sum2 << "\n";
    assert(sum2 == 8089);
    return 0;
}
