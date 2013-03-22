# DustJsCpp

An experimental C++ class for rendering a dust.js template using v8.

```
git clone git://github.com/jeffharrell/DustJsCpp.git --recursive
```


## Build 
```
make dependencies && make
```

## Usage
```
#include <string>
#include <iostream>
#include <map>
#include <dustjs.h>


using namespace std;


int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s template \n", argv[0]);
		return 1;
	}

	map<string, string> model;

	model["name"] = "Jeff Harrell";
	model["app"] = argv[0];

	return DustJs::render(argv[1], model);
}
```
