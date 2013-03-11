#ifndef DUSTJS_H
#define DUSTJS_H

int render(std::string filename, std::map<std::string, std::string> model);

int compile(std::string filename);

#endif