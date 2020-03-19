# iniReader
A C++ header that loops through a .INI file and maps values.

This is just the first version, future updates will not be uploaded here

Here is an example.
```cpp
try
{
  iniMap_t mappedINIFile = *MapIniFile("config.ini");
}
catch (std::exception& e)
{
  std::cerr << e.what() << std::endl;
}
```

iniMap_t is just a `typedef` for `std::map<std::string, std::map<std::string, std::string>>`.

This can confuse me, so I'm assuming it's going to confuse you too. Better think of it as JSON.

```json
{
  "category1": {
    "key1": "value1",
    "key2": "value2"
  },
  "category2": {
    "key1": "value1",
    "key2": "value2"
  }
}
```

The categories are just what I'm calling the headers in INI files `[Name]` which is followed by the set of keys and values.

An easy test to visualize this and ensure everything is parsing correctly is by using this code:

```cpp
#include <iostream>
#include "iniReader.h"

std::string iniMap_to_string(iniMap_t &m) {
        std::string str = "{\n";

        for(auto it = m.cbegin(); it != m.cend(); ++it)
        {
                str += '\t' + it->first;
                str += ":\n";
                for(auto it2 = it->second.cbegin(); it2 != it->second.cend(); ++it2)
                {
                        str += "\t\t{ " + it2->first + ", " + it2->second + " }\n";
                }
        }

        str += "}";
        return str;
}

int main(int argc, char** argv)
{
  try {
	  iniMap_t configIni = *MapIniFile("config.ini");
	  std::cout << iniMap_to_string(configIni) << std::endl;
  } catch (std::exception& e) { std::cerr << e.what() << std::endl;

	return 0;
}
```

Here is an example of an output:

```
"defs.ini"

{
        Definitions:
                { HELLO, DUDE }
                { HOW, AWESOME? }
                { THESE_ARE_STRINGS, YEAH }
}
```

which looks like this

```
[Definitions]
HELLO=DUDE
HOW=AWESOME?
THESE_ARE_STRINGS=YEAH
```

You can easily convert that to an actual INI->JSON converter if you want to. I'm going to leave this project unarchived so I can accept pull-requests from anybody interested in fixing up some stuff.

Project published under GNU Public License v3 (see LICENSE)
