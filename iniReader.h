#pragma once

#include <vector>
#include <map>
#include <fstream>
#include <stdexcept>
#include <functional>

typedef std::map<std::string, std::string> strMap_t;
typedef std::map<std::string, strMap_t>  iniMap_t;

// use of std::map and std::string instead of arrays and char*'s could increase loading times,
// however, it shouldn't have too much of a mid-runtime performance decrease and might be more
// beneficial since it'll be easier to manage so long as you're using C++ and not C
std::map< std::string, std::map<std::string, std::string>  >* MapIniFile(std::string szFileName)
{
	std::function<void(std::string, int,int)> reportErr = [szFileName](std::string szErr, int row, int col) {
		std::string error = szErr + " [" + std::to_string(row) + " : " + std::to_string(col) + "] in " + szFileName;
		throw std::runtime_error(error);
	};
	std::ifstream inputStream(szFileName, std::fstream::in);
	if(!inputStream.is_open()) throw std::runtime_error("Could not open .INI file for mapping!");
	std::string tmpStr;
	iniMap_t* iniMap = new iniMap_t{};
	long unsigned int iter = 0;
	long unsigned int currentLine = 0;
	long unsigned int currentCol = 0;
	std::string curCat = "";
	while(getline(inputStream, tmpStr))
	{
		++currentLine;
		// first we need to see what the line begins with:
		for(iter = 0; iter < tmpStr.length(); iter++)
		{
			// skip spaces, they don't matter
			if(tmpStr[iter] == ' ') continue;
			// skip line if it starts with comment
			else if(tmpStr[iter] == '#') break;
			// define a category if starts with '['
			else if(tmpStr[iter] == '[') {
				currentCol = iter;
				// define the new name
				std::string newCat = "";
				bool foundCat = false;
				while(++iter < tmpStr.length())
				{
					// detect mistaken comments
					if(tmpStr[iter] == '#') reportErr("Found comment when there shouldn't be one!", currentLine, iter);
					// detect end of category declaration
					if(tmpStr[iter] == ']') 
					{
						// ok, have the category name.
						foundCat = true;
						// make sure nothing unexpected after this declaration!
						while(++iter < tmpStr.length()) if(tmpStr[iter] != ' ' || tmpStr[iter] != '\r' || tmpStr[iter] != '\n') reportErr("Unexpected token!", currentLine, iter);
						break;
					}
					newCat += tmpStr[iter];
				}
				// report whether category was found or not
				if(foundCat == false) reportErr("Could not interpret category!", currentLine, currentCol);
				curCat = newCat; // set the category
				break;
			}
			// not a space, not a category, not a comment, must be a key + value
			else
			{
				// default values
				std::string key = "";
				std::string value = "";
				// loop through characters
				while(iter < tmpStr.length())
				{
					if(tmpStr[iter] != '=') key += tmpStr[iter++];
					else { 
						// hacky space removal
						long unsigned int _iter = iter - 1;
						while(key[_iter] == ' ') _iter--;
						int diff = (int)(iter - 1 - _iter);
						key = key.substr(0, key.length() - diff);
						++iter;  break;
				       	}
				}
				// same thing, except everything past '=' is a go!
				while(iter < tmpStr.length()) if(tmpStr[iter] == ' ') iter++; else break;
				while(iter < tmpStr.length())
				{
					value += tmpStr[iter++];
				}
				// now we add the values
				// if category exists (100%) and the key doesn't, add it!
				if(iniMap->count(curCat) >= 1 && iniMap->at(curCat).count(key) <= 0)
				{
					iniMap->at(curCat).insert(std::pair<std::string, std::string>(key, value));
				}
				// some reason this key at category exists, overwrite and ignore
				else if(iniMap->count(curCat) >= 1 && iniMap->at(curCat).count(key) >= 1)
				{
					iniMap->at(curCat).at(key) = value;
				}
				// the category doesn't exist, add that in
				else
				{
					// set the key
					iniMap->insert(std::pair<std::string, strMap_t>(curCat, strMap_t{std::pair<std::string, std::string>(key, value)}));
				}
			}
		}
	}
	return iniMap;
}
