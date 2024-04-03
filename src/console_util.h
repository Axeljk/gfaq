#pragma once


#include <cstdint>
#include <sstream>

#include "models/actor.h"
#include "models/career.h"
#include "models/pstr.h"
#include "models/race.h"
#include "motive.h"


const char* HorizontalLine();
void NewScreen();
template <class t> std::string Parse(const pstr<t> &p, const motive &m) {
	size_t start_pos = 0;
	std::string tmp(p.Print(), p.Length());

	while ((start_pos = tmp.find("%")) != std::string::npos) {
		if (tmp[start_pos + 1] == 'n' && tmp[start_pos + 2] == '1')
			tmp.replace(start_pos, 3, m.perpetrator->name.Print(), m.perpetrator->name.Length());
		else if (tmp[start_pos + 1] == 'n' && tmp[start_pos + 2] == '2')
			tmp.replace(start_pos, 3, m.target->name.Print(), m.target->name.Length());
		else if (tmp[start_pos + 1] == 'c' && tmp[start_pos + 2] == '1')
			tmp.replace(start_pos, 3, m.perpetrator->c->name.Print(), m.perpetrator->c->name.Length());
		else if (tmp[start_pos + 1] == 'c' && tmp[start_pos + 2] == '2')
			tmp.replace(start_pos, 3, m.target->c->name.Print(), m.target->c->name.Length());
        else if (tmp[start_pos + 1] == 'r' && tmp[start_pos + 2] == '1')
			tmp.replace(start_pos, 3, m.perpetrator->r->name.Print(), m.perpetrator->r->name.Length());
		else if (tmp[start_pos + 1] == 'r' && tmp[start_pos + 2] == '2')
			tmp.replace(start_pos, 3, m.target->r->name.Print(), m.target->r->name.Length());
		else if (tmp[start_pos + 1] == 'd' && tmp[start_pos + 2] == '1') {
			std::stringstream ss;
			ss << (m.damage >> 24);
			tmp.replace(start_pos, 3, ss.str().c_str(), ss.str().length());
		} else if (tmp[start_pos + 1] == 'h' && tmp[start_pos + 2] == '1') {
			std::stringstream ss;
			ss << static_cast<int>(m.times_hit);
			tmp.replace(start_pos, 3, ss.str().c_str(), ss.str().length());
		}
    }

	return tmp;
}
void Pause();
template <class t> std::string Paragraph(const pstr<t> &p,
										 const int &l_margin=4,
										 const int &r_margin=4,
										 const int &indent=4,
										 const int &lines=0) {
	std::stringstream original, paragraph;
	size_t row_width=0, pos=0, last_space=0;
	int newlines=0;

	original.write(p.Print(), p.Length());
	while (row_width < original.str().length()) {
		// Determine width of this line.
		row_width = pos + (80 - l_margin - r_margin);

		// Indent if first line.
		if (paragraph.str().length() == 0) {
			row_width -= indent;
			paragraph << std::string(indent, ' ');
		}

		// Limit to actual length.
		if (row_width > original.str().length())
			row_width = original.str().length();

		// l_margin + text (+ r_margin).
		paragraph << std::string(l_margin, ' ');
		for (size_t i = pos; i < row_width; ++i) {
			if (original.str()[i] == ' ')
				last_space = i;
		}

		// Word wrap.
		if (newlines < lines || lines == 0) {
			if ( last_space > pos && row_width < original.str().length()) {
				paragraph << original.str().substr(pos, (last_space - pos)) << '\n';
				pos = last_space + 1;
				newlines++;
			} else {
				paragraph << original.str().substr(pos) << '\n';
				newlines++;
			}
		} else {
			return paragraph.str();
		}
	}

	for (; newlines < lines; ++newlines)
		paragraph << '\n';

	return paragraph.str();
}
template <class t> std::string Truncate(const pstr<t> &p,
										const size_t &size) {
	std::stringstream original;

	original.write(p.Print(), p.Length());
	if (original.str().length() > size) {
		if (size <= 3)
			return original.str().substr(0, size);
		return original.str().substr(0, size - 3) + "...";
	} else
		return original.str();
}