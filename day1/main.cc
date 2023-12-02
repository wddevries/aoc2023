#include <fstream>
#include <iostream>
#include <assert.h>

bool
is_number(std::string ln, int offset, const char *num)
{
    int len = ln.length();

    int i = offset;
    for (; i < len; i++)
        if (ln[i] != num[i - offset])
            break;

    if (num[i - offset] == 0)
        return true;

    return false;
}

const char *words[] = { "one", "two", "three", "four", "five", "six", "seven",
                        "eight", "nine", "1", "2", "3", "4", "5", "6", "7",
                        "8", "9", NULL };
int values[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0 };

int
main()
{
    std::ifstream f("data.txt");
    if (!f) {
        std::cerr << "Could not open file!\n";
        return -1;
    }

    int sum  = 0;
    while (f.good()) {
        int sc = 0, lc = 0;
        std::string ln;
        f >> ln;

        int len = ln.length();
        if (!len)
            continue;
        for (int i = 0; i < len; i++) {
            char c = 0;
            for (int j = 0; words[j]; j++) {
                const char *w = words[j];
                if (is_number(ln, i, w)) {
                    lc = values[j];
                    if (!sc)
                        sc = lc;
                    break;
                }
            }
        }

        sum += sc * 10 + lc;
    }

    assert(sum == 53515);
    std::cout << sum << "\n";
    return 0;
}
