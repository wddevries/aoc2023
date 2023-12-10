#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;

char matrix[140][140];

pair<int, int>
read_data(ifstream &f)
{
    pair<int, int> start;

    string ln;
    int row = 139;

    while (getline(f, ln)) {
        if (ln.empty())
            break;

        int i = 0;
        for (auto c : ln) {
            if (c == 'S') {
                start.first = row;
                start.second = i;
            }
            matrix[row][i++] = c;
        }

        row--;
    }

    return start;
}

char
find_new_direction(int row, int col, char going)
{
    char tile = matrix[row][col];
    switch (tile) {
    case '|':
        if (going == 'n')
            return 'n';
        else if (going == 's')
            return 's';
        break;
    case '-':
        if (going == 'w')
            return 'w';
        else if (going == 'e')
            return 'e';
        break;
    case 'L':
        // L is a 90-degree bend connecting north and east.
        if (going == 's')
            return 'e';
        else if (going == 'w')
            return 'n';
        break;

    case 'J':
        // J is a 90-degree bend connecting north and west.
        if (going == 'e')
            return 'n';
        else if (going == 's')
            return 'w';
        break;
    case '7':
        // 7 is a 90-degree bend connecting south and west.
        if (going == 'n')
            return 'w';
        else if (going == 'e')
            return 's';
        break;
    case 'F':
        // F is a 90-degree bend connecting south and east.
        if (going == 'n')
            return 'e';
        else if (going == 'w')
            return 's';
        break;
    }

    return '\0';
}

void
move_tile(int &row, int &col, char going)
{
    switch (going) {
    case 'n':
        row++;
        break;
    case 'e':
        col++;
        break;
    case 's':
        row--;
        break;
    case 'w':
        col--;
        break;
    }
}

char
get_moded_tile(char tile) {
    switch (tile) {
    case '|':
        return 'V';
    case '-':
        return 'H';
    case 'L':
        return 'l';
    case 'J':
        return 'j';
    case '7':
        return 's';
    case 'F':
        return 'f';
    }
    return '\0';
}

char
get_start_type(int row, int col)
{
    char test_n = find_new_direction(row + 1, col, 'n');
    char test_e = find_new_direction(row, col + 1, 'e');
    char test_s = find_new_direction(row - 1, col, 's');
    char test_w = find_new_direction(row, col - 1, 'w');

    if (test_n && test_s) {
        return '|';
    } else if (test_e && test_w) {
        return '-';
    } else if (test_n && test_e) {
        return 'L';
    } else if (test_n && test_w) {
        return 'J';
    } else if (test_s && test_w) {
        return '7';
    } else if (test_s && test_e) {
        return 'F';
    }

    return '\0';
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    pair<int, int> start = read_data(f);
    int row = start.first;
    int col = start.second;
    char start_type = get_start_type(row, col);

    char going;
    switch (start_type) {
    case '|':
    case 'L':
    case 'J':
        going = 'n';
        break;
    case '-':
    case '7':
        going = 'w';
        break;
    case 'F':
        going = 's';
        break;
    }

    move_tile(row, col, going);     

    int count = 1;
    while (matrix[row][col] != 'S') {
        count++;
        going = find_new_direction(row, col, going);
        matrix[row][col] = get_moded_tile(matrix[row][col]);
        move_tile(row, col, going);     
    }
    
    cout << "part 1: " << count/2 << "\n";
    assert(count/2 == 7145);

    matrix[start.first][start.second] = get_moded_tile(start_type);
    bool count_tile = false;
    int tiles = 0;
    for (int r = 0; r < 140; r++) {
        assert (!count_tile);
        for (int c = 0; c < 140; c++) {
            if (matrix[r][c] == 'l') {
                count_tile = !count_tile;
                while (matrix[r][c + 1] == 'H')
                    c++;
                if (matrix[r][c + 1] == 'j') {
                    count_tile = !count_tile;
                }
                c++; // consume other corner
            } else if (matrix[r][c] == 'f') {
                count_tile = !count_tile;
                while (matrix[r][c + 1] == 'H')
                    c++;
                if (matrix[r][c + 1] == 's') {
                    count_tile = !count_tile;
                }
                c++; // consume other corner
            } else if (matrix[r][c] == 'V') {
                count_tile = !count_tile;
            } else if (count_tile) {
                tiles++;
            }
        }
    }

    cout << "part 2: " << tiles << "\n";
    assert(tiles == 445);

    return 0;
}
