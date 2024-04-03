#include "effect.h"

#include <iostream>


const uint32_t& effect::ID() const {
	return id_;
}
const char* effect::GetElementName() const {
	return elements::GetName(element_);
}
const size_t effect::Size() const {
	return kEffectSize + name.Size() + description.Size();
}

effect& effect::operator=(const effect &e) {
	if (this != &e) {
		id_ = e.id_;
		name = e.name;
		description = e.description;
		Apply = e.Apply;
		Tick = e.Tick;
		Remove = e.Remove;
		element_ = e.element_;
		base_cost_ = e.base_cost_;
		base_cost_magnitude_ = e.base_cost_magnitude_;
		base_cost_duration_ = e.base_cost_duration_;
		base_cost_area_ = e.base_cost_area_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const effect &e) {
	uint16_t effect_size = e.Size();

	out.write(reinterpret_cast<const char *>(&effect_size), 2);
	out.write(reinterpret_cast<const char *>(&e.id_), 4);
	out << e.name << e.description;
	out.write(reinterpret_cast<const char *>(&e.element_), 1);
	out.write(reinterpret_cast<const char *>(&e.base_cost_), 4);
	out.write(reinterpret_cast<const char *>(&e.base_cost_magnitude_), 4);
	out.write(reinterpret_cast<const char *>(&e.base_cost_duration_), 4);
	out.write(reinterpret_cast<const char *>(&e.base_cost_area_), 4);

	return out;
}
std::istream& operator>>(std::istream &in, effect &e) {
	uint16_t effect_size;

	in.read(reinterpret_cast<char *>(&effect_size), 2);
	in.read(reinterpret_cast<char *>(&e.id_), 4);
	in >> e.name >> e.description;
	in.read(reinterpret_cast<char *>(&e.element_), 1);
	in.read(reinterpret_cast<char *>(&e.base_cost_), 4);
	in.read(reinterpret_cast<char *>(&e.base_cost_magnitude_), 4);
	in.read(reinterpret_cast<char *>(&e.base_cost_duration_), 4);
	in.read(reinterpret_cast<char *>(&e.base_cost_area_), 4);

	return in;
}

const effect* effect_profile::Effect() const {
	return e;
}
const size_t effect_profile::Size() const {
	return kEffectProfileSize;
}

effect_profile& effect_profile::operator=(const effect_profile &ep) {
	if (this != &ep) {
		e = ep.e;
		effect_id = ep.effect_id;
		type_ = ep.type_;
		magnitude_ = ep.magnitude_;
		duration_ = ep.duration_;
		area_ = ep.area_;
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const effect_profile &e) {
	out.write(reinterpret_cast<const char *>(&e.effect_id), 4);
	out.write(reinterpret_cast<const char *>(&e.type_), 1);
	out.write(reinterpret_cast<const char *>(&e.magnitude_), 4);
	out.write(reinterpret_cast<const char *>(&e.duration_), 4);
	out.write(reinterpret_cast<const char *>(&e.area_), 4);

	return out;
}
std::istream& operator>>(std::istream &in, effect_profile &e) {
	in.read(reinterpret_cast<char *>(&e.effect_id), 4);
	in.read(reinterpret_cast<char *>(&e.type_), 1);
	in.read(reinterpret_cast<char *>(&e.magnitude_), 4);
	in.read(reinterpret_cast<char *>(&e.duration_), 4);
	in.read(reinterpret_cast<char *>(&e.area_), 4);

	return in;
}