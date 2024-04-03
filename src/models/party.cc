#include "party.h"

#include <algorithm>	// std::copy
#include <iostream>		// i/ostream overloads

#include "actor.h"


const uint32_t party::ID() const {
	return id_;
}
const size_t party::Size() const {
	return kPartySize + name.Size() + (count * 4);
}

actor** party::Begin() {
	return members;
}
actor** party::End() {
	return members + count;
}

int party::AddMember(actor *a) {
	actor **temp;

	if (count >= 250)
		return error_codes::kErr_Party_Full;

	temp = new actor*[count+1];
	if (count > 0)
		std::copy(members, members+count, temp);

	if (persistent == true)
		temp[count] = a;
	else {
		temp[count] = new actor();
		*temp[count] = *a;
	}

	delete[] members;
	++count;
	members = temp;

	return 0;
}
int party::RemoveMember(const int &i) {
	actor **temp;

	if (count == 0)
		return error_codes::kErr_Party_Empty;

	temp = new actor*[count-1];
	std::copy(members, members+i, temp);
	std::copy(members+i+1, members+count, temp+i);
	delete[] members;
	--count;
	members = temp;

	return 0;
}
int party::RemoveMember(actor *a) {
	uint32_t index = 256;

	if (count == 0)
		return error_codes::kErr_Party_Empty;

	for (uint32_t i = 0; i < static_cast<uint32_t>(count); ++i) {
		if (a->ID() == members[i]->ID()) {
			index = i;
		}
	}
	if (index != 256)
		RemoveMember(index);
	else
		return error_codes::kErr_Member_Not_Found;

	return 0;
}

party& party::operator=(const party &p) {
	if (this != &p) {
		id_ = p.id_;
		name = p.name;
		persistent = p.persistent;
		count = p.count;
		members_id = new uint32_t[count];
		std::copy(p.members_id, p.members_id+count, members_id);

		if (persistent)
			members = p.members;
		else {
			members = new actor*[count];
			for(uint8_t i = 0; i < count; ++i)
				members[i] = new actor(*p.members[i]);
		}
	}

	return *this;
}
std::ostream& operator<<(std::ostream &out, const party &p) {
	uint32_t size = p.Size();

	out.write(reinterpret_cast<const char *>(&size), 4);
	out.write(reinterpret_cast<const char *>(&p.id_), 4);
	out << p.name;
	out.write(reinterpret_cast<const char *>(&p.persistent), 1);
	out.write(reinterpret_cast<const char *>(&p.count), 1);
	for (uint8_t i=0; i < p.count; ++i) {
		uint32_t tmp = p.members[i]->ID();
		out.write(reinterpret_cast<const char *>(&tmp), 4);
	}

	return out;
}
std::istream& operator>>(std::istream &in, party &p) {
	uint32_t size = 0;

	if (p.members != NULL)
		delete[] p.members;

	in.read(reinterpret_cast<char *>(&size), 4);
	in.read(reinterpret_cast<char *>(&p.id_), 4);
	in >> p.name;
	in.read(reinterpret_cast<char *>(&p.persistent), 1);
	in.read(reinterpret_cast<char *>(&p.count), 1);
	p.members = new actor*[p.count];
	p.members_id = new uint32_t[p.count];
	for (uint8_t i = 0; i < p.count; ++i) {
		in.read(reinterpret_cast<char *>(&p.members_id[i]), 4);
	}

	return in;
}