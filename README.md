# DustJsCpp

An experimental C++ class for rendering a dust.js template using V8.

```
git clone git://github.com/jeffharrell/DustJsCpp.git --recursive
```


## Build 
```console
$ make dependencies && make
```

## Usage

### renderer.cpp
```cpp
#include <string>
#include <map>
#include <dustjs.h>

using namespace std;

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s template \n", argv[0]);
		return 1;
	}
	// Build out a data model
	map<string, string> model;

	model["name"] = "Jeff Harrell";
	model["app"] = argv[0];

	// Render
	return DustJs::render(argv[1], model);
}
```

### Input
``` 
./renderer sample/hello.js
```

### Output
```
Hey Jeff Harrell. ./renderer just rendered this!
```
