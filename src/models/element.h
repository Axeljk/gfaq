#pragma once


#include <cstdint>
#include <iosfwd>


/*******************************************************************************
*                                                                              *
*                                   Element                                    *
*                                                                              *
*******************************************************************************/
struct element {
	const char *name;

	const uint8_t get() const;
	void set(const uint8_t &e);

	element& operator=(const element &e);
	element& operator+=(const uint8_t &e);
	element& operator-=(const uint8_t &e);
	friend std::ostream& operator<<(std::ostream &out, const element &e);
	friend std::istream& operator>>(std::istream &in, element &e);

	element(const char *n, const uint8_t &b)
		: name(n)
		, base_(b) { }
	~element() { }

	private:
		uint8_t base_;
};

namespace elements {
	enum kElements {
		kWater,
		kWind,
		kEarth,
		kFire,
		kLight,
		kDark,
		kArcane
	};
	const char kWaterName[] = "Water";
	const char kWindName[] = "Wind";
	const char kEarthName[] = "Earth";
	const char kFireName[] = "Fire";
	const char kLightName[] = "Light";
	const char kDarkName[] = "Dark";
	const char kArcaneName[] = "Arcane";
	extern const char *kElementNames[];
	const char* GetName(const uint8_t &n);
}