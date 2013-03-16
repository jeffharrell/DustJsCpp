# DustJsCpp

An experimental C++ class for rendering a dust.js template using v8.


## Get the code
```
git clone git://github.com/jeffharrell/DustJsCpp.git --recursive
```


## Build as a static library
```
make dependencies && make
```

## Usage
```
DustJs::render(string templatePath, map<string, string> dataModel);
```
