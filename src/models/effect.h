#pragma once


#include <cstdint>					// uin8_t, uin32_t
#include <initializer_list>			// { } constructor.
#include <iosfwd>					// >>/<< overloads.

#include "element.h"
#include "pstr.h"


struct actor;
/*******************************************************************************
*                                                                              *
*                                  Effect_Base                                 *
*                                                                              *
*******************************************************************************/
struct effect_base {
	static const uint16_t kEffectBaseSize = 23;

	pstr<uint8_t> name, description, rule;
	bool (*Apply)(actor*, actor*);
	void (*Tick)(actor*, actor*);
	void (*Remove)(actor*, actor*);

	const uint32_t& ID() const;
	const char* GetElementName() const;
	const size_t Size() const;

	effect_base& operator=(const effect_base &e);
	friend std::ostream& operator<<(std::ostream &out, const effect_base &e);
	friend std::istream& operator>>(std::istream &in, effect_base &e);

	effect_base()
		: Apply(NULL)
		, Tick(NULL)
		, Remove(NULL)
		, id_(0)
		, element_(0)
		, base_cost_(0)
		, base_cost_magnitude_(0)
		, base_cost_duration_(0)
		, base_cost_area_(0) { }
	effect_base(const uint32_t &i, const char *n, const char *d,
				const uint8_t &e, const std::initializer_list<uint8_t> &r)
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
		, base_cost_area_(0) {
			// These extra steps are unnecessary with C++17 (call .data()).
			char *tmp = new char[r.size()];
			std::copy(r.begin(), r.end(), tmp);
			rule = pstr<uint8_t>(tmp, r.size());
		}
	~effect_base() { }

	private:
		friend struct effect;
		template<class t> friend struct record;
		uint32_t id_;
		uint8_t element_;
		uint32_t base_cost_, base_cost_magnitude_, base_cost_duration_,
			base_cost_area_;
		
};

/*******************************************************************************
*                                                                              *
*                                    Effect                                    *
*                                                                              *
*******************************************************************************/
struct effect {
	static const uint16_t kEffectSize = 17;

	enum Types {
		kSelf,
		kTarget,
		kArea
	};

	const effect_base *e;
	uint32_t effect_id;

	const effect_base* EffectBase() const;
	const uint32_t ID() const;
	const size_t Size() const;

	const uint32_t CalcCost();

	effect& operator=(const effect &e);
	friend std::ostream& operator<<(std::ostream &out, const effect &e);
	friend std::istream& operator>>(std::istream &in, effect &e);

	effect()
		: e(NULL)
		, effect_id(0)
		, type_(0)
		, magnitude_(0)
		, duration_(0)
		, area_(0) { }
	effect(const effect_base *e, const uint8_t &t, const uint32_t &m,
		   const uint32_t &d, const uint32_t &a)
		: e(e)
		, effect_id(e->id_)
		, type_(t)
		, magnitude_(m)
		, duration_(d)
		, area_(a) { }
	~effect() { }

	private:
		friend struct effect_base;
		friend int main();

		uint8_t type_;
		uint32_t magnitude_, duration_, area_;
};