#include "wordcreator.h" //dependancies are  <fstream> <vector> <unordered_map> <string> <cstring>

// switched from fmt to iostream before uploading to github
#include <iostream>
void logger(std::string _to_log) {
	std::cout << _to_log << std::endl;
}

int main() {
	//initialize with the sound directory and callback function to call on errors
	WordCreator sounds("./sound_dir/", logger);
	
	// fmt::print(sounds.make("{blends}{vowels}"));
	std::cout << sounds.make("Ta{vowels}{blends||vowels}") << std::endl; //prints something like Tawao or Tauysh
	std::cout << sounds.make("A{ |20|vowels}{ |70|vowels}{blends}") << std::endl; //prints something like Ashr

	auto vowels = sounds["vowels"];
	for (const auto& v : vowels) {
		std::cout << " |" << v << "|";
	} // prints |e| |ee| |ea| |ie| |ei| etc, 
	return 0;
}