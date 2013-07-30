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
using namespace v8;


Handle<Value> onRender(const Arguments &args) {
	// If dust does not return an error
	if (args[0]->IsNull()) {
		// args[1] is the rendered template
		String::Utf8Value data(args[1]);
		printf("%s\n", *data);
	} else {
		// Otherwise, args[0] is the error
		String::Utf8Value err(args[0]);
		printf("%s\n", *err);
	}

	return Undefined();
}


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
	return DustJs::render(argv[1], model, onRender);
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
