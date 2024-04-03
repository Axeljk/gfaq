#include "spell.h"


#include <algorithm>					// std::copy
#include <iostream>						// iostream overloads.

#include "effect.h"						// effect_profile class


const uint32_t spell::ID() const {
	return id_;
}
const size_t spell::Size() const {
	return kSpellSize + name.Size() + (effect_profile::kEffectProfileSize * effect_count_);
}

effect_profile* spell::Add(const effect_profile *e) {
	if (effect_count_ >= kMaxEffects)
		return NULL;

	effect_profile *tmp_effects = new effect_profile[effect_count_ + 1];
	std::copy(effects_, effects_ + effect_count_, tmp_effects);
	tmp_effects[effect_count_] = *e;
	delete[] effects_;
	effects_ = tmp_effects;
	effect_count_++;

	return (effects_ + effect_count_ - 1);
}
bool spell::Remove(const int &i) {
	if (effect_count_ <= 0)
		return false;

	effect_profile *tmp_effects = new effect_profile[effect_count_ - 1];
	std::copy(effects_, effects_ + i, tmp_effects);
	std::copy(effects_ + i + 1, effects_ + effect_count_, tmp_effects + i - 1);
	delete[] effects_;
	effects_ = tmp_effects;
	effect_count_--;

	return true;
}

spell& spell::operator=(const spell &s) {
	if (this != &s) {
		id_ = s.id_;
		experience_ = s.experience_;
		effect_count_ = s.effect_count_;
		std::copy(s.effects_, s.effects_ + effect_count_, effects_);
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const spell &s) {
	uint16_t spell_size = s.Size();

	out.write(reinterpret_cast<const char *>(&spell_size), 2);
	out << s.name;
	out.write(reinterpret_cast<const char *>(&s.id_), 4);
	out.write(reinterpret_cast<const char *>(&s.experience_), 4);
	out.write(reinterpret_cast<const char *>(&s.effect_count_), 1);
	for (uint8_t i = 0; i < s.effect_count_; ++i)
		out << s.effects_[i];

	return out;
}
std::istream& operator>>(std::istream &in, spell &s) {
	uint16_t spell_size;

	in.read(reinterpret_cast<char *>(&spell_size), 2);
	in >> s.name;
	in.read(reinterpret_cast<char *>(&s.id_), 4);
	in.read(reinterpret_cast<char *>(&s.experience_), 4);
	in.read(reinterpret_cast<char *>(&s.effect_count_), 1);
	s.effects_ = new effect_profile[s.effect_count_];
	for (uint8_t i = 0; i < s.effect_count_; ++i)
		in >> s.effects_[i];

	return in;
}