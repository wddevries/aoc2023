#include <fstream>
#include <iostream>
#include <vector>
#include <assert.h>

using std::cout;

struct trie;
struct trie {
    enum class trie_type { letter, leaf } type;
    //enum trie_type type;
    char letter;
    int value;
    std::vector<trie *> nodes;

    trie(char letter) :
        type(trie_type::letter), letter(letter), value(0)
    {
    }
};

// add or find the letter and return the trie node
trie *
trie_add_letter(trie *t, char letter)
{
    for (trie *tn : t->nodes)
        if (tn->letter == letter)
            return tn;

    trie *nt = new trie(letter);
    t->nodes.push_back(nt);

    return nt;
}

trie *
trie_build(std::vector<std::string> words, std::vector<int> values)
{
    trie *head = new trie(0);
    assert(words.size() == values.size());

    auto itr = values.begin();
    for (auto w : words) {
        trie *cur = head;
        int val = *itr;

        for (auto c : w) {
            cur = trie_add_letter(cur, c);
        }

        // value on leaf
        cur->type = trie::trie_type::leaf;
        cur->value = val;

        itr++;
    }

    return head;
}

trie *
trie_find(trie *t, char letter)
{
    for (trie *tn : t->nodes) {
        if (tn->letter == letter)
            return tn;
    }

    return nullptr;
}

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


int
main()
{
    std::vector<std::string> words = { "one", "two", "three", "four", "five",
                                       "six", "seven", "eight", "nine", "1",
                                       "2", "3", "4", "5", "6", "7", "8", "9" };
    std::vector<int> values = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 1, 2, 3, 4, 5, 6, 7,
                                8, 9 };
    trie *head = trie_build(words, values);

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
            trie *cur = head;
            for (int j = i; j < len; j++) {
                cur = trie_find(cur, ln[j]);
                if (!cur)
                    break;
                if (cur->type == trie::trie_type::leaf) {
                    lc = cur->value;
                    if (!sc)
                        sc = lc;
                    break;
                }
            }
        }

        sum += sc * 10 + lc;
    }

    assert(sum == 53515);
    cout << sum << "\n";
    return 0;
}
