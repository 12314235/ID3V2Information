#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>
#include "Parser.h"
#include "Frames.h"

int main(int argc, char** argv)
{

    Parser parse;

    std::vector<std::string> files;

    for (int i = 1; i < argc; i++) {
        if (!std::strcmp(argv[i], "-i") || !std::strcmp(argv[i], "--input")) {
            files.push_back(argv[i + 1]);
        }
    }

    std::vector<ID3Information*> storage;

    for (int i = 0; i < files.size(); i++) {
        storage.push_back(parse.parse(files[i]));
    }

    for (int i = 0; i < files.size(); i++) {
        std::cout << files[i] << '\n';
        storage[i]->ShowFrames();
    }

    for (ID3Information* i : storage) {
        delete i;
    }

    ID3Information* store = parse.parse("C:\\Users\\vitsl\\OneDrive\\Рабочий стол\\lab 7\\tag.mp3");

    store->ShowFrames();
}
