#pragma once

namespace vm_xbfq {
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
}