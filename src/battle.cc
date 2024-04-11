#include "battle.h"

#include <cstdint>			// uint32_t.
#include <cstdlib>			// srand.
#include <ctime>			// time.
#include <cmath>			// log, asinh.
#include <algorithm>		// sort.
#include <iostream>			// cout.
#include <iomanip>			// formatting cout.
#include <deque>			// deque<blurb> valid.

#include "models/actor.h"
#include "models/blurb.h"
#include "models/effect.h"
#include "models/party.h"
#include "models/spell.h"
#include "chargen.h"		// Header display.
#include "console_util.h"	// Newscreen(), Pause(), Parse(), etc.


bool testfunct(motive &i, motive &j) { return ( i.perpetrator->as > j.perpetrator->as); }
int battle::Attack(motive &m) {
	uint32_t damage = (m.perpetrator->ad.get(false) >> 3);
	double percent = static_cast<double>(1u + std::rand()/((RAND_MAX + 1u)/128u)) / 128.0 * 100.0;
	double percent2 = 128u * StatPercent(&m.perpetrator->ac, &m.target->re);
	bool defending = (m.target_motive->action == actions::kDefend);
	bool hit = (percent < percent2);
	uint32_t health_log = 0;

	if (m.target->hp == 0) {
		return 0;
	} else if (m.perpetrator->en.get(true) == 0) {
		return 0;
	}
	EnCost(m.perpetrator, m.perpetrator->ad);
//	std::cout << std::setprecision(0) << percent << "%/" << percent2 << "% (" << StatPercent(&m.perpetrator->ac, &m.target->re) << ")\t";
	if (hit) {
		if (defending) {
			damage = damage >> 1;
			hit = (percent < (percent2 * 0.8));
			percent2 *= 50.0;

			if (hit) { // Hit even when dodging.
				LevelStat(*m.perpetrator, (kXPAttack << 1), &m.perpetrator->ad, &m.target->hp);
				LevelStat(*m.target, (kXPAttack << 1), &m.target->hp, &m.perpetrator->ad);
				LevelStat(*m.perpetrator, (kXPHit << 1), &m.perpetrator->ac, &m.target->re);
				LevelStat(*m.target, (kXPHit >> 1), &m.target->re, &m.perpetrator->ac);
				health_log = m.target->hp.get();
				m.target->hp.Damage(damage);
				m.damage += health_log - m.target->hp.get();
				m.times_hit++;
			} else {
				m.times_missed++;
				m.dodged = true;
				LevelStat(*m.perpetrator, (kXPHit >> 1), &m.perpetrator->ac, &m.target->re);
				LevelStat(*m.target, (kXPHit << 1), &m.target->re, &m.perpetrator->ac);
			}
		} else {
			percent2 *= 50.0;
			LevelStat(*m.perpetrator, kXPAttack, &m.perpetrator->ad, &m.target->hp);
			LevelStat(*m.target, (kXPAttack >> 1), &m.target->hp, &m.perpetrator->ad);
			LevelStat(*m.perpetrator, kXPHit, &m.perpetrator->ac, &m.target->re);
			LevelStat(*m.target, (kXPHit >> 1), &m.target->re, &m.perpetrator->ac);
			health_log = m.target->hp.get();
			m.target->hp.Damage(damage);
			m.damage += health_log - m.target->hp.get();
			m.times_hit++;
		}
	} else { // Miss.
		m.times_missed++;
		LevelStat(*m.perpetrator, (kXPHit << 1), &m.perpetrator->ac, &m.target->re);
		LevelStat(*m.target, (kXPHit >> 1), &m.target->re, &m.perpetrator->ac);
	}

	return 0;
}
int battle::Battle() {
	int players_alive=0, enemies_alive=0;
	motives_count = static_cast<int>(player->count) + static_cast<int>(enemies->count);
	motives = new motive[motives_count];

	do {
		std::srand(std::time(NULL));
		std::cout << std::fixed;

		// Player input.
		for (uint8_t i = 0; i < player->count; ++i) {
			if (player->members[i]->hp > 0) {
				if (player->members[i]->playable == true) {
					// TODO: move all of this into BattleStatus (plus rename).
					NewScreen();
					CharGen::Header(player->members[i]);
					BattleStatus();
					PlayerInput(player->members[i], motives[i]);
				} else {
					motives[i] = motive(player->members[i], player->members[i], motives + i, actions::kIdle);
				}
			}
		}
		// Enemies input.
		for (uint8_t i = 0; i < enemies->count; ++i) {
			if (enemies->members[i]->en > 0x00800000) {
				int target = std::rand()/((RAND_MAX + 1u)/player->count);
				motives[player->count + i] = motive(enemies->members[i], player->members[target], motives + target, actions::kAttack);
			} else
				motives[player->count + i] = motive(enemies->members[i], enemies->members[i], motives + player->count + i, actions::kDefend);
		}
		ActionReport();

		players_alive = 0;
		enemies_alive = 0;
		for (actor **i = player->Begin(); i < player->End(); ++i) {
			if ((**i).hp > 0)
				++players_alive;
		}
		for (actor **i = enemies->Begin(); i < enemies->End(); ++i) {
			if ((**i).hp > 0)
				++enemies_alive;
		}
	} while (players_alive > 0 && enemies_alive > 0);

	// Victory!
	if (enemies_alive == 0) {
		// victory screen.
		NewScreen();
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n"
				  << "             You manage to stall the encroaching jaws of "
				  << "defeat\n           another day. The god of war has been "
				  << "satisfied."
				  << "\n\n\n\n\n\n\n\n\n\n\n";
		Pause();
	}

	// Defeat.
	if (players_alive == 0) {
		// defeat screen.
		NewScreen();
		std::cout << "\n\n\n\n\n\n\n\n\n\n\n"
				  << "             The last of your party slumps to the ground."
				  << " You have\n           been defeated. But no one shall "
				  << "mourn the loss."
				  << "\n\n\n\n\n\n\n\n\n\n\n";
		Pause();
		return 1;
	}

	return 0;
}
void battle::BattleStatus() {
	uint8_t party_length = (player->count > enemies->count ? player->count : enemies->count);
	party_length = 5;
	std::cout << "\n        " << std::left << std::setw(24) << Truncate(player->name, 24) << "           " << std::setw(24) << Truncate(enemies->name, 24);
	for (uint8_t i = 0; i < party_length; ++i) {
		if (player->count > i) {
			std::cout << "\n          " << std::setw(23) << Truncate(player->members[i]->name, 23);
		} else
			std::cout << "\n                                 ";
		if (enemies->count > i) {
			std::cout << "          " << std::setw(23) << Truncate(enemies->members[i]->name, 23);
		}
		
		if (player->count > i) {
			std::cout << "\n              HP";
			if (player->members[i]->hp == 0)
				std::cout << "  DEAD ";
			else
				std::cout << player->members[i]->hp;
			std::cout << "    EN" << player->members[i]->en << "      ";
		} else
			std::cout << "\n                                                 ";
		if (enemies->count > i) {
			std::cout << "     HP";
			if (enemies->members[i]->hp == 0)
				std::cout << "  DEAD ";
			else
				std::cout << enemies->members[i]->hp;
			std::cout << "    EN" << enemies->members[i]->en;
		}
	}
	std::cout << '\n';
}
void battle::ActionReport() {
	std::sort(motives, motives+motives_count, testfunct);
	base_speed = motives[motives_count-1].perpetrator->as;

	NewScreen();
	for (int i = 0; i < motives_count; ++i) {
		if (motives[i].perpetrator->hp > 0) {
			motives[i].damage = 0;
			motives[i].times_attacking = 0;
			motives[i].times_hit = 0;
			motives[i].times_missed = 0;
			motives[i].dodged = false;

			if (motives[i].action == actions::kIdle) {
				motives[i].perpetrator->en.mod(motives[i].perpetrator->en.get_max() >> 2);
			} else if (motives[i].action == actions::kDefend) {
				motives[i].perpetrator->en.mod(motives[i].perpetrator->en.get_max() >> 1);
			} else if (motives[i].action == actions::kAttack) {
				double attacks = static_cast<double>(motives[i].perpetrator->as % base_speed);
				double bonus_attack = std::rand() / ((RAND_MAX + 1u) / static_cast<double>(base_speed));

				motives[i].times_attacking = motives[i].perpetrator->as / base_speed + (attacks >= bonus_attack);
				for (uint32_t j = 0; j < motives[i].times_attacking; ++j)
					Attack(motives[i]);
			}
		}

//		std::cout << "dam: " << motives[i].damage << ", attacks: " << int(motives[i].times_attacking) << ", hit/miss: " << int(motives[i].times_hit) << '/' << int(motives[i].times_missed) << '\n';
		RandomBlurb(motives + i);
	}
	Pause();
}
void battle::PlayerInput(actor *p, motive &m) {
	int input = -1;

	while (input == -1) {
		std::cout << "\tCommand for " << p->name << ":\n\t  "
				  << "0) Attack\n\t  "
				  << "1) Cast\n\t  "
				  << "2) Defend\n\t  ";
		std::cin >> input;

		if (input == 0) {
			// Get target.
			actor *target = GetTarget(p, m, input);
			m = motive(p, target, motives + player->count + input, actions::kAttack);
		} else if (input == 1) {
			spell *casted = GetSpell(p, m, input);
			actor *target = GetTarget(p, m, input);
			m = motive(p, target, motives + player->count + input, actions::kCast);
		} else if (input == 2) {
			m = motive(p, p, motives, actions::kDefend);
		} else if (input == 666) {
			m = motive(p, p, motives, actions::kIdle);
			CheatLevel(p, 10);
		} else {
			int target = std::rand()/((RAND_MAX + 1u)/enemies->count);
			m = motive(p, enemies->members[target], motives + player->count + target, actions::kAttack);
		}
	}
}
actor* battle::GetTarget(actor *p, motive &m, int &i) {
	int selection = -1;

	while (selection > enemies->count || selection < 0) {
		NewScreen();
		CharGen::Header(p);
		std::cout << "\n\tTarget:\n\t  ";
		for (int i = 0; i < enemies->count; ++i)
			std::cout << i << ") " << enemies->members[i]->name << "\n\t  ";
		std::cout << static_cast<int>(enemies->count) << ") Go back\n\t  ";
		std::cin >> selection;
		if (selection < enemies->count && selection >= 0)
			return enemies->members[selection];
		else if (selection == enemies->count) {
			i = -1;
			return NULL;
		}
	}
	return NULL;
}
spell* battle::GetSpell(actor *p, motive &m, int &i) {
	int selection = -1;

	while (selection > p->spellbook_.Length() || selection < 0) {
		NewScreen();
		CharGen::Header(p);
		std::cout << "\n\tSelect Spell:\n\t  ";
		for (int i = 0; i < p->spellbook_.Length(); ++i)
			std::cout << i << ") " <<p->spellbook_[i].SpellBase()->name << "\n\t  ";
		std::cout << static_cast<int>(p->spellbook_.Length()) << ") Go back\n\t  ";
		std::cin >> selection;
		if (selection < p->spellbook_.Length() && selection >= 0)
			return &(p->spellbook_[selection]);
		else if (selection == p->spellbook_.Length()) {
			i = -1;
			return NULL;
		}
	}
	return NULL;
}
double battle::StatPercent(stat_base *a, stat_base *b) {
	return (static_cast<double>(a->get()) - static_cast<double>(b->get())) / (static_cast<double>(a->get()) + static_cast<double>(b->get())) / 2.0 + 0.5;
}
void battle::LevelStat(const actor &act, const uint32_t x, stat_aux *a, stat_aux *b) {
	uint32_t foo = a->dom()->Offset() + (a->rec()->Offset() >> 1) - a->Offset();
	uint32_t bar = b->dom()->Offset() + (b->rec()->Offset() >> 1) - b->Offset();
	uint32_t amt = x * XPMultiplier(foo - bar);

	if (a->dom()->kName == stats::kStr) {
		if (a->dom()->Level(amt * kXPMultStr) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->dom()->kName << " has leveled!\n";
	} else if (a->dom()->kName == stats::kWis) {
		if (a->dom()->Level(amt * kXPMultWis) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->dom()->kName << " has leveled!\n";
	} else {
		if (a->dom()->Level(amt * kXPMultAgi) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->dom()->kName << " has leveled!\n";
	}
	if (a->rec()->kName == stats::kStr) {
		if (a->rec()->Level((amt >> 2) * kXPMultStr) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->rec()->kName << " has leveled!\n";
	} else if (a->rec()->kName == stats::kWis) {
		if (a->rec()->Level((amt >> 2) * kXPMultStr) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->rec()->kName << " has leveled!\n";
	} else {
		if (a->rec()->Level((amt >> 2) * kXPMultAgi) && act.playable == true) { }
//			std::cout << "\t" << act.name << "'s " << a->rec()->kName << " has leveled!\n";
	}
}
void battle::EnCost(actor *a, const uint32_t &cost) {
	double tmp = static_cast<double>(cost) / pow(2, stat_base::kDecimalPlaces);
	tmp = 0.4 * log(static_cast<double>(tmp)/5+7) / log(1.1) - 8;
	tmp *= pow(2, stat_base::kDecimalPlaces);
	if (a->en.get(false) > static_cast<uint32_t>(tmp))
		a->en.mod(-static_cast<uint32_t>(tmp));
	else
		a->en.set(0);
}
double battle::XPMultiplier(const int &x) {
	double tmp = x / 16777216.0;
	return asinh(static_cast<double>(tmp)*0.7)*0.2+1.2;
}
void battle::RandomBlurb(motive *m) {
	std::deque<blurb*> valid;

	for (blurb &i : test_file->blurbs) {
		if (v_.Execute(m, reinterpret_cast<const uint8_t *>(i.condition.Print())) && i.Type() == m->action)
			valid.push_back(&i);
	}
	if (valid.size() > 0) {
		int picked = std::rand()/((RAND_MAX + 1u)/valid.size());
//		std::cout << "total_valid: " << valid.size() << ". Picked: #" << picked + 1 << "\n";
		std::cout << Parse(valid[picked]->message, *m) << '\n';
	}
}
void battle::CheatLevel(actor *a, const int &l) {
	a->Level(l);
}