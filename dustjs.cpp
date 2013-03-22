#include <string>
#include <iostream>
#include <map>
#include "lib/v8/include/v8.h"
#include "dustjs.h"
#include "libdust.h"
#include "libcompiler.h"
#include "libparser.h"


using namespace std;
using namespace v8;


// Performs a dust.js render inside of V8
int DustJs::render(const string filename, const map<string, string> &model) {
	HandleScope handle_scope;

	// Load the dust libraries
	// Note: The libraries are generated at build time to bundle dust.js
	// rather than load it at runtime from an arbitrary location
	const char* lib_dust = reinterpret_cast<const char*>(lib_dust_js);
	const char* lib_compiler = reinterpret_cast<const char*>(lib_compiler_js);
	const char* lib_parser = reinterpret_cast<const char*>(lib_parser_js);

	// Get the base template name (minus the extension)
	const string tmpl = filename.substr(0, filename.find_last_of("."));

	// Create a new scope
	Persistent<Context> context = Context::New(NULL);
	Context::Scope context_scope(context);

	// Execute dust
	eval(lib_dust);
	eval(lib_compiler);
	eval(lib_parser);

	// Get dust out of the global scope so we can bind it with C++ methods
	Handle<Object> global = context->Global();
	Handle<Object> dust = global->Get(String::New("dust"))->ToObject();

	dust->Set(String::New("onLoad"), FunctionTemplate::New(onLoad)->GetFunction());

	// Call dust.render with our arguments
	Handle<Value> cbargs[3];

	cbargs[0] = String::New(tmpl.c_str());
	cbargs[1] = mapToJson(model);
	cbargs[2] = FunctionTemplate::New(onRender)->GetFunction();

	Handle<Function> render = Handle<Function>::Cast(dust->Get(String::New("render")));
	render->Call(dust, 3, cbargs);

	// Done!
	return 0;
}


// Callback used by dust.render once it's rendered the template
Handle<Value> DustJs::onRender(const Arguments &args) {
	String::Utf8Value err(args[0]);
	String::Utf8Value data(args[1]);

	// Output either the data or the error
	if (data.length() > 0) {
		printf("%s\n", *data);
	} else {
		printf("%s\n", *err);
	}

	return Undefined();
}


// Callback method used by dust.render to load a template
Handle<Value> DustJs::onLoad(const Arguments &args) {
	string tmpl(*String::Utf8Value(args[0]));
	string contents = loadFile(tmpl.append(".js"));

	// Execute the compiled template in the scope
	eval(contents);

	// Run the onLoad callback
	Handle<Value> cbargs[0];
	Handle<Function> callback = Handle<Function>::Cast(args[1]);

	return callback->Call(Context::GetCurrent()->Global(), 0, cbargs);
}


// Executes the source code in the current V8 context
void DustJs::eval(const string &source) {
	Handle<Script> script = Script::Compile(String::New(source.c_str()));
	script->Run();
}


// Creates a new JavaScript object inside of V8 and maps the model onto it
Handle<Object> DustJs::mapToJson(const map<string, string> &model) {
	Handle<Object> json = Object::New();

	map<string, string>::const_iterator iter;

	for (iter = model.begin(); iter != model.end(); iter++) {
		json->Set(String::New(iter->first.c_str()), String::New(iter->second.c_str()));
	}

	return json;
}


// Reads a file and returns it as a string
string DustJs::loadFile(const string filename) {
	FILE *fp = std::fopen(filename.c_str(), "rb");

	if (fp) {
		string contents;
		fseek(fp, 0, SEEK_END);
		contents.resize(ftell(fp));
		rewind(fp);
		fread(&contents[0], 1, contents.size(), fp);
		fclose(fp);

		return(contents);
	}

	return NULL;
}
