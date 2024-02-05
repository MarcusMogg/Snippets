// https://stackoverflow.com/questions/76437902/importing-a-module-with-classes-that-uses-standard-library-causes-redefinition-e
// https://www.youtube.com/watch?v=Dk_C_E8AtRs&t=1990s&ab_channel=MicrosoftVisualStudio
// If you use import std, you should not include any STL header

// module;

// #include <array>
// #include <concepts>
// #include <ranges>
// #include <string_view>
// #include <type_traits>
// #include <utility>
export module stl;

export constexpr inline const int kA = 1;

// export namespace std {
//
// using std::array;
// using std::common_type_t;
// using std::forward;
// using std::index_sequence;
// using std::is_object_v;
// using std::make_index_sequence;
// using std::make_integer_sequence;
// using std::remove_cv_t;
// using std::same_as;
// using std::string_view;
//
// namespace ranges {
// using ranges::range;
// }
// }  // namespace std