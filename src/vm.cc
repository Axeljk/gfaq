#include "vm.h"

#include <algorithm>

#include "models/actor.h"
#include "models/aspect.h"
#include "models/career.h"
#include "models/race.h"
#include "models/stat.h"
#include "motive.h"


bool vm::Execute(motive *perp, const uint8_t *input) {
	stack_size_ = 0;
	valid_ = true;

	for (uint8_t i = 0; i < kStackSize; ++i) {
		switch(input[i]) {
			case (Instructions::kExit):
				i = kStackSize - 1;
				break;
			case (Instructions::kGoTo):
				i = input[i + 1];
				break;
			case (Instructions::kLiteral):
				Push_(input[++i]);
				break;
			case (kLiteral4):
				Push4_(*(reinterpret_cast<const uint32_t *> \
						(input + i + 1)));
				i += 4;
				break;
			case (kCompare):
				if (Compare_() == false)
					valid_ = false;
				break;
			case (kCompare4):
				if (Compare4_() == false)
					valid_ = false;
				break;
			case (kCheckAction):
				Push_(GetMotive_(perp)->action);
				break;
			case (kCheckGender):
				Push_(GetActor_(perp)->gender);
				break;
			case (kCheckPlayable):
				Push_(GetActor_(perp)->playable);
				break;
			case (kCheckRace):
				Push4_(GetActor_(perp)->r->ID());
				break;
			case (kCheckCareer):
				Push4_(GetActor_(perp)->c->ID());
				break;
			case (kCheckAspect):
				Push_(GetAspect_(GetActor_(perp))->add);
				break;
			case (kCheckStat):
				Push4_(GetStat_(GetActor_(perp))->get());
				break;
			case (kCheckDamage):
				Push4_(perp->damage);
				break;
			case (kCheckAttacks):
				Push_(perp->times_attacking);
				break;
			case (kCheckHits):
				Push_(perp->times_hit);
				break;
			case (kCheckMisses):
				Push_(perp->times_missed);
				break;
			default:
				valid_ = false;
		}
	}

	return valid_;
}

void vm::Push_(const uint8_t &v) {
	if (stack_size_ >= kStackSize)
		return;

	stack_[stack_size_++] = v;
}
void vm::Push4_(const uint32_t &v) {
	if (stack_size_ + 4 >= kStackSize)
		return;

	std::copy(reinterpret_cast<const char *>(&v),
			  reinterpret_cast<const char *>(&v + 4), (stack_ + stack_size_));
	stack_size_ += 4;
}
uint8_t vm::Pop_() {
	if (stack_size_ < 1) {
		stack_size_ = 0;
		return 0;
	}

	return stack_[--stack_size_];
}
uint32_t vm::Pop4_() {
	if (stack_size_ < 4) {
		stack_size_ = 0;
		return 0;
	}

	stack_size_ -= 4;
	return *(reinterpret_cast<uint32_t *>(stack_ + int(stack_size_)));
}
bool vm::Compare_() {
	uint8_t a = Pop_();
	uint8_t b = Pop_();

	switch(Pop_()) {
		case (Operations::kEqual):
			return (a == b);
		case (Operations::kNotEqual):
			return (a != b);
		case (Operations::kGreaterThan):
			return (a > b);
		case (Operations::kLessThan):
			return (a < b);
		case (Operations::kPercentGreater):
		case (Operations::kPercentLess):
		default:
			return false;
	}
}
bool vm::Compare4_() {
	uint32_t a = Pop4_();
	uint32_t b = Pop4_();

	switch(Pop_()) {
		case (Operations::kEqual):
			return (a == b);
		case (Operations::kNotEqual):
			return (a != b);
		case (Operations::kGreaterThan):
			return (a > b);
		case (Operations::kLessThan):
			return (a < b);
		case (Operations::kPercentGreater):
		case (Operations::kPercentLess):
		default:
			return false;
	}
}
motive* vm::GetMotive_(motive *perp) {
	if (Pop_() == Motive::kPerpetrator)
		return perp;
	else
		return perp->target_motive;
}
actor* vm::GetActor_(motive *perp) {
	if (Pop_() == Motive::kPerpetrator)
		return perp->perpetrator;
	else
		return perp->target;
}
const aspect* vm::GetAspect_(actor *a) {
	if (Pop_() == Aspect::kVirtue)
		return a->virtue;
	else
		return a->vice;
}
const stat_base* vm::GetStat_(actor *a) {
//	return reinterpret_cast<stat_base *>(a + Pop4_());

	switch (Pop4_()) {
		case (kStr):
			return &(a->str);
			break;
		case (kWis):
			return &(a->wis);
			break;
		case (kAgi):
			return &(a->agi);
			break;
		case (kHP):
			return &(a->hp);
			break;
		case (kEN):
			return &(a->en);
			break;
		case (kAD):
			return &(a->ad);
			break;
		case (kRE):
			return &(a->re);
			break;
		case (kAS):
			return &(a->as);
			break;
		case (kAC):
			return &(a->ac);
			break;
	}

	return &(a->str);
}