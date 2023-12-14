#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;


vector<vector<string>>
read_data(ifstream &f)
{
    vector<vector<string>> out;

    string ln;

    vector<string> pattern;
    while (getline(f, ln)) {
        if (ln.empty()) {
            out.push_back(move(pattern));
            pattern.clear();
            continue;
        }

        pattern.push_back(ln);
    }

    if (!pattern.empty())
        out.push_back(pattern);

    return out;
}

bool
test_vertical(vector<string> &p, int i) {
    int len = p[0].length();
    for (auto l : p) {
        for (int s = i, e = i + 1; s >= 0 && e < len; s--, e++) {
            if (l[s] != l[e]) {
                return false;
            }
        }
    }

    return true;
}

long
get_vertical(vector<string> &p, int skip)
{
    string p0 = p[0];
    int len = p0.length();

    for (int i = 0; i < len - 1; i++)
    {
        if (i + 1 == skip)
            continue;

        if (p0[i] == p0[i + 1]) {
            if (test_vertical(p, i))
                return i + 1;
        }
    }

    return 0;
}

bool
test_horizontal(vector<string> &p, int i)
{
    int len = p.size();
    for (int s = i, e = i + 1; s >= 0 && e < len; s--, e++) {
        if (p[s] != p[e])
            return false;
    }

    return true;
}

long
get_horizontal(vector<string> &p, int skip)
{
    int len = p.size();
    for (int i = 0; i < len - 1; i++)
    {
        if (i + 1 == skip)
            continue;

        if (p[i] == p[i + 1]) {
            if (test_horizontal(p, i))
                return i + 1;
        }
    }

    return 0;
}

long
get_sum2(vector<string> &p, long h, long v)
{
    int plen = p.size();
    int clen = p[0].length();
    for (int i = 0; i < plen; i++) {
        for (int j = 0; j < clen; j++) {
            char c = p[i][j];
            if (c == '#')
                p[i][j] = '.';
            else
                p[i][j] = '#';

            long h2 = get_horizontal(p, h);
            long v2 = get_vertical(p, v);
            long out = 0;
            if (h2 && h2 != h) {
                out += 100 * h2;
            }
            if (v2 && v2 != v) {
                out += v2;
            }
            if (out) {
                return out;
            }

            p[i][j] = c;
        }
    }

    assert(false);
    return 0;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<vector<string>> patterns = read_data(f);
    int sum1 = 0;
    int sum2 = 0;
    for (auto p : patterns) {
        int v = get_vertical(p, -1);
        int h = get_horizontal(p, -1);
        sum1 += v + 100 * h;
        sum2 += get_sum2(p, h, v);
    }

    cout << "part 1: " << sum1 << "\n";
    assert(sum1 == 33735);

    cout << "part 2: " << sum2 << "\n";
    assert(sum2 == 38063);

    return 0;
}
