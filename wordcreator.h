#pragma once

#include <fstream>
#include <vector> //always
#include <string>
#include <unordered_map>
#include <random>

// strtok etc
#include <cstring>

// started with std::string but ended up using so many c functions I just swapped everything for const char* 's
// I would be using std::string if it had a split() strip() etc...but no luck I have to implement that either way
// and I'm more comfortable with implementing it with c style strings...uglier but makes more sense to me.

static void no_log(std::string) { return; }

// storage class with a mini langauge to return a mash of the strings stored in it.
class WordCreator {
public:
	using str_vec = std::vector<std::string>;
	using callback = void (*)(std::string);

	WordCreator() : _dir_path("./"), log(no_log) {}
	WordCreator(const char* file_path) : _dir_path(file_path), log(no_log) {}
	WordCreator(const char* file_path, callback logger): _dir_path(file_path), log(logger) {}

	const str_vec& operator[](const char*);

	std::string make(const char* expression);
private:
	//strip source by delimters and return a vector of split strings
	str_vec split(const char* source, const char* delimeters, bool strip = false);
	bool add(const char* type);

	const str_vec& loadKey(const char*);

	std::string choice(str_vec);
	std::string choice_weighted(str_vec, std::vector<int>); 
	// {"A", "B", "C"} {-1, 20, 30} will chose A 50%, B, 20%, and C 30% of the time
	// {"A", "B", "C"} {-1, -1, 50} will chose A 25%, B, 25%, and C 50% of the time
	callback log;
	const char* _dir_path;
	std::unordered_map<std::string, str_vec> _store; // sound type as key and vector of sounds from file for each type.
};

