#include "AConfParser.hpp"

AConfParser::AConfParser()
	: mbIsDuplicatedAutoIndex(false)
	, mbIsDuplicatedLimitExcept(false)
	, mbIsDuplicatedRoot(false)
	, mbAutoIndex(false)
	, mRoot("/")
{}

AConfParser::AConfParser(const AConfParser& rhs)
{
	static_cast<void>(rhs);
}

AConfParser::~AConfParser()
{}

AConfParser& AConfParser::operator=(const AConfParser& rhs)
{
	if (this == &rhs)
		return *this;
	mbIsDuplicatedAutoIndex = rhs.mbIsDuplicatedAutoIndex;
	mbIsDuplicatedLimitExcept = rhs.mbIsDuplicatedLimitExcept;
	mbAutoIndex = rhs.mbAutoIndex;
	mHttpMethod = rhs.mHttpMethod;
	mRoot = rhs.mRoot;
	mIndex = rhs.mIndex;
	mCGI = rhs.mCGI;
	return *this;
}

void AConfParser::parseRoot(std::stringstream& ss, std::string& word)
{
	if (mbIsDuplicatedRoot == true)
		throw std::runtime_error("root duplicated");
	mbIsDuplicatedRoot = true;
	if (ss >> mRoot && ss >> word && isEnd(ss, word))
	{
		if (mRoot.find("//") != std::string::npos)
			throw std::runtime_error("invalid root path");
		if (mRoot[mRoot.size()-1] == '/' && mRoot.length() > 1)
			mRoot.erase(mRoot.size()-1);
		return ;
	}
	throw std::runtime_error("bad end logic");
}

void AConfParser::parseIndex(std::stringstream& ss, std::string& word)
{
	int	tmp = 0;
	while (ss >> word)
	{
		if (isEnd(ss, word))
		{
			if (tmp == 0)
				throw std::runtime_error("index file duplicated");
			return ;
		}
		tmp ++;
		if (mIndex.find(word) == mIndex.end())
			mIndex.insert(word);
		else
			throw std::runtime_error("index file duplicated");
	}
}

void AConfParser::parseCGI(std::stringstream& ss, std::string& word)
{
	std::string cgiExec;
	if ((ss >> word) && (ss >> cgiExec))
	{
		mCGI[word] = cgiExec;
		if (ss >> word && isEnd(ss, word))
			return ;
	}
	throw std::runtime_error("invalid cgi format");
}

void AConfParser::parseAutoIndex(std::stringstream& ss, std::string& word)
{
	if (mbIsDuplicatedAutoIndex == true)
		throw std::runtime_error("auto index duplicated");
	mbIsDuplicatedAutoIndex = true;
	if (ss >> word)
	{
		if (word == "on")
			mbAutoIndex = true;
		else if (word == "off")
			mbAutoIndex = false;
		else
			throw std::runtime_error("invalid auto index format");
		if (ss >> word && isEnd(ss, word))
			return ;
	}
	else
		throw std::runtime_error("invalid auto index format");
}

void AConfParser::parseLimitExcept(std::stringstream& ss, std::string& word)
{
	if (mbIsDuplicatedLimitExcept == true)
		throw std::runtime_error("limit except duplicated");
	mbIsDuplicatedLimitExcept = true;
	while (ss >> word)
	{
		if (isEnd(ss, word))
			return ;
		mHttpMethod.push_back(word);
	}
}

bool	AConfParser::isEnd(std::stringstream& ss, std::string& word)
{
	if (word != ";")
		return (false);
	else if (ss >> word)
		throw std::runtime_error("bad end logic");
	else
		return (true);
}

void AConfParser::parseClosedBracket(std::stringstream& ss, std::string& word)
{
	if (ss >> word)
		throw std::runtime_error("wrong bracket format");
}