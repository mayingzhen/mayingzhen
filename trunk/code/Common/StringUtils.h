#ifndef _STRING_UTILS_
#define _STRING_UTILS_


namespace ma
{
	inline void Tokenize(const std::string& str, std::vector<std::string>& tokens, 
							 const std::string& delimiters = " ", const bool trimEmpty = false) 
	{ 
		std::string::size_type pos, lastPos = 0; 
		while(true) 
		{ 
			pos = str.find_first_of(delimiters, lastPos); 
			if(pos == std::string::npos) 
			{ 
				pos = str.length(); 

				if(pos != lastPos || !trimEmpty) 
				{
					tokens.push_back( std::string( str.data() + lastPos, pos - lastPos ) ); 
				}

				break; 
			} 
			else 
			{ 
				if(pos != lastPos || !trimEmpty) 
				{
					tokens.push_back( std::string( str.data() + lastPos, pos - lastPos ) );
				} 
			} 

			lastPos = pos + 1; 
		} 
	}
}

#endif
