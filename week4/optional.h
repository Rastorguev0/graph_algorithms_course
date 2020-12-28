#pragma once
#include <utility>

struct nullopt_t {
	explicit nullopt_t() {}
};

const nullopt_t nullopt{};

template<class T>
class optional {
public:
	optional() {}
	optional(nullopt_t null) {}
	optional(const optional& other) {
		*this = other;
	}

	optional(optional&& other) {
		*this = std::move(other);
	}

	optional& operator = (const optional& other) {
		if (initialized = other.initialized) {
			value_ = other.value_;
		}
		return *this;
	}

	optional& operator = (optional&& other) {
		if (initialized = other.initialized) {
			value_ = std::move(other.value_);
			other.initialized = false;
		}
		return *this;
	}

	T& operator = (const T& val) {
		initialized = true;
		return value_ = val;
	}
	T& operator = (T&& val) {
		initialized = true;
		return value_ = std::move(val);
	}

	operator bool() const {
		return initialized;
	}

	bool has_value() const {
		return initialized;
	}

	const T& value() const {
		return value_;
	}

	T& value() {
		return value_;
	}

private:
	T value_;
	bool initialized = false;
};