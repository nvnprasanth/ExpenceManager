#include "accounts.hpp"
using namespace std;

char Accounting::Accountingoptions()
{
	uint32_t UserID=0;
	char choice;
    cout << "********** GROUP ACCOUNTING MENU ********** " << endl << endl;
    cout << "n(N) -> New Spending" << endl;
    cout << "u(U) -> Update existing Spending" << endl;
    cout << "d(D) -> Delete Spending" << endl;
	cout << "r(R) -> Get Detailed Report" << endl;
	cout << "q(Q) -> Logout the user: " << GetUserName(&UserID );
	cout << "(USER ID: " << UserID << ")" << endl;
    cout << "Enter your choice:" << endl;
    cin >> choice;
    return choice;
}

void Accounting::Menu()
{
	char choice;
    while (choice != 'q' || choice != 'Q')
    {
        choice = Accountingoptions();
        if (choice == 'q' || choice == 'Q'){break;}
        switch (choice)
        {
        case 'n':
        case 'N':
//			cin >> spendingType
//			cin >> TotalAmount;
			AddNewExpense();
            break;
        case 'u':
        case 'U':
            break;
		case 'd':
		case 'D':
			break;
		case 'r':
		case 'R':
			GenerateReport();
			break;
		case 'q':
		case 'Q':
			break;
         default:
            cout << "Invalid choice!" << endl;
        }
    }
}

Account_Error_t ExpenseManagement::GenerateReport()
{
   db_iterator m_curr_iter;
   struct BasicUserInfo * pUserInfo = NULL;
   for( m_curr_iter = this->begin(); m_curr_iter != end(); ++m_curr_iter)
   {
	    int32_t TotalGiven =0, TotalExpense =0;
		std::string userName;
		pUserInfo = *m_curr_iter;
		TiXmlElement * pElem =  _accountDB.FirstChildElement(), * pSpendElem=NULL;
	
		pElem = pElem->FirstChildElement("Expense");
		for( pElem; pElem; pElem=pElem->NextSiblingElement())
		{
			pSpendElem = pElem->FirstChildElement("Spend");
		
			for( pSpendElem; pSpendElem; pSpendElem=pSpendElem->NextSiblingElement())
			{
				uint32_t AttribUserID(stol(pSpendElem->Attribute("userID")));
				if( pUserInfo->userUniqueID == AttribUserID ){
					TotalGiven += stol(pSpendElem->Attribute("credit"));
					TotalExpense += stol(pSpendElem->Attribute("debit"));
					userName = pSpendElem->Attribute("username");
				}
			}
		}
		std::cout << "user:" << userName << "\n\tGiven: " << TotalGiven << "\tExpense: " << TotalExpense <<  "\tBalance: " << TotalGiven - TotalExpense << std::endl; 
   }
   
   return ERROR_SUCCESS;
}

Account_Error_t ExpenseManagement::IsNodeExist(std::string findInfo, char * AttrType, TiXmlElement **ppElem)
{
	TiXmlElement * pElem =  _accountDB.FirstChildElement();
   
	pElem = pElem->FirstChildElement("Expense");
	for( pElem; pElem; pElem=pElem->NextSiblingElement())
	{
		std::string AttribName(pElem->Attribute(AttrType));
		if( findInfo == AttribName ){
			*ppElem = pElem;
			return ERROR_SUCCESS;
		}
	}
	
	return ERROR_FAILED;
}

void ExpenseManagement::AddNewExpense(){
		db_iterator m_curr_iter;
		struct BasicUserInfo * pUserInfo = NULL;
		std::string SpendDate;
		TiXmlElement * root =  _accountDB.FirstChildElement();
		std::cout << "Name the expense:" << std::endl;
		std::cin >> _expenseName;
		TiXmlElement * expense = NULL, * spend = NULL;
		TiXmlText * text = NULL;
		/*
		if( ERROR_SUCCESS == IsNodeExist( _expenseName,"name", &expense))
		{
			std::cout << "Expense name already found!" << std::endl;
			return;
		}
		*/
		expense = new TiXmlElement( "Expense" );
		text = new TiXmlText( _expenseName );
		expense->LinkEndChild( text );
		expense->SetAttribute("createDate", _todayDate.str());
		expense->SetAttribute("expenseName", _expenseName);
		expense->SetAttribute("createUserID", GetUserName());
		do{
			std::cout << "Enter total expense amount spend:" << std::endl;
			std::cin >> _totalExpense;
			if( 0 >= _totalExpense)
			 std::cout << "Entered expense amount too low ! re-edit." << std::endl;
		}while( 0 >= _totalExpense);
		
		expense->SetAttribute("grandTotal", _totalExpense);
		std::cout << "Enter spend date [e.g; 07/01/2018]:" << std::endl;
		std::cin >> SpendDate;
		expense->SetAttribute("spendDate", SpendDate);
		uint32_t TotalCredit =0, TotalDebit =0;
		
		for( m_curr_iter = this->begin(); m_curr_iter != end(); ++m_curr_iter)
		{
			int32_t SpendUserID=0, DebitAmount=0, CreditAmount =0;
			spend = new TiXmlElement( "Spend" );
			pUserInfo = *(m_curr_iter);
			SpendUserID = pUserInfo->userUniqueID;
			spend->SetAttribute("userID", SpendUserID);
			spend->SetAttribute("username", pUserInfo->loginUserName);
			
			std::cout << "Remaining expense amount:" << _totalExpense - TotalDebit << std::endl;
			if(TotalCredit < _totalExpense)
			{
				do{
					std::cout << "User: " << pUserInfo->fullName << " given amount:" << std::endl;
					std::cin >> CreditAmount;
				
					if( (CreditAmount+ TotalCredit) >  _totalExpense || CreditAmount <= 0)
						std::cout << "Entered given amount is higher than total | too low ! re-edit." << std::endl;
				}while((CreditAmount+ TotalCredit) >  _totalExpense );
				
				TotalCredit += CreditAmount;
			}
			else if(TotalCredit > _totalExpense)
			{
				std::cout << "[WARNING]Given amount: " << TotalCredit << " grater than total spend:"<< _totalExpense << std::endl;
				return;
			}
			
			spend->SetAttribute("credit", CreditAmount);		
			if(TotalDebit < _totalExpense)
			{
				do{
					std::cout <<  "User: " << pUserInfo->fullName <<  " expense amount:" << std::endl;
					std::cin >> DebitAmount;
					if( (DebitAmount+ TotalCredit) >  _totalExpense || DebitAmount <= 0)
						std::cout << "Entered expense amount is higher than total | too low ! re-edit." << std::endl;
				}while((DebitAmount+ TotalDebit) >  _totalExpense );
				TotalDebit += DebitAmount;
			}
			spend->SetAttribute("debit", DebitAmount);
			expense->LinkEndChild( spend );
		}
		
		if( _totalExpense == TotalDebit )
		{
			if(  _totalExpense == TotalCredit )
			{
				root->LinkEndChild( expense );
				_accountDB.SaveFile();
			}
			else
			{
				std::cout << "[WARNING]Given amount mismatching! recreate IT. Balance:"<< _totalExpense - TotalCredit << std::endl;
			}
		}
		else
		{
			std::cout << "[WARNING]spend amount mismatching! recreate IT. Balance:"<< _totalExpense - TotalDebit << std::endl;
		}
	}
ExpenseManagement::ExpenseManagement(){
	std::cout << "Loading DataBase:" << _accountDB << std::endl;

	bool loadOkay = _accountDB.LoadFile();
	unsigned int ErrorId = _accountDB.ErrorId();
	
	if ( !loadOkay )
	{
		std::cout << "[WARNING] " << _accountDB.ErrorDesc() << std::endl;
		switch(ErrorId)
		{
			case TiXmlBase::TIXML_ERROR_OPENING_FILE:
			case TiXmlBase::TIXML_ERROR_DOCUMENT_EMPTY:
				TiXmlDeclaration * decl = new TiXmlDeclaration( "1.0", "", "" );
				TiXmlElement * element = new TiXmlElement( "DB" );
				TiXmlText * text = new TiXmlText( "Monthly Group" );
				element->LinkEndChild( text );
				_accountDB.LinkEndChild( decl );
				_accountDB.LinkEndChild( element );
				_accountDB.SaveFile();
			break;
		}
	}
	
	time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
	_todayDate <<  now->tm_mday << '/' << (now->tm_mon + 1) << '/' <<  (now->tm_year + 1900);
}

Account_Error_t Accounting::StartAccounting( void )
{

	Menu();
	return ERROR_SUCCESS;
}



