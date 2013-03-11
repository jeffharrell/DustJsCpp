#ifndef __DUSTJS_H__
#define __DUSTJS_H__

int render(std::string filename, std::map<std::string, std::string> model);

int compile(std::string filename);

#endif