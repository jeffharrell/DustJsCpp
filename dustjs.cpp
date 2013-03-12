#include <string>
#include <iostream>
#include <map>
#include "lib/v8/include/v8.h"
#include "dustJs.h"


#define DUST_JS "../lib/dustjscpp/lib/dustjs/dist/dust-full-1.2.0.js" //FIXME
#define DUST_RENDER "dust.render(template, model, callback);"


using namespace std;
using namespace v8;


int DustJs::render(const string filename, const map<string, string> &model) {
	const string tmpl = filename.substr(0, filename.find_last_of("."));

	// Create a stack-allocated handle scope
	HandleScope handle_scope;

	// Parse the model into a native JSON object
	Handle<ObjectTemplate> json = ObjectTemplate::New();

	map<string, string>::const_iterator iter;

	for (iter = model.begin(); iter != model.end(); iter++) {
		json->Set(String::New(iter->first.c_str()), String::New(iter->second.c_str()));
	}

	// Populate the global scope
	Handle<ObjectTemplate> global = ObjectTemplate::New();

	global->Set(String::New("callback"), FunctionTemplate::New(DustJs::onRender));
	global->Set(String::New("template"), String::New(tmpl.c_str()));
	global->Set(String::New("model"), json);


	// Create a new context with the global scope
	Persistent<Context> context = Context::New(NULL, global);

	// Enter the created context
	Context::Scope context_scope(context);

	// Create a string containing the JavaScript source code
	const string dust_source = load(DUST_JS);
	const string tmpl_source = load(filename);

	// Compile the source code
	Handle<Script> dust_script = Script::Compile(String::New(dust_source.c_str()));
	Handle<Script> tmpl_script = Script::Compile(String::New(tmpl_source.c_str()));
	Handle<Script> render_script = Script::Compile(String::New(DUST_RENDER));

	// Run the script to get the result
	Handle<Value> dust_result = dust_script->Run();
	Handle<Value> tmpl_result = tmpl_script->Run();
	Handle<Value> render_result = render_script->Run();

	return 0;
}


int DustJs::compile (const string filename) {
	// Unimplemented
	return 1;
}


Handle<Value> DustJs::onRender(const Arguments &args) {
	String::Utf8Value err(args[0]);
	String::Utf8Value data(args[1]);

	if (data.length() > 0) {
		printf("%s\n", *data);
	} else {
		printf("%s\n", *err);
	}

	return Undefined();
}


// Reads a file
string DustJs::load(const string &name) {
	FILE* file = fopen(name.c_str(), "rb");
	if (file == NULL) return NULL;

	fseek(file, 0, SEEK_END);
	int size = ftell(file);
	rewind(file);

	char* chars = new char[size + 1];
	chars[size] = '\0';

	for (int i = 0; i < size;) {
		int read = fread(&chars[i], 1, size - i, file);
		i += read;
	}

	fclose(file);

	string result = string(chars);
	delete[] chars;

	return result;
}
