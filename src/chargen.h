#pragma once


#include <cstdint>


struct actor;
struct career;
struct xbfq;
struct race;
struct stat_base;

namespace CharGen {
	enum flags {
		kName =		0x01, // 0b00000001
		kRace =		0x02, // 0b00000010
		kCareer =	0x04, // 0b00000100
		kAspects =	0x08, // 0b00001000
		kLevel =	0x16  // 0b00010000
	};

	void Aspects(actor *a, const unsigned char &flags=0xff);
	void Career(actor *a, xbfq *f, const unsigned char &flags=0xff);
	void Header(actor *a, const unsigned char &flags=0xff);
	void Name(actor* a, const unsigned char &flags=0xff);
	actor* NewActor(actor *a, xbfq *f);
	void Race(actor* a, xbfq *f, const unsigned char &flags=0xff);
	void Review(actor *a, xbfq *f);
	void StatInput(int &i, uint32_t &v, int &t);
	void StatList(const int &i, const stat_base *s, const uint32_t &v,
				  const char *tokens);
}