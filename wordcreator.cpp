#include "wordcreator.h"

using str_vec = std::vector<std::string>;

static bool is_in(char val, const char* group) {
	for (size_t i = 0; i < strlen(group); i++) {
		if (val == group[i]) return true;
	}
	return false;
}

str_vec WordCreator::split(const char* source, const char* delimeters, bool strip) {
	// don't "pop the hood" and read my shitty implementation of split
	int start = -1;
	int end = -1;

	const char* whitespace = " \t\r\n\v\f";

	str_vec split_strings;

	for (size_t i = 0; i < strlen(source); i++) {
		//skip multiple delimeters in a row
		if ((start == -1) && is_in(source[i], delimeters)) continue;
		// if strip then strip the first whitespace characters
		if ((start == -1) && strip && is_in(source[i], whitespace)) continue;

		if (start == -1) start = i;

		if (is_in(source[i], delimeters)) {

			if (i > 0) end = i - 1;
			if (strip) {
				// move end down unstil its no longer a whitespace character (and stop if end is zero)
				while (is_in(source[end], whitespace) && end) end--;
			}
			if (end >= start) {
				split_strings.emplace_back(source, start, (end - start + 1));
			}
			start = -1;
		}
	}
	return split_strings;
}

bool WordCreator::add(const char* type) {
	char file_path[50];
	file_path[0] = '\0'; //this and the _s fixes visual studio being annoying with safety warnings
	strcat_s(file_path, _dir_path);
	strcat_s(file_path, type);
	strcat_s(file_path, ".csv");

	std::ifstream in(file_path);
	if (in.is_open()) {
		in.seekg(0, std::ios::end);
		auto file_len = in.tellg();
		in.seekg(0, std::ios::beg);
		const char* buffer = new char[(size_t)file_len + 1];
		in.read((char*)buffer, file_len);
		in.close();

		_store[type] = split(buffer, ",\n", true);

		delete[] buffer;

		return true;
	}
	else {
		return false;
	}
}

const str_vec& WordCreator::operator[](const char* key) { return loadKey(key); }
const str_vec& WordCreator::loadKey(const char* key) {

	if (std::string(key).find_first_not_of(" \t\n\v\f\r") == std::string::npos) {
		const static str_vec _null(1, { "" });
		return _null;
	}
	auto value = _store.find(key);
	if (value == _store.end()) {
		if (this->add(key)) {
			return _store[key];
		}else {
			const static str_vec _err(1, { "ERROR" });
			// log( fmt::format("Error: problem occured while reading file for {}", key));
			std::string l("WordCreator Error: problem occured while trying to read a file for ");
			l += key;
			log(l);
			return _err;
		}
	}
	//else
	return _store[key];
}

// compile time stuff would be real neat but this library isn't performance dependant

std::string WordCreator::make(const char* expression) {
	std::string output;
	bool closed = true;
	// using this like a queue
	str_vec temp; 
	std::vector<int> weights;
	for(size_t i=0; i<strlen(expression);  i++) {
		if (closed){
			if (expression[i] == '{') {
				closed = false;
				temp.clear();
				temp.emplace_back("");
				weights.clear();
				weights.emplace_back(-1);
			}
			else if (expression[i] == '}') {
				log("WordCreator Error: Unexpected '}' in expression. All closing brackets must be preceded by a opening bracket.");
				return "ERROR";
			}
			else {
				output += expression[i];
			}
		}else { //open
			if (expression[i] == '|') {
				temp.emplace_back("");
				auto start = ++i;
				while (expression[i] != '|' && expression[i] != '\0') {
					i++;
				}
				try {
					auto extracted = std::string(expression, start, i - start);
					int weight = std::stoi(std::string(expression, start, i - start));
					weights.push_back(weight);
				}catch(...){

					weights.push_back(-1);
				}
			}
			else if (expression[i] == '{') {
				log("WordCreator Error: Unexpected { in expression. No nested values in expression.");
				return "ERROR";
			}
			else if (expression[i] == '}') {
				closed = true;
				std::string value = choice_weighted(temp, weights);
				if (value != "") {
					output += choice(loadKey(value.c_str()));
				}
			}
			else {
				temp.back() += expression[i];
			}
		}
	}
	if(closed) return output;

	log("WordCreator Error: Missing a closing braket in expression.");
	return "ERROR";
}

std::string WordCreator::choice(str_vec vec) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	if (vec.size() < 1) return "";
	std::uniform_int_distribution<> dis(0, vec.size()-1);
	return vec[dis(gen)];
}
std::string WordCreator::choice_weighted(str_vec vec, std::vector<int> weights) {
	static std::random_device rd;
	static std::mt19937 gen(rd());
	if (vec.size() < 1) return "";
	if (vec.size() == 1) return vec.back();
	int percent = 100;
	int empty = 0;
	for (const auto& w : weights) {
		if (w == -1) {
			empty++;
		}else {
			percent -= w;
		}
	}
	percent /= empty;
	for (auto& w : weights) {
		if (w == -1) w = percent;
	}
	std::discrete_distribution<> dis(weights.begin(), weights.end());
	return vec[dis(gen)];
}
