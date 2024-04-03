#include "chargen.h"

#include <cmath>
#include <iomanip>
#include <limits>
#include <string>

#include "models/actor.h"
#include "models/aspect.h"
#include "models/blurb.h"
#include "models/career.h"
#include "models/effect.h"
#include "models/party.h"
#include "models/race.h"
#include "models/spell.h"
#include "models/stat.h"
#include "models/xbfq.h"
#include "console_util.h"


void CharGen::Aspects(actor *a, const unsigned char &flags) {
	size_t input;
	bool done = false;

	if (a->virtue != NULL) {
		a->elements[a->virtue->add] -= aspects::kElementModifier;
		a->elements[a->virtue->sub] += aspects::kElementModifier;
	}
	if (a->vice != NULL) {
		a->elements[a->vice->add] -= aspects::kElementModifier;
		a->elements[a->vice->sub] += aspects::kElementModifier;
	}
	while (done == false) {
		Header(a, flags);
		std::cout << "\n\tPick your poison:\n\t  "
				  << "0) Temperance\n\t  "
				  << "1) Charity\n\t  "
				  << "2) Diligence\n\t  "
				  << "3) Kindness\n\t  "
				  << "4) Chastity\n\t  "
				  << "5) Patience\n\t  "
				  << "6) Humility\n\t  ";
		std::cin >> input;

		if (input >= 0 && input < 7) {
			aspect *pick = &aspects::FindVirtue(input);

			Header(a, flags);
			std::cout << "\n\t" << pick->name << '\n'
					  << Paragraph(pstr<uint16_t>(pick->description), 8, 8, 2, 7) << "\n"
					  << "\n\tStrengthens: " << elements::kElementNames[pick->add]
					  << "\n\tWeakens:     " << elements::kElementNames[pick->sub]
					  << std::endl;
			std::cout << "\n\n\tIs this correct?\n\t  "
				  << "0) No.\n\t  "
				  << "1) Yes.\n\t  ";
			std::cin >> input;

			if (input == 1) {
				a->virtue = pick;
				a->elements[a->virtue->add] += aspects::kElementModifier;
				a->elements[a->virtue->sub] -= aspects::kElementModifier;
				done = true;
			}
		}
	}

	done = false;
	while (done == false) {
		Header(a, flags);
		std::cout << "\n\tPick your poison:\n\t  "
				  << "0) Sloth\n\t  "
				  << "1) Wrath\n\t  "
				  << "2) Greed\n\t  "
				  << "3) Gluttony\n\t  "
				  << "4) Pride\n\t  "
				  << "5) Envy\n\t  "
				  << "6) Lust\n\t  ";
		std::cin >> input;

		if (input >= 0 && input < 7) {
			aspect *pick = &aspects::FindVice(input);

			Header(a, flags);
			std::cout << "\n\t" << pick->name << '\n'
					  << Paragraph(pstr<uint16_t>(pick->description), 8, 8, 2, 7) << "\n"
					  << "\n\tStrengthens: " << elements::kElementNames[pick->add]
					  << "\n\tWeakens:     " << elements::kElementNames[pick->sub]
					  << std::endl;
			std::cout << "\n\n\tIs this correct?\n\t  "
				  << "0) No.\n\t  "
				  << "1) Yes.\n\t  ";
			std::cin >> input;

			if (input == 1) {
				a->vice = pick;
				a->elements[a->vice->add] += aspects::kElementModifier;
				a->elements[a->vice->sub] -= aspects::kElementModifier;
				done = true;
			}
		}
	}
}
void CharGen::Career(actor *a, xbfq *f, const unsigned char &flags) {
	int tokens = 6;
	bool done = false;

	if (a->c != NULL) {
		a->str.mod_max(-a->c->str);
		a->wis.mod_max(-a->c->wis);
		a->agi.mod_max(-a->c->agi);
		a->hp.mod_max(-a->c->hp);
		a->en.mod_max(-a->c->en);
		a->ad.mod_max(-a->c->ad);
		a->re.mod_max(-a->c->re);
		a->as.mod_max(-a->c->as);
		a->ac.mod_max(-a->c->ac);
	}

	while (done == false) {
		size_t input = 0;
		career *pick = NULL;

		Header(a, flags);
		std::cout << "\n\tWhat is your career?\n\t  ";
		for (career &i : f->careers) {
			if (i.Playable() == true)
				std::cout << i.ID() << ") " << i.name << "\n\t  ";
		}
		std::cout << "\n\t  " << f->careers.Count() << ") Custom\n\t  ";
		std::cin >> input;

		if (input >= 0 && input < f->careers.Count()) {
			Header(a, flags);
			pick = f->careers.Find(input);

			if (pick != NULL && pick->Playable() == true) {
				std::cout << "\n        " << Truncate(pick->name, 20) << '\n'
						  << Paragraph(pick->description, 8, 8, 2, 7) << '\n'
						  << std::setw(14) << a->str.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->str)
						  << std::setw(13) << a->hp.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->hp)
						  << std::setw(6) << a->ad.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->ad) << '\n'
						  << std::setw(14) << a->wis.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->wis)
						  << std::setw(13) << a->en.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->en)
						  << std::setw(6) << a->ac.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->ac) << '\n'
						  << std::setw(14) << a->agi.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->agi)
						  << std::setw(13) << a->as.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->as)
						  << std::setw(6) << a->re.kNameShort
						  << std::setw(3) << stat_base::Shift(pick->re) << '\n';
				std::cout << "\n\n\tIs this correct?\n\t  "
						  << "0) No.\n\t  "
						  << "1) Yes.\n\t  ";
				std::cin >> input;

				if (input == 1) {
					a->PickCareer(pick);
					done = true;
					return;
				}
			}
		} else if (input == f->careers.Count())
			done = true;
	}

	// Custom class creation.
	career *pick = f->careers.Add(career());
	done = false;
	// SWAg.
	while (done == false) {
		int input = 0;
		std::string token_chars(tokens, '*');
		std::string token_chars_str(stat_base::Shift(pick->str), '*');
		std::string token_chars_wis(stat_base::Shift(pick->wis), '*');
		std::string token_chars_agi(stat_base::Shift(pick->agi), '*');

		Header(a, flags);
		std::cout << std::left << "\n\tYou have " << tokens
				  << " SWAg tokens.\t\t" << token_chars << '\n';
		StatList(0, &a->str, pick->str, token_chars_str.c_str());
		StatList(1, &a->wis, pick->wis, token_chars_wis.c_str());
		StatList(2, &a->agi, pick->agi, token_chars_agi.c_str());

		if (tokens == 0)
			std::cout << "\n\n\t  3) Finished?\n\t  ";
		else
			std::cout << "\n\t  ";
		std::cin >> input;

		if (input == 0) {
			StatInput(input, pick->str, tokens);
		} else if (input == 1) {
			StatInput(input, pick->wis, tokens);
		} else if (input == 2) {
			StatInput(input, pick->agi, tokens);
		} else {
			if (tokens == 0)
				done = true;
		}
	}

	// Aux.
	done = false;
	tokens = 12;
	while (done == false) {
		int input = 0;
		std::string token_chars(tokens, '*');
		std::string token_chars_hp(stat_base::Shift(pick->hp), '*');
		std::string token_chars_en(stat_base::Shift(pick->en), '*');
		std::string token_chars_ad(stat_base::Shift(pick->ad), '*');
		std::string token_chars_re(stat_base::Shift(pick->re), '*');
		std::string token_chars_as(stat_base::Shift(pick->as), '*');
		std::string token_chars_ac(stat_base::Shift(pick->ac), '*');

		Header(a, flags);
		std::cout << std::left << "\n\tYou have " << tokens
				  << " aux tokens.\t\t" << token_chars
				  << "\n";
		StatList(0, &a->hp, pick->hp, token_chars_hp.c_str());
		StatList(1, &a->en, pick->en, token_chars_en.c_str());
		StatList(2, &a->ad, pick->ad, token_chars_ad.c_str());
		StatList(3, &a->re, pick->re, token_chars_re.c_str());
		StatList(4, &a->as, pick->as, token_chars_as.c_str());
		StatList(5, &a->ac, pick->ac, token_chars_ac.c_str());
		if (tokens == 0)
			std::cout << "\n\n\t  6) Finished?\n\t  ";
		else
			std::cout << "\n\t  ";
		std::cin >> input;

		if (input == 0) {
			StatInput(input, pick->hp, tokens);
		} else if (input == 1) {
			StatInput(input, pick->en, tokens);
		} else if (input == 2) {
			StatInput(input, pick->ad, tokens);
		} else if (input == 3) {
			StatInput(input, pick->re, tokens);
		} else if (input == 4) {
			StatInput(input, pick->as, tokens);
		} else if (input == 5) {
			StatInput(input, pick->ac, tokens);
		} else {
			if (tokens == 0)
				done = true;
		}
	}

	a->PickCareer(pick);
}
void CharGen::Header(actor *a, const unsigned char &flags) {

/*********************************EXAMPLE BELOW*********************************
    NAME    JOHN DOE               SEX  M  LV  1    WATER  100%  LIGHT  100%    
    RACE    NIMISUM        STR  5   HP 10  AD 10    WIND   100%  DARK   100%    
    CAREER  JANITOR        WIS  5   EN 10  PR 10    EARTH  100%  ARCANE 100%    
    TEMPERANCE  GLUTTONY   AGI  5   DE 10  RE 10    FIRE   100%                 
================================================================================
**********************************EXAMPLE ABOVE********************************/

	NewScreen();

	// First line.
	if (flags & flags::kName)
		std::cout << "    NAME    " << std::left << std::setw(21)
				  << Truncate(a->name, 21) << std::right << "  SEX  "
				  << (a->gender == gender::kMale ? 'M' : 'F');
	else
		std::cout << "                                         ";
	if (flags & flags::kLevel)
		std::cout << "  LV" << std::setw(3) << a->get_level();
	else
		std::cout << "       ";
	if (flags & flags::kAspects)
		std::cout << "    WATER  " << std::setw(3)
				  << a->elements[elements::kWater] << "  LIGHT  "
				  << std::setw(3) << a->elements[elements::kLight];
	std::cout << '\n';

	// Second line.
	if (flags & flags::kRace)
		std::cout << "    RACE    " << std::left << std::setw(13)
				  << Truncate(a->r->name, 13) << std::right;
	else
		std::cout << "                         ";
	if (flags & flags::kRace || flags & flags::kCareer)
		std::cout << "  STR" << std::setw(3) << a->str.get(true) << "   HP"
				  << std::setw(3) << a->hp.get(true) << "  AD" << std::setw(3)
				  << a->ad.get(true);
	else
		std::cout << "                       ";
	if (flags & flags::kAspects)
		std::cout << "    WIND   " << std::setw(3)
				  << a->elements[elements::kWind] << "  DARK   "
				  << std::setw(3) << a->elements[elements::kDark];
	std::cout << '\n';

	// Third line.
	if (flags & flags::kCareer)
		std::cout << "    CAREER  " << std::left << std::setw(13)
				  << Truncate(a->c->name, 13) << std::right;
	else
		std::cout << "                         ";
	if (flags & flags::kRace || flags & flags::kCareer)
		std::cout << "  WIS" << std::setw(3) << a->wis.get(true) << "   EN"
				  << std::setw(3) << a->en.get(true) << "  PR" << std::setw(3)
				  << a->ac.get(true);
	else
		std::cout << "                       ";
	if (flags & flags::kAspects)
		std::cout << "    EARTH  " << std::setw(3)
				  << a->elements[elements::kEarth] << "  ARCANE "
				  << std::setw(3) << a->elements[elements::kArcane];
	std::cout << '\n';

	// Fourth line.
	if (flags & flags::kAspects)
		std::cout << std::setw(11) << a->virtue->name << '/'
				  << std::left << std::setw(15) << a->vice->name << std::right;
	else
		std::cout << "                           ";
	if (flags & flags::kRace || flags & flags::kCareer)
		std::cout << "AGI" << std::setw(3) << a->agi.get(true) << "   DE"
				  << std::setw(3) << a->as.get(true) << "  RE" << std::setw(3)
				  << a->re.get(true);
	else
		std::cout << "                     ";
	if (flags & flags::kAspects)
		std::cout << "    FIRE   " << std::setw(3)
				  << a->elements[elements::kFire];
	std::cout << '\n' << HorizontalLine();
}
void CharGen::Name(actor *a, const unsigned char &flags) {
	std::string data;

	Header(a, flags);
	std::cout << "\n\tWhat is your name?\n\t  ";
	std::cin.clear();
	if (std::cin.peek() == '\n')
		std::cin.ignore();
	getline(std::cin, data);
	a->name = data.c_str();
	std::cout << "\n\tWhat is your sex?\n\t  "
			  << "0) Male\n\t  "
			  << "1) Female\n\t  ";
	getline(std::cin, data);
	if (data[0] == 'f' || data[0] == 'F' || data[0] == '1')
		a->gender = gender::kFemale;
	else
		a->gender = gender::kMale;
}
actor* CharGen::NewActor(actor *a, xbfq *f) {
	unsigned char flags = 0x00;

	Name(a, flags);
	flags |= flags::kName;
	Race(a, f, flags);
	flags |= flags::kRace;
	Career(a, f, flags);
	flags |= flags::kCareer;
	Aspects(a, flags);
	flags |= flags::kAspects;
	Review(a, f);

	return a;
}
void CharGen::Race(actor *a, xbfq *f, const unsigned char &flags) {
	bool done = false;

	if (a->r != NULL) {
		a->str.mod_max(-a->r->str);
		a->wis.mod_max(-a->r->wis);
		a->agi.mod_max(-a->r->agi);
		a->hp.mod_max(-a->r->hp);
		a->en.mod_max(-a->r->en);
		a->ad.mod_max(-a->r->ad);
		a->re.mod_max(-a->r->re);
		a->as.mod_max(-a->r->as);
		a->ac.mod_max(-a->r->ac);
		a->elements[a->r->virtue->add] -= aspects::kElementModifier;
		a->elements[a->r->virtue->sub] += aspects::kElementModifier;
		a->elements[a->r->vice->add] -= aspects::kElementModifier;
		a->elements[a->r->vice->sub] += aspects::kElementModifier;
	}

	while (done == false) {
		int input = 0;
		race *pick = NULL;

		Header(a, flags);
		std::cout << "\n\tWhat race are you?\n\t  ";
		for (race &i : f->races) {
			if (i.Playable() == true)
				std::cout << i.ID() << ") " << i.name << "\n\t  ";
		}
		std::cin >> input;

		Header(a, flags);
		pick = f->races.Find(input);
		if (pick != NULL && pick->Playable() == true) {
			std::cout << "\n        " << Truncate(pick->name, 20) << '\n'
					  << Paragraph(pick->description, 8, 8, 2, 7) << '\n'
					  << std::setw(14) << a->str.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->str)
					  << std::setw(13) << a->hp.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->hp)
					  << std::setw(6) << a->ad.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->ad)
					  << "         Virtue: " << pick->virtue->name << '\n'
					  << std::setw(14) << a->wis.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->wis)
					  << std::setw(13) << a->en.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->en)
					  << std::setw(6) << a->ac.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->ac)
					  << "           Vice: " << pick->vice->name << '\n'
					  << std::setw(14) << a->agi.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->agi)
					  << std::setw(13) << a->as.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->as)
					  << std::setw(6) << a->re.kNameShort
					  << std::setw(3) << stat_base::Shift(pick->re) << '\n';
			std::cout << "\n\n\tIs this correct?\n\t  "
					  << "0) No.\n\t  "
					  << "1) Yes.\n\t  ";
			std::cin >> input;
			if (input == 1) {
				a->PickRace(pick);
				done = true;
			}
		}
	}
}
void CharGen::Review(actor *a, xbfq *f) {
	bool done = false;

	while (done == false) {
		int input;

		Header(a);
		std::cout << "\n\tIs everything correct?\n\t  "
				  << "0) Name and Sex\n\t  "
				  << "1) Race\n\t  "
				  << "2) Career\n\t  "
				  << "3) Aspects of virtue and vice\n\t  "
				  << "\n\t  4) Complete\n\t  ";
		std::cin >> input;

		if (input == 0)
			Name(a);
		else if (input == 1)
			Race(a, f);
		else if (input == 2)
			Career(a, f);
		else if (input == 3)
			Aspects(a);
		else if (input == 4)
			done = true;
	}
}
void CharGen::StatInput(int &i, uint32_t &v, int &t) {
	std::cout << "\n\t  Amount: ";
	std::cin >> i;
	if (i >= static_cast<int>(stat_base::Shift(0 - v)) && i <= t)
		t -= i;
	else if (i > t) {
		i = t;
		t = 0;
	} else
		i = 0;
	v += stat_base::Unshift(i);
}
void CharGen::StatList(const int &i, const stat_base *s, const uint32_t &v,
					   const char *tokens) {
	std::cout << "\n\t  " << i << ") " << std::setw(13) << s->kName
			  << stat_base::Shift(s->get(false) + v) << "\t\t" << tokens;
}