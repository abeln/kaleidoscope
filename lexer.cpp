#include <string>

enum class Token {
    tok_eof,

    // commands
    tok_def,
    tok_extern,

    // primary
    tok_id,
    tok_num,
    tok_unknown
};

std::string id_val; // Filled in if tok_id lexed
char unknown_val;   // Filled in if tok_unknown lexed
double num_val;     // Filled in if tok_num lexed

// Is `c` part of a number?
bool is_num_char(char c) {
    return isdigit(c) || c == '.';
}

// Computes the next token. Possibly fills in `tok_id` or `tok_num` as a side effect.
Token get_token() {
    int last_char = getchar();
    while (isspace(last_char)) last_char = getchar();

    if (isalpha(last_char)) {
        id_val = static_cast<char>(last_char);
        last_char = getchar();
        while (isalpha(last_char)) {
            id_val += static_cast<char>(last_char);
            last_char = getchar();
        }

        if (id_val == "def") return Token::tok_def;
        if (id_val == "extern") return Token::tok_extern;
        return Token::tok_id;
    }

    if (is_num_char(last_char)) {
        std::string num_str;
        do {
            num_str += static_cast<char>(last_char);
            last_char = getchar();
        } while (is_num_char(last_char));

        num_val = strtod(num_str.c_str(), 0);
        return Token::tok_num;
    }

    if (last_char == '#') {
        // skip comment until end-of-line
        do {
            last_char = getchar();
        } while (last_char != EOF && last_char != '\n' && last_char != '\r');

        if (last_char != EOF) return get_token();
    }

    unknown_val = last_char;
    return Token::tok_unknown;
}

