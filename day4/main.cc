#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <cassert>

int
main() {
    std::fstream f("data.txt");

    if (!f) {
        std::cerr << "No file?";
        exit(-1);
    }

    std::string ln;
    int cardid = 0;
    int cards[205];

    while (getline(f, ln)) {
        if (ln.empty())
            continue;

        ln = ln.substr(ln.find(":") + 1);

        std::set<int> winners;
        std::istringstream wn (ln.substr(0, ln.find("|")));
        int wnum;
        while (wn >> wnum)
            winners.insert(wnum);

        std::istringstream nums (ln.substr(ln.find("|") + 1));
        int num;
        int cnt = 0;
        while (nums >> num) {
            if (winners.contains(num))
                cnt++;
        }

        cards[cardid++] = cnt;
    }
    
    // part 1
    int points = 0;
    for (int i = 0; i < cardid; i++) {
        int cnt = cards[i];
        if (cnt)
            points += 1 << (cnt - 1);
    }
    
    assert(points == 20829);
    std::cout << "points " << points << "\n";

    // part 2
    int card_cnt[cardid];
    for (int i = 0; i < cardid; i++)
        card_cnt[i] = 1;
    for (int i = 0; i < cardid; i++) {
        int cnt = cards[i];
        for (int j = 1; j <= cnt; j++)
            card_cnt[i + j] += card_cnt[i];
    }

    int sum = 0;
    for (int i = 0; i < cardid; i++) {
        sum += card_cnt[i];
    }

    assert(sum == 12648035);
    std::cout << "sum " << sum << "\n";
}
