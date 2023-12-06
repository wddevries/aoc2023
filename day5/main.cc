#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <limits>
#include <cassert>

using namespace std;

struct val_range {
    long value;
    long range;
    val_range(long value, long range) : value(value), range(range) {}
};

vector<long>
read_seeds(ifstream &f)
{
    vector<long> v;

    string seed_ln;
    getline(f, seed_ln);
    istringstream ss(seed_ln);

    string title;
    ss >> title;

    string seed_str;
    while (ss >> seed_str) {
        long seedid = stol(seed_str);
        v.push_back(seedid);
    }

    string blank;
    getline(f, blank);

    assert(v.size() % 2 == 0);

    return v;
}

struct map_item {
    long dest;
    long src;
    long range;

    map_item(long dest, long src, long range) :
        dest(dest), src(src), range(range) {}


    bool
    in_range(long v)
    {
        return src <= v && v < src + range;
    }

    long
    mapped_value(long v) {
        return dest + (v - src);
    }

    friend bool
    operator<(const map_item& l, const map_item& r) {
        return l.src < r.src;
    }
};

vector<map_item>
read_map(ifstream &f)
{
    vector<map_item> maps;

    string title;
    getline(f, title);

    while (!f.eof()) {
        string ln;
        getline(f, ln);
        if (ln.empty()) {
            break;
        }

        istringstream ss(ln);
        long dest, src, range;

        ss >> dest;
        ss >> src;
        ss >> range;
        maps.push_back(map_item(dest, src, range));
    }

    sort(maps.begin(), maps.end());

    return maps;
}

vector<val_range>
seeds_to_ranges_a(vector<long> seed_values)
{
    vector<val_range> seeds;
    for (int i = 0; i < seed_values.size() - 1; i++) {
        long value = seed_values[i];
        long range = 1;
        seeds.push_back(val_range(value, range));
    }
    return seeds;
}

vector<val_range>
seeds_to_ranges_b(vector<long> seed_values)
{
    vector<val_range> seeds;
    for (int i = 0; i < seed_values.size() - 1; i++) {
        long value = seed_values[i++];
        long range = seed_values[i];
        seeds.push_back(val_range(value, range));
    }
    return seeds;
}

vector<val_range>
process_range(vector<val_range> rngs, vector<map_item> maps)
{
    vector<val_range> out;
    for (auto r : rngs) {
        long start = r.value;
        long count = r.range;

        // maps are in order of src
        for (auto m : maps) {

            // push any of range before this map
            if (start < m.src) {
                long end = min(start + count, m.src) - start;

                out.push_back(val_range(start, end));

                count -= end;
                start = start + end;

                if (!count)
                    break; // nothing less of range r;
            }

            assert(start >= m.src);

            // push any overlapping part
            if (m.src <= start && start < m.src + m.range) {
                long end = min(start + count, m.src + m.range) - start;

                long start2 = m.dest + (start - m.src);
                out.push_back(val_range(start2, end));

                count -= end;
                start = start + end;

                if (!count)
                    break; // nothing less of range r;
            }

            assert(start > m.src + m.range - 1);
        }

        // add parts not caught by maps
        if (count)
            out.push_back(val_range(start, count));
    }
    return out;
}

int
main ()
{
    ifstream f("data.txt");
    if (!f) {
        cout << "Could not open file.";
        exit(-1);
    }

    vector<long> seed_values = read_seeds(f);
    vector<map_item> seed_soil = read_map(f);
    vector<map_item> soil_fertilizer = read_map(f);
    vector<map_item> fertilizer_water = read_map(f);
    vector<map_item> water_light = read_map(f);
    vector<map_item> light_temperature = read_map(f);
    vector<map_item> temperature_humidity = read_map(f);
    vector<map_item> humidity_location = read_map(f);


    vector<val_range> rngs = seeds_to_ranges_a(seed_values);
    rngs = process_range(rngs, seed_soil);
    rngs = process_range(rngs, soil_fertilizer);
    rngs = process_range(rngs, fertilizer_water);
    rngs = process_range(rngs, water_light);
    rngs = process_range(rngs, light_temperature);
    rngs = process_range(rngs, temperature_humidity);
    rngs = process_range(rngs, humidity_location);

    long lowest_a = LONG_MAX;
    for (auto &r : rngs) {
        lowest_a = min(r.value, lowest_a);
    }

    assert(lowest_a == 388071289);
    cout << "Part A: " << lowest_a << "\n";

    rngs = seeds_to_ranges_b(seed_values);
    rngs = process_range(rngs, seed_soil);
    rngs = process_range(rngs, soil_fertilizer);
    rngs = process_range(rngs, fertilizer_water);
    rngs = process_range(rngs, water_light);
    rngs = process_range(rngs, light_temperature);
    rngs = process_range(rngs, temperature_humidity);
    rngs = process_range(rngs, humidity_location);

    long lowest_b = LONG_MAX;
    for (auto &r : rngs) {
        lowest_b = min(r.value, lowest_b);
    }

    assert(lowest_b == 84206669);
    cout << "Part B: " << lowest_b << "\n";

    return 0;
}
