#pragma once
#include "cpplox.hh"

// 💠
namespace utils {
        namespace {
                std::wstring_convert<std::codecvt_utf8<wchar_t>> u8str_conv;
        }

        fn to_wstring(std::any& value) -> std::wstring {
                if (value.type() == typeid(std::wstring)) {
                        return std::any_cast<std::wstring>(value);
                } else if (value.type() == typeid(double)) {
                        return std::to_wstring(
                                std::any_cast<double>(value)
                        );
                } else if (value.type() == typeid(int)) {
                        return std::to_wstring(
                                std::any_cast<int>(value)
                        );
                }
                return L"empty";
        }

        fn to_string(std::wstring wstr) -> std::string {
                return u8str_conv.to_bytes(wstr);
        }

        fn to_wstring(char* str) -> std::wstring {
                return u8str_conv.from_bytes(str);
        }

        fn to_wstring(std::string str) -> std::wstring {
                return u8str_conv.from_bytes(str.c_str());
        }

        fn get_file(std::wstring& path) -> std::wstring {
                std::wifstream f(to_string(path));
                if (!f.is_open()) {
                        throw std::runtime_error(
                                std::format(
                                        "utils: file '{}' is inaccessable",
                                        to_string(path)
                                )
                        );
                }
                std::wstringstream str;
                str << f.rdbuf();
                return str.str();
        }

        fn get_file_lines(std::wstring& path) -> std::vector<std::wstring> {
                std::vector<std::wstring> lines;
                std::wifstream f(to_string(path));
                if (!f.is_open()) {
                        throw std::runtime_error(
                                std::format(
                                        "utils: file '{}' is inaccessable",
                                        to_string(path)
                                )
                        );
                }
                for (std::wstring line; std::getline(f, line);) {
                        lines.push_back(line);
                }
                return lines;
        }

        fn panic(std::wstring msg) {
                std::wcout << msg << std::endl;
                //std::abort();
                std::exit(1);
        }

        template<typename ... Ts>
        struct Overload : Ts ... {
                using Ts::operator() ...;
        };
        template<class... Ts> Overload(Ts...) -> Overload<Ts...>;
}
