#pragma once


#include <cstdint>
#include <initializer_list>
#include <iosfwd>

#include "pstr.h"


struct actor;
/*******************************************************************************
*                                                                              *
*                                    Party                                     *
*                                                                              *
*******************************************************************************/
struct party {
	static const uint32_t kPartySize = 10;

	pstr<uint8_t> name;
	bool persistent;
	uint8_t count;
	actor **members;
	uint32_t *members_id;

	const uint32_t ID() const;
	const size_t Size() const;

	actor** Begin();
	actor** End();

	int AddMember(actor *a);
	int RemoveMember(const int &i);
	int RemoveMember(actor *a);

	party& operator=(const party &p);
	friend std::ostream& operator<<(std::ostream &out, const party &p);
	friend std::istream& operator>>(std::istream &in, party &p);

	party()
		: name("Adventurers")
		, persistent(false)
		, count(0)
		, members(NULL)
		, members_id(NULL)
		, id_(0) { }
	party(const uint32_t &id, const pstr<uint8_t> &n,
		  const std::initializer_list<actor*> &m)
		: name(n)
		, count(m.size())
		, id_(id) {
			members = new actor*[count];
			for (int i = 0; i < count; ++i) {
				members[i] = m.begin()[i];
			}
		}
	party(const party &p)
		: name(p.name)
		, persistent(p.persistent)
		, count(p.count)
		, id_(p.id_) {
			if (p.members_id != NULL) {
				members_id = new uint32_t[count];
				std::copy(p.members_id, p.members_id+count, members_id);
			}
			if (persistent)
				members = p.members;
			else {
				members = new actor*[count];
				for (int i = 0; i < count; ++i)
					members[i] = p.members[i];
			}
		}
	~party() { }

	private:
		template<class t> friend class record;
		uint32_t id_;
};

namespace error_codes {
	enum kErrorCodes {
		kErr_Party_Full = 1,
		kErr_Party_Empty = 2,
		kErr_Member_Not_Found = 3
	};
}