#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cassert>

enum class keywords {
    GAME,
    INT,
    RED,
    GREEN,
    BLUE,
    COMMA,
    COLON,
    SEMICOLON,
    NEWLINE,
    END,
};

class lexer {
    std::ifstream &f;
    keywords next;
    int offset;
    int line;
    int number;

    char
    read_char() {
        char ch = 0;
        ch = f.get();
        offset++;
        return ch; 
    }

    char
    checked_read() {
        char ch = read_char();
        if (f.eof()) {
            std::cerr << "Charector expected at " << line << ":"
                      << offset << ".\n";
            exit(-1);
        }

        return ch;
    }

    void
    report_invalid_ch(char ch, std::string c)
    {
        std::cerr << "Invalid input character at " << line << ":" << offset
                  << ". " << "Found " << ch << " but expected " << c << ".\n";
        exit(-1);
    }

    void
    consume(char c) {
        char ch = checked_read();
        if (ch != c)
            report_invalid_ch(ch, std::string{c});
    }

    keywords
    advance()
    {
again:
        char ch = read_char();
        if (f.eof())
            return keywords::END;

        switch (ch) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            f.putback(ch);
            f >> number;
            return keywords::INT;
        case ' ':
            goto again;
        case 'r':
            consume('e');
            consume('d');
            return keywords::RED; 
        case 'G':
            consume('a');
            consume('m');
            consume('e');
            return keywords::GAME; 
        case 'g':
            consume('r');
            consume('e');
            consume('e');
            consume('n');
            return keywords::GREEN; 
        case 'b':
            consume('l');
            consume('u');
            consume('e');
            return keywords::BLUE; 
        case ',':
            return keywords::COMMA; 
        case ':':
            return keywords::COLON; 
        case ';':
            return keywords::SEMICOLON; 
        case '\n':
        case '\r':
            line++;
            return keywords::NEWLINE;
        }

        report_invalid_ch(ch, "something else");
        return keywords::END;
    }

public:
    lexer(std::ifstream &f) : f(f), offset(-1), line(0) {
        next = advance();
    }

    lexer& operator++()
    {
        next = advance();
        return *this;
    }
    
    lexer operator++(int)
    {
        lexer old = *this; // copy old value
        operator++();  // prefix increment
        return old;    // return old value
    }
                                             
    keywords
    operator*()
    {
        return next;
    }

    int
    get_number()
    {
        assert(next == keywords::INT);
        return number;
    }

    void
    unexpected_token(keywords expected)
    {
        std::cerr << "Invalid symbol at " << line << ":" << offset << "\n";
        exit(-1);
    }
};

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

bool
accept(lexer &lex, keywords k)
{
    if (*lex == k) {
        lex++;
        return true;
    }

    return false;
}

bool
expect(lexer &lex, keywords k)
{
    if (accept(lex, k)) {
        return true;
    }

    lex.unexpected_token(k);
    return false;
}

game
parse_game(lexer &lex)
{
    game g;

    expect(lex, keywords::GAME);
    if (*lex == keywords::INT)
        g.gameid = lex++.get_number();
    else
        lex.unexpected_token(keywords::INT);
    expect(lex, keywords::COLON);

    int red = 0, green = 0, blue = 0;
    while (*lex == keywords::INT) {
        int count = lex.get_number();
        lex++;
        
        if (accept(lex, keywords::RED)) {
            red += count;
        } else if (accept(lex, keywords::GREEN)) {
            green += count;
        } else if (expect(lex, keywords::BLUE)) {
            blue += count;
        }

        if (accept(lex, keywords::COMMA))
            continue;

        if (accept(lex, keywords::SEMICOLON) || 
            accept(lex, keywords::NEWLINE) ||
            expect(lex, keywords::END)) {
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


    lexer lex(f);
    int sum1 = 0, sum2 = 0;
    while (*lex != keywords::END) {
        game g = parse_game(lex);

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
