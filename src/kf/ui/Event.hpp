#pragma once

#define enumerate(x) ((x) << value_bits)

namespace kf::ui {

/// @brief Входящее событие
struct Event {

private:
    using Storage = u8;

    static constexpr unsigned event_bits_total = sizeof(Storage) * 8;
    static constexpr Storage event_value_full = (1 << event_bits_total) - 1;

    static constexpr unsigned type_bits = 3;
    static constexpr unsigned value_bits = event_bits_total - type_bits;

    static constexpr Storage value_mask = (1 << value_bits) - 1;
    static constexpr Storage type_mask = event_value_full & ~value_mask;

    Storage storage;

public:
    /// @brief Тип события
    enum class Type : Storage {
        // Управление

        /// @brief Ничего (Заглушка)
        None = enumerate(0),

        /// @brief Принудительный рендер
        Update = enumerate(1),

        // События страницы

        /// @brief Смещение курсора
        /// @details Может содержать value
        PageCursorMove = enumerate(2),

        // События виджета

        /// @brief Клик
        WidgetClick = enumerate(3),

        /// @brief Получено значение
        /// @details Может содержать value
        WidgetValueChange = enumerate(4),
    };

    /// @brief Примитив значения
    using Value = i8;

    /// @param type Тип события
    /// @param value Значение
    constexpr explicit Event(Type type, Value value = 0) :
        storage{
            static_cast<Storage>(
                (static_cast<Storage>(type) & type_mask) |
                (static_cast<Storage>(value) & value_mask)
            )
        } {}

    [[nodiscard]] constexpr Type type() const {
        return static_cast<Type>(storage & type_mask);
    }

    [[nodiscard]] constexpr Value value() const {
        constexpr auto sign_bit_mask = 1 << (value_bits - 1);

        const auto result = static_cast<Value>(storage & value_mask);

        return (result & sign_bit_mask) ?
               static_cast<Value>(result | ~value_mask) :
               result;
    }
};

}

#undef enumerate

