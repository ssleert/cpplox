#pragma once
#include "cpplox.hh"
#include "utils.hh"

namespace token {
        enum token_type {
                // Single-character tokens.
                LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
                COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

                // One or two character tokens.
                BANG, BANG_EQUAL,
                EQUAL, EQUAL_EQUAL,
                GREATER, GREATER_EQUAL,
                LESS, LESS_EQUAL,

                // Literals.
                IDENTIFIER, STRING, NUMBER,

                // Keywords.
                AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
                PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

                FILE_EOF
        };

        const std::wstring token_type_strs[] = {
                L"LEFT_PAREN", L"RIGHT_PAREN", L"LEFT_BRACE", L"RIGHT_BRACE",
                L"COMMA", L"DOT", L"MINUS", L"PLUS", L"SEMICOLON", L"SLASH", L"STAR",

                L"BANG", L"BANG_EQUAL",
                L"EQUAL", L"EQUAL_EQUAL",
                L"GREATER", L"GREATER_EQUAL",
                L"LESS", L"LESS_EQUAL",

                L"IDENTIFIER", L"STRING", L"NUMBER",

                L"AND", L"CLASS", L"ELSE", L"FALSE", L"FUN", L"FOR", L"IF", L"NIL", L"OR",
                L"PRINT", L"RETURN", L"SUPER", L"THIS", L"TRUE", L"VAR", L"WHILE",

                L"FILE_EOF"
        };


        class Token {
        public:
                token_type type;
                std::wstring      lexeme;
                std::any          literal;
                int               line;

                Token(token_type type,
                        std::wstring lexeme,
                        std::any literal,
                        int line
                ) : type(type), lexeme(lexeme),
                        literal(literal), line(line)
                {}

        };

}

namespace std {
        fn to_wstring(token::token_type& t) -> std::wstring {
                return token::token_type_strs[t];
        }

        fn to_wstring(std::wstring& s) -> std::wstring {
                return s;
        }

        fn to_wstring(token::Token& t) -> std::wstring {
                return std::format(
                        L"token: {}\nlexeme: {}literal: {}",
                        to_wstring(t.type),
                        ((t.lexeme.ends_with(L"\n")) ? t.lexeme + L" |" : (t.lexeme + L" |\n")),
                        utils::to_wstring(t.literal)
                );
        }
}
