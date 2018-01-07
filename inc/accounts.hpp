#include "user.hpp"
#ifndef __ACCOUNTS_H__
#define __ACCOUNTS_H__

class ExpenseManagement : public UserDetails
{
	private:	
	std::stringstream _todayDate;
	TiXmlDocument _accountDB="db.xml";
	std::string _expenseName;
	std::string _spendDate;
	int32_t _totalExpense;
	uint32_t _spendUserID;
	
	public:
	ExpenseManagement();
	Account_Error_t GenerateReport(void);
	Account_Error_t IsNodeExist(std::string findInfo, char* AttrType, TiXmlElement **ppElem);
	void AddNewExpense();
	
};

class Accounting : public ExpenseManagement
{
	private:
	void Menu();
	char Accountingoptions();
	public:
	Accounting(){
	};
	Account_Error_t StartAccounting(void);

};

#endif //__ACCOUNTS_H__