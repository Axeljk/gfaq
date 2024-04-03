#pragma once


#include <cstdint>


struct actor;
struct aspect;
struct motive;
struct stat_base;
/*******************************************************************************
*                                                                              *
*                                      VM                                      *
*                                                                              *
*******************************************************************************/
struct vm {
	static const int kStackSize = 255;

	enum Instructions {
		kExit,
		kGoTo,
		kLiteral,
		kLiteral4,
		kCompare,
		kCompare4,
		kCheckAction,
		kCheckName,
		kCheckGender,
		kCheckPlayable,
		kCheckRace,
		kCheckCareer,
		kCheckAspect,
		kCheckStat,
		kCheckDamage,
		kCheckAttacks,
		kCheckHits,
		kCheckMisses
	};
	enum Operations {
		kEqual,
		kNotEqual,
		kGreaterThan,
		kLessThan,
		kPercentGreater,
		kPercentLess
	};
	enum Motive {
		kPerpetrator,
		kTarget
	};
	enum Aspect {
		kVirtue,
		kVice
	};
	enum Stats {
		kStr = 44,
		kWis = 88,
		kAgi = 132,
		kHP = 176,
		kEN = 208,
		kAD = 240,
		kRE = 272,
		kAS = 304,
		kAC = 336
	};
	
	bool Execute(motive *perp, const uint8_t *instructions);
	const bool Valid() const;

	vm() 
		: stack_size_(0)
		, valid_(true) { }
	~vm() { }

	private:
		uint8_t stack_[kStackSize];
		uint8_t stack_size_;
		bool valid_;

		void Push_(const uint8_t &v);
		void Push4_(const uint32_t &v);
		uint8_t Pop_();
		uint32_t Pop4_();
		bool Compare_();
		bool Compare4_();
		motive* GetMotive_(motive *perp);
		actor* GetActor_(motive *perp);
		const aspect* GetAspect_(actor *a);
		const stat_base* GetStat_(actor *a);
};