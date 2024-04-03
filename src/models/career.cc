#include "career.h"

#include <iostream>


const uint32_t career::ID() const {
	return id_;
}
const bool career::Playable() const {
	return playable_;
}
const size_t career::Size() const {
	return kCareerSize + name.Size() + description.Size();
}

career& career::operator=(const career &c) {
	if ( this != &c ) {
		id_ = c.id_;
		name = c.name;
		description = c.description;
		playable_ = c.playable_;
		str = c.str;
		wis = c.wis;
		agi = c.agi;
		hp = c.hp;
		en = c.en;
		ad = c.ad;
		re = c.re;
		as = c.as;
		ac = c.ac;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const career &c) {
	uint16_t career_size = c.Size();

	out.write(reinterpret_cast<const char *>(&career_size), 2);
	out.write(reinterpret_cast<const char *>(&c.id_), 4);
	out << c.name;
	out << c.description;
	out.write(reinterpret_cast<const char *>(&c.playable_), 1);
	out.write(reinterpret_cast<const char *>(&c.str), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.wis), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.agi), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.hp), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.en), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.ad), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.re), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.as), career::kVarBytes);
	out.write(reinterpret_cast<const char *>(&c.ac), career::kVarBytes);

	return out;
}
std::istream& operator>>(std::istream &in, career &c) {
	uint16_t career_size = 0;

	in.read(reinterpret_cast<char *>(&career_size), 2);
	in.read(reinterpret_cast<char *>(&c.id_), 4);
	in >> c.name;
	in >> c.description;
	in.read(reinterpret_cast<char *>(&c.playable_), 1);
	in.read(reinterpret_cast<char *>(&c.str), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.wis), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.agi), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.hp), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.en), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.ad), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.re), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.as), career::kVarBytes);
	in.read(reinterpret_cast<char *>(&c.ac), career::kVarBytes);

	return in;
}