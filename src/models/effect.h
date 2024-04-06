#pragma once


#include <cstdint>
#include <iosfwd>

#include "element.h"
#include "pstr.h"


struct actor;
/*******************************************************************************
*                                                                              *
<<<<<<< HEAD
*                                  Effect_Base                                 *
*                                                                              *
*******************************************************************************/
struct effect_base {
	static const uint16_t kEffectBaseSize = 23;
=======
*                                    Effect                                    *
*                                                                              *
*******************************************************************************/
struct effect {
	static const uint16_t kEffectSize = 23;
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf

	pstr<uint8_t> name, description;
	bool (*Apply)(actor*, actor*);
	void (*Tick)(actor*, actor*);
	void (*Remove)(actor*, actor*);

	const uint32_t& ID() const;
	const char* GetElementName() const;
	const size_t Size() const;

	const uint32_t CalcCost(const uint32_t &b, const uint32_t &m, const uint32_t &d, const uint32_t &a);

<<<<<<< HEAD
	effect_base& operator=(const effect_base &e);
	friend std::ostream& operator<<(std::ostream &out, const effect_base &e);
	friend std::istream& operator>>(std::istream &in, effect_base &e);

	effect_base()
=======
	effect& operator=(const effect &e);
	friend std::ostream& operator<<(std::ostream &out, const effect &e);
	friend std::istream& operator>>(std::istream &in, effect &e);

	effect()
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
		: Apply(NULL)
		, Tick(NULL)
		, Remove(NULL)
		, id_(0)
		, element_(0)
		, base_cost_(0)
		, base_cost_magnitude_(0)
		, base_cost_duration_(0)
		, base_cost_area_(0) { }
<<<<<<< HEAD
	effect_base(const uint32_t &i, const char *n, const char *d, const uint8_t &e)
=======
	effect(const uint32_t &i, const char *n, const char *d, const uint8_t &e)
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
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
<<<<<<< HEAD
	~effect_base() { }

	private:
		friend struct effect;
=======
	~effect() { }

	private:
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
		template<class t> friend struct record;
		uint32_t id_;
		uint8_t element_;
		uint32_t base_cost_, base_cost_magnitude_, base_cost_duration_, base_cost_area_;
		
};

<<<<<<< HEAD
/*******************************************************************************
*                                                                              *
*                                    Effect                                    *
*                                                                              *
*******************************************************************************/
struct effect {
	static const uint16_t kEffectSize = 17;
=======
struct effect_profile {
	static const uint16_t kEffectProfileSize = 17;
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf

	enum Types {
		kSelf,
		kTarget,
		kArea
	};

<<<<<<< HEAD
	const effect_base *e;
	uint32_t effect_id;

	const effect_base* EffectBase() const;
	const size_t Size() const;

	effect& operator=(const effect &e);
	friend std::ostream& operator<<(std::ostream &out, const effect &e);
	friend std::istream& operator>>(std::istream &in, effect &e);

	effect()
=======
	const effect *e;
	uint32_t effect_id;

	const effect* Effect() const;
	const size_t Size() const;

	effect_profile& operator=(const effect_profile &e);
	friend std::ostream& operator<<(std::ostream &out, const effect_profile &e);
	friend std::istream& operator>>(std::istream &in, effect_profile &e);

	effect_profile()
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
		: e(NULL)
		, effect_id(0)
		, type_(0)
		, magnitude_(0)
		, duration_(0)
		, area_(0) { }
<<<<<<< HEAD
	effect(const effect_base *e, const uint8_t &t, const uint32_t &m, const uint32_t &d, const uint32_t &a)
=======
	effect_profile(const effect *e, const uint8_t &t, const uint32_t &m, const uint32_t &d, const uint32_t &a)
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
		: e(e)
		, effect_id(e->ID())
		, type_(t)
		, magnitude_(m)
		, duration_(d)
		, area_(a) { }
<<<<<<< HEAD
	~effect() { }

	private:
		friend struct effect_base;
=======
	~effect_profile() { }

	private:
		friend struct effect;
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
		friend int main();

		uint8_t type_;
		uint32_t magnitude_, duration_, area_;
};