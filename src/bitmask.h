#pragma once
#include <type_traits>

template <typename T>
class bit_mask {
	using U = typename std::underlying_type<T>::type;
public:
	constexpr bit_mask()
		: _flags()
	{
	}
	constexpr bit_mask(const bit_mask<T> &other) = default;

	template <typename... ARGS>
	bit_mask(T flag, ARGS... args)
		: bit_mask(args...)
	{
		_flags |= static_cast<U>(flag);
	}
	constexpr bit_mask(T flag)
		: _flags(static_cast<U>(flag))
	{
	}

	explicit constexpr bit_mask(U u)
		: _flags(u)
	{
	}

	constexpr  bit_mask<T> operator~() const { return bit_mask(~_flags); }
	constexpr  bit_mask<T> operator|(const bit_mask<T> &other) const { return bit_mask<T>(_flags | other._flags); }
	constexpr  bit_mask<T> operator&(const bit_mask<T> &other) const { return bit_mask<T>(_flags & other._flags); }
	constexpr  bit_mask<T> operator&(const T &other) const { return bit_mask<T>(_flags & static_cast<U>(other)); }
	constexpr  bit_mask<T> operator^(const bit_mask<T> &other) const { return bit_mask<T>(_flags ^ other._flags); }
	bit_mask<T>& operator|= (const bit_mask<T> &other) { _flags |= other._flags; return *this; }
	bit_mask<T>& operator&= (const bit_mask<T> &other) { _flags &= other._flags; return *this; }
	bit_mask<T>& operator^= (const bit_mask<T> &other) { _flags ^= other._flags; return *this; }

	constexpr operator bool() const { return _flags;  }
private:
	U _flags;
	template<typename V>
	friend constexpr bit_mask<V> operator|(V a, bit_mask<V> &b);
	template<typename V>
	friend constexpr bit_mask<V> operator&(V a, const bit_mask<V> &b);
	template<typename V>
	friend constexpr bit_mask<V> operator^(V a, const bit_mask<V> &b);
};

template<typename T>
constexpr bit_mask<T> operator|(T a, bit_mask<T> &b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) | b._flags); }
template<typename T>
constexpr  bit_mask<T> operator&(T a, const bit_mask<T> &b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) & b._flags); }
template<typename T>
constexpr  bit_mask<T> operator^(T a, const bit_mask<T> &b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) ^ b._flags); }

//this is to eager
//template<typename T>
//constexpr  inline bit_mask<T> operator|(T a, T b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) | static_cast<typename std::underlying_type<T>::type>(b)); }
//template<typename T>
//constexpr  inline bit_mask<T> operator&(T a, T b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) & static_cast<typename std::underlying_type<T>::type>(b)); }
//template<typename T>
//constexpr  inline bit_mask<T> operator^(T a, T b) { return bit_mask<T>(static_cast<typename std::underlying_type<T>::type>(a) ^ static_cast<typename std::underlying_type<T>::type>(b)); }
