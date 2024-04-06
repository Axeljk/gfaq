#pragma once


<<<<<<< HEAD
//#include` "models/party.h"
=======
//#include "models/party.h"
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
//#include "models/stat.h"
#include "models/xbfq.h"
#include "motive.h"
#include "vm.h"


struct actor;
struct party;
<<<<<<< HEAD
struct spell;
=======
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
struct stat_aux;

struct battle {
	static const uint32_t kXPAttack			= 0x00002000;
	static const uint32_t kXPHit			= 0x00020000;
	static const uint32_t kXPMultStr		= 0x00000008;
	static const uint32_t kXPMultWis		= 0x00000001;
	static const uint32_t kXPMultAgi		= 0x00000010;

	xbfq *test_file;
	party *player, *enemies;
	motive *motives;
	int motives_count;
	int base_speed;

	int Attack(motive &m);
	int Battle();
	void BattleStatus();
	void ActionReport();
	void PlayerInput(actor *p, motive &m);
	actor* GetTarget(actor *p, motive &m, int &i);
<<<<<<< HEAD
	spell* GetSpell(actor *p, motive &m, int &i);
=======
>>>>>>> fe6d835ca773d27af637d99b81826ec41bb45cdf
	double StatPercent(stat_base *a, stat_base *b);
	void LevelStat(const actor &act, const uint32_t x, stat_aux *a, stat_aux *b);
	void EnCost(actor *a, const uint32_t &cost);
	double XPMultiplier(const int &x);
	void RandomBlurb(motive *m);
	void CheatLevel(actor *a, const int &l);

	battle(party *p, party *e, xbfq *f)
	: test_file(f)
	, player(p)
	, enemies(e)
	, motives_count(0) {
		Battle();
	}

	~battle() {
		delete[] motives;
	}

	private:
		vm v_;
};