#ifndef STYLE_H
#define STYLE_H

#include <iostream>
#include <string>
#include <string_view>

namespace Style {

    // Formatting
    inline constexpr std::string_view reset = "\033[0m";
    inline constexpr std::string_view bold = "\033[1m";
    inline constexpr std::string_view underline = "\033[4m";
    inline constexpr std::string_view reversed = "\033[7m";

    // Regular foreground colors
    inline constexpr std::string_view black   = "\033[30m";
    inline constexpr std::string_view red     = "\033[31m";
    inline constexpr std::string_view green   = "\033[32m";
    inline constexpr std::string_view yellow  = "\033[33m";
    inline constexpr std::string_view blue    = "\033[34m";
    inline constexpr std::string_view magenta = "\033[35m";
    inline constexpr std::string_view cyan    = "\033[36m";
    inline constexpr std::string_view white   = "\033[37m";

    // Bright foreground colors
    inline constexpr std::string_view bright_black   = "\033[90m";
    inline constexpr std::string_view bright_red     = "\033[91m";
    inline constexpr std::string_view bright_green   = "\033[92m";
    inline constexpr std::string_view bright_yellow  = "\033[93m";
    inline constexpr std::string_view bright_blue    = "\033[94m";
    inline constexpr std::string_view bright_magenta = "\033[95m";
    inline constexpr std::string_view bright_cyan    = "\033[96m";
    inline constexpr std::string_view bright_white   = "\033[97m";

    // Background colors
    inline constexpr std::string_view bg_black   = "\033[40m";
    inline constexpr std::string_view bg_red     = "\033[41m";
    inline constexpr std::string_view bg_green   = "\033[42m";
    inline constexpr std::string_view bg_yellow  = "\033[43m";
    inline constexpr std::string_view bg_blue    = "\033[44m";
    inline constexpr std::string_view bg_magenta = "\033[45m";
    inline constexpr std::string_view bg_cyan    = "\033[46m";
    inline constexpr std::string_view bg_white   = "\033[47m";

    // Bright background colors
    inline constexpr std::string_view bg_bright_black   = "\033[100m";
    inline constexpr std::string_view bg_bright_red     = "\033[101m";
    inline constexpr std::string_view bg_bright_green   = "\033[102m";
    inline constexpr std::string_view bg_bright_yellow  = "\033[103m";
    inline constexpr std::string_view bg_bright_blue    = "\033[104m";
    inline constexpr std::string_view bg_bright_magenta = "\033[105m";
    inline constexpr std::string_view bg_bright_cyan    = "\033[106m";
    inline constexpr std::string_view bg_bright_white   = "\033[107m";

    inline void print(std::string_view text, std::string_view style) {
        std::cout << style << text << reset;
    }

}

#endif // STYLE_H
