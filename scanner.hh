#pragma once
#include "cpplox.hh"
#include "token.hh"

namespace scanner {
        class Scanner {
                std::wstring source;
                std::vector<token::Token> tokens;
                size_t start;
                size_t current;
                size_t line;
                size_t comment_embeds;

                const std::unordered_map<std::wstring, token::token_type> keywords = {
                        {L"and",    token::AND},
                        {L"class",  token::CLASS},
                        {L"else",   token::ELSE},
                        {L"false",  token::FALSE},
                        {L"for",    token::FOR},
                        {L"fun",    token::FUN},
                        {L"if",     token::IF},
                        {L"nil",    token::NIL},
                        {L"or",     token::OR},
                        {L"return", token::RETURN},
                        {L"super",  token::SUPER},
                        {L"this",   token::THIS},
                        {L"true",   token::TRUE},
                        {L"var",    token::VAR},
                        {L"while",  token::WHILE},
                };

                fn is_at_end() -> bool {
                        return current >= source.length();
                }

                fn advance() -> wchar_t {
                        return source.at(current++);
                }

                fn is_alpha_numeric(wchar_t c) -> bool {
                        return std::iswalpha(c) || std::iswdigit(c);
                }

                fn add_token(token::token_type type, std::any literal) {
                        std::wstring text = source.substr(start, current-start);
                        tokens.push_back(token::Token(type, text, literal, line));
                }

                fn add_token(token::token_type type) {
                        add_token(type, std::any());
                }

                fn match(wchar_t expected) -> bool {
                        if (source.at(current) != expected || is_at_end()) {
                                return false;
                        }

                        current++;
                        return true;
                }

                fn peek() -> wchar_t {
                        if (is_at_end()) {
                                return '\0';
                        }
                        return source.at(current);
                }

                fn peek_next() -> wchar_t {
                        if (current + 1 >= source.length()) {
                                return '\0';
                        }
                        return source.at(current + 1);
                }

                fn string() {
                        while (peek() != '"' && !is_at_end()) {
                                if (peek() == '\n') {
                                        line++;
                                }
                                advance();
                        }

                        if (is_at_end()) {
                                errors::error(line, L"Unterminated string.");
                                return;
                        }
                        advance();

                        std::wstring v = source.substr(start + 1, current-start - 2);
                        add_token(token::STRING, v);
                }

                fn number() {
                        while (std::iswdigit(peek())) {
                                advance();
                        }

                        if (peek() == '.' && std::iswdigit(peek_next())) {
                                advance();
                                while (std::iswdigit(peek())) {
                                        advance();
                                }
                        }

                        add_token(token::NUMBER,
                                std::stod(source.substr(start, current-start))
                        );
                }

                fn identifier() {
                        while (is_alpha_numeric(peek())) {
                                advance();
                        }

                        std::wstring key = source.substr(start, current-start);
                        token::token_type type = (
                                keywords.contains(key)
                        ) ? keywords.at(key)
                          : token::IDENTIFIER;

                        add_token(type);
                }

                fn comment() {
                        while (peek() != '\n' && !is_at_end()) {
                                advance();
                        }
                }

                fn multiline_comment() {
                        comment_embeds++;
                        wchar_t ch = advance();

                        if (is_at_end()) {
                                errors::error(line, L"Unterminated comment.");
                                return;
                        }

                        while (comment_embeds > 0 && !is_at_end()) {
                                if (ch == '/' && match('*')) {
                                        comment_embeds++;
                                } else if (ch == '*' && match('/')) {
                                        comment_embeds--;
                                }
                                if (ch == '\n') {
                                        line++;
                                }
                                ch = advance();
                        }

                }

                fn scan_token() {
                        wchar_t ch = advance();
                        switch (ch) {
                        case '(': add_token(token::LEFT_PAREN); break;
                        case ')': add_token(token::RIGHT_PAREN); break;
                        case '{': add_token(token::LEFT_BRACE); break;
                        case '}': add_token(token::RIGHT_BRACE); break;
                        case ',': add_token(token::COMMA); break;
                        case '.': add_token(token::DOT); break;
                        case '-': add_token(token::MINUS); break;
                        case '+': add_token(token::PLUS); break;
                        case ';': add_token(token::SEMICOLON); break;
                        case '*': add_token(token::STAR); break;
                        case '!':
                                add_token(match('=')
                                        ? token::BANG_EQUAL
                                        : token::BANG);
                                break;
                        case '=':
                                add_token(match('=')
                                        ? token::EQUAL_EQUAL
                                        : token::EQUAL);
                                break;
                        case '<':
                                add_token(match('=')
                                        ? token::LESS_EQUAL
                                        : token::LESS);
                                break;
                        case '>':
                                add_token(match('=')
                                        ? token::GREATER_EQUAL
                                        : token::GREATER);
                                break;
                        case '/':
                                if (match('/')) {
                                        comment();
                                } else if (match('*')) {
                                        multiline_comment();
                                } else {
                                        add_token(token::SLASH);
                                }
                                break;
                        case ' ':
                        case '\r':
                        case '\t':
                                // ignore
                                break;
                        case '\n':
                                line++;
                                break;
                        case '"': string(); break;
                        default:
                                if (std::iswdigit(ch)) {
                                        number();
                                } else if (std::iswalpha(ch)) {
                                        identifier();
                                } else {
                                        errors::error(line, L"Unexpected character");
                                }
                                break;
                        }
                }

        public:
                Scanner(std::wstring& source)
                        : source(source), start(0),
                                current(0), line(1),
                                comment_embeds(0)
                {}

                fn scan_tokens() -> std::vector<token::Token> {
                        while (!is_at_end()) {
                                start = current;
                                scan_token();
                        }

                        tokens.push_back(token::Token(token::FILE_EOF, L"", std::any(), line));
                        return tokens;
                }
        };
}
