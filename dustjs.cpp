#include <string>
#include <iostream>
#include <map>
#include "lib/v8/include/v8.h"
#include "dustJs.h"


#define DUST_JS "../lib/dustjscpp/lib/dustjs/dist/dust-full-1.2.0.js" //FIXME
#define DUST_RENDER "dust.render(template, model, callback);"
#define DUST_COMPILE "dust.compile(source, name);"


using namespace std;
using namespace v8;


// Performs a dust.render() inside of the v8 JavaScript environment
int DustJs::render(const string filename, const map<string, string> &model) {
	HandleScope handle_scope;

	// Get the base template name (minus the extension)
	const string tmpl = filename.substr(0, filename.find_last_of("."));

	// Populate the global scope
	Handle<ObjectTemplate> global = ObjectTemplate::New();

	global->Set(String::New("callback"), FunctionTemplate::New(onRender));
	global->Set(String::New("template"), String::New(tmpl.c_str()));
	global->Set(String::New("model"), mapToJson(model));

	// Create a new context with that global scope
	Persistent<Context> context = Context::New(NULL, global);

	// Enter the created context
	Context::Scope context_scope(context);

	// Load the JavaScript files
	evalJs(loadFile(DUST_JS));
	evalJs(loadFile(filename));
	evalJs(DUST_RENDER);

	return 0;
}


// Performs a dust.compile() inside of the v8 JavaScript environment
int DustJs::compile (const string filename) {
	// TODO
	return 1;
}


// Callback function used by JavaScript dust.render in DustJs::render
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


// Executes the source code in the current v8 context
void DustJs::evalJs(const string &source) {
	Handle<Script> script = Script::Compile(String::New(source.c_str()));
	script->Run();
}


// Creates a new JavaScript object inside of v8 and maps the model onto it
Handle<ObjectTemplate> DustJs::mapToJson(const map<string, string> &model) {
	Handle<ObjectTemplate> json = ObjectTemplate::New();

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
