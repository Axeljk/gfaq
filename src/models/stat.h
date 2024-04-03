#pragma once


#include <cstdint>
#include <iosfwd>


struct stat_base;
struct stat_swag;
struct stat_aux;

/*******************************************************************************
*                                                                              *
*                                   Stat Base                                  *
*                                                                              *
*******************************************************************************/
struct stat_base {
	static const int kDecimalPlaces = 24;
	static const int kVarBytes = 4;
	static const double kExperienceMultiplier;
	static const uint32_t kSoftLimit = 0x64000000; // 100 when shifted.

	const char *kName;
	const char *kNameShort;

	static uint32_t Shift(const uint32_t &v);
	static uint32_t Unshift(const uint32_t &v);

	virtual const uint32_t get(const bool &shifted=false) const = 0;
	virtual const uint32_t get_max(const bool &shifted=false) const = 0;
	virtual void mod(const uint32_t &v) = 0;
	virtual void mod_max(const uint32_t &v) = 0;
	virtual void set(const uint32_t &v) = 0;
	virtual void set_max(const uint32_t &v) = 0;

	virtual void Damage(const uint32_t &v) = 0;
	virtual void Restore() = 0;

	stat_base& operator=(const stat_base &s);

	stat_base(const char *n, const char *n2, const int &b=0, const int &d=0, const int &v=0)
		: kName(n)
		, kNameShort(n2)
		, base_(b)
		, decay_(d)
		, value_(v) { }
	stat_base(const stat_base &s)
		: kName(s.kName)
		, base_(s.base_)
		, decay_(s.decay_)
		, value_(s.value_) { }
	virtual ~stat_base() { }

	protected:
		uint32_t base_, decay_, value_;
};

/*******************************************************************************
*                                                                              *
*                                   Stat SWAg                                  *
*                                                                              *
*******************************************************************************/
struct stat_swag : stat_base {
	static const int kSize = 16;

	const uint32_t get(const bool &shifted=false) const;
	const uint32_t get_max(const bool &shifted=false) const;
	const uint32_t get_xp() const;
	void mod(const uint32_t &v);
	void mod_max(const uint32_t &v);
	void set(const uint32_t &v);
	void set_max(const uint32_t &v);

	const uint32_t Offset() const;
	void Damage(const uint32_t &v);
	bool Level(const uint32_t &v);
	void Link(stat_aux *d1, stat_aux *d2, stat_aux *r1, stat_aux *r2);
	void Restore();

	operator uint32_t() const { return value_; }
	stat_swag& operator=(const stat_swag &s);
	friend std::ostream& operator<<(std::ostream &out, const stat_swag &s);
	friend std::istream& operator>>(std::istream &in, stat_swag &s);

	stat_swag(const char *n, const char *n2, const uint32_t &b)
		: stat_base(n, n2, b, 0, b)
		, xp_(0) { }
	stat_swag(const char *n, const char *n2, const uint32_t &b,
			  const uint32_t &d, const uint32_t &v, const int16_t &x)
		: stat_base(n, n2, b, d, v)
		, xp_(x) { }
	stat_swag(const stat_swag &s)
		: stat_base(s.kName, s.kNameShort, s.base_, s.decay_, s.value_)
		, xp_(s.xp_) { }

	private:
		uint32_t xp_;
		stat_aux *d1_;
		stat_aux *d2_;
		stat_aux *r1_;
		stat_aux *r2_;
};

/*******************************************************************************
*                                                                              *
*                                   Stat Aux                                   *
*                                                                              *
*******************************************************************************/
struct stat_aux : stat_base {
	static const int kSize = 12;

	const uint32_t get(const bool &shifted=false) const;
	const uint32_t get_max(const bool &shifted=false) const;
	stat_swag* dom();
	stat_swag* rec();
	void mod(const uint32_t &v);
	void mod_max(const uint32_t &v);
	void set(const uint32_t &v);
	void set_max(const uint32_t &v);

	const uint32_t Offset() const;
	void Damage(const uint32_t &v);
	void Restore();

	operator uint32_t() const { return value_; }
	stat_aux& operator=(const stat_aux &s);
	friend std::ostream& operator<<(std::ostream &out, const stat_aux &s);
	friend std::istream& operator>>(std::istream &in, stat_aux &s);

	/* Constructors. */
	stat_aux(const char *n, const char *n2, stat_swag *d, stat_swag *r,
			 const uint32_t &b)
		: stat_base(n, n2, b, 0, b+d->get()+r->get())
		, dominant_(d)
		, recessive_(r){ }
	stat_aux(const stat_aux &s, stat_swag *d=NULL, stat_swag *r=NULL)
		: stat_base(s.kName, s.kNameShort, s.base_, s.decay_, s.value_)
		, dominant_(d)
		, recessive_(r) { }

	private:
		stat_swag *dominant_, *recessive_;
};

namespace stats {
	const char kStr[] = "Strength";
	const char kWis[] = "Wisdom";
	const char kAgi[] = "Agility";
	const char kHp[] = "Health";
	const char kEn[] = "Energy";
	const char kAd[] = "Attack Damage";
	const char kRe[] = "Reflex";
	const char kAs[] = "Dexterity";
	const char kAc[] = "Precision";

	const char kStrShort[] = "STR";
	const char kWisShort[] = "WIS";
	const char kAgiShort[] = "AGI";
	const char kHpShort[] = "HP";
	const char kEnShort[] = "EN";
	const char kAdShort[] = "AD";
	const char kReShort[] = "RE";
	const char kAsShort[] = "DE";
	const char kAcShort[] = "PR";
}