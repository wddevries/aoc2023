#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <cassert>
#include <map>
#include <numeric>

using namespace std;

struct net_node {
    string left;
    string right;
    
    net_node () {}
    net_node (string left, string right) : left(left), right(right) {}
};

map<string, net_node>
read_map(ifstream &f)
{
    map<string, net_node> nmap;

    string ln;
    while (getline(f, ln)) {
        if (ln.empty())
            break;
        istringstream ss(ln);

        string node;
        ss >> node;

        char c;
        ss >> c; // =
        ss >> c; // (

        string left;
        ss >> left; 
        left = left.substr(0, 3);

        string right;
        ss >> right; 
        right = right.substr(0, 3);
        
        nmap[node] = net_node(left, right);
    }

    return nmap;
}

vector<string>
find_ends_with(map<string, net_node> &nmap, string end)
{
    vector<string> out;
    for (auto p : nmap)
        if (p.first.ends_with(end))
            out.push_back(p.first);

    return out;
}

long
get_cycle_count(map<string, net_node> &nmap, string dir, string start,
                string end) {
    int dirlen = dir.length();
    string first = "";
    int first_cnt = 0;
    int cnt = 0;

    while (true) {
        char d = dir[cnt++ % dirlen];
        assert(d == 'L' || d == 'R');
        start = (d == 'L') ? nmap[start].left : nmap[start].right;

        if (first == start)
            return first_cnt;

        if (start.ends_with(end)) {
            if (first == "") {
                first_cnt = cnt;
                first = start;
            }
            assert(first == start);
        }
    }

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

    string dir;
    getline(f, dir);
    string blank;
    getline(f, blank);
    map<string, net_node> nmap = read_map(f);

    int part1 = get_cycle_count(nmap, dir, "AAA", "ZZZ");
    cout << "part 1: " << part1 << "\n"; 
    assert(part1 == 19637);
    
    vector<string> part2_starts = find_ends_with(nmap, "A");
    vector<long> part2_cycles;
    for (auto p : part2_starts)
         part2_cycles.push_back(get_cycle_count(nmap, dir, p, "Z"));

    long part2 = part2_cycles[0];
    for (int i = 1; i < part2_cycles.size(); i++)
        part2 = lcm(part2, part2_cycles[i]);

    assert(part2 == 8811050362409);
    cout << "part 2: " << part2 << "\n";

    return 0;
}


