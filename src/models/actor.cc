#include "actor.h"

#include <algorithm>					// std::copy
#include <iostream>						// iostream overloads.

#include "aspect.h"
#include "career.h"
#include "race.h"


const uint32_t actor::ID() const {
	return id_;
}
const uint32_t actor::get_level() const {
	uint64_t total_xp = 0;
	total_xp += str.get_xp();
	total_xp += wis.get_xp();
	total_xp += agi.get_xp();
	total_xp /= 3;
	total_xp = (total_xp >> stat_base::kDecimalPlaces);
	total_xp += 1;

	return static_cast<uint32_t>(total_xp);
}
const spell* actor::SpellBook() const {
	return spellbook_.begin();
}

void actor::Level(const int &x) {
	for (int i = 0; i < x; ++i) {
		str.Level(0x01000000);
		wis.Level(0x01000000);
		agi.Level(0x01000000);
	}
}
void actor::PickRace(const race *ra) {
	r = ra;
	race_id = ra->ID();
	str.mod_max(ra->str);
	wis.mod_max(ra->wis);
	agi.mod_max(ra->agi);
	hp.mod_max(ra->hp);
	en.mod_max(ra->en);
	ad.mod_max(ra->ad);
	re.mod_max(ra->re);
	as.mod_max(ra->as);
	ac.mod_max(ra->ac);
	PickVirtueVice(ra->virtue, ra->vice);
	Restore();
}
void actor::PickCareer(const career *ca) {
	c = ca;
	career_id = ca->ID();
	str.mod_max(ca->str);
	wis.mod_max(ca->wis);
	agi.mod_max(ca->agi);
	hp.mod_max(ca->hp);
	en.mod_max(ca->en);
	ad.mod_max(ca->ad);
	re.mod_max(ca->re);
	as.mod_max(ca->as);
	ac.mod_max(ca->ac);
	Restore();
}
void actor::PickVirtueVice(const aspect *virt, const aspect *vi) {
	elements[virt->add] += aspects::kElementModifier;
	elements[virt->sub] -= aspects::kElementModifier;
	elements[vi->add] += aspects::kElementModifier;
	elements[vi->sub] -= aspects::kElementModifier;
}
void actor::Restore() {
	str.Restore();
	wis.Restore();
	agi.Restore();
	hp.Restore();
	en.Restore();
	ad.Restore();
	re.Restore();
	as.Restore();
	ac.Restore();
}
const size_t actor::Size() const {
	return kActorSize + name.Size() + spellbook_.Size() + afflictions_.Size();
}

actor& actor::operator=(const actor &a) {
	if (this != &a) {
		id_ = a.id_;
		name = a.name;
		gender = a.gender;
		playable = a.playable;
		r = a.r;
		c = a.c;
		virtue = a.virtue;
		vice = a.vice;
		str = a.str;
		wis = a.wis;
		agi = a.agi;
		hp = a.hp;
		en = a.en;
		ad = a.ad;
		re = a.re;
		as = a.as;
		ac = a.ac;
		elements[0] = a.elements[0];
		elements[1] = a.elements[1];
		elements[2] = a.elements[2];
		elements[3] = a.elements[3];
		elements[4] = a.elements[4];
		elements[5] = a.elements[5];
		elements[6] = a.elements[6];
		spellbook_ = a.spellbook_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const actor &a) {
	uint16_t actor_size = a.Size();

	out.write(reinterpret_cast<const char *>(&actor_size), 2);
	out.write(reinterpret_cast<const char *>(&a.id_), 4);
	out << a.name;
	out.write(reinterpret_cast<const char *>(&a.gender), 1);
	out.write(reinterpret_cast<const char *>(&a.playable), 1);
	out.write(reinterpret_cast<const char *>(&a.race_id), 4);
	out.write(reinterpret_cast<const char *>(&a.career_id), 4);
	out.write(reinterpret_cast<const char *>(&a.virtue->add), 1);
	out.write(reinterpret_cast<const char *>(&a.vice->add), 1);
	out << a.str;
	out << a.wis;
	out << a.agi;
	out << a.hp;
	out << a.en;
	out << a.ad;
	out << a.re;
	out << a.as;
	out << a.ac;
	out << a.elements[0];
	out << a.elements[1];
	out << a.elements[2];
	out << a.elements[3];
	out << a.elements[4];
	out << a.elements[5];
	out << a.elements[6];
	out << a.spellbook_;
	out << a.afflictions_;

	return out;
}
std::istream& operator>>(std::istream &in, actor &a) {
	uint16_t actor_size = 0;
	uint8_t v = 0;

	in.read(reinterpret_cast<char *>(&actor_size), 2);
	in.read(reinterpret_cast<char *>(&a.id_), 4);
	in >> a.name;
	in.read(reinterpret_cast<char *>(&a.gender), 1);
	in.read(reinterpret_cast<char *>(&a.playable), 1);
	in.read(reinterpret_cast<char *>(&a.race_id), 4);
	in.read(reinterpret_cast<char *>(&a.career_id), 4);
	in.read(reinterpret_cast<char *>(&v), 1);
	a.virtue = &aspects::FindVirtue(v);
	in.read(reinterpret_cast<char *>(&v), 1);
	a.vice = &aspects::FindVice(v);
	in >> a.str;
	in >> a.wis;
	in >> a.agi;
	in >> a.hp;
	in >> a.en;
	in >> a.ad;
	in >> a.re;
	in >> a.as;
	in >> a.ac;
	in >> a.elements[0];
	in >> a.elements[1];
	in >> a.elements[2];
	in >> a.elements[3];
	in >> a.elements[4];
	in >> a.elements[5];
	in >> a.elements[6];
	in >> a.spellbook_;
	in >> a.afflictions_;

	return in;
}