#pragma once
#include "cpplox.hh"
#include "utils.hh"
#include "ast.hh"

namespace ast_printer {
        class AstPrinter : public ast::Visitor<std::any> {
                fn parenthesize(
                        std::wstring name,
                        std::vector<std::shared_ptr<ast::Expr>> exprs
                ) -> std::wstring {
                        std::wstring builder;
                        builder.append(L"(").append(name);
                        for (auto& expr : exprs) {
                                builder.append(L" ");
                                builder.append(
                                        std::any_cast<std::wstring>(
                                                expr->accept(*this)
                                        )
                                );
                        }
                        builder.append(L")");

                        return builder;
                }

        public:
                fn print(std::shared_ptr<ast::Expr> expr) -> std::wstring {
                        return std::any_cast<std::wstring>(
                                expr->accept(*this)
                        );
                }

                fn visitBinaryExpr(ast::Binary& expr) -> std::any {
                        return parenthesize(
                                expr.op.lexeme,
                                {expr.left, expr.right}
                        );
                }

                fn visitGroupingExpr(ast::Grouping& expr) -> std::any {
                        return parenthesize(
                                L"group",
                                {expr.expression}
                        );
                }

                fn visitLiteralExpr(ast::Literal& expr) -> std::any {
                        if (!expr.value.has_value()) {
                                return L"nil";
                        }
                        return utils::to_wstring(expr.value);
                }

                fn visitUnaryExpr(ast::Unary& expr) -> std::any {
                        return parenthesize(
                                expr.op.lexeme,
                                {expr.right}
                        );
                }
        };
}
