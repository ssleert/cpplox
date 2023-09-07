#pragma once
#include "cpplox.hh"
#include "token.hh"
#include "ast.hh"
#include "stmt.hh"
#include "environment.hh"
#include "errors.hh"

namespace interpreter {
        class Interpreter : public ast::Visitor<std::any>,
                            public stmt::Visitor<std::any>
        {
                environment::Environment env;                

                fn evaluate(std::shared_ptr<ast::Expr> expr) -> std::any {
                        return expr->accept(*this);
                }

                fn execute(std::shared_ptr<stmt::Stmt> statement) {
                        statement->accept(*this);
                }

                fn is_truthy(std::any& obj) -> bool {
                        if (!obj.has_value()) {
                                return false;
                        }
                        if (obj.type() == typeid(bool)) {
                                return std::any_cast<bool>(obj);
                        }
                        return true;
                }

                fn check_number_operand(token::Token& op, std::any& operand) {
                        if (operand.type() == typeid(double)) {
                                return;
                        }
                        throw errors::runtime_panic(op, "Operator must be a number.");
                }

                fn check_number_operands(token::Token& op, std::any& left, std::any& right) {
                        if (left.type() == typeid(double) && right.type() == typeid(double)) {
                                return;
                        }
                        throw errors::runtime_panic(op, "Operands must be a number.");
                }

                fn is_equal(std::any& a, std::any& b) -> bool {
                        if (!a.has_value() && !b.has_value()) {
                                return true;
                        }
                        if (!a.has_value() || !b.has_value()) {
                                return false;
                        }

                        if (a.type() == typeid(std::wstring)
                                && b.type() == typeid(std::wstring)) {
                                return std::any_cast<std::wstring>(a) == std::any_cast<std::wstring>(b);
                        }

                        if (a.type() == typeid(double)
                                && b.type() == typeid(double)) {
                                return std::any_cast<double>(a) == std::any_cast<double>(b);
                        }

                        // unreachable
                        utils::panic(L"interpreter.hh: is_equal() method. objects is incomparable");
                        return false;
                }


                fn stringify(std::any obj) -> std::wstring {
                        if (!obj.has_value()) {
                                return L"nil";
                        }

                        if (obj.type() == typeid(double)) {
                                auto text = std::to_wstring(std::any_cast<double>(obj));
                                if (text.ends_with(L".000000")) {
                                        text = text.substr(0, text.length() - 7);
                                }
                                return text;
                        } else if (obj.type() == typeid(std::wstring)) {
                                return std::any_cast<std::wstring>(obj);
                        } else if (obj.type() == typeid(bool)) {
                                return std::any_cast<bool>(obj)
                                        ? L"true"
                                        : L"false";
                        }

                        return L"NO_STRING_FOR_OBJECT";
                }

        public:
                fn interpret(std::vector<std::shared_ptr<stmt::Stmt>> statements) {
                        try {
                                for (auto& statement : statements) {
                                        execute(statement);
                                }
                        } catch (errors::runtime_panic& err) {
                                errors::runtime_err(err);
                        }
                }

                fn visitLiteralExpr(ast::Literal& expr) -> std::any {
                        return expr.value;
                }

                fn visitGroupingExpr(ast::Grouping& expr) -> std::any {
                        return evaluate(expr.expression);
                }

                fn visitUnaryExpr(ast::Unary& expr) -> std::any {
                        auto right = evaluate(expr.right);

                        switch (expr.op.type) {
                        case token::BANG:
                                return !is_truthy(right);
                        case token::MINUS:
                                check_number_operand(expr.op, right);
                                return -(std::any_cast<double>(right));
                        default:
                                break;
                        }

                        // unreachable
                        utils::panic(L"interpreter.hh: unary expr, unreachable code");
                        return std::any();
                }

                fn visitBinaryExpr(ast::Binary& expr) -> std::any {
                        auto left = evaluate(expr.left);
                        auto right = evaluate(expr.right);

                        switch (expr.op.type) {
                        case token::GREATER:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) > std::any_cast<double>(right);
                        case token::GREATER_EQUAL:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) >= std::any_cast<double>(right);
                        case token::LESS:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) < std::any_cast<double>(right);
                        case token::LESS_EQUAL:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) <= std::any_cast<double>(right);
                        case token::MINUS:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) - std::any_cast<double>(right);
                        case token::PLUS:
                                if (left.type() == typeid(double) && right.type() == typeid(double)) {
                                        return std::any_cast<double>(left) + std::any_cast<double>(right);
                                }

                                if (left.type() == typeid(std::wstring) && right.type() == typeid(std::wstring)) {
                                        return std::any_cast<std::wstring>(left) + std::any_cast<std::wstring>(right);
                                }

                                throw errors::runtime_panic(expr.op, "Operands must be two numbers or two strings.");
                        case token::SLASH:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left) / std::any_cast<double>(right);
                        case token::STAR:
                                check_number_operands(expr.op, left, right);
                                return std::any_cast<double>(left)  * std::any_cast<double>(right);
                        case token::BANG_EQUAL:
                                return !is_equal(left, right);
                        case token::EQUAL_EQUAL:
                                return is_equal(left, right);
                        default:
                                break;
                        }

                        // unreachable
                        utils::panic(L"interpreter.hh: binary expr, unreachable code");
                        return std::any();
                }

                fn visitVariableExpr(ast::Variable& expr) -> std::any {
                        return env.get(expr.name);
                }

                fn visitAssignExpr(ast::Assign& expr) -> std::any {
                        auto value = evaluate(expr.value);
                        env.assign(expr.name, value);
                        return value;
                }

                // ---------------STATEMENTS---------------

                fn visitExpressionStmt(stmt::Expression& stmt) -> std::any {
                        evaluate(stmt.expression);
                        return std::any();
                }

                fn visitPrintStmt(stmt::Print& stmt) -> std::any {
                        auto value = evaluate(stmt.expression);
                        std::wcout << stringify(value) << std::endl;
                        return std::any();
                }

                fn visitVarStmt(stmt::Var& stmt) -> std::any {
                        auto value = std::any();

                        if (stmt.initializer != nullptr) {
                                value = evaluate(stmt.initializer);
                        }

                        env.define(stmt.name.lexeme, value);
                        return std::any();
                }
        };
}
