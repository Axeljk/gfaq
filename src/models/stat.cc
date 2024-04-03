#include "stat.h"

#include <cstdio>   // For sprintf.
#include <cmath>    // For log10.
#include <cstring>  // For memcpy.
#include <iostream> // For stream >> << overloads.


/*******************************************************************************
*                                                                              *
*                                   Stat Base                                  *
*                                                                              *
*******************************************************************************/
const double stat_base::kExperienceMultiplier = 0.0625;

uint32_t stat_base::Shift(const uint32_t &v) {
	return (v >> kDecimalPlaces);
}
uint32_t stat_base::Unshift(const uint32_t &v) {
	return (v << kDecimalPlaces);
}

stat_base& stat_base::operator=(const stat_base &s) {
	if ( this != &s ) {
		base_ = s.base_;
		decay_ = s.decay_;
		value_ = s.value_;
	}

	return *this;
}

/*******************************************************************************
*                                                                              *
*                                   Stat SWAg                                  *
*                                                                              *
*******************************************************************************/
const uint32_t stat_swag::get(const bool &shifted) const {
//	if (value_ > get_max())
//		value_ = get_max();

	if (shifted)
		return (value_ >> kDecimalPlaces);
	else
		return value_;
}
const uint32_t stat_swag::get_max(const bool &shifted) const {
	if (shifted)
		return ((base_ + xp_ - decay_) >> kDecimalPlaces);
	else
		return base_  + xp_ - decay_;
}
const uint32_t stat_swag::get_xp() const {
	return xp_;
}
void stat_swag::mod(const uint32_t &v) {
	value_ += v;

	if (value_ > get_max())
		value_ = get_max();
}
void stat_swag::mod_max(const uint32_t &v) {
	base_ += v;
	if (base_ > kSoftLimit)
		base_ = kSoftLimit;

	mod(v);
}
void stat_swag::set(const uint32_t &v) {
	value_ = v;

	if (value_ > get_max())
		value_ = get_max();
}
void stat_swag::set_max(const uint32_t &v) {
	if (v > kSoftLimit) {
		value_ += kSoftLimit - base_;
		base_ = kSoftLimit;
	} else {
		if (base_ > v) {
			if (value_ > base_ - v)
				value_ -= base_ - v;
			else
				value_ = 0;
		} else
			value_ += v - base_;

		base_ = v;
	}
}

const uint32_t stat_swag::Offset() const {
	return base_ + xp_;
}
void stat_swag::Damage(const uint32_t &v) {
	if (value_ > v) {
		decay_ = v * v / value_;
		value_ -= v;
	} else {
		decay_ += value_;
		value_ = 0;
	}
}
bool stat_swag::Level(const uint32_t &v) {
	uint32_t old = ((base_ + xp_) >> kDecimalPlaces);

	double tmp1 = static_cast<double>(base_ / 16777216.0);
	double tmp2 = static_cast<double>(xp_) / 16777216.0;
	double tmp = tanh(pow(tmp1, 2.0) / pow(tmp2, 1.3));
	tmp *= v;
	if (tmp < 1)
		tmp = 1;

	if (xp_ + static_cast<uint32_t>(tmp) > kSoftLimit) {
		value_ += xp_ + static_cast<uint32_t>(tmp) - kSoftLimit;
		xp_ = kSoftLimit;

		if (value_ > get_max())
			value_ = get_max();
	} else {
		value_ += static_cast<uint32_t>(tmp);
		xp_ += static_cast<uint32_t>(tmp);
		d1_->mod(static_cast<uint32_t>(tmp));
		d2_->mod(static_cast<uint32_t>(tmp));
		r1_->mod(static_cast<uint32_t>(tmp) >> 1);
		r2_->mod(static_cast<uint32_t>(tmp) >> 1);
	}
	if (((base_ + xp_) >> kDecimalPlaces) > old)
		return true;
	else
		return false;
}
void stat_swag::Link(stat_aux *d1, stat_aux *d2, stat_aux *r1, stat_aux *r2) {
	d1_ = d1;
	d2_ = d2;
	r1_ = r1;
	r2_ = r2;
}
void stat_swag::Restore() {
	value_ = base_ + xp_;
	decay_ = 0;
}

stat_swag& stat_swag::operator=(const stat_swag &s) {
	if ( this != &s ) {
		base_ = s.base_;
		decay_ = s.decay_;
		value_ = s.value_;
		xp_ = s.xp_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const stat_swag &s) {
	if (&out == &std::cout) {
		char output[8];

		sprintf(output, "%3d/%-3d", static_cast<uint8_t>(s.get(true)),
				static_cast<uint8_t>(s.get_max(true)));
		out.write(output, 7);
	} else {
		out.write(reinterpret_cast<const char *>(&s.base_), stat_base::kVarBytes);
		out.write(reinterpret_cast<const char *>(&s.decay_), stat_base::kVarBytes);
		out.write(reinterpret_cast<const char *>(&s.value_), stat_base::kVarBytes);
		out.write(reinterpret_cast<const char *>(&s.xp_), stat_base::kVarBytes);
	}

	return out;
}
std::istream& operator>>(std::istream &in, stat_swag &s) {
	in.read(reinterpret_cast<char *>(&s.base_), stat_base::kVarBytes);
	in.read(reinterpret_cast<char *>(&s.decay_), stat_base::kVarBytes);
	in.read(reinterpret_cast<char *>(&s.value_), stat_base::kVarBytes);
	in.read(reinterpret_cast<char *>(&s.xp_), stat_base::kVarBytes);

	return in;
}


/*******************************************************************************
*                                                                              *
*                                   Stat Aux                                   *
*                                                                              *
*******************************************************************************/
const uint32_t stat_aux::get(const bool &shifted) const {
//	if (value_ > get_max())
//		value_ = get_max();

	if (shifted)
		return (value_ >> kDecimalPlaces);
	else
		return value_;
}
const uint32_t stat_aux::get_max(const bool &shifted) const {
	if (shifted)
		return ((base_ - decay_ + *dominant_ + (*recessive_ >> 1)) \
				>> kDecimalPlaces);
	else
		return base_ - decay_ + *dominant_ + (*recessive_ >> 1);
}
stat_swag* stat_aux::dom() {
	return dominant_;
}
stat_swag* stat_aux::rec() {
	return recessive_;
}
void stat_aux::mod(const uint32_t &v) {
	value_ += v;

	if (value_ > get_max())
		value_ = get_max();
}
void stat_aux::mod_max(const uint32_t &v) {
	base_ += v;
	if (base_ > kSoftLimit)
		base_ = kSoftLimit;

	mod(v);
}
void stat_aux::set(const uint32_t &v) {
	value_ = v;

	if (value_ > get_max())
		value_ = get_max();
}
void stat_aux::set_max(const uint32_t &v) {
	if (v > kSoftLimit) {
		value_ += kSoftLimit - base_;
		base_ = kSoftLimit;
	} else {
		if (base_ > v) {
			if (value_ > base_ - v)
				value_ -= base_ - v;
			else
				value_ = 0;
		} else
			value_ += v - base_;

		base_ = v;
	}
}

const uint32_t stat_aux::Offset() const {
	return base_;
}
void stat_aux::Damage(const uint32_t &v) {
	if (value_ > v) {
		decay_ = v * v / value_;
		value_ -= v;
	} else {
		decay_ += value_;
		value_ = 0;
	}
}
void stat_aux::Restore() {
	value_ = base_ + *dominant_ + (*recessive_ >> 1);
	decay_ = 0;
}

stat_aux& stat_aux::operator=(const stat_aux &s) {
	if ( this != &s ) {
		base_ = s.base_;
		decay_ = s.decay_;
		value_ = s.value_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const stat_aux &s) {
	if (&out == &std::cout) {
		char output[8];

		sprintf(output, "%3d/%-3d", static_cast<uint8_t>(s.get(true)),
				static_cast<uint8_t>(s.get_max(true)));
		out.write(output, 7);
	} else {
		out.write(reinterpret_cast<const char *>(&s.base_), stat_base::kVarBytes);
		out.write(reinterpret_cast<const char *>(&s.decay_), stat_base::kVarBytes);
		out.write(reinterpret_cast<const char *>(&s.value_), stat_base::kVarBytes);
	}

	return out;
}
std::istream& operator>>(std::istream &in, stat_aux &s) {
	in.read(reinterpret_cast<char *>(&s.base_), stat_base::kVarBytes);
	in.read(reinterpret_cast<char *>(&s.decay_), stat_base::kVarBytes);
	in.read(reinterpret_cast<char *>(&s.value_), stat_base::kVarBytes);

	return in;
}