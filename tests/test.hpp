#pragma once
////////////////////////////////////////////////////////////////////////////////
/// \file test.hpp
///
/// Test utilities
///
/// Adapted from Eric Niebler's range-v3 library
/// https://github.com/ericniebler/range-v3
////////////////////////////////////////////////////////////////////////////////
// Includes:
#include <cstdlib>
#include <utility>
#include <iostream>
#include <nmp/detail/arithmetic_io.hpp>
////////////////////////////////////////////////////////////////////////////////
namespace nmp {
namespace test {
namespace detail {

inline int& failures() {
  static int no_failures = 0;
  return no_failures;
}

template <class T> struct streamable_base {};

template <class T>
std::ostream& operator<<(std::ostream& sout, streamable_base<T> const&) {
  return sout << "<non-streamable type>";
}

template <class T> struct streamable : streamable_base<T> {
 private:
  T const& t_;

 public:
  explicit streamable(T const& t) : t_(t) {}
  template <class U = T>
  friend auto operator<<(std::ostream& sout, streamable const& s)
   -> decltype(sout << std::declval<U const&>()) {
    return sout << s.t_;
  }
};

template <class T> streamable<T> stream(T const& t) { return streamable<T>{t}; }

template <class T> struct R {
 private:
  char const* filename_;
  int lineno_;
  char const* expr_;
  T t_;
  bool dismissed_ = false;

  template <class U> void oops(U const& u) const {
    std::cerr << "> ERROR: CHECK failed \"" << expr_ << "\"\n"
              << "> \t" << filename_ << '(' << lineno_ << ')' << '\n';
    if (dismissed_)
      std::cerr << "> \tEXPECTED: " << stream(u)
                << "\n> \tACTUAL: " << stream(t_) << '\n';
    ++failures();
  }
  void dismiss() { dismissed_ = true; }
  template <class V = T> auto eval_(int) -> decltype(!std::declval<V>()) {
    return !t_;
  }
  bool eval_(long) { return true; }

 public:
  R(char const* filename, int lineno, char const* expr, T t)
   : filename_(filename), lineno_(lineno), expr_(expr), t_(std::move(t)) {}
  ~R() {
    if (!dismissed_ && eval_(42)) { this->oops(42); }
  }
  template <class U> void operator==(U const& u) {
    dismiss();
    if (!(t_ == u)) { this->oops(u); }
  }

  template <class U> void operator!=(U const& u) {
    return dismiss();
    if (!(t_ != u)) { this->oops(u); }
  }
  template <class U> void operator<(U const& u) {
    dismiss();
    if (!(t_ < u)) { this->oops(u); }
  }
  template <class U> void operator<=(U const& u) {
    dismiss();
    if (!(t_ <= u)) { this->oops(u); }
  }
  template <class U> void operator>(U const& u) {
    dismiss();
    if (!(t_ > u)) { this->oops(u); }
  }
  template <class U> void operator>=(U const& u) {
    dismiss();
    if (!(t_ >= u)) { this->oops(u); }
  }
};

struct S {
 private:
  char const* filename_;
  int lineno_;
  char const* expr_;

 public:
  S(char const* filename, int lineno, char const* expr)
   : filename_(filename), lineno_(lineno), expr_(expr) {}
  template <class T> R<T> operator->*(T t) {
    return {filename_, lineno_, expr_, std::move(t)};
  }
};

}  // namespace detail

inline int result() { return detail::failures() ? EXIT_FAILURE : EXIT_SUCCESS; }

/// CHECK(ACTUAL op EXPECTED)
#define CHECK(...) \
  (void)(          \
   nmp::test::detail::S{__FILE__, __LINE__, #__VA_ARGS__}->*__VA_ARGS__) /**/

#define THROWS(expr, ExceptionType)                                        \
  do {                                                                     \
    bool exception_thrown_ = false;                                        \
    try {                                                                  \
      (expr);                                                              \
    } catch (const ExceptionType&) { exception_thrown_ = true; }           \
                                                                           \
    if (exception_thrown_) { break; }                                      \
                                                                           \
    TERMINATE("Expression: " #expr                                         \
              " doesn't throw an exception of type: " #ExceptionType "."); \
  } while (false)

}  // namespace test
}  // namespace nmp
////////////////////////////////////////////////////////////////////////////////
