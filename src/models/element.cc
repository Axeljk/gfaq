#include "element.h"

#include <iostream>


namespace elements {
	const char *kElementNames[] = { kWaterName, kWindName, kEarthName, kFireName
								  , kLightName, kDarkName, kArcaneName };
	const char *GetName(const uint8_t &n) {
		return kElementNames[n];
	}
}

const uint8_t element::get() const {
	return base_;
}
void element::set(const uint8_t &e) {
	base_ = e;
}

element& element::operator=(const element &e) {
	if (this != &e)
		base_ = e.base_;

	return *this;
}
element& element::operator+=(const uint8_t &e) {
	if (base_ + e <= 0xff)
		base_ += e;
	else
		base_ = 0xff;

	return *this;
}
element& element::operator-=(const uint8_t &e) {
	if (base_ > e)
		base_ -= e;
	else
		base_ = 0;

	return *this;
}
std::ostream& operator<<(std::ostream &out, const element &e) {
	if (&out == &std::cout)
		out << static_cast<int>(e.base_) * 25 / 32 << '%';
	else
		out.write(reinterpret_cast<const char *>(&e.base_), 1);

	return out;
}
std::istream& operator>>(std::istream &in, element &e) {
	in.read(reinterpret_cast<char *>(&e.base_), 1);

	return in;
}