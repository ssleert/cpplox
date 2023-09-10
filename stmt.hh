#pragma once
#include "cpplox.hh"
#include "./token.hh"
#include "./ast.hh"

namespace stmt {
        class Expression;
        class Print;
        class Var;
        class Block;

        template<class R>
        class Visitor {
        public:
                virtual fn visitExpressionStmt(Expression& stmt) -> R = 0;
                virtual fn visitPrintStmt(Print& stmt) -> R = 0;
                virtual fn visitVarStmt(Var& stmt) -> R = 0;
                virtual fn visitBlockStmt(Block& stmt) -> R = 0;
        };

        class Stmt {
        public:
                virtual ~Stmt() = default;
                virtual fn accept(Visitor<std::any>& visitor) -> std::any = 0;
        };

        class Expression : public Stmt {
        public:
                std::shared_ptr<ast::Expr> expression;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitExpressionStmt(*this);
                }

                Expression(
                        std::shared_ptr<ast::Expr> expr
                ) : expression(expr)
                {}
        };

        class Print : public Stmt {
        public:
                std::shared_ptr<ast::Expr> expression;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitPrintStmt(*this);
                }

                Print(
                        std::shared_ptr<ast::Expr> expr
                ) : expression(expr)
                {}
        };

        class Var : public Stmt {
        public:
                token::Token name;
                std::shared_ptr<ast::Expr> initializer;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitVarStmt(*this);
                }

                Var(
                        token::Token token,
                        std::shared_ptr<ast::Expr> expr
                ) : name(token), initializer(expr)
                {}
        };

        class Block : public Stmt {
        public:
                std::vector<std::shared_ptr<stmt::Stmt>> statements;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitBlockStmt(*this);
                }

                Block(
                        std::vector<std::shared_ptr<stmt::Stmt>> s
                ) : statements(s)
                {}
        };
}
