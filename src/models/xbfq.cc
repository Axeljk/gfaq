#include "xbfq.h"

#include <cstring>
#include <ctime>
#include <iostream>
#ifdef _WIN32
	#include <direct.h>
	#define GetDirectory _getcwd
#else
	#include <unistd.h>
	#define GetDirectory getcwd
#endif

#include "actor.h"
#include "aspect.h"
#include "blurb.h"
#include "career.h"
#include "effect.h"
#include "party.h"
#include "race.h"
#include "spell.h"


/* Static members defined in xbfq.h. */
const char xbfq::kMagicNumber[4] = { 0x78, 0x42, 0x46, 0x51 };			 // xBFQ
const char xbfq::kFormatVersion[3] = { 0x00, 0x00, 0x01 };				 //  001
const char xbfq::kFileExtension[4] = { 0x2E, 0x62, 0x66, 0x71 };		 // .bfq
const char xbfq::kMasterFile[4] = { 0x63, 0x6F, 0x72, 0x65 };			 // core
const char xbfq::kTagActor[4] = { 0x41, 0x43, 0x54, 0x52 };				 // ACTR
const char xbfq::kTagBlurb[4] = { 0x42, 0x4C, 0x52, 0x42 };				 // BLRB
const char xbfq::kTagCareer[4] = { 0x43, 0x41, 0x52, 0x52 };			 // CARR
const char xbfq::kTagEffect[4] = { 0x45, 0x46, 0x43, 0x54 };			 // EFCT
const char xbfq::kTagEffectProfile[4] = { 0x50, 0x52, 0x46, 0x4C };		 // PRFL
const char xbfq::kTagItem[4] = { 0x49, 0x54, 0x45, 0x4D };				 // ITEM
const char xbfq::kTagParty[4] = { 0x50, 0x52, 0x54, 0x59 };				 // PRTY
const char xbfq::kTagRace[4] = { 0x52, 0x41, 0x43, 0x45 };				 // RACE
const char xbfq::kTagSpell[4] = { 0x53, 0x50, 0x45, 0x4C };				 // SPEL

/* File read-and-write functions. */
void xbfq::Close() {
	base_file_.close();
	temp_file_.close();
}
std::string xbfq::CoreFile() {
	return std::string().append(kMasterFile, 4).append(kFileExtension, 4);
}
void xbfq::DefineCurrentDirectory() {
	base_directory_ = GetDirectory(NULL, 0);
	base_directory_.append("/data/");
}
int xbfq::Open(const std::string &file, const bool &silent) {
	char file_magic_number[4];
	char file_version[3];
	base_filename_ = file;
	std::string file_location = base_directory_ + file; file_location.append(kFileExtension, 4);

	base_file_.open(file_location.c_str(), std::ios::in | std::ios::binary);

	//	Make sure it's a .fag or else why have that header data, ya?
	base_file_.read(file_magic_number, 4);
	if (strncmp(file_magic_number, kMagicNumber, 4) != 0) return 1;

	// Header data.
	base_file_.read(file_version, 3);
	if (strncmp(file_version, kFormatVersion, 3) != 0) return 2;
	base_file_.read(date_, 3);
	base_file_.read(reinterpret_cast<char *>(&header_length_), 2);
	base_file_ >> title_;
	base_file_ >> author_;
	base_file_ >> description_;

	if (silent == false) {
		std::cout << "\nReading file " << file << "...\n" << std::endl;
		std::cout << "\tDated:           "
				  << static_cast<int>(date_[1]) << "/"
				  << static_cast<int>(date_[2]) << "/"
				  << static_cast<int>(date_[0])
				  << "\n\tHeader length:   " << header_length_
				  << "\n\tTitle:           " << title_
				  << "\n\tAuthor:          " << author_
				  << "\n\tDescription:     " << description_ << "\n\n"
				  << std::endl;
	}

	return 0;
}
void xbfq::ReadAll(const bool &silent) {
	base_file_.seekg(header_length_);
	while (base_file_.peek() != std::char_traits<char>::eof()) {
		char tag[4];

		base_file_.read(tag, 4);
		if (ReadTag(tag, races, silent)) { }
		else if (ReadTag(tag, careers, silent)) { }
		else if (ReadTag(tag, effects, silent)) { }
		else if (ReadTag(tag, spells, silent)) {
			for (spell_base &i : spells) {
				for ( uint8_t j = 0; j < i.effect_count_; ++j) {
					i.effects_[j].e = effects.Find(i.effects_[j].effect_id);
				}
			}
		}
		else if (ReadTag(tag, actors, silent)) {
			for (actor &i : actors) {
				i.r = races.Find(i.race_id);
				i.c = careers.Find(i.career_id);
			}
		} else if (ReadTag(tag, parties, silent)) {
			for ( party &i : parties) {
				for (uint16_t j = 0; j < i.count; ++j) {
					if (i.persistent == true)
						i.members[j] = actors.Find(i.members_id[j]);
					else {
						i.members[j] = new actor;
						*(i.members[j]) = *(actors.Find(i.members_id[j]));
					}
				}
			}
		} else if (ReadTag(tag, blurbs, silent)) { }
		else {
			if (silent == false) {
				std::cout << "\n\n\t\tERROR: UNKNOWN TAG.\n\n" << std::endl;
				std::system("pause");
			}
			return;
		}
	}

	Close();
}
void xbfq::RenameFile(const std::string &name, const bool &overwrite) {
	if (overwrite) {
		if (std::ifstream(base_filename_.c_str()).good())
			rename(base_filename_.c_str(), name.c_str());
	}

	base_filename_ = name;
}
void xbfq::SetHeaderLength() {
	header_length_ = 12;
	header_length_ += title_.Size();
	header_length_ += author_.Size();
	header_length_ += description_.Size();
}
void xbfq::SetTime() {
	time_t time = std::time(0);
	tm *date = std::localtime(&time);

	date_[0] = static_cast<char>(date->tm_year%100);
	date_[1] = static_cast<char>(date->tm_mon+1);
	date_[2] = static_cast<char>(date->tm_mday);
}
void xbfq::Write(const bool &silent) {
	std::string file_location = base_directory_ + '~' + base_filename_;
	file_location.append(kFileExtension, 4);

	temp_file_.open(file_location.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);

	/* Header. */
	temp_file_.write(reinterpret_cast<const char *>(kMagicNumber), 4);
	temp_file_.write(reinterpret_cast<const char *>(kFormatVersion), 3);
	SetTime();
	temp_file_.write(reinterpret_cast<char *>(date_), 3);
	SetHeaderLength();
	temp_file_.write(reinterpret_cast<char *>(&header_length_), 2);
	temp_file_ << title_ << author_ << description_;
	if (silent == false) {
		std::cout << title_ << '\n' << author_ << '\n' << description_
				  << "\n\n\n";
	}

	/* Organize data. */
	RestructureIDs();

	/* Write data. */
	temp_file_ << races << careers << effects << spells << actors << parties << blurbs;

	Close();

	/* Replace original with written temp file. */
	std::string base_filename = base_directory_ + base_filename_;
	base_filename.append(kFileExtension, 4);
	std::string temp_filename = base_directory_ + '~' + base_filename_;
	temp_filename.append(kFileExtension, 4);
	remove(base_filename.c_str());
	rename(temp_filename.c_str(), base_filename.c_str());
}

/* Data functions. */
void xbfq::RestructureIDs() {
/*	for (int i = 0; i < races.size(); ++i) {
		races[i].id = i;
	}
	for (int i = 0; i < careers.size(); ++i) {
		careers[i].id = i;
	}
	for (int i = 0; i < actors.size(); ++i) {
		actors[i].id = i;
	}
	for (int i = 0; i < parties.size(); ++i) {
		parties[i].id = i;
	}
	for (int i = 0; i < blurbs.size(); ++i) {
		blurbs[i].id = i;
	}*/
}