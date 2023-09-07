#include "cpplox.hh"
#include "errors.hh"
#include "utils.hh"
#include "scanner.hh"
#include "ast.hh"
#include "ast_printer.hh"
#include "parser.hh"
#include "interpreter.hh"

class Lox {
        fn run(std::wstring& source) {
                // scan tokens
                scanner::Scanner sc(source);
                auto tokens = sc.scan_tokens();

                // parse ast from tokens
                parser::Parser pr(tokens);
                auto statements = pr.parse();
                std::wcout << "parsing" << std::endl;
                // stop on syntax error
                if (errors::hadError) {
                        return;
                }

                // interpret
                std::wcout << "\n-----result-----" << std::endl;
                interpreter::Interpreter it;
                it.interpret(statements);
        }

        fn run_file(std::wstring& path) -> int {
                auto code = utils::get_file(path);
                run(code);
                if (errors::hadError) {
                        return 1;
                }
                if (errors::had_runtime_error) {
                        return 2;
                }
                return 0;
        }

        fn run_prompt() {
                std::wstring line;
                while (true) {
                        std::wcout << ">>> ";
                        std::getline(std::wcin, line);
                        if (line == L"") {
                                break;
                        }
                        run(line);
                        errors::hadError = false;
                        errors::had_runtime_error = false;
                        std::wcout << std::endl;
                }
                return;
        }

public:
        fn main(std::vector<std::wstring>& args) -> int {
                //using expr_ptr = std::shared_ptr<ast::Expr>;

                //std::shared_ptr<ast::Expr> expr(
                //        new ast::Binary(
                //                expr_ptr(new ast::Unary(
                //                        token::Token(token::MINUS, L"-", std::any(), 1),
                //                        expr_ptr(new ast::Literal(1234))
                //                )),
                //                token::Token(token::STAR, L"*", std::any(), 1),
                //                expr_ptr(new ast::Grouping(
                //                        expr_ptr(new ast::Literal(45.67))
                //                ))
                //        )
                //);

                //std::shared_ptr<ast_printer::AstPrinter> p(new ast_printer::AstPrinter);
                //std::wcout << p->print(expr) << std::endl;

                if (args.size() > 2) {
                        std::cerr << "Usage: cpplox [script]\n";
                        return 1;
                } else if (args.size() == 2) {
                        return run_file(args[1]);
                } else {
                        run_prompt();
                }

                return 0;
        }
};

fn main(int argc, char* argv[]) -> int {
        std::setlocale(LC_ALL, "");
        std::locale::global(std::locale(""));
        std::vector<std::wstring> args(argc);
        for (int i = 0; i < argc; ++i) {
                args[i] = utils::to_wstring(argv[i]);
        }

        Lox lox;
        return lox.main(args);
}
