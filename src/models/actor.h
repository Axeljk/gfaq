#pragma once


#include <cstdint>
#include <iosfwd>

#include "element.h"
#include "pstr.h"
#include "stat.h"


struct aspect;
struct career;
struct race;
/*******************************************************************************
*                                                                              *
*                                     Actor                                    *
*                                                                              *
*******************************************************************************/
namespace gender {
	enum kGenders {
		kMale,
		kFemale
	};
}

struct actor {
	static const uint16_t kActorSize = 145;

	pstr<uint8_t> name;
	bool gender, playable;
	const race *r;
	uint32_t race_id;
	const career *c;
	uint32_t career_id;
	const aspect *virtue, *vice;
	stat_swag str, wis, agi;
	stat_aux hp, en, ad, re, as, ac;
	element elements[7];

	const uint32_t ID() const;
	const uint32_t get_level() const;

	void Level(const int &x);
	void PickRace(const race *ra);
	void PickCareer(const career *ca);
	void PickVirtueVice(const aspect *virt, const aspect *vi);
	void Restore();
	const size_t Size() const;

	actor& operator=(const actor &a);
	friend std::ostream& operator<<(std::ostream &out, const actor &a);
	friend std::istream& operator>>(std::istream &in, actor &a);

	actor()
		: name("JOHN DOE")
		, gender(false)
		, playable(false)
		, r(NULL)
		, race_id(0)
		, c(NULL)
		, career_id(0)
		, virtue(NULL)
		, vice(NULL)
		, str(stats::kStr, stats::kStrShort, 0)
		, wis(stats::kWis, stats::kWisShort, 0)
		, agi(stats::kAgi, stats::kAgiShort, 0)
		, hp(stats::kHp, stats::kHpShort, &str, &agi, 0)
		, en(stats::kEn, stats::kEnShort, &wis, &str, 0)
		, ad(stats::kAd, stats::kAdShort, &str, &wis, 0)
		, re(stats::kRe, stats::kReShort, &agi, &wis, 0)
		, as(stats::kAs, stats::kAsShort, &agi, &str, 0)
		, ac(stats::kAc, stats::kAcShort, &wis, &agi, 0)
		, elements({
			element(elements::kWaterName, 128),
			element(elements::kFireName, 128),
			element(elements::kEarthName, 128),
			element(elements::kWindName, 128),
			element(elements::kLightName, 128),
			element(elements::kDarkName, 128),
			element(elements::kArcaneName, 128)})
		, id_(0) {
				str.Link(&hp, &ad, &en, &as);
				wis.Link(&en, &ac, &ad, &re);
				agi.Link(&re, &as, &hp, &ac);
			}
	actor(const uint32_t &i, const bool &play, const int &lvl, const char *n,
		  const bool &g, const race *ra, const career *ca, const aspect *virtue,
		  const aspect *vice)
		: name(n)
		, gender(g)
		, playable(play)
		, r(ra)
		, race_id(0)
		, c(ca)
		, career_id(0)
		, virtue(virtue)
		, vice(vice)
		, str(stats::kStr, stats::kStrShort, 0)
		, wis(stats::kWis, stats::kWisShort, 0)
		, agi(stats::kAgi, stats::kAgiShort, 0)
		, hp(stats::kHp, stats::kHp, &str, &agi, 0)
		, en(stats::kEn, stats::kEn, &wis, &str, 0)
		, ad(stats::kAd, stats::kAd, &str, &wis, 0)
		, re(stats::kRe, stats::kRe, &agi, &wis, 0)
		, as(stats::kAs, stats::kAs, &agi, &str, 0)
		, ac(stats::kAc, stats::kAc, &wis, &agi, 0)
		, elements({
			element(elements::kWaterName, 128),
			element(elements::kFireName, 128),
			element(elements::kEarthName, 128),
			element(elements::kWindName, 128),
			element(elements::kLightName, 128),
			element(elements::kDarkName, 128),
			element(elements::kArcaneName, 128)})
		, id_(i) {
			str.Link(&hp, &ad, &en, &as);
			wis.Link(&en, &ac, &ad, &re);
			agi.Link(&re, &as, &hp, &ac);
			PickRace(ra);
			PickCareer(ca);
			PickVirtueVice(virtue, vice);
			Level(lvl);
		}
	actor(const actor &a)
		: name(a.name)
		, gender(a.gender)
		, playable(a.playable)
		, r(a.r)
		, race_id(a.race_id)
		, c(a.c)
		, career_id(a.career_id)
		, virtue(a.virtue)
		, vice(a.vice)
		, str(a.str)
		, wis(a.wis)
		, agi(a.agi)
		, hp(a.hp, &str, &agi)
		, en(a.en, &wis, &str)
		, ad(a.ad, &str, &wis)
		, re(a.re, &agi, &wis)
		, as(a.as, &agi, &str)
		, ac(a.ac, &wis, &agi)
		, elements(a.elements)
		, id_(a.id_)
		{
			str.Link(&hp, &ad, &en, &as);
			wis.Link(&en, &ac, &ad, &re);
			agi.Link(&re, &as, &hp, &ac);
		}
	~actor() { }

	private:
		template<class t> friend class record;
		uint32_t id_;
};