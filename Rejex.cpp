#include "Rejex.hpp"
#include <limits>

#define REJEX_SIZE_MAX std::numeric_limits<size_t>::max()

Rejex::Rejex()
{
	_extra_matched_string = NULL;
}

Rejex::Rejex(std::string &matched)
{
	_extra_matched_string = &matched;
}

Rejex::Rejex(const Rejex &other)
{
	*this = other;
}

Rejex::Rejex(const Rejex &other, std::string &matched)
{
	*this = other;
	_extra_matched_string = &matched;
}

Rejex::~Rejex()
{
	clear();
}

Rejex &Rejex::operator=(const Rejex &other)
{
	size_t i;

	if (this != &other)
	{
		clear();
		_element_list.reserve(other._element_list.size());
		for (i = 0; i < other._element_list.size(); i++)
			_element_list.push_back(other._element_list[i]);
		_matched_string = other._matched_string;
		_remainder_string = other._remainder_string;
		_extra_matched_string = other._extra_matched_string;
	}
	return (*this);
}

/*
 * match the string pattern min to max times
 */
Rejex &Rejex::all(const std::string &pattern, size_t min, size_t max)
{
	struct Element e;

	e.pattern = pattern;
	e.min = min;
	e.max = max;
	e.type = REJEX_ALL;
	_element_list.push_back(e);

	return (*this);
}

/*
 * match the string pattern min or more times
 */
Rejex &Rejex::all(const std::string &pattern, size_t min)
{
	return (all(pattern, min, REJEX_SIZE_MAX));
}

/*
 * match the string pattern 1 time
 */
Rejex &Rejex::all(const std::string &pattern)
{
	return (all(pattern, 1, 1));
}

/*
 * match any char in the string pattern min to max times
 */
Rejex &Rejex::any(const std::string &pattern, size_t min, size_t max)
{
	struct Element e;

	e.pattern = pattern;
	e.min = min;
	e.max = max;
	e.type = REJEX_ANY;
	_element_list.push_back(e);

	return (*this);
}

/*
 * match any char in the string pattern min or more times
 */
Rejex &Rejex::any(const std::string &pattern, size_t min)
{
	return (any(pattern, min, REJEX_SIZE_MAX));
}

/*
 * match any char in the string pattern 1 time
 */
Rejex &Rejex::any(const std::string &pattern)
{
	return (any(pattern, 1, 1));
}

/*
 * match any char in the range min to max times
 */
Rejex &Rejex::any(char start, char end, size_t min, size_t max)
{
	struct Element e;

	e.start = start;
	e.end = end;
	e.min = min;
	e.max = max;
	e.type = REJEX_ANYR;
	_element_list.push_back(e);

	return (*this);
}

/*
 * match any char in the range min or more times
 */
Rejex &Rejex::any(char start, char end, size_t min)
{
	return (any(start, end, min, REJEX_SIZE_MAX));
}

/*
 * match any char in the range 1 time
 */
Rejex &Rejex::any(char start, char end)
{
	return (any(start, end, 1, 1));
}

/*
 * match any char not in the string pattern min to max times
 */
Rejex &Rejex::inv(const std::string &pattern, size_t min, size_t max)
{
	struct Element e;

	e.pattern = pattern;
	e.min = min;
	e.max = max;
	e.type = REJEX_INV;
	_element_list.push_back(e);

	return (*this);
}

/*
 * match any char not in the string pattern min or more times
 */
Rejex &Rejex::inv(const std::string &pattern, size_t min)
{
	return (inv(pattern, min, REJEX_SIZE_MAX));
}

/*
 * match any char not in the string pattern 1 time
 */
Rejex &Rejex::inv(const std::string &pattern)
{
	return (inv(pattern, 1, 1));
}

/*
 * match any char not in the range min to max times
 */
Rejex &Rejex::inv(char start, char end, size_t min, size_t max)
{
	struct Element e;

	e.start = start;
	e.end = end;
	e.min = min;
	e.max = max;
	e.type = REJEX_INVR;
	_element_list.push_back(e);

	return (*this);
}

/*
 * match any char not in the range min or more times
 */
Rejex &Rejex::inv(char start, char end, size_t min)
{
	return (any(start, end, min, REJEX_SIZE_MAX));
}

/*
 * match any char not in the range 1 time
 */
Rejex &Rejex::inv(char start, char end)
{
	return (any(start, end, 1, 1));
}

/*
 * match the sub-expression min to max times
 */
Rejex &Rejex::exp(const Rejex &expression, size_t min, size_t max)
{
	struct Element e;

	e.expression = new Rejex(expression);
	e.expression->stp();
	e.min = min;
	e.max = max;
	e.type = REJEX_EXP;
	_element_list.push_back(e);

	return (*this);	
}

/*
 * match the sub-expression min or more times
 */
Rejex &Rejex::exp(const Rejex &expression, size_t min)
{
	return (exp(expression, min, REJEX_SIZE_MAX));
}

/*
 * match the sub-expression 1 time
 */
Rejex &Rejex::exp(const Rejex &expression)
{
	return (exp(expression, 1, 1));
}

/*
 * match the sub-expression min to max times
 * change the matched string destination
 */
Rejex &Rejex::exp(const Rejex &expression, std::string &matched, size_t min, size_t max)
{
	struct Element e;

	e.expression = new Rejex(expression, matched);
	e.expression->stp();
	e.min = min;
	e.max = max;
	e.type = REJEX_EXP;
	_element_list.push_back(e);

	return (*this);	
}

/*
 * match the sub-expression min or more times
 * change the matched string destination
 */
Rejex &Rejex::exp(const Rejex &expression, std::string &matched, size_t min)
{
	return (exp(expression, matched, min, REJEX_SIZE_MAX));
}

/*
 * match the sub-expression 1 time
 * change matched string destination
 */
Rejex &Rejex::exp(const Rejex &expression, std::string &matched)
{
	return (exp(expression, matched, 1, 1));
}

/*
 * on success of the previous match jump to the next con or stp
 */
Rejex &Rejex::jmp()
{
	struct Element e;

	e.type = REJEX_JMP;
	_element_list.push_back(e);

	return (*this);	
}

/*
 * on success of the previous match jump to the next con or stp
 * also sets tag on success of the previous match
 */
Rejex &Rejex::jmp(int num)
{
	struct Element e;

	e.type = REJEX_JMPT;
	e.tag = num;
	_element_list.push_back(e);

	return (*this);	
}

/*
 * continue processing from the previous jmp
 */
Rejex &Rejex::con()
{
	struct Element e;

	e.type = REJEX_CON;
	_element_list.push_back(e);

	return (*this);	
}

/*
 * add a tag to the Rejex
 */
Rejex &Rejex::tag(int num)
{
	struct Element e;

	e.type = REJEX_TAG;
	e.tag = num;
	_element_list.push_back(e);

	return (*this);
}

/*
 * stop matching the Rejex
 */
Rejex &Rejex::stp()
{
	struct Element e;

	if (!_element_list.empty() && _element_list.back().type == REJEX_END)
		return (*this);
	e.type = REJEX_END;
	_element_list.push_back(e);

	return (*this);
}

Rejex &Rejex::digit(size_t min, size_t max)
{
	return (any('0', '9', min, max));
}

Rejex &Rejex::digit(size_t min)
{
	return (digit(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::digit()
{
	return (digit(1, 1));
}

Rejex &Rejex::xdigit(size_t min, size_t max)
{
	return (exp(Rejex().digit().jmp().any('a', 'f').jmp().any('A', 'F'), min, max));
}

Rejex &Rejex::xdigit(size_t min)
{
	return (xdigit(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::xdigit()
{
	return (xdigit(1, 1));
}

Rejex &Rejex::upper(size_t min, size_t max)
{
	return (any('A', 'Z', min, max));
}

Rejex &Rejex::upper(size_t min)
{
	return (upper(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::upper()
{
	return (upper(1, 1));
}

Rejex &Rejex::lower(size_t min, size_t max)
{
	return (any('a', 'z', min, max));
}

Rejex &Rejex::lower(size_t min)
{
	return (lower(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::lower()
{
	return (lower(1, 1));
}

Rejex &Rejex::alpha(size_t min, size_t max)
{
	return (exp(Rejex().lower().jmp().upper(), min, max));
}

Rejex &Rejex::alpha(size_t min)
{
	return (alpha(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::alpha()
{
	return (alpha(1, 1));
}

Rejex &Rejex::alnum(size_t min, size_t max)
{
	return (exp(Rejex().digit().jmp().lower().jmp().upper(), min, max));
}

Rejex &Rejex::alnum(size_t min)
{
	return (alnum(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::alnum()
{
	return (alnum(1, 1));
}

Rejex &Rejex::space(size_t min, size_t max)
{
	return (any(" \n\t\v\f\r", min, max));
}

Rejex &Rejex::space(size_t min)
{
	return (space(min, REJEX_SIZE_MAX));
}

Rejex &Rejex::space()
{
	return (space(1, 1));
}

/*
 * try to match the Rejex to string
 * returns true if the Rejex matches the string
 * the matched string and remainder are only stored on success
 */
bool Rejex::match(const std::string &str)
{
	return (match(str, 0));
}

bool Rejex::match(const std::string &str, size_t pos)
{
	iterator_type it;
	size_t previous;
	size_t current;
	size_t count;
	bool jump;
	bool skip;
	int next;

	// set all tags and matched strings to empty
	_tag_list.clear();
	_matched_string.clear();
	_remainder_string.clear();
	if (_extra_matched_string != NULL)
		_extra_matched_string->clear();
	jump = false;
	skip = false;
	previous = pos;
	current = pos;
	for (it = _element_list.begin(); it != _element_list.end(); it++)
	{
		count = 0;
		if (it->type == REJEX_ALL && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (is_string_in_string(str, current, it->pattern))
					current += it->pattern.size();
				else
					break;
		}
		else if (it->type == REJEX_ANY && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (is_char_in_string(str, current, it->pattern))
					current++;
				else
					break;
		}
		else if (it->type == REJEX_ANYR && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (is_char_in_range(str, current, it->start, it->end))
					current++;
				else
					break;
		}
		else if (it->type == REJEX_INV && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (!is_char_in_string(str, current, it->pattern))
					current++;
				else
					break;
		}
		else if (it->type == REJEX_INVR && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (!is_char_in_range(str, current, it->start, it->end))
					current++;
				else
					break;
		}
		else if (it->type == REJEX_EXP && !jump)
		{
			for (count = 0; count < it->max; count++)
				if (it->expression->match(str, current))
					current += it->expression->result().size();
				else
					break;
		}
		else if (it->type == REJEX_TAG && !jump)
		{
			_tag_list.push_back(it->tag);
		}
		else if (it->type == REJEX_JMP && !jump && !skip)
		{
			jump = true;
		}
		else if (it->type == REJEX_JMPT && !jump && !skip)
		{
			_tag_list.push_back(it->tag);
			jump = true;
		}
		else if (it->type == REJEX_CON)
		{
			jump = false;
		}
		else if (it->type == REJEX_END)
		{
			_remainder_string = str.substr(current);
			_matched_string = str.substr(pos, current - pos);
			if (_extra_matched_string != NULL)
				*_extra_matched_string = _matched_string;
			return (true);
		}
		skip = false;
		if (count < it->min && !jump)
		{
			// roll back failed match
			current = previous;
			// if next element is not a jump then fail
			next = next_type(it);
			if (next != REJEX_JMP && next != REJEX_JMPT)
				return (false);
			// else skip the next jump
			skip = true;
		}
		// commit match
		previous = current;
	}
	if (current != str.size())
		return (false);
	_remainder_string.clear();
	_matched_string = str.substr(pos);
	if (_extra_matched_string != NULL)
		*_extra_matched_string = _matched_string;
	return (true);
}

const std::string &Rejex::result() const
{
	return (_matched_string);
}

const std::string &Rejex::remainder() const
{
	return (_remainder_string);
}

/*
 * returns true if the tag num is in the Rejex
 */
bool Rejex::tagged(int num) const
{
	size_t i;

	for (i = 0; i < _tag_list.size(); i++)
		if (_tag_list[i] == num)
			return (true);
	return (false);
}

/*
 * deletes all data in the Rejex
 */
void Rejex::clear()
{
	_element_list.clear();
	_tag_list.clear();
	_matched_string.clear();
	_remainder_string.clear();
	_extra_matched_string = NULL;
}

/*
 * returns the type of pattern after the iterator
 */
int Rejex::next_type(iterator_type it)
{
	if (it != _element_list.end() && (it + 1) != _element_list.end())
		return ((it + 1)->type);
	return (REJEX_END);
}

/*
 * returns true if the char at i is between c1 and c2 inclusive
 */
bool Rejex::is_char_in_range(const std::string &str, size_t pos, char start, char end) const
{
	if (pos < str.size() && str[pos] >= start && str[pos] <= end)
		return (true);
	return (false);
}

/*
 * returns true if the char at i is in string p
 */
bool Rejex::is_char_in_string(const std::string &str, size_t pos, const std::string &set) const
{
	if (pos < str.size() && set.find(str[pos]) != std::string::npos)
		return (true);
	return (false);
}

/*
 * returns true if the string at pos is string sub
 */
bool Rejex::is_string_in_string(const std::string &str, size_t pos, const std::string &sub) const
{
	size_t i;

	if (str.size() < pos + sub.size())
		return (false);
	for (i = 0; i < sub.size(); i++)
		if (str[pos + i] != sub[i])
			return (false);
	return (true);
}
