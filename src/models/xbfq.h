#pragma once


#include <deque>
#include <fstream>
#include <string>

#include "pstr.h"


/*******************************************************************************
*                                                                              *
*                                    Record                                    *
*                                                                              *
*******************************************************************************/
template <class t> struct record {
	const char *kMagicNumber;
	const uint16_t kMaxCount;

	t& get(const int &i) { return records_[i]; }
	const uint32_t Size() const { return size_; }
	const uint16_t Count() const { return count_; }
	typename std::deque<t>::iterator begin() { return records_.begin(); }
	typename std::deque<t>::iterator end() { return records_.end(); }
	t* Add(const t &x) {
		if (count_ < kMaxCount) {
			size_ += x.Size();
			count_++;
			records_.push_back(x);
			records_.back().id_ = largest_id_++;
			return &records_.back();
		} else
			return NULL;
	}
	bool Remove(const int &i) {
		if (count_ > 0 && i < count_) {
			size_ -= records_[i].Size();
			count_--;
			records_[i].erase(records_.begin() + i);
			return true;
		} else
			return false;
	}
	void RecalculateSize() {
		size_ = kSizeOverhead_;
		count_ = static_cast<uint16_t>(records_.size());

		for (size_t &i : records_)
			size_ += records_[i].Size();
	}
	t* Find(const uint32_t &id) {
		for (t& i : records_) {
			if (i.ID() == id)
				return &i;
		}

		return NULL;
	}

	t& operator[](size_t i) { return records_[i]; }
	friend std::ostream& operator<<(std::ostream &out, const record<t> &r) {
		if (r.count_ > 0) {
			out.write(r.kMagicNumber, 4);
			out.write(reinterpret_cast<const char *>(&r.size_), kSizeSize);
			out.write(reinterpret_cast<const char *>(&r.count_), kCountSize);

			for (const t &i : r.records_)
				out << i;
		}

		return out;
	}
	friend std::istream& operator>>(std::istream &in, record<t> &r) {
		in.read(reinterpret_cast<char *>(&r.size_), kSizeSize);
		in.read(reinterpret_cast<char *>(&r.count_), kCountSize);

		if (r.count_ > 0) {
			r.records_.resize(r.count_);

			for (t &i : r.records_)
				in >> i;
		}

		// Find largest ID for adding new records later.
		r.largest_id_ = r.records_[0].ID();
		for ( t &i : r.records_) {
			if (r.largest_id_ < i.ID())
				r.largest_id_ = i.ID() + 1;
		}

		return in;
	}

	record(const char *magic, const uint16_t &max)
		: kMagicNumber(magic)
		, kMaxCount(max)
		, size_(kSizeOverhead_)
		, count_(0)
		, largest_id_(0) { }

	private:
		const static uint32_t kSizeOverhead_ = 10;
		const static int kSizeSize = 4;
		const static int kCountSize = 2;
		uint32_t size_;
		uint16_t count_;
		uint32_t largest_id_;
		std::deque<t> records_;
};


struct actor;
struct blurb;
struct career;
struct effect;
struct effect_profile;
struct party;
struct race;
struct spell;
/*******************************************************************************
*                                                                              *
*                                     xBFQ                                     *
*                                                                              *
*******************************************************************************/
class xbfq {
	/* Magic numbers and other const data. */
	const static char kFileExtension[];
	const static char kMagicNumber[];
	const static char kFormatVersion[];
	const static char kMasterFile[];
	const static char kTagActor[];
	const static char kTagBlurb[];
	const static char kTagCareer[];
	const static char kTagEffect[];
	const static char kTagEffectProfile[];
	const static char kTagItem[];
	const static char kTagParty[];
	const static char kTagRace[];
	const static char kTagSpell[];

	std::string base_directory_;
	std::ifstream base_file_;
	std::ofstream temp_file_;
	std::string base_filename_;
	uint16_t header_length_;
	pstr<uint8_t> title_, author_, description_;
	char date_[3];

	public:
		/* Public data. */
		record<race> races;
		record<career> careers;
		record<effect> effects;
//		record<effect_profile> effect_profiles;
		record<spell> spells;
		record<actor> actors;
		record<party> parties;
		record<blurb> blurbs;

		/* File read-and-write functions. */
		void Close(); // Called when no more members of file are being used.
		std::string CoreFile();
		void DefineCurrentDirectory();
		int Open(const std::string &file, const bool &silent=true);
		void ReadAll(const bool &silent=true);
		template <typename t>bool ReadTag(const char *tag, record<t> &r,
										  const bool &silent=true) {
			if (strncmp(tag, r.kMagicNumber, 4) == 0) {
				if (silent == false) {
					std::cout << "READ ";
					std::cout.write(r.kMagicNumber, 4);
					std::cout << " DATA" << std::endl;
				}
				base_file_ >> r;
				return true;
			} else
				return false;
		}
		void RenameFile(const std::string &name, const bool &overwrite=false);
		void SetHeaderLength();
		void SetTime();
		void Write(const bool &silent=true);

		/* Data functions. */
		void RestructureIDs();

		xbfq()
			: base_filename_("DEFAULT")
			, header_length_(57)
			, title_("GFAQ BFQ Datafile", 18)
			, author_("Axel", 4)
			, description_("Standard data library.", 22)
			, date_{1, 1, 70}
			, races(kTagRace, 512)
			, careers(kTagCareer, 512)
			, effects(kTagEffect, 512)
			, spells(kTagSpell, 512)
			, actors(kTagActor, 512)
			, parties(kTagParty, 512)
			, blurbs(kTagBlurb, 512) {
				DefineCurrentDirectory();
			}
		xbfq(const std::string &name)
			: base_filename_(name)
			, races(kTagRace, 512)
			, careers(kTagCareer, 512)
			, effects(kTagEffect, 512)
			, spells(kTagSpell, 512)
			, actors(kTagActor, 512)
			, parties(kTagParty, 512)
			, blurbs(kTagBlurb, 512) {
				DefineCurrentDirectory();
				Open(name);
				ReadAll();
			}
		~xbfq() { }
};