#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <set>
#include <numeric>

using namespace std;


vector<string>
read_data(ifstream &f)
{
    vector<string> out;

    string ln;

    while (getline(f, ln)) {
        if (ln.empty())
            continue;

        out.push_back(ln);
    }

    return out;
}

vector<string>
tilt_n(vector<string> &platform)
{
    int rlen = platform.size();
    int clen = platform[0].length();
    for (int r = 0; r < rlen; r++) {
        for (int c = 0; c < clen; c++) {
            for (int r2 = r; r2 < rlen; r2++) {
                if (platform[r2][c] == '#' )
                    break;
                if (platform[r2][c] == '.' )
                    continue;
                swap(platform[r2][c], platform[r][c]);
            }
        }
    }
    return platform;
}

vector<string>
tilt_w(vector<string> &platform)
{
    int rlen = platform.size();
    int clen = platform[0].length();
    for (int c = 0; c < clen; c++) {
        for (int r = 0; r < rlen; r++) {
            for (int c2 = c; c2 < clen; c2++) {
                if (platform[r][c2] == '#' )
                    break;
                if (platform[r][c2] == '.' )
                    continue;
                swap(platform[r][c2], platform[r][c]);
            }
        }
    }
    return platform;
}

vector<string>
tilt_s(vector<string> &platform)
{
    int rlen = platform.size();
    int clen = platform[0].length();
    for (int r = rlen - 1; r >= 0; r--) {
        for (int c = 0; c < clen; c++) {
            for (int r2 = r; r2 >= 0; r2--) {
                if (platform[r2][c] == '#' )
                    break;
                if (platform[r2][c] == '.' )
                    continue;
                swap(platform[r2][c], platform[r][c]);
            }
        }
    }
    return platform;
}

vector<string>
tilt_e(vector<string> &platform)
{
    int rlen = platform.size();
    int clen = platform[0].length();
    for (int c = clen - 1; c >= 0; c--) {
        for (int r = 0; r < rlen; r++) {
            for (int c2 = c; c2 >= 0; c2--) {
                if (platform[r][c2] == '#' )
                    break;
                if (platform[r][c2] == '.' )
                    continue;
                swap(platform[r][c2], platform[r][c]);
            }
        }
    }
    return platform;
}

long
platform_sum(vector<string> &platform)
{
    long out = 0;
    int rlen = platform.size();
    for (auto r : platform) {
        for (auto c : r) {
            if (c == 'O')
                out += rlen;
        }
        rlen--;
    }

    return out;
}

void
platform_cycle(vector<string> &platform)
{
    tilt_n(platform);
    tilt_w(platform);
    tilt_s(platform);
    tilt_e(platform);
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.\n";
        exit(-1);
    }

    vector<string> platform = read_data(f);

    vector<string> platform1 = platform;
    tilt_n(platform1);
    long sum1 = platform_sum(platform1);
    cout << "part 1: " << sum1 << "\n";
    assert(sum1 == 102497);

    vector<string> platform2 = platform;
    int cnt = 0;
    map<vector<string>, int> m;
    while (!m.contains(platform2)) {
        m[platform2] = cnt++;
        platform_cycle(platform2);
    }

    long remaining = (1000000000 - m[platform2]) % (cnt - m[platform2]);
    while (remaining--)
        platform_cycle(platform2);

    long sum2 = platform_sum(platform2);
    cout << "part 2: " << sum2 << "\n";
    assert(sum2 == 105008);

    return 0;
}
