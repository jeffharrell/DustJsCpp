#ifndef __DUSTJS_H__
#define __DUSTJS_H__

#include "lib/v8/include/v8.h"


class DustJs {

	public:
		static int render(const std::string filename, const std::map<std::string, std::string> &model);
		static int compile(const std::string filename);

	protected:
		static v8::Handle<v8::Value> onRender(const v8::Arguments &args);
		static v8::Handle<v8::Value> onLoad(const v8::Arguments &args);

	private:
		static void eval(const std::string &source);
		static std::string loadFile(const std::string filename);
		static v8::Handle<v8::Object> mapToJson(const std::map<std::string, std::string> &model);

};


#endif