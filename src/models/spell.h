#pragma once


#include <array>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>

#include "effect.h"
#include "pstr.h"


struct actor;
int main();
/*******************************************************************************
*                                                                              *
*                                   Spell_Base                                 *
*                                                                              *
*******************************************************************************/
struct spell_base {
	static const uint16_t kSpellBaseSize = 7;
	static const uint8_t kMaxEffects = 8;

	pstr<uint8_t> name;

	const uint32_t ID() const;
	const size_t Size() const;

	effect* Add(const effect *e);
	bool Remove(const int &i);

	spell_base& operator=(const spell_base &s);
	friend std::ostream& operator<<(std::ostream &out, const spell_base &s);
	friend std::istream& operator>>(std::istream &in, spell_base &s);

	spell_base()
		: name("Fart")
		, id_(0)
		, effect_count_(0)
		, effects_(NULL) { }
	spell_base(const uint32_t &i, const char *n, const std::initializer_list<effect> &ep)
		: name(n)
		, id_(i)
		, effect_count_(ep.size()) {
			effects_ = new effect[effect_count_];
			for (int i = 0; i < effect_count_; ++i) {
				effects_[i] = ep.begin()[i];
			}
		}
	~spell_base() { }

	private:
		friend class spell;
		template<class t> friend struct record;
		friend class xbfq;
		friend class battle;
		friend int main();
		uint32_t id_;
		uint8_t effect_count_;
		effect *effects_;
};

/*******************************************************************************
*                                                                              *
*                                     Spell                                    *
*                                                                              *
*******************************************************************************/
struct spell {
	static const uint16_t kSpellSize = 8;

	const spell_base* SpellBase() const;
	const uint32_t ID() const;
	const size_t Size() const;
	const uint32_t Cost() const;

	bool AddExperience(const uint32_t &e);
	bool Cast(const actor *caster, const actor *target);

	spell& operator=(const spell &s);
	friend std::ostream& operator<<(std::ostream &out, const spell &s);
	friend std::istream& operator>>(std::istream &in, spell &s);

	spell()
		: s_(NULL)
		, id_(0)
		, experience_(0) { }
	spell(const spell_base *s, const uint32_t &e)
		: s_(s)
		, id_(s->id_)
		, experience_(e) { }
	~spell() { }

	private:
		template<class t> friend struct record;
		friend class xbfq;
		friend class battle;
		friend int main();

		const spell_base *s_;
		uint32_t id_, experience_;
};