#pragma once
#include "cpplox.hh"
#include "token.hh"

namespace errors {
        class runtime_panic : public std::runtime_error {
        public:
                token::Token token;
                runtime_panic(
                        token::Token t,
                        std::string msg
                ) : std::runtime_error(msg), token(t)
                {}
        };

        bool hadError = false;
        bool had_runtime_error = false;
        fn report(int line, std::wstring where, std::wstring msg) {
                std::wcerr << std::format(
                        L"[ line {} ] Error{}: {}",
                        line, where, msg
                ) << std::endl;
                hadError = true;
        }

        fn error(token::Token t, std::wstring msg) {
                if (t.type == token::FILE_EOF) {
                        report(t.line, L" at end", msg);
                } else {
                        report(t.line, std::format(
                                L" at '{}'", t.lexeme
                        ), msg);
                }
        }

        fn error(int line, std::wstring msg) {
                report(line, L"", msg);
        }

        fn runtime_err(runtime_panic err) {
                std::cerr << std::format(
                        "[ line {} ] {}",
                        err.token.line,
                        err.what()
                ) << std::endl;
                had_runtime_error = true;
        }

}
