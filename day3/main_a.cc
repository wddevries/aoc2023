#include <iostream>
#include <fstream>
#include <cassert>

bool
is_part(char ch)
{
    return !std::isdigit(ch) && ch != '.';
}

bool
is_part_number(std::string line, int i)
{
    int len = line.length();
    int start = i ? i - 1 : i;
    int end = (i + 1 < len) ? i + 1 : i;
    for (; start <= end; start++)
        if (is_part(line[start]))
            return true;

    return false;
}

int
process_line(std::string &above, std::string &line, std::string &below)
{
    int sum = 0;
    int len = line.length();
    std::string number;
    bool is_part = false;
    for (int i = 0; i < len; i++) {
        bool isdig = std::isdigit(line[i]);
        if (isdig) {
            if (is_part_number(above, i) ||
                is_part_number(line, i) ||
                is_part_number(below, i))
                is_part = true;
            number.push_back(line[i]);
        }

        if (!isdig || i+1 == len) {
            // end of number
            if (is_part) {
                sum += stoi(number);
            }
            number.clear();
            is_part = false;
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
    }

    above = line;
    line = below;
    below = dots;
    sum += process_line(above, line, below);

    assert(sum == 557705);
    std::cout << sum << "\n";
    return 0;
}
