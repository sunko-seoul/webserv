#include <fstream>
#include <sstream>

#include "Location.hpp"
#include "parseUtils.hpp"

Location::Location()
	: AConfParser()
{ }

Location::Location(std::ifstream& confFile)
	: AConfParser()
{
	mRoot = "";
	mMaxSize[0] = 0;
	mMaxSize[1] = 0;
	mMaxSize[2] = 0;
	parse(confFile);
}
Location::Location(const Location& src)
{
	AConfParser::operator=(src);
}

Location& Location::operator=(const Location& rhs)
{
	if (this == &rhs)
		return (*this);
	AConfParser::operator=(rhs);
	return (*this);
}

Location::~Location()
{ }

void Location::GetRoot(std::string& path)
{
	if (mRoot != "")
		path = mRoot;
}

void Location::GetMaxSize(size_t* res)
{
	if (mMaxSize[0] || mMaxSize[1] || mMaxSize[2])
		res = mMaxSize;
}

// server {
// 	root /var;
// 	index index.html;
// 	cgi php /usr/bin/php7.1;
// 	location /html {
// 		root /var/www;
// 		index index.php;
// 		cgi php /usr/bin/php8.1;
// 	}
// }
// URI(입력): /html/index.php => ABSPath(결과): /var/www/html/index.php(8.1)

void Location::SetResource(struct Resource& res)
{
	if (mHttpMethod.size() != 0)
		res.HttpMethod = mHttpMethod;
	if (mRoot != "")
		res.Root = mRoot;
	if (!mIndex.empty())
		res.Index = mIndex;
	for (std::map<std::string, std::string>::iterator it = mCGI.begin(); it != mCGI.end(); it ++)
	{
		res.CGIBinaryPath[it->first] = it->second;
	}
}

void Location::parse(std::ifstream& confFile)
{
	std::stringstream ss;
	std::string line;
	std::string word;

	while (std::getline(confFile, line))
	{
		seperateMetaChar(line, "{};");
		ss.clear();
		ss << line;
		if (!(ss >> word))
			continue;
		else if (word == "}")
			return (parseClosedBracket(ss, word));
		else if (word == "root")
			parseRoot(ss, word);
		else if (word == "index")
			parseIndex(ss, word);
		else if (word == "autoindex")
			parseAutoIndex(ss, word);
		else if (word == "limit_except")
			parseLimitExcept(ss, word);
		else if (word == "cgi")
			parseCGI(ss, word);
		else if (word == "client_max_size")
			parseClientMaxSize(ss, word);
		else
			throw std::runtime_error("Invalid symbol or syntax");
		if (confFile.eof())
			throw std::runtime_error("Invalid location block");
	}
}

void	Location::PrintInfo()
{
	std::cout << "HttpMethod: ";
	printVec(mHttpMethod);
	std::cout << "Root: " << mRoot << std::endl;
	std::cout << "Index: ";
	printSet(mIndex);
	std::cout << "AutoIndex: " << mbAutoIndex << std::endl;
	std::cout << "CGI: ";
	printMap(mCGI);
}
