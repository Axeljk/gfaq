#include "aspect.h"


namespace aspects {
	aspect& FindVirtue(const int &v) {
		if (v == 0)
			return temperment;
		else if (v == 1)
			return charity;
		else if (v == 2)
			return diligence;
		else if (v == 3)
			return kindness;
		else if (v == 4)
			return chastity;
		else if (v == 5)
			return patience;
		else
			return humility;
	}
	aspect& FindVice(const int &v) {
		if (v == 0)
			return sloth;
		else if (v == 1)
			return wrath;
		else if (v == 2)
			return greed;
		else if (v == 3)
			return gluttony;
		else if (v == 4)
			return pride;
		else if (v == 5)
			return envy;
		else
			return lust;
	}
}