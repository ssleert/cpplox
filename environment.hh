#pragma once
#include "cpplox.hh"
#include "errors.hh"
#include "token.hh"

namespace environment {
        class Environment {
                std::shared_ptr<Environment> enclosing;
                std::unordered_map<std::wstring, std::any> values;

        public:
                Environment() {
                        enclosing.reset();
                }

                Environment(
                        std::shared_ptr<Environment> e
                ) : enclosing(e)
                {}

                fn define(std::wstring name, std::any value) {
                        std::wcout << L"--- define()" << std::endl;
                        std::wcout << L"| " << name << std::endl;
                        std::wcout << L"| " << enclosing << std::endl;
                        std::wcout << L"---" << std::endl;

                        values[name] = value;
                }

                fn assign(token::Token name, std::any value) {
                        std::wcout << L"--- assign()" << std::endl;
                        std::wcout << L"| " << name.lexeme << std::endl;
                        std::wcout << L"| " << enclosing << std::endl;
                        std::wcout << L"---" << std::endl;

                        if (values.contains(name.lexeme)) {
                                values[name.lexeme] = value;
                                return;
                        }
                        if (enclosing != NULL) {
                                enclosing->assign(name, value);
                                return;
                        }

                        throw errors::runtime_panic(name,
                                utils::to_string(std::format(
                                        L"Undefined variable for assignment '{}'.", name.lexeme
                                ))
                        );
                }

                fn get(token::Token name) -> std::any {
                        std::wcout << L"--- get()" << std::endl;
                        std::wcout << L"| " << name.lexeme << std::endl;
                        std::wcout << L"| " << enclosing << std::endl;
                        std::wcout << L"---" << std::endl;

                        if (values.contains(name.lexeme)) {
                                return values.at(name.lexeme);
                        }
 
                        if (enclosing != NULL) {
                                enclosing->get(name);
                        }

                        throw errors::runtime_panic(name,
                                utils::to_string(std::format(
                                        L"Undefined variable '{}'.", name.lexeme
                                ))
                        );
                }
        };
}
