#pragma once
#include "cpplox.hh"
#include "errors.hh"
#include "token.hh"

namespace environment {
        class Environment {
                std::shared_ptr<Environment> enclosing;
                std::unordered_map<std::wstring, std::any> values;

        public:
                Environment() : enclosing(nullptr) {}
                Environment(
                        std::shared_ptr<Environment> e
                ) : enclosing(e)
                {}

                fn define(std::wstring name, std::any value) {
                        values[name] = value;
                }

                fn assign(token::Token name, std::any value) {
                        if (values.contains(name.lexeme)) {
                                values[name.lexeme] = value;
                                return;
                        }
                        if (enclosing != nullptr) {
                                enclosing->assign(name, value);
                                return;
                        }

                        throw errors::runtime_panic(name, 
                                utils::to_string(std::format(
                                        L"Undefined variable '{}'.", name.lexeme
                                ))
                        );
                }

                fn get(token::Token name) -> std::any {
                        if (values.contains(name.lexeme)) {
                                return values.at(name.lexeme);
                        } 

                        if (enclosing != nullptr) {
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
