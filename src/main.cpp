#include "accounts.hpp"

using namespace std;

char mainMenu() // Function to display the main menu, not banking menu
{
    char choice;
    cout << "********** MAIN MENU ********** " << endl << endl;
    cout << "c(C) -> Create User Account" << endl;
	cout << "l(L) -> Login to Account" << endl;
	cout << "d(D) -> Debug" << endl;
	cout << "q(Q) -> Quit the Program" << endl;
    cout << "Enter your choice:" << endl;
    cin >> choice;
    return choice;
}

int main(int argc, char* argv[])
{
	char choice;
	Accounting *GroupAccounting = NULL;
    while (choice != 'q' || choice != 'Q')
    {
        if(NULL != GroupAccounting )
		{
			delete GroupAccounting;
			GroupAccounting = NULL;
		}
		
		choice = mainMenu();
        if (choice == 'q' || choice == 'Q'){
         cout << "Thanks for Accounting!";   break;} // I use break a lot in this program. Great for terminating/going back a menu
        switch (choice)
        {
        case 'l':
        case 'L':
			GroupAccounting = new Accounting();
            if( ERROR_PASSWORD_SUCCESS == GroupAccounting->login())
				GroupAccounting->StartAccounting();
            break;
        case 'c':
        case 'C':
			GroupAccounting = new Accounting();
			GroupAccounting->createAccount();
			break;
		case 'd':
		case 'D':
			//GroupAccounting = new Accounting();
			GroupAccounting = new Accounting();
			GroupAccounting->DisplayInfo();
			break;
         default:
            cout << "Invalid choice!" << endl;
        }
    }
  

	cout << "Process ended!!!" << endl;
	
    return 0;
}
