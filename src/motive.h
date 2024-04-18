#pragma once


#include <cstdint>
#include <cstddef>


/*******************************************************************************
*                                                                              *
*                                   Motive                                     *
*                                                                              *
*******************************************************************************/
struct actor;
struct spell;

struct motive {
	actor *perpetrator, *target;
	motive *target_motive;
	int action;
	uint32_t damage;
	uint8_t times_attacking;
	uint8_t times_hit;
	uint8_t times_missed;
	bool dodged;
	spell* spell_casted;

	motive& operator=(const motive &m) {
		if (this != &m) {
			perpetrator = m.perpetrator;
			target = m.target;
			target_motive = m.target_motive;
			action = m.action;
			damage = m.damage;
			times_attacking = m.times_attacking;
			times_hit = m.times_hit;
			times_missed = m.times_missed;
			dodged = m.dodged;
			spell_casted = m.spell_casted;
		}

		return *this;
	}
	motive()
		: perpetrator(NULL)
		, target(NULL)
		, target_motive(NULL)
		, action(0)
		, damage(0)
		, times_attacking(0)
		, times_hit(0)
		, times_missed(0)
		, dodged(false)
		, spell_casted(NULL) { }
	motive(actor *p, actor *t, motive *tm, const int &a) 
		: perpetrator(p)
		, target(t)
		, target_motive(tm)
		, action(a)
		, damage(0)
		, times_attacking(0)
		, times_hit(0)
		, times_missed(0)
		, dodged(false)
		, spell_casted(NULL) { }
	~motive() { }
};

namespace actions {
	enum kActions {
		kIdle,
		kAttack,
		kDefend,
		kCast
	};
}