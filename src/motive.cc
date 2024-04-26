#include "motive.h"

#include <cstdlib>			// srand.

#include "models/actor.h"
#include "algorithm.cc"     // Base128Comparison()


/*******************************************************************************
*                                                                              *
*                                   Motive                                     *
*                                                                              *
*******************************************************************************/
actor* motive::Instigator() const {
	return instigator_;
}
actor* motive::Target() const {
	return target_;
}
const int motive::Action() const {
	return action_;
}

/*******************************************************************************
*                                                                              *
*                                    Attack                                    *
*                                                                              *
*******************************************************************************/
const uint32_t attack::Damage() {
    damage_ = instigator_->ad.get() >> 3;

    return damage_;
}
const bool attack::Execute()  {
	uint32_t damage = Damage();
	int roll = std::rand() % 128;
	int chance = 128 * Multiplier(instigator_->ac.get(), target_->re.get());
	bool defending = (target_motive_->Action() == actions::kDefend);
	bool hit = (roll < chance);
	uint32_t health_log = 0;

	if (target_->hp.get(true) == 0) {
		return 0;
	} else if (instigator_->en.get(true) == 0) {
		return 0;
	}
	//EnCost(m.perpetrator, m.perpetrator->ad);

	if (defending) {
		damage = damage >> 1;
		hit = (roll < (chance * 0.8));
	}

	if (hit) {
		LevelAux(&instigator_->ad, &target_->hp, 0x00002000);
		LevelAux(&target_->hp, &instigator_->ad, 0x00001000);
		LevelAux(&instigator_->ac, &target_->re, 0x00020000);
		LevelAux(&target_->re, &instigator_->ac, 0x00010000);
		health_log = target_->hp.get();
		target_->hp.Damage(damage);
		damage_ += health_log - target_->hp.get();
		times_hit_++;
	} else { // Miss.
		times_missed_++;
		LevelAux(&instigator_->ac, &target_->re, 0x00010000);
		LevelAux(&target_->re, &instigator_->ac, 0x00040000);
	}

	if (times_missed_ > 0 && times_hit_ == 0)
		success_ = false;

    return success_;
}
const uint8_t attack::Attacks() const {
    times_attacking_;
}
const uint8_t attack::Hits() const {
    return times_hit_;
}
const uint8_t attack::Misses() const {
    return times_missed_;
}

attack& attack::operator=(const attack &a) {
    if (this != &a) {
        instigator_ = a.instigator_;
        target_ = a.target_;
        target_motive_ = a.target_motive_;
        action_ = a.action_;
        damage_ = a.damage_;
        success_ = a.success_;
        times_attacking_ = a.times_attacking_;
		times_hit_ = a.times_hit_;
		times_missed_ = a.times_missed_;
    }

    return *this;
}

/*******************************************************************************
*                                                                              *
*                                     Cast                                     *
*                                                                              *
*******************************************************************************/
const uint32_t cast::Damage() {
    return 0x00000000;
}
const bool cast::Execute() {
    return true;
}
const spell* cast::Spell() const {
    return spell_casted_;
}
const effect* cast::Effect() const {
    return current_effect_;
}

cast& cast::operator=(const cast &c) {
	if (this != &c) {
		instigator_ = c.instigator_;
        target_ = c.target_;
        target_motive_ = c.target_motive_;
        action_ = actions::kCast;
        damage_ = c.damage_;
        success_ = c.success_;
        spell_casted_ = c.spell_casted_;
        current_effect_ = c.current_effect_;
	}

	return *this;
}

/*******************************************************************************
*                                                                              *
*                                    Defend                                    *
*                                                                              *
*******************************************************************************/
const uint32_t defend::Damage() {
    return 0x00000000;
}
const bool defend::Execute() {
    return true;
}

defend& defend::operator=(const defend &d) {
	if (this != &d) {
		instigator_ = d.instigator_;
        target_ = d.target_;
        target_motive_ = d.target_motive_;
        action_ = actions::kDefend;
        damage_ = 0x00000000;
        success_ = true;
	}

	return *this;
}

/*******************************************************************************
*                                                                              *
*                                     Idle                                     *
*                                                                              *
*******************************************************************************/
const uint32_t idle::Damage() {
    return 0x00000000;
}
const bool idle::Execute() {
    return true;
}

idle& idle::operator=(const idle &i) {
	if (this != &i) {
		instigator_ = i.instigator_;
        target_ = i.target_;
        target_motive_ = i.target_motive_;
        action_ = actions::kIdle;
        damage_ = 0x00000000;
		success_ = true;
	}

	return *this;
}