#pragma once
#include "cpplox.hh"
#include "errors.hh"
#include "token.hh"
#include "ast.hh"

namespace parser {
        class parse_error : public std::exception {};

        using expr_ptr = std::shared_ptr<ast::Expr>;

        class Parser {
                std::vector<token::Token> tokens;
                size_t current;

                fn previous() -> token::Token {
                        return tokens.at(current - 1);
                }

                fn peek() -> token::Token {
                        return tokens.at(current);
                }

                fn is_at_end() -> bool {
                        return peek().type == token::FILE_EOF;
                }

                fn check(token::token_type type) -> bool {
                        if (is_at_end()) {
                                return false;
                        }
                        return peek().type == type;
                }

                fn advance() -> token::Token {
                        if (!is_at_end()) {
                                current++;
                        }
                        return previous();
                }

                fn match(std::initializer_list<token::token_type> types) -> bool {
                        for (auto type : types) {
                                if (check(type)) {
                                        advance();
                                        return true;
                                }
                        }

                        return false;
                }

                fn synchronize() {
                        advance();

                        while (!is_at_end()) {
                                if (previous().type == token::SEMICOLON) {
                                        return;
                                }

                                switch (peek().type) {
                                case token::CLASS:
                                case token::FUN:
                                case token::VAR:
                                case token::FOR:
                                case token::IF:
                                case token::WHILE:
                                case token::PRINT:
                                case token::RETURN:
                                        return;
                                default:
                                        break;
                                }

                                advance();
                        }

                }

                fn error(token::Token t, std::wstring msg) -> parse_error {
                        errors::error(t, msg);
                        return parse_error();
                }

                fn consume(token::token_type type, std::wstring msg) -> token::Token {
                        if (check(type)) {
                                return advance();
                        }

                        throw error(peek(), msg);
                }

                fn primary() -> std::shared_ptr<ast::Expr> {
                        if (match({token::FALSE})) {
                                return expr_ptr(new ast::Literal(false));
                        } else if (match({token::TRUE})) {
                                return expr_ptr(new ast::Literal(true));
                        } else if (match({token::NIL})) {
                                return expr_ptr(new ast::Literal(std::any()));
                        }

                        if (match({token::NUMBER, token::STRING})) {
                                return expr_ptr(new ast::Literal(previous().literal));
                        }

                        if (match({token::LEFT_PAREN})) {
                                std::shared_ptr<ast::Expr> expr = expression();
                                consume(token::RIGHT_PAREN, L"Expect ')' after expression");
                                return expr_ptr(new ast::Grouping(expr));
                        }

                        throw error(peek(), L"Except expression.");
                }

                fn unary() -> std::shared_ptr<ast::Expr> {
                        if (match({token::BANG, token::MINUS})) {
                                token::Token op = previous();
                                std::shared_ptr<ast::Expr> right = unary();
                                return expr_ptr(new ast::Unary(op, right));
                        }

                        return primary();
                }

                fn factor() -> std::shared_ptr<ast::Expr> {
                        std::shared_ptr<ast::Expr> expr = unary();
                        while (match({token::SLASH, token::STAR})) {
                                token::Token op = previous();
                                std::shared_ptr<ast::Expr> right = unary();
                                expr = expr_ptr(new ast::Binary(expr, op, right));
                        }

                        return expr;
                }

                fn term() -> std::shared_ptr<ast::Expr> {
                        std::shared_ptr<ast::Expr> expr = factor();
                        while (match({token::MINUS, token::PLUS})) {
                                token::Token op = previous();
                                std::shared_ptr<ast::Expr> right = factor();
                                expr = expr_ptr(new ast::Binary(expr, op, right));
                        }

                        return expr;
                }

                fn comparison() -> std::shared_ptr<ast::Expr> {
                        std::shared_ptr<ast::Expr> expr = term();
                        while (match({
                                token::GREATER, token::GREATER_EQUAL,
                                token::LESS, token::LESS_EQUAL,
                        })) {
                                token::Token op = previous();
                                std::shared_ptr<ast::Expr> right = term();
                                expr = expr_ptr(new ast::Binary(expr, op, right));
                        }

                        return expr;
                }

                fn equality() -> std::shared_ptr<ast::Expr> {
                        std::shared_ptr<ast::Expr> expr = comparison();
                        while (match({token::BANG_EQUAL, token::EQUAL_EQUAL})) {
                                token::Token op = previous();
                                std::shared_ptr<ast::Expr> right = comparison();
                                expr = expr_ptr(new ast::Binary(expr, op, right));
                        }

                        return expr;
                }

                fn expression() -> std::shared_ptr<ast::Expr> {
                        return equality();
                }

        public:

                fn parse() -> std::shared_ptr<ast::Expr> {
                        try {
                                return expression();
                        } catch(parse_error& err)  {
                                return nullptr;
                        }
                }

                Parser(
                        std::vector<token::Token>& t
                ) : tokens(t), current(0) {}
        };
}
