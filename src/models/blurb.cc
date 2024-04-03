#include "blurb.h"

#include <iostream>


const uint32_t blurb::ID() const {
	return id_;
}
const size_t blurb::Size() const {
	return kBlurbSize + message.Size() + condition.Size();
}
const uint8_t blurb::Type() const {
	return type_;
}

blurb& blurb::operator=(const blurb &b) {
	if (this != &b) {
		id_ = b.id_;
		type_ = b.type_;
		message = b.message;
		condition = b.condition;
	}
	return *this;
}
std::ostream& operator<<(std::ostream &out, const blurb &b) {
	uint16_t blurb_size = b.Size();

	out.write(reinterpret_cast<const char *>(&blurb_size), 2);
	out.write(reinterpret_cast<const char *>(&b.id_), 4);
	out.write(reinterpret_cast<const char *>(&b.type_), 1);
	out << b.message;
	out << b.condition;

	return out;
}
std::istream& operator>>(std::istream &in, blurb &b) {
	uint16_t blurb_size = 0;

	in.read(reinterpret_cast<char *>(&blurb_size), 2);
	in.read(reinterpret_cast<char *>(&b.id_), 4);
	in.read(reinterpret_cast<char *>(&b.type_), 1);
	in >> b.message;
	in >> b.condition;

	return in;
}