#include "spell.h"

#include <algorithm>					// std::copy
#include <iostream>						// iostream overloads.

#include "actor.h"						// actor class
#include "effect.h"						// effect class


const uint32_t spell_base::ID() const {
	return id_;
}
const dynarray<effect> spell_base::Effects() const {
	return effects_;
}
const size_t spell_base::Size() const {
	return kSpellBaseSize + name.Size() + effects_.Size();
}

effect* spell_base::Add(const effect *e) {
/*	if (effect_count_ >= kMaxEffects)
		return NULL;

	effect *tmp_effects = new effect[effect_count_ + 1];
	std::copy(effects_, effects_ + effect_count_, tmp_effects);
	tmp_effects[effect_count_] = *e;
	delete[] effects_;
	effects_ = tmp_effects;
	effect_count_++;

	return (effects_ + effect_count_ - 1); */
	return effects_.begin();
}
bool spell_base::Remove(const int &i) {
/*	if (effect_count_ <= 0)
		return false;

	effect *tmp_effects = new effect[effect_count_ - 1];
	std::copy(effects_, effects_ + i, tmp_effects);
	std::copy(effects_ + i + 1, effects_ + effect_count_, tmp_effects + i - 1);
	delete[] effects_;
	effects_ = tmp_effects;
	effect_count_--;*/

	return true;
}

spell_base& spell_base::operator=(const spell_base &s) {
	if (this != &s) {
		id_ = s.id_;
		effects_ = s.effects_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const spell_base &s) {
	uint16_t spell_base_size = s.Size();

	out.write(reinterpret_cast<const char *>(&spell_base_size), 2);
	out << s.name;
	out.write(reinterpret_cast<const char *>(&s.id_), 4);
	out << s.effects_;

	return out;
}
std::istream& operator>>(std::istream &in, spell_base &s) {
	uint16_t spell_base_size;

	in.read(reinterpret_cast<char *>(&spell_base_size), 2);
	in >> s.name;
	in.read(reinterpret_cast<char *>(&s.id_), 4);
	in >> s.effects_;

	return in;
}

const spell_base* spell::SpellBase() const {
	return s_;
}
const uint32_t spell::ID() const {
	return id_;
}
const size_t spell::Size() const {
	return kSpellSize;
}
const uint32_t spell::Cost() const {
	return 1;
}

bool spell::AddExperience(const uint32_t &e) {
	return true;
}
bool spell::Cast(const actor *caster, const actor *target) {
	if (caster->en.get() < Cost())
		return false;

	return true;
}

spell& spell::operator=(const spell &s) {
	if (this != &s) {
		s_ = s.s_;
		id_ = s.id_;
		experience_ = s.experience_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const spell &s) {
	out.write(reinterpret_cast<const char *>(&s.id_), 4);
	out.write(reinterpret_cast<const char *>(&s.experience_), 4);

	return out;
}
std::istream& operator>>(std::istream &in, spell &s) {
	in.read(reinterpret_cast<char *>(&s.id_), 4);
	in.read(reinterpret_cast<char *>(&s.experience_), 4);

	return in;
}