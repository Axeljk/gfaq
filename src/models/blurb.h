#pragma once


#include <cstdint>					// uin8_t, uin32_t
#include <initializer_list>			// { } constructor.
#include <iosfwd>					// >>/<< overloads.

#include "pstr.h"					// message and condition pstrs.


/*******************************************************************************
*                                                                              *
*                                   Blurb                                      *
*                                                                              *
*******************************************************************************/
struct blurb {
	static const uint16_t kBlurbSize = 7;

	enum Types { // Duplicate of motive actions.
		kIdle,
		kAttack,
		kDefend,
		kCast
	};

	pstr<uint8_t> message;
	pstr<uint8_t> condition;

	const uint32_t ID() const;
	const size_t Size() const;
	const uint8_t Type() const;

	blurb& operator=(const blurb &b);
	friend std::ostream& operator<<(std::ostream &out, const blurb &b);
	friend std::istream& operator>>(std::istream &in, blurb &b);

	blurb()
		: message("hurp a derp")
		, condition("")
		, id_(0)
		, type_(0) { }
	blurb(const uint32_t &i, const uint8_t &t, const pstr<uint8_t> &m,
			const char *c, const uint8_t &bytes)
		: message(m)
		, condition(c, bytes)
		, id_(i)
		, type_(t) { }
	blurb(const uint32_t &i, const uint8_t &t, const pstr<uint8_t> &m,
			const std::initializer_list<uint8_t> &c)
		: message(m)
		, id_(i)
		, type_(t)		{
			// These extra steps are unnecessary with C++17 (call .data()).
			char *tmp = new char[c.size()];
			std::copy(c.begin(), c.end(), tmp);
			condition = pstr<uint8_t>(tmp, c.size());
		}
	~blurb() { }

	private:
		template<class t> friend class record;
		uint32_t id_;
		uint8_t type_;
};