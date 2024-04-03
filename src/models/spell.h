#pragma once


#include <array>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>

#include "effect.h"
#include "pstr.h"


int main();
/*******************************************************************************
*                                                                              *
*                                     Spell                                    *
*                                                                              *
*******************************************************************************/
struct spell {
	static const uint16_t kSpellSize = 11;
	static const uint8_t kMaxEffects = 8;

	pstr<uint8_t> name;

	const uint32_t ID() const;
	const size_t Size() const;

	effect_profile* Add(const effect_profile *e);
	bool Remove(const int &i);

	spell& operator=(const spell &s);
	friend std::ostream& operator<<(std::ostream &out, const spell &s);
	friend std::istream& operator>>(std::istream &in, spell &s);

	spell()
		: name("Fart")
		, id_(0)
		, experience_(0)
		, effect_count_(0)
		, effects_(NULL) { }
	spell(const uint32_t &i, const char *n, const std::initializer_list<effect_profile> &ep)
		: name(n)
		, id_(i)
		, experience_(0)
		, effect_count_(ep.size()) {
			effects_ = new effect_profile[effect_count_];
			for (int i = 0; i < effect_count_; ++i) {
				effects_[i] = ep.begin()[i];
			}
		}
	~spell() { }

	private:
		template<class t> friend struct record;
		friend class xbfq;
		friend class battle;
		friend int main();
		uint32_t id_, experience_;
		uint8_t effect_count_;
		effect_profile *effects_;
};