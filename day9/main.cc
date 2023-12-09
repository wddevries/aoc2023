#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;

vector<vector<int>>
read_numbers(ifstream &f)
{
    vector<vector<int>> rows;

    string ln;
    while (getline(f, ln)) {
        if (ln.empty())
            break;
        istringstream ss(ln);
        vector<int> row;
        int number;
        while (ss >> number)
            row.push_back(number);
        rows.push_back(row);
    }

    return rows;
}

int
calculate_row(vector<int> &row)
{
    bool nonzero = true;
    vector<vector<int>> intpro;
    intpro.push_back(row);
    vector<int> *cur = &row;
    while (nonzero) {
        nonzero = false;
        int len = cur->size();
        int last = (*cur)[0];
        vector<int> next_row;
        for (int i = 1; i < len; i++) {
            int diff = (*cur)[i] - last;
            last = (*cur)[i];
            if (diff)
                nonzero = true;
            next_row.push_back(diff);
        }

        intpro.push_back(next_row);
        cur = &intpro.back();
    }

    int val = 0;
    for (int i = intpro.size(); i; i--)
        val += intpro[i - 1].back();
    
    return val;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<vector<int>> rows = read_numbers(f);

    int sum1 = 0;
    for (auto row : rows)
        sum1 += calculate_row(row); 
    cout << "part1: " << sum1 << "\n";
    assert (sum1 == 1969958987);

    int sum2 = 0;
    for (auto row : rows) {
        reverse(row.begin(), row.end());
        sum2 += calculate_row(row); 
    }
    assert (sum2 == 1068);

    cout << "part2: " << sum2 << "\n";

    return 0;
}
