#include <iostream>
#include "main.hpp"
#include "tinyxml.h"
#include <sstream>

#ifndef __USER_H__
#define __USER_H__

#define USER_ID_PREFIX 10

typedef enum 
{
	ERROR_SUCCESS,
	ERROR_FAILED,
	ERROR_USER_ACC_FAILED,
	ERROR_PASSWORD_SUCCESS,
	ERROR_PASSWORD_INCORRECT,
	ERROR_USER_EXIST,
	ERROR_USER_NOT_FOUND
	
}Account_Error_t;

struct BasicUserInfo
{
	std::string fullName;
	std::string loginUserName;
	std::size_t userUniqueID;
	std::string password;
};

class UserDetails
{
	private:
	std::string _userDBFile="accounts.xml";
	struct BasicUserInfo _user; 
	TiXmlDocument _UserDB;
	uint32_t _errorID=0;
	uint32_t _totalUsers=0;
	std::vector<struct BasicUserInfo *> _pUserDB;
	std::vector<struct BasicUserInfo *>::iterator m_curr_iter;
	
	Account_Error_t IsUsernameExist(std::string userName);
	Account_Error_t VerifyPassword(std::string userName);
	
	public:
	typedef std::vector<struct BasicUserInfo *>::iterator db_iterator;
	UserDetails();
	Account_Error_t createAccount();
	Account_Error_t login(void);
	void DisplayInfo();
	std::string GetUserName();
	std::string GetUserName(uint32_t * pUserID );
	db_iterator begin(){ return m_curr_iter = _pUserDB.begin();}
	db_iterator end(){ return m_curr_iter = _pUserDB.end(); }

};

#endif // __USER_H__
