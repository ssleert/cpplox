#pragma once
#include "cpplox.hh"
#include "token.hh"

namespace ast {
        template<class R>
        class Visitor;

        class Binary;
        class Grouping;
        class Literal;
        class Unary;
        class Variable;
        class Assign;

        template<class R>
        class Visitor {
        public:
                virtual ~Visitor() = default;

                virtual fn visitBinaryExpr(Binary& expr) -> R = 0;
                virtual fn visitGroupingExpr(Grouping& expr) -> R = 0;
                virtual fn visitLiteralExpr(Literal& expr) -> R = 0;
                virtual fn visitUnaryExpr(Unary& expr) -> R = 0;
                virtual fn visitVariableExpr(Variable& expr) -> R = 0;
                virtual fn visitAssignExpr(Assign& expr) -> R = 0;
        };

        class Expr {
        public:
                virtual ~Expr() = default;
                virtual fn accept(Visitor<std::any>& visitor) -> std::any = 0;
        };

        class Binary : public Expr {
        public:
                std::shared_ptr<Expr> left;
                token::Token op;
                std::shared_ptr<Expr> right;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitBinaryExpr(*this);
                }

                Binary(
                        std::shared_ptr<Expr> l,
                        token::Token o,
                        std::shared_ptr<Expr> r
                ) : left(l),
                        op(o),
                        right(r)
                {}
        };

        class Grouping : public Expr {
        public:
                std::shared_ptr<Expr> expression;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitGroupingExpr(*this);
                }

                Grouping(
                        std::shared_ptr<Expr> e
                ) : expression(e)
                {}
        };

        class Literal : public Expr {
        public:
                std::any value;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitLiteralExpr(*this);
                }

                Literal(
                        std::any v
                ) : value(v)
                {}
        };

        class Unary : public Expr {
        public:
                token::Token op;
                std::shared_ptr<Expr> right;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitUnaryExpr(*this);
                }

                Unary(
                        token::Token o,
                        std::shared_ptr<Expr> r
                ) : op(o),
                        right(r)
                {}
        };

        class Variable : public Expr {
        public:
                token::Token name;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitVariableExpr(*this);
                }

                Variable(
                        token::Token t
                ) : name(t)
                {}
        };

        class Assign : public Expr {
        public:
                token::Token name;
                std::shared_ptr<Expr> value;

                fn accept(Visitor<std::any>& visitor) -> std::any {
                        return visitor.visitAssignExpr(*this);
                }

                Assign(
                        token::Token t,
                        std::shared_ptr<Expr> v
                ) : name(t), value(v)
                {}
        };
}
