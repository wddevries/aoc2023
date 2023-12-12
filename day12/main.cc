#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;

char matrix[140][140];

vector<pair<string, vector<int>>> 
read_data(ifstream &f)
{
    vector<pair<string, vector<int>>>  out;
    string ln;

    while (getline(f, ln)) {
        if (ln.empty())
            break;

        istringstream ss(ln);
        string map;
        ss >> map;
        int num;
        vector<int> sizes;
        while (ss >> num) {
            sizes.push_back(num);
            char comma;
            ss >> comma;
        }

        out.push_back({map, sizes});
    }

    return out;
}

long
count_options(map<tuple<int, int, int>, long> &m, string &line, vector<int> &sizes,
              int line_s, int cur_broken, int size_off)
{
    tuple<int, int, int> t(line_s, cur_broken, size_off);
    if (m.contains(t))
        return m[t];

    long cnt = 0;
    bool groupend = false;
    if (cur_broken) {
        if (cur_broken == sizes[size_off]) {
            cur_broken = 0;
            size_off++;
            groupend = true;
        } else if (cur_broken > sizes[size_off]) {
            return 0;
        }
    }

    if (line_s == line.length()) {
        if (!cur_broken && size_off == sizes.size())
            return 1;

        return 0;
    }

    if (line[line_s] == '.') {
        if (!cur_broken)
            cnt = count_options(m, line, sizes, line_s + 1, 0, size_off);
    } else if (line[line_s] == '#') {
        if (!groupend && size_off < sizes.size())
            cnt = count_options(m, line, sizes, line_s + 1, cur_broken + 1, size_off);
    } else if (line[line_s] == '?') {
        // make it a #
        if (!groupend && size_off < sizes.size())
            cnt += count_options(m, line, sizes, line_s + 1, cur_broken + 1, size_off);

        // make it a .
        if (cur_broken == 0)
            cnt += count_options(m, line, sizes, line_s + 1, 0, size_off);
    }

    m[t] = cnt;
    return cnt;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<pair<string, vector<int>>>  m = read_data(f);

    long sum1 = 0;
    for (auto p : m) {
        map<tuple<int, int, int>, long> m;
        sum1 += count_options(m, p.first, p.second, 0, 0, 0);
    }

    cout << "part 1: " << sum1 << "\n";
    assert(sum1 == 7169);

    long sum2 = 0;
    for (auto p : m) {
        vector<int> newsizes;
        string unfolded;
        for (int i = 0; i < 5; i++) {
            newsizes.insert(newsizes.end(), p.second.begin(), p.second.end());
            unfolded.append(p.first);
            if (i < 4)
                unfolded.append("?");
        }

        map<tuple<int, int, int>, long> m;
        long ans = count_options(m, unfolded, newsizes, 0, 0, 0);
        sum2+=ans;
    }

    cout << "part 2: " << sum2 << "\n";
    assert(sum2 == 1738259948652);

    return 0;
}
