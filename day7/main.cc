#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cassert>

using namespace std;

struct hand {
    string cards;
    int bid;
    hand(string cards, int bid) : cards(cards), bid(bid) { }
};

class hand_comparator {
    bool j_is_wild;

    int
    get_hand_type(const hand &h)
    {
        string copy = h.cards;
        sort(copy.begin(), copy.end());

        int singles = 0;
        int pairs = 0;
        int threes = 0;
        int fours = 0;
        int fives = 0;
        int jays = 0;

        int len = copy.length();
        int cnt = 1;
        for (int i = 0; i < len; i++) {
            if (j_is_wild && copy[i] == 'J') {
                jays++;
                cnt = 1;
                continue;
            }

            if (i + 1 == len || copy[i] != copy[i + 1]) {
                switch (cnt) {
                case 5:
                    fives++;
                    break;
                case 4:
                    fours++;
                    break;
                case 3:
                    threes++;
                    break;
                case 2:
                    pairs++;
                    break;
                case 1:
                    singles++;
                    break;
                default:
                    cout << "oh no!";
                    exit(-1);
                }

                cnt = 1;
            } else {
                cnt++;
            }
        }

        int rank = 0;
        if (fives || (fours && jays) || (threes && jays == 2) ||
            (pairs && jays == 3) || jays == 4 || jays == 5)
            rank = 6;
        else if (fours || (threes && jays) || (pairs && jays == 2) || jays == 3)
            rank = 5;
        else if ((threes && pairs == 1) || (pairs == 2 && jays))
            rank = 4;
        else if (threes || (pairs && jays) || jays == 2)
            rank = 3;
        else if (pairs == 2)
            rank = 2;
        else if (pairs == 1 || jays)
            rank = 1;
        else
            rank = 0;

        return rank;
    }

    int
    compare_card(char l, char r)
    {
        const char * vals = "AKQJT98765432";
        if (j_is_wild)
            vals = "AKQT98765432J";

        if (l == r)
            return 0;

        char *lv = index(vals, l);
        char *rv = index(vals, r);
        // lower index is greater
        if (lv < rv)
            return 1;
        else
            return -1;
    }

    bool
    less_than(const hand& l, const hand& r)
    {
        int lrank = get_hand_type(l);
        int rrank = get_hand_type(r);
        if (lrank == rrank) {
            for (int i = 0; i < l.cards.length(); i++) {
                if (l.cards[i] == r.cards[i])
                    continue;
                return compare_card(l.cards[i], r.cards[i]) < 0;
            }

            return false;
        }

        return lrank < rrank;
    }

public:
    bool operator()(const hand &l, const hand &r)
    {
        return less_than(l, r);
    }

    hand_comparator(bool j_is_wild) : j_is_wild(j_is_wild) {}
};

vector<hand>
read_hands(ifstream &f)
{
    vector<hand> v;
    string ln;
    while (getline(f, ln)) {
        istringstream ss(ln);
        string cards;
        ss >> cards;
        int bid;
        ss >> bid;
        v.push_back(hand(cards, bid));
    }

    return v;
}

int
calculate_sum(vector<hand> hands) {
    int rank = 1;
    int sum = 0;
    for (auto h : hands)
        sum += rank++ * h.bid;
    return sum;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<hand> hands = read_hands(f);

    // part 1
    sort(hands.begin(), hands.end(), hand_comparator(false));
    int sum1 = calculate_sum(hands);
    assert(sum1 == 246912307);
    cout << "Part 1: " << sum1 << "\n";

    sort(hands.begin(), hands.end(), hand_comparator(true));
    int sum2 = calculate_sum(hands);
    assert(sum2 == 246894760);
    cout << "Part 2: " << sum2 << "\n";

    return 0;
}
