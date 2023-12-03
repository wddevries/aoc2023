#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>

void
find_numbers_in_row(std::vector<int> &v, std::string line, int i)
{
    int first = i;
    while (first - 1 >= 0 && std::isdigit(line[first - 1]))
        first--;

    int last = i + 1;
    while (last < line.length() && std::isdigit(line[last]))
        last++;

    std::string num;
    for (int j = first; j < last; j++) {
        if (std::isdigit(line[j])) {
            num.push_back(line[j]);
        }

        if (num.length() && (!std::isdigit(line[j]) || j+1 == last)) {
            int n = stoi(num);
            v.push_back(n);
            num.clear();
        }
    }
}

std::vector<int>
get_numbers(std::string above, std::string line, std::string below, int i)
{
    std::vector<int> v;
    find_numbers_in_row(v, above, i);
    find_numbers_in_row(v, line, i);
    find_numbers_in_row(v, below, i);
    return v;
}

int
process_line(std::string &above, std::string &line, std::string &below)
{
    int sum = 0;
    int len = line.length();
    for (int i = 0; i < len; i++) {
        bool numdone = false;
        if (line[i] == '*') {
            std::vector nums = get_numbers(above, line, below, i);
            if (nums.size() == 2)
                sum += nums[0] * nums[1];
        }
    }

    return sum;
}

int main()
{
    std::ifstream f("data.txt");
    if (!f) {
        std::cerr << "Could not open file!";
        exit(1);
    }

    std::string dots = "............................................................................................................................................";
    int len = dots.length();

    std::string above = dots;
    std::string line = dots;
    std::string below = dots;

    int sum = 0;
    int line_num = -1;

    while (!f.eof()) {
        std::string ln;
        std::getline(f, ln);
        if (ln.empty())
            break;

        assert(ln.length() == len);

        above = line;
        line = below;
        below = ln;
        sum += process_line(above, line, below);
        line_num++;
    }

    above = line;
    line = below;
    below = dots;
    sum += process_line(above, line, below);

    assert(sum == 84266818);
    std::cout << sum << "\n";
    return 0;
}
