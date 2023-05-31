#ifndef _REJEX_HPP_
# define _REJEX_HPP_

# include <string>
# include <vector>

class Rejex
{
	public:
		Rejex();
		Rejex(std::string &matched);
		Rejex(const Rejex &other);
		Rejex(const Rejex &other, std::string &matched);
		virtual ~Rejex();
		Rejex &operator=(const Rejex &other);

		Rejex &all(const std::string &pattern, size_t min, size_t max);
		Rejex &all(const std::string &pattern, size_t min);
		Rejex &all(const std::string &pattern);

		Rejex &any(const std::string &pattern, size_t min, size_t max);
		Rejex &any(const std::string &pattern, size_t min);
		Rejex &any(const std::string &pattern);

		Rejex &any(char start, char end, size_t min, size_t max);
		Rejex &any(char start, char end, size_t min);
		Rejex &any(char start, char end);

		Rejex &inv(const std::string &pattern, size_t min, size_t max);
		Rejex &inv(const std::string &pattern, size_t min);
		Rejex &inv(const std::string &pattern);

		Rejex &inv(char start, char end, size_t min, size_t max);
		Rejex &inv(char start, char end, size_t min);
		Rejex &inv(char start, char end);

		Rejex &exp(const Rejex &expression, size_t min, size_t max);
		Rejex &exp(const Rejex &expression, size_t min);
		Rejex &exp(const Rejex &expression);

		Rejex &exp(const Rejex &expression, std::string &matched, size_t min, size_t max);
		Rejex &exp(const Rejex &expression, std::string &matched, size_t min);
		Rejex &exp(const Rejex &expression, std::string &matched);

		Rejex &jmp();
		Rejex &jmp(int num);
		Rejex &con();
		Rejex &tag(int num);
		Rejex &stp();

		Rejex &digit(size_t min, size_t max);
		Rejex &digit(size_t min);
		Rejex &digit();
		Rejex &xdigit(size_t min, size_t max);
		Rejex &xdigit(size_t min);
		Rejex &xdigit();
		Rejex &upper(size_t min, size_t max);
		Rejex &upper(size_t min);
		Rejex &upper();
		Rejex &lower(size_t min, size_t max);
		Rejex &lower(size_t min);
		Rejex &lower();
		Rejex &alpha(size_t min, size_t max);
		Rejex &alpha(size_t min);
		Rejex &alpha();
		Rejex &alnum(size_t min, size_t max);
		Rejex &alnum(size_t min);
		Rejex &alnum();
		Rejex &space(size_t min, size_t max);
		Rejex &space(size_t min);
		Rejex &space();

		bool match(const std::string &str);
		const std::string &result() const;
		const std::string &remainder() const;
		bool tagged(int num) const;

	private:
		enum Command
		{
			REJEX_ALL = 1,
			REJEX_ANY,
			REJEX_ANYR,
			REJEX_INV,
			REJEX_INVR,
			REJEX_EXP,
			REJEX_JMP,
			REJEX_JMPT,
			REJEX_CON,
			REJEX_TAG,
			REJEX_END
		};

		struct Element
		{
			Element()
			{
				expression = NULL;
				start = '\0';
				end = '\0';
				min = 0;
				max = 0;
				type = 0;
				tag = 0;
			}

			Element(const Element &other)
			{
				expression = NULL;
				*this = other;
			}

			~Element()
			{
				if (expression != NULL)
					delete expression;
			}

			Element &operator=(const Element &other)
			{
				if (this != &other)
				{
					if (expression != NULL)
						delete expression;
					expression = NULL;
					if (other.expression != NULL)
						expression = new Rejex(*(other.expression));
					pattern = other.pattern;
					start = other.start;
					end = other.end;
					min = other.min;
					max = other.max;
					type = other.type;
					tag = other.tag;
				}
				return (*this);
			}

			Rejex *expression;
			std::string pattern;
			char start;
			char end;
			size_t min;
			size_t max;
			int type;
			int tag;
		};

		typedef std::vector<struct Element> container_type;
		typedef container_type::iterator iterator_type;

		void clear();
		int next_type(iterator_type it);
		bool match(const std::string &str, size_t pos);
		bool is_char_in_range(const std::string &str, size_t pos, char start, char end) const;
		bool is_char_in_string(const std::string &str, size_t pos, const std::string &set) const;
		bool is_string_in_string(const std::string &str, size_t pos, const std::string &sub) const;

		container_type _element_list;
		std::vector<int> _tag_list;
		std::string _matched_string;
		std::string _remainder_string;
		std::string *_extra_matched_string;
};

#endif
