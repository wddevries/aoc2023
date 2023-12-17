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
    getline(f, ln);
    istringstream ss(ln);

    while (getline(ss, ln, ',')) {
        if (ln.empty())
            continue;

        out.push_back(ln);
    }

    return out;
}

int
hash_str(string s)
{

    int hash = 0;
    for (auto c : s) {
        hash += c;
        hash *= 17;
        hash = hash % 256;
    }

    return hash;           
}

struct lens {
    string label;
    int power;
    lens(string label, int power) : label(label), power(power) {}
};

void
print_boxes(vector<vector<lens>> boxes)
{
    for (int i = 0; i < 256; i++) {
        if (boxes[i].size() > 0) {
            cout << "Box "<<i<<": ";
            for (auto &l : boxes[i]) {
                cout << "[" << l.label << " " << l.power << "] ";
            }
            cout << endl;
        }
    }
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

    long sum1 = 0;
    for (auto c : platform)
        sum1 += hash_str(c);

    cout << "part 1: " << sum1 << "\n";
    assert(sum1 == 513172);

    vector<vector<lens>> boxes;
    boxes.resize(256);
    for (auto c : platform) {
        int idx = 0;
        for(; idx < c.length(); idx++)
            if (c[idx] == '=' || c[idx] == '-')
                break;
        string label = c.substr(0, idx);
        char cmd = c[idx];
        string power = c.substr(idx + 1);
        int hash = hash_str(label);

        if (cmd == '-') {
            for (auto it = boxes[hash].begin(); it != boxes[hash].end(); it++) {
                if (it->label == label) {
                    boxes[hash].erase(it);
                    break;
                }
            }
        } else if (cmd == '=') {
            bool found = false;
            for (auto it = boxes[hash].begin(); it != boxes[hash].end(); it++) {
                if (it->label == label) {
                    found = true;
                    it->power = stoi(power);
                    break;
                }
            }

            if (!found)
                boxes[hash].push_back({label, stoi(power)});
        }
    }

    int sum2 = 0;
    for (int i = 0; i < 256; i++)
        for (int j = 0; j < boxes[i].size(); j++)
            sum2 += (i+1) * (j+1) * boxes[i][j].power;

    cout << "part 2: " << sum2 << "\n";
    assert(sum2 == 237806);

    return 0;
}
