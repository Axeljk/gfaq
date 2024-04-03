#pragma once


#include <cstdint>

#include "element.h"


/*******************************************************************************
*                                                                              *
*                                   Aspect                                     *
*                                                                              *
*******************************************************************************/
struct aspect {
	const char *name, *description;
	const uint8_t add, sub;

	aspect(const char *n, const char *d, const int &a, const int &s)
		: name(n)
		, description(d)
		, add(a)
		, sub(s) { }
	virtual ~aspect() { }
};

namespace aspects {
	const uint8_t kElementModifier = 16;

	// Virtues.
	static aspect temperment("Temperance", "Moderation and the mastery of desire is the essence of temperance. Like consumption of water, taking enough can provide life, yet overindulgence may bring about your demise.", elements::kWater, elements::kFire);
	static aspect charity("Charity", "Charity is justice for the needy. What's givable is pity; what's achievable is charity. As the virtue of wind, it's filling the sails of the destitute to reach new distances. Fairness and love of one's neighbor for a better world define it.", elements::kWind, elements::kEarth);
	static aspect diligence("Diligence", "Diligence, with its steady and unwavering commitment, is the virtue of earth. It is the determination of every tiny seed growing into magnificent trees; the unending labor of every ant. Witness the power of mountains piercing the heavens!", elements::kEarth, elements::kWater);
	static aspect kindness("Kindness", "The warmth and gentle glow of kindness brings life to the virtue of fire. Whether helpfulness toward others without compensation, or through the heating of our homes and meals; kindness warms our hearts. The gentle paragon gives until nothing remains; like a flame that burns itself out.", elements::kFire, elements::kDark);
	static aspect chastity("Chastity", "Light is virtuous and as blinding in its purity as the light that shines upon us all. It claims the virtue of chastity, untouched by all that would detract from its brilliance. Chastity maintains the unity of body and spirit. With this it defends one's integrity. Diversion from wild diets, substances, or relations is a must.", elements::kLight, elements::kArcane);
	static aspect patience("Patience", "Biding its time to envelop the world, darkness -- and its virtue of patience -- is the master over desire. It does not squander reward because the path there is difficult. Through patience a ruler can be persuaded. One must endure and forgive to seize victory.", elements::kDark, elements::kWind);
	static aspect humility("Humility", "Humility is the understanding of ones place in the greater workings of the world. It is the virtue of the arcane, suggesting a clairvoyant sight of the world others cannot see.", elements::kArcane, elements::kLight);

	// Vices.
	static aspect sloth("Sloth", "Sloth, sluggish as molasses at times in its failure to act, allows fate to control you. It is the vice of water, which without proper tributaries, sits still and unmoving. Forgoing opportunity for the comfort of listlessness. Those who fear consequence fall pray to it.", elements::kWater, elements::kEarth);
	static aspect wrath("Wrath", "Failure to control one's emotions results in a wrath that cannot be tamed. It is the vice of wind that rampages aimlessly with no rhyme or reason.", elements::kWind, elements::kDark);
	static aspect greed("Greed", "Greed is the insatiable desire for personal gain. It is the vice of earth that slowly claims the world and the remains of all.", elements::kEarth, elements::kWind);
	static aspect gluttony("Gluttony", "Gluttony is the inability to stop consuming. It is the vice of fire eating inching across the world to consume everything.", elements::kFire, elements::kWater);
	static aspect pride("Pride", "Pride is the overvalue of ones self. It is the vice of dazzling light stealing all attention.", elements::kLight, elements::kArcane);
	static aspect envy("Envy", "Envy is the desire for what others possess and achieve. It is the the vice of lurking shadows as light passes.", elements::kDark, elements::kFire);
	static aspect lust("Lust", "Lust is obsession with pleasure. It is the vice of the arcane edging with hint after hint with no reveal of its secrets.", elements::kArcane, elements::kLight);

	aspect& FindVirtue(const int &v);
	aspect& FindVice(const int &v);
}