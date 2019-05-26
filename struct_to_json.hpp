#ifndef STRUCT_TO_JSON_HPP
#define STRUCT_TO_JSON_HPP

#include<string>
#include<sstream>

namespace {

	inline bool is_ctrl_char(const char c)
	{
		return c < 32 || c == 127;
	}

	inline std::string trim_ctrl_char(const char* data, unsigned int size)
	{
		std::string ret(data, size);
		unsigned int index = 0;
		while (index < size)
		{
			int pos = size - 1 - index;
			if (is_ctrl_char(data[pos]) || (pos > 0 && ' ' == data[pos] && ' ' == data[pos - 1]))
			{
				ret.erase(pos, 1);
			}
			++index;
		}
		return ret;
	}

	template<int size>
	inline std::string to_string(const char(&char_array)[size])
	{
		return trim_ctrl_char(char_array, size);
	}

#define TO_JSON_Begin(struct_type) \
	inline std::stringstream& operator<<(std::stringstream& os, const struct_type& data) \
	{ \
		os << "{"; \
		bool is_first = true;

#ifdef __GNUC__ 
#if (__GNUC__ > 4 && __GNUC_MINOR__ > 8)
#define TO_JSON_Add(element) os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":" << std::to_string(data.element);
#else
#define TO_JSON_Add(element) os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":" << std::to_string((long long int)data.element); 
#endif
#else
#define TO_JSON_Add(element) os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":" << std::to_string(data.element); 
#endif

#define TO_JSON_Add_CharArray(element) os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":\"" << trim_ctrl_char(data.element, sizeof(data.element)) << "\"";

#define TO_JSON_Add_Struct(element) os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":"; os << data.element;

#define TO_JSON_Add_StructArray(element, size) \
		os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":["; \
		for (int index = 0; index < data.size; ++index) \
		{ \
			os << (0 == index ? "" : ",");\
			os << data.element[index]; \
		} \
		os << "]";

#define TO_JSON_Add_AsString(element) \
		os << (is_first ? is_first = false, "" : ",")<< "\"" <<#element << "\":\"" << to_string(data.element) << "\"";

#define TO_JSON_End(struct_type) \
		os << "}"; \
		return os; \
	} \
	inline std::string to_string(const struct_type& data) \
	{ \
		std::string error_info; \
		try \
		{ \
			std::stringstream ss; \
			ss << "\"" <<#struct_type << "\":"; \
			ss<< data; \
			return ss.str(); \
		} \
		catch (const std::exception& e) \
		{ \
			error_info = std::string("convert ") + #struct_type + std::string(" to str error, ") + e.what(); \
		} \
		catch (...) \
		{ \
			error_info = std::string("convert ") + std::string(#struct_type) +" to str error, unknow reason."; \
		} \
		return error_info; \
	}

}

namespace {

	template<typename series_type>
	std::string to_string(const series_type& series)
	{
		char buffer[128];
		sprintf(buffer, "%u_%u_%u_%u_%u_%u_%d",
			series.country_c,
			series.market_c,
			series.instrument_group_c,
			series.modifier_c,
			series.commodity_n,
			series.expiration_date_n,
			series.strike_price_i);
		return buffer;
	}

	template<>
	std::string to_string(const quad_word& data)
	{
		char order_number[20] = { 0 };
		sprintf(order_number, "%016llX", *(uint64*)&data);
		return std::string(order_number);
	}

	template<>
	std::string to_string(const transaction_type_t& data)
	{
		char transaction_t[20] = { 0 };
		sprintf(transaction_t, "%c%c%hu", 
			data.central_module_c, 
			data.server_type_c,
			data.transaction_number_n);
		return std::string(transaction_t);
	}

	template<>
	std::string to_string(const match_id_t& data)
	{
		char match_id[64] = { 0 };
		sprintf(match_id, "%llu_%u_%u",
			data.execution_event_nbr_u,
			data.match_group_nbr_u,
			data.match_item_nbr_u);
		return std::string(match_id);
	}
#endif // !STRUCT_TO_JSON_HPP

