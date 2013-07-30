#ifndef __DUSTJS_H__
#define __DUSTJS_H__

#include "lib/v8/include/v8.h"


typedef v8::Handle<v8::Value> (*onRenderFn)(const v8::Arguments&);


class DustJs {

	public:
		static int render(const std::string filename, const std::map<std::string, std::string> &model, onRenderFn);
		static int compile(const std::string filename);

	protected:
		static v8::Handle<v8::Value> onLoad(const v8::Arguments &args);

	private:
		static void eval(const std::string &source);
		static std::string loadFile(const std::string filename);
		static v8::Handle<v8::Object> mapToJson(const std::map<std::string, std::string> &model);

};


#endif