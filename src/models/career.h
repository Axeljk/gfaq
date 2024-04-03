#pragma once


#include <cstdint>
#include <iosfwd>

#include "pstr.h"


/*******************************************************************************
*                                                                              *
*                                    Career                                    *
*                                                                              *
*******************************************************************************/
struct career {
	static const uint16_t kCareerSize = 43;
	static const int kVarBytes = 4;

	pstr<uint8_t> name;
	pstr<uint16_t> description;
	uint32_t str, wis, agi;
	uint32_t hp, en, ad, re, as, ac;

	const uint32_t ID() const;
	const bool Playable() const;
	const size_t Size() const;

	career& operator=(const career &r);
	friend std::ostream& operator<<(std::ostream &out, const career &c);
	friend std::istream& operator>>(std::istream &in, career &c);

	career()
		: name("Janitor")
		, description("This rag technician will sweep you off your feet!")
		, str(0)
		, wis(0)
		, agi(0)
		, hp(0)
		, en(0)
		, ad(0)
		, re(0)
		, as(0)
		, ac(0)
		, id_(0)
		, playable_(false) { }
	career(const uint32_t &i, const bool &play, const char *n, const char *d,
		   const uint32_t &str, const uint32_t &wis, const uint32_t &agi,
		   const uint32_t &hp, const uint32_t &en, const uint32_t &ad,
		   const uint32_t &re, const uint32_t &as, const uint32_t &ac)
		: name(n)
		, description(d)
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
	~career() { }

	private:
		template<class t> friend class record;
		uint32_t id_;
		bool playable_;
};