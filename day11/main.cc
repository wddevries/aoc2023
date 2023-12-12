#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;

vector<pair<long, long>>
read_data(ifstream &f)
{
    vector<pair<long, long>> galaxies;
    string ln;
    long row = 0;

    while (getline(f, ln)) {
        if (ln.empty())
            break;

        for (long col = 0; col < 140; col++)
            if (ln[col] == '#')
                galaxies.push_back({row, col});

        row++;
    }

    return galaxies;
}

vector<pair<long, long>>
expand_galaxies(vector<pair<long, long>> &galaxies, long empty_dist)
{
    vector<pair<long, long>> expanded;

    vector<bool> used_cols(140, false);
    vector<bool> used_rows(140, false);
    for (auto &g : galaxies) {
        used_cols[g.second] = true;
        used_rows[g.first] = true;
    }

    vector<long> row_offsets(140, 0);
    long row_offset = 0;
    vector<long> col_offsets(140, 0);
    long col_offset = 0;
    for (long i = 0; i < 140; i++) {
        if (!used_cols[i])
            col_offset += empty_dist - 1;
        col_offsets[i] = col_offset;
        if (!used_rows[i])
            row_offset += empty_dist - 1;
        row_offsets[i] = row_offset;

    }

    for (auto &g : galaxies)
        expanded.push_back({g.first + row_offsets[g.first],
                            g.second + col_offsets[g.second]});

    return expanded;
}

long
gal_diff(pair<long, long> g, pair<long, long> h)
{
    long dist = 0;
    if (h.first > g.first ||
        (h.first == g.first && h.second > g.second)) {
        dist = h.first - g.first;
        if (h.second > g.second)
            dist += h.second - g.second;
        else
            dist += g.second - h.second;
    }

    return dist;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<pair<long, long>> galaxies = read_data(f);
    vector<pair<long, long>> part1 = expand_galaxies(galaxies, 2);
    long sum1 = 0;
    for (auto &g : part1) {
        for (auto &h : part1) {
            sum1 += gal_diff(g, h);
        }
    }

    assert(sum1 == 9648398);
    cout << "sum 1: " << sum1 << endl;

    vector<pair<long, long>> part2 = expand_galaxies(galaxies, 1000000);
    long sum2 = 0;
    for (auto &g : part2) {
        for (auto &h : part2) {
            sum2 += gal_diff(g, h);
        }
    }

    assert(sum2 == 618800410814);
    cout << "sum 2: " << sum2 << endl;

    return 0;
}
