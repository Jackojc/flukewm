#ifndef FLUKE_ZIP_HPP
#define FLUKE_ZIP_HPP

#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <iterator>

// Implementation of a zip function with support for variadic number
// of arguments.

namespace fluke::detail {
	// Implements std::remove_cvref_t which is >=c++20.
	template <typename T>
	struct remove_cvref {
		using type = std::remove_cv_t<std::remove_reference_t<T>>;
	};

	template <typename T>
	using remove_cvref_t = typename remove_cvref<T>::type;


	// Remove r-value reference, just get T
	template <typename T>
	using type_ref_t = std::conditional_t<
		std::is_rvalue_reference_v<T>,
		std::remove_reference_t<T>, // if true, remove ref
		T  // else, just T
	>;
}


namespace fluke {
	template <typename... Ts>
	class zipper {
		private:
			std::tuple<detail::type_ref_t<Ts>...> containers;  // holds all containers which are to be zipped.


		// Constructors
		public:
			template <typename... Vs>
			constexpr zipper(Vs&&... args):
				containers{std::forward<Vs>(args)...} {}

			template <typename... Vs>
			constexpr zipper(std::tuple<Vs...> args):
				containers{std::move(args)} {}


		// Implementation of iterator.
		private:
			class iterator {
				// Data
				private:
					std::tuple<typename detail::remove_cvref_t<Ts>::const_iterator...> its;


				// Constructors
				public:
					constexpr iterator(typename detail::remove_cvref_t<Ts>::const_iterator... args):
						its{std::move(args)...} {}

					constexpr iterator(std::tuple<typename detail::remove_cvref_t<Ts>::const_iterator...> args):
						its{std::move(args)} {}

					constexpr iterator(const iterator& other):
						its(other.its) {}


					// Assignment operator
					constexpr iterator& operator=(const iterator& other) {
						if (this != &other)
							its = other.its;

						return *this;
					}


				// Interface functions
				public:
					// Increment all iterators.
					constexpr iterator& operator++() {
						std::apply([] (auto&&... x) {
							(..., ++x);
						}, its);
						return *this;
					}

					constexpr iterator operator++(int) {
						iterator tmp(*this);
						operator++();
						return tmp;
					}


					// Dereference operator
					constexpr auto operator*() const {
						// Return tuple of dereferenced iterators.
						return std::apply([] (auto&&... x) {
							return std::forward_as_tuple(*x...);
						}, its);
					}


					// Compare tuples.
					constexpr bool operator==(const iterator& other) const {
						return its == other.its;
					}

					constexpr bool operator!=(const iterator& other) const {
						return not(its == other.its);
					}
			};


		// Iterators...
		public:
			// Construct iterator class while passing tuple of iterators from containers tuple.
			constexpr iterator begin() const {
				return std::apply([] (auto&&... x) {
					using std::begin;  // trick for ADL allowing custom size functions to be used.
					return iterator(begin(x)...);  // variadic template expansion, call std::begin on all elements
				}, containers);
			}

			constexpr iterator end() const {
				return std::apply([] (auto&&... x) {
					using std::end;  // trick for ADL allowing custom size functions to be used.
					return iterator(end(x)...);
				}, containers);
			}
	};


	// Helper function
	// for (auto [a, b, c]: zip(d, e, f))
	template <typename T, typename... Ts>
	auto zip(T&& first, Ts&&... args) {
		// Make sure all containers are the same size.
		if constexpr(sizeof...(Ts) != 0) {
			using std::size;  // trick for ADL allowing custom size functions to be used.
			if (((size(first) != size(args)) or ...))
				throw std::length_error("containers differ in size!");
		}

		return zipper<T, Ts...>{std::forward<T>(first), std::forward<Ts>(args)...};
	}
}

#endif
