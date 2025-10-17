/*
    THIS IS REALLY A LEXER FOR THE SFK PROGRAMMING LANGUAGE.
    IF YOU ARE A FAN OF CLEAN CODE, THEN FUCK YOU!!!

                                            -SlonikFromKartohka
*/

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

namespace sfk {
    enum TokenType {
        // Types
        IDENTIFIER,
        INT,
        FLOAT,

        // KEYWORDS
        SLONIK,
        FROM,           
        KARTOHKA,       
        OUTPUT,             // >>

        // Separators
        COLON               // .
    };

    struct Token {           // I swear brother, this is the Token class
        Token(TokenType t, std::string v, size_t p, size_t l) : type(t), value(v), pos(p), line(l) {}    // It's a constructor, really.

        TokenType type;             // Well, supposedly this is a type of token
        std::string value;          // Epta meaning
        size_t pos;                 // POSITION
        size_t line;                // LINE
    };

    std::vector<Token> tokenize(std::string source) {      
        std::vector<Token> tokens;
        size_t pos = 0;
        size_t line =  1; 
        /*
        ⣿⣿⣿⣿⣿⣿⣿⣿⠟⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⣿
        ⣿⣿⣿⣿⣿⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢺⣿
        ⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠆⠜⣿
        ⣿⣿⣿⣿⠿⠿⠛⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠉⠻⣿⣿
        ⣿⣿⡏⠁⠀⠀⠀⠀⠀⣀⣠⣤⣤⣶⣶⣶⣶⣶⣦⣤⡄⠀⠀⠀⠀⢀⣴⣿
        ⣿⣿⣷⣄⠀⠀⠀⢠⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⢿⡧⠇⢀⣤⣶
        ⣿⣿⣿⣿⣿⣿⣾⣮⣭⣿⡻⣽⣒⠀⣤⣜⣭⠐⢐⣒⠢⢰
        ⣿⣿⣿⣿⣿⣿⣿⣏⣿⣿⣿⣿⣿⣿⡟⣾⣿⠂⢈⢿⣷⣞
        ⣿⣿⣿⣿⣿⣿⣿⣿⣽⣿⣿⣷⣶⣾⡿⠿⣿⠗⠈⢻⣿
        ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠻⠋⠉⠑⠀⠀⢘⢻
        ⣿⣿⣿⣿⣿⣿⣿⡿⠟⢹⣿⣿⡇⢀⣶⣶⠴⠶⠀⠀⢽
        ⣿⣿⣿⣿⣿⣿⡿⠀⠀⢸⣿⣿⠀⠀⠣⠀⠀⠀⠀⠀⡟⢿⣿
        ⣿⣿⣿⡿⠟⠋⠀⠀⠀⠀⠹⣿⣧⣀⠀⠀⠀⠀⡀⣴⠁⢘⡙
        ⠉⠉⠁⠀⠀⠀⠀⠀⠀⠀⠀⠈⠙⢿⠗⠂⠄⠀⣴⡟⠀⠀⡃
        */ 
       while(pos < source.length()) {
            char current = source[pos];
            if(current == ' ' || current == '\t') {
                ++pos;
                continue;
             } else if(current == '\n') {
                ++pos;
                ++line;
                continue;
             } else if(current == '%' && pos+1 < source.length() && source[pos+1] == '%') {
                pos += 2;
                while(pos < source.length() && source[pos] != '\n') {
                    ++pos;
                }
                continue;
            } else if(current == '>' && pos+1 < source.length() && source[pos+1] == '>') {
                tokens.push_back(Token(OUTPUT, ">>", pos, line));
                pos += 2;
                continue;
            } else if(std::isalpha(current) || current == '_') {        // Fuck checking this is an indexer or a keyword 
                size_t start = pos;
                while(pos < source.length() && (std::isalnum(source[pos]) || source[pos] == '_')) {
                    ++pos;
                }
                std::string value = source.substr(start, pos - start);
                TokenType type = IDENTIFIER;
                if(value == "slonik") {
                    type = SLONIK;
                } else if(value == "from") {
                    type = FROM;
                } else if(value == "kartohka") {
                    type = KARTOHKA;
                }
                tokens.push_back(Token(type, value, start, line));
                continue;
            } else if(isdigit(current)) {
                size_t start = pos;
                bool isFloat = false;
                while(pos < source.length() && (isdigit(source[pos]) || source[pos] == '.')) {
                    if(source[pos] == '.') {
                        if(!isFloat) {
                            isFloat = true;
                            ++pos;
                        } else {
                            throw std::runtime_error("ERROR: extra point on line " + std::to_string(line));
                        }
                    } else {
                        ++pos;
                    }
                }
                std::string value = source.substr(start, pos - start);
                if(!isFloat) {
                    tokens.push_back(Token(INT, value, start, line));
                } else {
                    tokens.push_back(Token(FLOAT, value, start, line));
                }
                continue;
            } else {
                throw std::runtime_error("ERROR: UNKNOWN TOKEN '" + std::string(1, current) + "' on line " + std::to_string(line));
            }
        }
        return tokens;
    }

    /*⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣀⣀⣀⣠⠤⠤⢤⣀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣠⠶⠋⠉⣻⠿⠿⠟⠿⠛⠛⠛⠛⠛⠛⠒⠒⠒⠋⠁⢩⣳⣶⡶⠶⡶⢶⠶⣶⣶⣤⡀⠀⠀
⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣀⣀⣤⣤⡴⠶⢶⣾⣿⣿⣦⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣤⣤⡴⣳⡞⠁⣤⣥⣾⣿⣿⣿⣿⣿⣆⠀
    ⠀⠀⠀⠀⠀⠀⣀⣀⣠⣤⣤⠤⠴⠖⠚⢿⡿⣿⣍⣉⣋⣹⣎⠙⠻⣷⣷⣿⣯⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣠⣤⡿⠛⠋⠛⠃⣸⣿⡿⢿⣿⣿⡏⠛⠹⣿⡆
    ⠀⠀⠀⣠⣴⣾⡿⣟⢻⣷⣤⣀⣀⣠⣤⣤⣷⣿⣷⣶⣶⣶⣿⡶⣶⣿⡟⣿⣻⡆⠘⡆⢲⠈⢁⣀⣀⣀⣀⡤⠿⠿⢿⣖⢒⣚⣋⢉⡽⢷⣀⣸⣿⣿⣿⡆⢠⣿⡇
    ⠀⠀⣰⣿⣿⣧⠀⠉⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠛⠛⡛⣻⡇⠀⢸⡋⢻⣿⣇⠀⢿⣿⣻⣿⠟⣟⡏⡗⡶⡶⡒⡟⢿⡟⡟⡼⣫⣹⣸⣿⣽⣯⣿⣿⣷⠁⣿⡇
    ⢰⣿⣿⣿⣙⣾⣦⣞⣹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⡇⢾⢿⣷⣼⣷⡟⠀⢨⣿⣿⣿⡄⣸⣹⣿⣽⣼⣽⣼⣼⣾⣷⣿⣷⣶⣿⣿⣿⣿⣿⣿⣯⣾⡿⠁
    ⠈⠉⠻⣿⣿⣏⡉⠉⢩⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⣀⣸⣿⣿⣿⡇⢠⡟⡛⠛⠛⠃⠬⠿⠛⠛⣿⣶⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡟⠁⠀
⠀⠀⠀ ⠙⠛⠿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠉⣹⣿⣿⣿⣿⣤⣼⣿⣷⣖⡒⠒⢒⡒⢚⠛⢻⣷⣬⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⠉⠀⠀⠀
⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠉⠉⠉⠀⠀⠀⠈⠙⠛⠛⠛⠉⠉⠉⠛⠋⠉⠛⢻⣟⣿⡟⠻⣿⣿⣿⣿⣿⡿⠇⠀⠀⢠⣿⣿⣿⣿⣿⣿⡿⠿⠟⠛⠉⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿⣿⣿⣇⠀⠀⠀⠀⠀⠀⠀⣰⡿⣿⠞⠋⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⢹⡟⣿⣿⡀⠀⠀⢀⣀⣀⣴⠋⢱⠏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣼⣏⣾⣾⣿⡗⣷⣿⣿⣿⣿⡟⣿⢧⣸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠈⠉⠻⠿⣯⣴⣿⣿⣿⣿⣿⣧⡾⣾⠋⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⣿⡄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⢹⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⢸⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿⣿⣿⣿⣿⣿⣿⢸⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢻⣿⣿⣿⣿⣿⣿⣿⣿⠀⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣧⡯⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣀⣠⣾⡿⣿⣿⣿⣿⣿⣿⣿⢿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣤⠶⠿⢿⣿⣿⣿⣿⣷⣾⣿⣿⡿⣇⣲⣾⣿⣿⣷⣤⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣴⠞⠯⠀⡤⠖⠛⠉⠈⠙⠻⢿⣿⣿⣿⡿⠿⠟⠛⢛⣉⣩⣭⠷⠿⢷⣄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡼⠻⢦⣀⠚⠄⠀⠀⠀⠀⠀⠂⠀⠉⠻⠤⠴⠒⣚⣋⣉⣥⣴⣶⣾⣿⣿⣿⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⢠⠎⢀⣴⠃⠈⠙⠳⢤⣄⡠⠀⣇⠀⣀⣠⣤⣶⣶⣿⣿⣿⣯⣿⣷⣿⣍⣹⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⡴⠃⣠⡾⠃⠀⠀⢩⠄⠀⠀⠉⡉⠉⢳⣻⣿⣿⣿⣿⣿⣿⡿⠿⢟⣛⣻⡭⠭⠖⠛⢃⠀⠀⠀⠀⠀⠀⠀⠀⠀
    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣇⢰⣧⣄⡀⠀⡰⠃⠀⠀⢀⠞⠀⠀⠀⢿⣛⣻⡭⠭⠷⠒⠚⠉⠉⢀⣀⣠⣤⣤⣶⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣯⣙⢦⡞⠁⠀⠀⢠⠋⠀⠀⠀⠀⠈⢁⣀⣠⣤⣴⣶⣶⠿⠿⠟⣛⣋⣹⡉⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣷⣦⣤⣤⣧⣤⣴⣶⣶⣿⡿⠟⢻⣿⣿⡟⠀⠀⣶⣶⣾⣿⣿⣿⣷⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠀⠘⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠉⠀⣾⣿⣿⣫⣿⣿⣿⣶⣶⣾⣿⣿⠿⠿⠛⠛⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠀⠉⠻⢿⣿⣿⣿⣿⣿⣶⣶⣿⣿⣿⠿⠿⠟⠛⠋⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀    ⠀⠀⠀⠀⠈⢙⢛⠛⠛⠉⠉⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀
    */
}

int main() {
    const std::string source = R"(
    slonik from
        >> 43.2
    kartohka)";
    std::vector<sfk::Token> tokens = sfk::tokenize(source);
    int index = 1;
    auto tokenTypeToString = [](sfk::TokenType type) -> std::string {
        switch(type) {
            case sfk::IDENTIFIER: return "IDENTIFIER";
            case sfk::SLONIK:     return "SLONIK";
            case sfk::FROM:       return "FROM";
            case sfk::KARTOHKA:   return "KARTOHKA";
            case sfk::OUTPUT:     return "OUTPUT";
            case sfk::INT:        return "INT";
            case sfk::FLOAT:      return "FLOAT";
            default:              return "UNKNOWN";
        }
    };
    for(sfk::Token token : tokens) {
        std::cout << "TOKEN "  << index                          << std::endl;
        std::cout << "TYPE: "  << tokenTypeToString(token.type)  << std::endl;
        std::cout << "VALUE: " << token.value                    << std::endl;
        std::cout << "LINE: "  << token.line                     << std::endl;
        std::cout << "POS: "   << token.pos                      << std::endl;
        std::cout << "------"                                    << std::endl;
        ++index;
    }
    return 0;
}

/*
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿
    ⣿⠟⠫⢻⣿⣿⣿⣿⢟⣩⡍⣙⠛⢛⣿⣿⣿⠛⠛⠛⠛⠻⣿⣿⣿⣿⣿⡿⢿⣿
    ⣿⠤⠄⠄⠙⢿⣿⣿⣿⡿⠿⠛⠛⢛⣧⣿⠇⠄⠂⠄⠄⠄⠘⣿⣿⣿⣿⠁⠄⢻
    ⣿⣿⣿⣿⣶⣄⣾⣿⢟⣼⠒⢲⡔⣺⣿⣧⠄⠄⣠⠤⢤⡀⠄⠟⠉⣠⣤⣤⣤⣾
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣀⣬⣵⣿⣿⣿⣶⡤⠙⠄⠘⠃⠄⣴⣾⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢻⠿⢿⣿⣿⠿⠋⠁⠄⠂⠉⠒⢘⣿⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⣿⡿⣡⣷⣶⣤⣤⣀⡀⠄⠄⠄⠄⠄⠄⠄⣾⣿⣿⣿⣿⣿⣿
    ⣿⣿⣿⣿⣿⣿⣿⡿⣸⣿⣿⣿⣿⣿⣿⣿⣷⣦⣰⠄⠄⠄⠄⢾⠿⢿⣿⣿⣿⣿
    ⣿⡿⠋⣡⣾⣿⣿⣿⡟⠉⠉⠈⠉⠉⠉⠉⠉⠄⠄⠄⠑⠄⠄⠐⡇⠄⠈⠙⠛⠋
    ⠋⠄⣾⣿⣿⣿⣿⡿⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄⢠⡇⠄⠄⠄⠄⠄
    ⠄⢸⣿⣿⣿⣿⣿⣯⠄⢠⡀⠄⠄⠄⠄⠄⠄⠄⠄⣀⠄⠄⠄⠄⠁⠄⠄⠄⠄⠄
    ⠁⢸⣿⣿⣿⣿⣿⣯⣧⣬⣿⣤⣐⣂⣄⣀⣠⡴⠖⠈⠄⠄⠄⠄⠄⠄⠄⠄⠄⠄
    ⠈⠈⣿⣟⣿⣿⣿⣿⣿⣿⣿⣿⣽⣉⡉⠉⠈⠁⠄⠁⠄⠄⠄⠄⡂⠄⠄⠄⠄⠄
    ⠄⠄⠙⣿⣿⠿⣿⣿⣿⣿⣷⡤⠈⠉⠉⠁⠄⠄⠄⠄⠄⠄⠄⠠⠔⠄⠄⠄⠄⠄
    ⠄⠄⠄⡈⢿⣷⣿⣿⢿⣿⣿⣷⡦⢤⡀⠄⠄⠄⠄⠄⠄⢐⣠⡿⠁⠄⠄⠄⠄⠄
 */