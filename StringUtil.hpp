#ifndef StringUtil_hpp
#define StringUtil_hpp

#include <string>


inline std::string backToFrontSlash(const std::string& fileName) {
	std::string str = fileName;
	for (size_t i=0; i<str.size(); ++i)
		if(str[i]=='\\') str[i]='/';
	return str;
}

inline std::string getExtension(const std::string& fileName) {
	size_t dotPos = fileName.find_last_of('.');
	if(dotPos==std::string::npos) return "";
	if(dotPos==fileName.length()-1) return "";
	return fileName.substr(dotPos+1);
}

inline std::string getName(const std::string& fileName) {
	size_t slashPos = fileName.find_last_of('/');
	if(slashPos==std::string::npos) return fileName;
	if(slashPos==fileName.length()-1) return "";
	return fileName.substr(slashPos+1);
}



#endif StringUtil_hpp