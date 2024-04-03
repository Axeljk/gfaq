#pragma once


#include <cstdint>
#include <iosfwd>

#include "element.h"
#include "pstr.h"


struct actor;
/*******************************************************************************
*                                                                              *
*                                    Effect                                    *
*                                                                              *
*******************************************************************************/
struct effect {
	static const uint16_t kEffectSize = 23;

	pstr<uint8_t> name, description;
	bool (*Apply)(actor*, actor*);
	void (*Tick)(actor*, actor*);
	void (*Remove)(actor*, actor*);

	const uint32_t& ID() const;
	const char* GetElementName() const;
	const size_t Size() const;

	const uint32_t CalcCost(const uint32_t &b, const uint32_t &m, const uint32_t &d, const uint32_t &a);

	effect& operator=(const effect &e);
	friend std::ostream& operator<<(std::ostream &out, const effect &e);
	friend std::istream& operator>>(std::istream &in, effect &e);

	effect()
		: Apply(NULL)
		, Tick(NULL)
		, Remove(NULL)
		, id_(0)
		, element_(0)
		, base_cost_(0)
		, base_cost_magnitude_(0)
		, base_cost_duration_(0)
		, base_cost_area_(0) { }
	effect(const uint32_t &i, const char *n, const char *d, const uint8_t &e)
		: name(n)
		, description(d)
		, Apply(NULL)
		, Tick(NULL)
		, Remove(NULL)
		, id_(i)
		, element_(e)
		, base_cost_(0)
		, base_cost_magnitude_(0)
		, base_cost_duration_(0)
		, base_cost_area_(0) { }
	~effect() { }

	private:
		template<class t> friend struct record;
		uint32_t id_;
		uint8_t element_;
		uint32_t base_cost_, base_cost_magnitude_, base_cost_duration_, base_cost_area_;
		
};

struct effect_profile {
	static const uint16_t kEffectProfileSize = 17;

	enum Types {
		kSelf,
		kTarget,
		kArea
	};

	const effect *e;
	uint32_t effect_id;

	const effect* Effect() const;
	const size_t Size() const;

	effect_profile& operator=(const effect_profile &e);
	friend std::ostream& operator<<(std::ostream &out, const effect_profile &e);
	friend std::istream& operator>>(std::istream &in, effect_profile &e);

	effect_profile()
		: e(NULL)
		, effect_id(0)
		, type_(0)
		, magnitude_(0)
		, duration_(0)
		, area_(0) { }
	effect_profile(const effect *e, const uint8_t &t, const uint32_t &m, const uint32_t &d, const uint32_t &a)
		: e(e)
		, effect_id(e->ID())
		, type_(t)
		, magnitude_(m)
		, duration_(d)
		, area_(a) { }
	~effect_profile() { }

	private:
		friend struct effect;
		friend int main();

		uint8_t type_;
		uint32_t magnitude_, duration_, area_;
};