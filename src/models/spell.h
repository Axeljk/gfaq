#pragma once


#include <array>
#include <cstdint>
#include <initializer_list>
#include <iosfwd>

#include "dynarray.h"
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
	static const uint16_t kSpellBaseSize = 6;
	static const uint8_t kMaxEffects = 8;

	pstr<uint8_t> name;

	const uint32_t ID() const;
	const dynarray<effect> Effects() const;
	const size_t Size() const;

	effect* Add(const effect *e);
	bool Remove(const int &i);

	spell_base& operator=(const spell_base &s);
	friend std::ostream& operator<<(std::ostream &out, const spell_base &s);
	friend std::istream& operator>>(std::istream &in, spell_base &s);

	spell_base()
		: name("Fart")
		, id_(0) { }
	spell_base(const uint32_t &i, const char *n, std::initializer_list<effect> e)
		: name(n)
		, id_(i)
		, effects_(e) { }
	~spell_base() { }

	private:
		friend class spell;
		template<class t> friend struct record;
		friend class xbfq;
		friend class battle;
		friend int main();
		uint32_t id_;
		dynarray<effect> effects_;
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