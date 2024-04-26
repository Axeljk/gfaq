#pragma once


#include <cstdint>			// uint8_t and uint32_t
#include <cstddef>			// NULL


struct actor;
struct effect;
struct spell;
/*******************************************************************************
*                                                                              *
*                                   Motive                                     *
*                                                                              *
*******************************************************************************/
struct motive {
	actor* Instigator() const;
	actor* Target() const;
	const int Action() const;
	virtual const uint32_t Damage() = 0;
	virtual const bool Execute() = 0;

	motive& operator=(const motive &m) {
		if (this != &m) {
			instigator_ = m.instigator_;
			target_ = m.target_;
			target_motive_ = m.target_motive_;
			action_ = m.action_;
			damage_ = m.damage_;
			success_ = m.success_;
		}

		return *this;
	}
	motive()
		: instigator_(NULL)
		, target_(NULL)
		, target_motive_(NULL)
		, action_(0)
		, damage_(0)
		, success_(false) { }
	motive(actor *i, actor *t, motive *tm, const int &a) 
		: instigator_(i)
		, target_(t)
		, target_motive_(tm)
		, action_(a)
		, damage_(0)
		, success_(false) { }
	~motive() { }

	protected:
		actor *instigator_, *target_;
		motive *target_motive_;
		int action_;
		uint32_t damage_;
		bool success_;
};

/*******************************************************************************
*                                                                              *
*                                    Attack                                    *
*                                                                              *
*******************************************************************************/
struct attack : motive {
	const uint32_t Damage();
	const bool Execute();

	const uint8_t Attacks() const;
	const uint8_t Hits() const;
	const uint8_t Misses() const;

	attack& operator=(const attack &a);

	attack() 
		: motive(NULL, NULL, NULL, actions::kAttack)
		, times_attacking_(0)
		, times_hit_(0)
		, times_missed_(0) { }
	attack(actor *i, actor *t, motive *tm)
		: motive(i, t, tm, actions::kAttack) { }
	attack(const attack &a)
		: motive(a.instigator_, a.target_, a.target_motive_, actions::kAttack)
		, times_attacking_(a.times_attacking_)
		, times_hit_(a.times_hit_)
		, times_missed_(a.times_missed_) { }

	private:
		uint8_t times_attacking_;
		uint8_t times_hit_;
		uint8_t times_missed_;
};

/*******************************************************************************
*                                                                              *
*                                     Cast                                     *
*                                                                              *
*******************************************************************************/
struct cast : motive {
	const uint32_t Damage();
	const bool Execute();

	const spell* Spell() const;
	const effect* Effect() const;

	cast& operator=(const cast &a);

	cast()
		: motive(NULL, NULL, NULL, actions::kCast)
		, spell_casted_(NULL)
		, current_effect_(NULL) { }
	cast(actor *i, actor *t, motive *tm)
		: motive(i, t, tm, actions::kCast) { }
	cast(const cast &c)
		: motive(c.instigator_, c.target_, c.target_motive_, actions::kCast)
		, spell_casted_(c.spell_casted_)
		, current_effect_(c.current_effect_) { }

	private:
		spell* spell_casted_;
		effect* current_effect_;
};

/*******************************************************************************
*                                                                              *
*                                    Defend                                    *
*                                                                              *
*******************************************************************************/
struct defend : motive {
	const uint32_t Damage();
	const bool Execute();

	defend& operator=(const defend &d);

	defend()
		: motive(NULL, NULL, NULL, actions::kDefend) { }
	defend(actor *i, actor *t, motive *tm)
		: motive(i, t, tm, actions::kDefend) { }
	defend(const defend &d)
		: motive(d.instigator_, d.target_, d.target_motive_, actions::kDefend) { }
};

/*******************************************************************************
*                                                                              *
*                                     Idle                                     *
*                                                                              *
*******************************************************************************/
struct idle : motive {
	const uint32_t Damage();
	const bool Execute();

	idle& operator=(const idle &i);

	idle()
		: motive(NULL, NULL, NULL, actions::kIdle) { }
	idle(actor *i, actor *t, motive *tm)
		: motive(i, t, tm, actions::kIdle) { }
	idle(const idle &i)
		: motive(i.instigator_, i.target_, i.target_motive_, actions::kIdle) { }
};

namespace actions {
	enum kActions {
		kIdle,
		kAttack,
		kDefend,
		kCast
	};
}