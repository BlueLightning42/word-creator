Just a repo to mash together words/ sounds into names or sentences.

to setup simply create a WordCreator with a link to the directory with all your csv files-
`WordCreator sounds("./sound_dir/", logger);`

where logger is an optional function pointer you pass to hook into the logging function pointer of the form `void (*)(std::string);` (check main for an example)

then create a list sentense of the form `sounds.make("xxxxx {csvname1} xxx");` to fill with a random value from csvname1.csv
optional capability is to include multiple options for the random csv value. `sounds.make("{sound1|80|sound2}");` will chose from sound1.csv 20% of the time and sound2.csv 80% of the time.


finally the code is a mess but functional...I made this because I was curious if mashing constenant blends and vowels made things that looked like real words.
I may use this in a larger project or a modified version of it but for now its just a small program to satisfy my curiosity.