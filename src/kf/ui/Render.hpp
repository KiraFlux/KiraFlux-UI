#pragma once

#include <type_traits>

namespace kf::ui {

template<typename Impl> struct Render {
public:
    void prepare() { impl().prepareImpl(); }

    /// @brief После рендера кадра
    void finish() { impl().finishImpl(); }

    // Значения

    void string(const char *str) { impl().stringImpl(str); }

    void number(i32 integer) { impl().numberImpl(integer); }

    void number(f64 real, u8 rounding) { impl().numberImpl(real, rounding); }

    // Оформление

    void arrow() { impl().arrowImpl(); }

    void colon() { impl().colonImpl(); }

    void contrastBegin() { impl().contrastBeginImpl(); }

    void contrastEnd() { impl().contrastEndImpl(); }

    void blockBegin() { impl().blockBeginImpl(); }

    void blockEnd() { impl().blockEnd(); }

    void variableBegin() { impl().variableBeginImpl(); }

    void variableEnd() { impl().variableEndImpl(); }

    // Управление

    void widgetEnd() { impl().widgetEndImpl(); }

private:
    Impl &impl() { return *static_cast<Impl *>(this); }
};

}// namespace kf::ui
