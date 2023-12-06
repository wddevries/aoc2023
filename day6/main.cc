#include <fstream>
#include <iostream>
#include <vector>
#include <cassert>

using namespace std;

long
calculate_output(vector<pair<long, long>> &races)
{
    long i = 0;
    long output = 0;
    for (auto r : races) {
        long wins = 0;
        for (long hold = 0; hold <= r.first; hold++) {
            long d = (hold * (r.first - hold));
            if (d > r.second)
                wins++;
        }

        if (!output)
            output = wins;
        else
            output *= wins;
    }

    return output;
}

int
main() 
{
    vector<pair<long, long>> races1 = { {54, 239}, {70, 1142}, {82, 1295}, {75, 1253} };
    long output = calculate_output(races1);
    assert(output == 800280);
    cout << output << "\n";

    vector<pair<long, long>> races2 = { { 54708275, 239114212951253 }};
    output = calculate_output(races2);
    assert(output == 45128024);
    cout << output << "\n";

    
}
