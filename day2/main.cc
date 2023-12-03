#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

struct draw {
    int red;
    int green;
    int blue;

    draw(int red, int green, int blue) : red(red), green(green), blue(blue) { }
};

struct game {
    int gameid;
    std::vector<draw> draws;
};

void
check_str(std::string &s, std::string exp)
{
    if (s != exp) {
        std::cerr << "Unexpected value (" << s << ") in stream.  Expected " << exp << ".\n";
        exit(-1);
    }
}

void
consume(std::istringstream &f, std::string val)
{
    std::string s;
    f >> s;
    check_str(s, val);
}

game
parse_game(std::string ln)
{
    game g;
    std::istringstream is(ln);

    consume(is, "Game");
    is >> g.gameid;
    consume(is, ":");

    int red = 0, green = 0, blue = 0;
    while (is.good()) {
        int count;
        is >> count;
        std::string color;
        is >> color;
        
        if (color.starts_with("red")) {
            red += count;
        } else if (color.starts_with("green")) {
            green += count;
        } else if (color.starts_with("blue")) {
            blue += count;
        }

        if (!color.ends_with(",")) {
            g.draws.push_back(draw(red, green, blue));
            red = green = blue = 0;
        }
    }

    return std::move(g);
}

const int max_red = 12;
const int max_green = 13;
const int max_blue = 14;

int
main()
{
    std::ifstream f("data.txt");
    if (!f) {
        std::cerr << "Could not open file!\n";
    }

    int sum1 = 0, sum2 = 0;
    while (!f.eof()) {
        std::string ln;
        std::getline(f, ln);
        if (!ln.length())
            continue;

        game g = parse_game(ln);
        bool p1_good = true;
        int p2_red = 0, p2_green = 0, p2_blue = 0;
        for (auto &d : g.draws) {
            //part 1
            if (d.red > max_red || d.green > max_green || d.blue > max_blue)
                p1_good = false;

            //part 2
            if (p2_red < d.red)
                p2_red = d.red;
            if (p2_green < d.green)
                p2_green = d.green;
            if (p2_blue < d.blue)
                p2_blue = d.blue;
        }
        
        if (p1_good)
            sum1 += g.gameid;

        sum2 += p2_red * p2_green * p2_blue;
    }

    std::cout << "Part 1 " << sum1 << "\n";
    assert(sum1 == 1867);
    std::cout << "Part 2 " << sum2 << "\n";
    assert(sum2 == 84538);
}
