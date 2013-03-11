#include <string>
//#include <iostream>
//#include <fstream>
//#include <cerrno>
#include <map>
#include <v8.h>
#include "dustJs.h"


#define DUST_JS "lib/dustjs/lib/dust.js"
#define DUST_EXT ".js"
#define DUST_RENDER "dust.render(template, model, callback);"


using namespace std;
using namespace v8;


class DustJs {

	public:
		static int render(const string filename, const map<string, string> &model) {
			const string tmpl = filename + DUST_EXT;

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
			global->Set(String::New("template"), String::New(filename.c_str()));
			global->Set(String::New("model"), json);

			// Create a new context
			Persistent<Context> context = Context::New(NULL, global);

			// Enter the created context
			Context::Scope context_scope(context);

			// Create a string containing the JavaScript source code
			Handle<String> dust_source = load(DUST_JS);
			Handle<String> tmpl_source = load(tmpl);
			Handle<String> render_source = String::New(DUST_RENDER);

			// Compile the source code
			Handle<Script> dust_script = Script::Compile(dust_source);
			Handle<Script> tmpl_script = Script::Compile(tmpl_source);
			Handle<Script> render_script = Script::Compile(render_source);

			// Run the script to get the result
			Handle<Value> dust_result = dust_script->Run();
			Handle<Value> tmpl_result = tmpl_script->Run();
			Handle<Value> render_result = render_script->Run();

			return 0;
		}


		static int compile (std::string filename);


	protected:
		static Handle<Value> onRender(const Arguments &args) {
			String::Utf8Value err(args[0]);
			String::Utf8Value data(args[1]);

			printf("%s\n", *data);

			return Undefined();
		}


	private:
		// Reads a file into a v8 string
		static Handle<String> load(const string &name) {
			FILE* file = fopen(name.c_str(), "rb");
			if (file == NULL) return Handle<String>();

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
			Handle<String> result = String::New(chars, size);

			delete[] chars;
			return result;
		}

};



int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Usage: %s template \n", argv[0]);
		return 1;
	}

	map<string, string> model;

	model["name"] = "John Doe";
	model["count"] = "3000";

	return DustJs::render("sample/template", model);
}