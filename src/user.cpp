#include "user.hpp"

UserDetails::UserDetails() : _UserDB(_userDBFile.c_str())
{
	bool loadOkay = _UserDB.LoadFile();
	_errorID = _UserDB.ErrorId();	
	TiXmlElement * pElem;
	
	if ( !loadOkay )
	{
		std::cout << "[WARNING] " << _UserDB.ErrorDesc() << std::endl;
		switch(_errorID)
		{
			case TiXmlBase::TIXML_ERROR_OPENING_FILE:
			case TiXmlBase::TIXML_ERROR_DOCUMENT_EMPTY:
				TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
				TiXmlElement * element = new TiXmlElement( "UserDB" );
				_UserDB.LinkEndChild( decl );
				_UserDB.LinkEndChild( element );	
				_UserDB.SaveFile();
			break;
		}
	}
	else
	{
		pElem =  _UserDB.FirstChildElement();
		pElem = pElem->FirstChildElement("User");
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			struct BasicUserInfo * puserInfo = new (struct BasicUserInfo);
			std::string AttribUserName(pElem->Attribute("username"));
			std::string AttribUserID(pElem->Attribute("id"));
			std::string AttribFullName(pElem->Attribute("name"));
			std::string AttribPassword(pElem->Attribute("password"));
			if( AttribUserName != "" && std::stoi(AttribUserID) > USER_ID_PREFIX  ){
				std::cout<< "USER ID:" << AttribUserID << " username:" << AttribUserName <<  std::endl;
				puserInfo->fullName = AttribFullName;
				puserInfo->userUniqueID = std::stoi(AttribUserID);
				puserInfo->loginUserName = AttribUserName;
				puserInfo->password = AttribPassword;
				_pUserDB.push_back(puserInfo);
				++_totalUsers;
			}
		}
		std::cout << "Total active user's: " << _totalUsers << " Total DB: " << _pUserDB.size() <<std::endl;
	}
	
}

Account_Error_t UserDetails::IsUsernameExist(std::string userName)
{
	TiXmlElement * pElem =  _UserDB.FirstChildElement();
   
	pElem = pElem->FirstChildElement("User");
	for( pElem; pElem; pElem=pElem->NextSiblingElement())
	{
		std::string AttribName(pElem->Attribute("username"));
		if( userName == AttribName ){
			return ERROR_SUCCESS;
		}
	}
	
	return ERROR_FAILED;
}

Account_Error_t UserDetails::createAccount() // Takes and stores users login information, username and password
{
   std::string username, fullname;
   std::string password;
   TiXmlElement* pElem;
   TiXmlElement * root =  _UserDB.FirstChildElement();
   std::cout << "Please enter your Name" << std::endl;
   std::cin >> fullname;
   _user.fullName = fullname;
   std::cout << "Please create a username" << std::endl;
   std::cin >> username;
   if(ERROR_SUCCESS == IsUsernameExist(username)){
	std::cout<< "User already exist!" << std::endl;
	return ERROR_USER_EXIST;
   }
   std::cout << "Please create a password" << std::endl;
   std::cin >> password;
	std::stringstream ss;
   _user.loginUserName = username;

	
   if( 8 > password.length()){
	   return ERROR_FAILED;
   }
   
   ss << std::hash<std::string>{}(password);
   _user.password = ss.str();
	TiXmlElement * element = new TiXmlElement( "User" );
	TiXmlText * text = new TiXmlText( username );
	element->LinkEndChild( text );
	_user.userUniqueID = USER_ID_PREFIX +_totalUsers+1;
	element->SetAttribute("id", _user.userUniqueID);
	element->SetAttribute("name", _user.fullName);
	element->SetAttribute("username", _user.loginUserName);
	element->SetAttribute("password", _user.password);
	root->LinkEndChild( element );
	_UserDB.SaveFile();
	std::cout << "Account created! USER ID:"<< _user.userUniqueID << std::endl;
   return ERROR_SUCCESS;

}

void UserDetails::DisplayInfo()
{
	db_iterator m_curr_iter;
	struct BasicUserInfo * pUserInfo = NULL;
	for( m_curr_iter = this->begin(); m_curr_iter != end(); ++m_curr_iter)
	{
		pUserInfo = *(m_curr_iter);
		std::cout <<  "Name: " << pUserInfo->fullName <<  " UserID: " << pUserInfo->loginUserName << std::endl;
	}
}

Account_Error_t UserDetails :: login(void) // Takes information stored in create account. Most loaded function, also holds banking menu.
{
   Account_Error_t ErrorID = ERROR_SUCCESS;
   std::string username;
   std::string password;
   char choice;
   std::cout << "Enter your username:" << std::endl;
   std::cin >> username;
   std::cout << "Enter your password:" << std::endl;
   std::cin >> password;
   _user.loginUserName = username;
   std::stringstream ss;
   ss << std::hash<std::string>{}(password);
	_user.password = ss.str();
	ErrorID = IsUsernameExist(_user.loginUserName);
	
	if(ERROR_SUCCESS == ErrorID)
	{
		ErrorID = VerifyPassword(_user.loginUserName);
	
		if( ERROR_PASSWORD_SUCCESS != ErrorID) // Achieved by using accounts.txt match
		{		
			std::cout << "Incorrect login! Create new account or try again." << std::endl;
			return ERROR_PASSWORD_INCORRECT;
		}
	}
	else
	{
		std::cout << "Incorrect login! Create new account or try again." << std::endl;
	}
		
	return ErrorID;
}

Account_Error_t UserDetails::VerifyPassword(std::string userName)
{
	TiXmlElement * pElem =  _UserDB.FirstChildElement();
   
	pElem = pElem->FirstChildElement("User");
	for( pElem; pElem; pElem=pElem->NextSiblingElement())
	{
		std::string AttribUserName(pElem->Attribute("username"));		
		std::string Attribpassword (pElem->Attribute("password"));
		if(userName == AttribUserName ){
			if( Attribpassword == _user.password){
				std::string AttribFullName(pElem->Attribute("name"));
				_user.userUniqueID = std::stoi(pElem->Attribute("id"));
				_user.fullName = AttribFullName;
				return ERROR_PASSWORD_SUCCESS;
			}
		}
	}
	
	return ERROR_FAILED;
}

std::string UserDetails::GetUserName()
{
	return _user.loginUserName;
}

std::string UserDetails::GetUserName(uint32_t * pUserID )
{
	*pUserID = _user.userUniqueID;
	return _user.loginUserName;
}
