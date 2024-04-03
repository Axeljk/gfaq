#include "race.h"

#include <iostream>

#include "aspect.h"


const uint32_t race::ID() const {
	return id_;
}
const bool race::Playable() const {
	return playable_;
}
const size_t race::Size() const {
	return kRaceSize + name.Size() + description.Size();
}

race& race::operator=(const race &r) {
	if ( this != &r ) {
		id_ = r.id_;
		name = r.name;
		description = r.description;
		playable_ = r.playable_;
		virtue = r.virtue;
		vice = r.vice;
		str = r.str;
		wis = r.wis;
		agi = r.agi;
		hp = r.hp;
		en = r.en;
		ad = r.ad;
		re = r.re;
		as = r.as;
		ac = r.ac;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const race &r) {
	uint16_t race_size = r.Size();

	out.write(reinterpret_cast<const char *>(&race_size), 2);
	out.write(reinterpret_cast<const char *>(&r.id_), 4);
	out << r.name;
	out << r.description;
	out.write(reinterpret_cast<const char *>(&r.playable_), 1);
	out.write(reinterpret_cast<const char *>(&r.virtue->add), 1);
	out.write(reinterpret_cast<const char *>(&r.vice->add), 1);
	out.write(reinterpret_cast<const char *>(&r.str), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.wis), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.agi), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.hp), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.en), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.ad), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.re), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.as), race::kVarBytes);
	out.write(reinterpret_cast<const char *>(&r.ac), race::kVarBytes);

	return out;
}
std::istream& operator>>(std::istream &in, race &r) {
	uint16_t race_size = 0;
	uint8_t v = 0;

	in.read(reinterpret_cast<char *>(&race_size), 2);
	in.read(reinterpret_cast<char *>(&r.id_), 4);
	in >> r.name;
	in >> r.description;
	in.read(reinterpret_cast<char *>(&r.playable_), 1);
	in.read(reinterpret_cast<char *>(&v), 1);
	r.virtue = &aspects::FindVirtue(v);
	in.read(reinterpret_cast<char *>(&v), 1);
	r.vice = &aspects::FindVice(v);
	in.read(reinterpret_cast<char *>(&r.str), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.wis), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.agi), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.hp), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.en), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.ad), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.re), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.as), race::kVarBytes);
	in.read(reinterpret_cast<char *>(&r.ac), race::kVarBytes);

	return in;
}