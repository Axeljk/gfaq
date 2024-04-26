#include <cmath>			// tanh
#include <cstdint>			// uint8_t and uint32_t

#include "models/actor.h"	// Actor functions
#include "models/stat.h"	// Stat functions


double Multiplier;

void EnCost(actor *a, const uint32_t &cost) {
	double tmp = static_cast<double>(cost) / pow(2, stat_base::kDecimalPlaces);
	tmp = 0.4 * log(static_cast<double>(tmp)/5+7) / log(1.1) - 8;
	tmp *= pow(2, stat_base::kDecimalPlaces);
	if (a->en.get() > static_cast<uint32_t>(tmp))
		a->en.mod(-static_cast<uint32_t>(tmp));
	else
		a->en.set(0);
}
void LevelAux(stat_aux *stat_to_level, stat_aux *stat_level_against,
			  const uint32_t &amount) {
	double multiplier = Multiplier(stat_level_against->Offset(),
													stat_to_level->Offset());
	uint32_t amt = static_cast<uint32_t>(amount * multiplier);

	stat_to_level->dom()->Level(amt);
	stat_to_level->rec()->Level(amt >> 2);
}
double Multiplier(const uint32_t &numerator, const uint32_t &demoninator) {
	const double kNumberScale = 1.5;
	const double kPercentBaseline = 0.2;

	double n = static_cast<double>(numerator);
	double d = static_cast<double>(demoninator);
	double percent = (n - d) / (n + d) * kNumberScale + kPercentBaseline;

	return tanh(percent) / 2.0 + 0.5;
}