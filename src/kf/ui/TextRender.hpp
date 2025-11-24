#pragma once

#include <array>
#include <cmath>
#include <functional>

#include <kf/aliases.hpp>

namespace kf::ui {

/// @brief Система отрисовки
struct TextRender {

    /// @brief Настройки рендера 
    struct Settings {
        using RenderHandler = std::function<void(const kf::slice<const u8> &)>;

        static constexpr auto rows_default{4};
        static constexpr auto cols_default{16};

        /// @brief Обработчик отрисовки
        RenderHandler render_handler{nullptr};

        /// @brief Кол-во строк
        u16 rows{rows_default};

        /// @brief Максимальная длина строки
        u16 cols{cols_default};

    } settings{};

private:
    std::array<u8, 128> buffer{};
    usize cursor{0};

public:
    /// @brief Подготовить буфер отрисовки
    void prepare() {
        cursor = 0;
    }

    /// @brief После рендера кадра
    void finish() {
        buffer[cursor - 1] = '\0';

        settings.render_handler({
            buffer.data(),
            cursor,
        });
    }

    // Значения

    void string(const char *str) {
        (void) print(str);
    }

    void number(i32 integer) {
        (void) print(integer);
    }

    void number(f64 real, u8 rounding) {
        (void) print(real, rounding);
    }

    // Оформление

    void arrow() {
        (void) write('-');
        (void) write('>');
        (void) write(' ');
    }

    void colon() {
        (void) write(':');
        (void) write(' ');
    }

    void contrastBegin() {
        (void) write(0x81);
    }

    void contrastEnd() {
        (void) write(0x80);
    }

    void blockBegin() {
        (void) write('[');
    }

    void blockEnd() {
        (void) write(']');
    }

    void variableBegin() {
        (void) write('<');
    }

    void variableEnd() {
        (void) write('>');
    }

    // Управление

    void widgetEnd() {
        (void) write('\n');
    }

private:
    [[nodiscard]] usize print(const char *str) {
        if (nullptr == str) {
            str = "nullptr";
        }

        usize written{0};

        while (*str) {
            written += write(*str);
            str += 1;
        }

        return written;
    }

    [[nodiscard]] usize print(i32 integer) {
        usize written{0};

        if (integer < 0) {
            integer = -integer;
            written += write('-');
        }

        char digits_buffer[12];

        auto digits_total{0};
        while (integer > 0) {
            const auto base = 10;

            digits_buffer[digits_total] = static_cast<char>(integer % base + '0');
            digits_total += 1;
            integer /= base;
        }

        for (auto i = digits_total - 1; i >= 0; i -= 1) {
            written += write(digits_buffer[i]);
        }

        return written;
    }

    [[nodiscard]] usize print(f64 real, u8 rounding) {
        if (std::isnan(real)) {
            return print("nan");
        }

        if (std::isinf(real)) {
            return print("inf");
        }

        usize written{0};

        if (real < 0) {
            real = -real;
            written += write('-');
        }

        written += print(i32(real));

        if (rounding > 0) {
            written += write('.');

            auto fractional = real - i32(real);

            for (auto i = 0; i < rounding; i += 1) {
                const auto base = 10;

                fractional *= base;
                const auto digit = u8(fractional);
                written += write('0' + digit);
                fractional -= digit;
            }
        }

        return written;
    }

    /// @brief Записать байт в буфер
    [[nodiscard]] usize write(u8 c) {
        if (cursor >= buffer.size()) {
            return 0;
        }

        buffer[cursor] = c;
        cursor += 1;
        return 1;
    }
};

}// namespace kf::ui
