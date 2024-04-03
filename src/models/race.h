#pragma once


#include <cstdint>
#include <iosfwd>

#include "pstr.h"


struct aspect;
template <class t> struct record;
/*******************************************************************************
*                                                                              *
*                                     Race                                     *
*                                                                              *
*******************************************************************************/
struct race {
	static const uint16_t kRaceSize = 45;
	static const int kVarBytes = 4;

	pstr<uint8_t> name;
	pstr<uint16_t> description;
	aspect *virtue, *vice;
	uint32_t str, wis, agi;
	uint32_t hp, en, ad, re, as, ac;

	const uint32_t ID() const;
	const bool Playable() const;
	const size_t Size() const;

	race& operator=(const race &r);
	friend std::ostream& operator<<(std::ostream &out, const race &r);
	friend std::istream& operator>>(std::istream &in, race &r);

	race()
		: name("Thing")
		, description("A thing.")
		, virtue(NULL)
		, vice(NULL)
		, str(512)
		, wis(512)
		, agi(512)
		, hp(1024)
		, en(1024)
		, ad(1024)
		, re(1024)
		, as(1024)
		, ac(1024)
		, id_(0)
		, playable_(false) { }
	race(const uint32_t &i, const bool &play, const char *n, const char *d,
		 aspect &virtue, aspect &vice,
		 const uint32_t &str, const uint32_t &wis, const uint32_t &agi,
		 const uint32_t &hp, const uint32_t &en, const uint32_t &ad,
		 const uint32_t &re, const uint32_t &as, const uint32_t &ac)
		: name(n)
		, description(d)
		, virtue(&virtue)
		, vice(&vice)
		, str(str)
		, wis(wis)
		, agi(agi)
		, hp(hp)
		, en(en)
		, ad(ad)
		, re(re)
		, as(as)
		, ac(ac)
		, id_(i)
		, playable_(play) { }
	~race() { }

	private:
		template<class t> friend struct record;
		uint32_t id_;
		bool playable_;
};