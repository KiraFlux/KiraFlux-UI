#pragma once

static constexpr auto counter_start = __COUNTER__ + 1;

#define enumerate() ((__COUNTER__ - counter_start) << value_bits)

namespace kf::ui {

/// @brief Входящее событие
struct Event {

public:
    using Value = u8;

private:

    static constexpr unsigned event_bits_total = sizeof(Value) * 8;
    static constexpr Value event_value_full = (1 << event_bits_total) - 1;

    static constexpr unsigned type_bits = 3;
    static constexpr unsigned value_bits = event_bits_total - type_bits;

    static constexpr Value value_mask = (1 << value_bits) - 1;
    static constexpr Value type_mask = event_value_full & ~value_mask;

    Value e;
public:

    /// @brief Тип события
    enum class Type : Value {
        // Управление

        /// @brief Ничего (Заглушка)
        None = enumerate(),

        /// @brief Принудительный рендер
        Update = enumerate(),

        // События страницы

        /// @brief Смещение курсора
        PageCursorMove = enumerate(),

        // События виджета

        /// @brief Клик
        WidgetClick = enumerate(),

        /// @brief Получено значение
        WidgetValueChange = enumerate(),
    };

    constexpr explicit Event(Type type, Value value = 0) :
        e{static_cast<Value>((static_cast<Value>(type) & type_mask) | (value & value_mask))} {}

    [[nodiscard]] inline Type type() const {
        return static_cast<Type>(e & type_mask);
    }

    [[nodiscard]] inline Value value() const {
        return static_cast<Value>(e & value_mask);
    }
};

}

#undef enumerate

