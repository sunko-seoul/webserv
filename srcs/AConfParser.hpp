#ifndef ACONFPARSER_HPP
# define ACONFPARSER_HPP
# include <sstream>
# include <string>
# include <set>
# include <vector>




class AConfParser {
protected:
	AConfParser();
	AConfParser(const AConfParser& rhs);
	AConfParser& operator=(const AConfParser& rhs);
	virtual ~AConfParser();

	void	parseRoot(std::stringstream& ss, std::string& word);
	void	parseIndex(std::stringstream& ss, std::string& word);
	void	parseAutoIndex(std::stringstream& ss, std::string& word);
	void	parseLimitExcept(std::stringstream& ss, std::string& word);
	void	parseClosedBracket(std::stringstream& ss, std::string& word);
	virtual void 	PrintInfo() = 0; //debug

	bool	isEnd(std::stringstream& ss, std::string& word);
	virtual void	parse(std::ifstream& confFile) = 0;

	bool							mbAutoIndex;
	std::vector<std::string>		mHttpMethod;
	std::string						mRoot;
	std::set<std::string>			mIndex;
private:

};

#endif