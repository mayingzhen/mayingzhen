#pragma once


#include <type_traits> // for std::false_type

#include <assert.h>
#include <stddef.h>
#include <stdint.h>

namespace ma 
{
template<typename Enum>
struct EnableBitMaskOperators : public std::false_type { };
} // namespace ma

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr bool operator!(Enum rhs) noexcept {
    using underlying = std::underlying_type_t<Enum>;
    return underlying(rhs) == 0;
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator~(Enum rhs) noexcept {
    using underlying = std::underlying_type_t<Enum>;
    return Enum(~underlying(rhs));
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator|(Enum lhs, Enum rhs) noexcept {
    using underlying = std::underlying_type_t<Enum>;
    return Enum(underlying(lhs) | underlying(rhs));
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator&(Enum lhs, Enum rhs) noexcept {
    using underlying = std::underlying_type_t<Enum>;
    return Enum(underlying(lhs) & underlying(rhs));
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator^(Enum lhs, Enum rhs) noexcept {
    using underlying = std::underlying_type_t<Enum>;
    return Enum(underlying(lhs) ^ underlying(rhs));
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator|=(Enum& lhs, Enum rhs) noexcept {
    return lhs = lhs | rhs;
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator&=(Enum& lhs, Enum rhs) noexcept {
    return lhs = lhs & rhs;
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr Enum operator^=(Enum& lhs, Enum rhs) noexcept {
    return lhs = lhs ^ rhs;
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr bool none(Enum lhs) noexcept {
    return !lhs;
}

template<typename Enum, typename std::enable_if_t<
        std::is_enum<Enum>::value && ma::EnableBitMaskOperators<Enum>::value, int> = 0>
inline constexpr bool any(Enum lhs) noexcept {
    return !none(lhs);
}
