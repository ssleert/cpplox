#include <iostream>
#include <locale>
#include <string>
#include <codecvt>

int main() {
        std::u32string str = U"немного русского текста";

        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        std::cout << converter.to_bytes(str[0]) << '\n';
}
