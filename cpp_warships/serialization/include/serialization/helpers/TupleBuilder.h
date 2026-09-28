#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

namespace cpp_warships::serialization::helpers {
    template <typename T, typename... Ts>
    struct is_one_of;

    template <typename... TChildren>
    struct TupleBuilder;

    /** @brief Helper type trait to check if a type T is in a parameter pack Ts.. @tparam T The
     * type to check. */
    template <typename T, typename... Ts>
    struct is_one_of : std::disjunction<std::is_same<T, Ts>...> {};

    /** @brief Helper to create a tuple with filtered or default-constructed elements @tparam
     * TChildren The types to include in the final output tuple. */
    template <typename... TChildren>
    struct TupleBuilder {
        /** @brief Unified entrypoint to building tuple. */
        template <typename... Args>
        static std::tuple<TChildren...> build(Args&&... args) {
            return build_impl(std::index_sequence_for<TChildren...>{}, std::forward<Args>(args)...);
        }

    private:
        /** @brief Helper to find the first argument of type T @tparam T The
         * type to search for in the arguments. */
        template <typename T, typename... Args>
        static T select_arg(Args&&... args) {
            if constexpr (sizeof...(Args) == 0) {
                return T{};
            } else {
                T* result = nullptr;
                (
                    [&]<typename T0>(T0&& arg) {
                        if constexpr (std::is_same_v<std::remove_cvref_t<T0>, T>) {
                            if (result == nullptr) {
                                result = &arg;
                            }
                        }
                    }(args),
                    ...);

                return result == nullptr ? T{} : *result;
            }
        }

        /** @brief Build tuple by applying select_arg for each type in Args. */
        template <size_t... Is, typename... Args>
        static std::tuple<TChildren...> build_impl(std::index_sequence<Is...>, Args&&... args) {
            return std::tuple<TChildren...>(
                select_arg<std::tuple_element_t<Is, std::tuple<TChildren...>>>(
                    std::forward<Args>(args)...
                )...
            );
        }
    };
}  // namespace cpp_warships::serialization::helpers