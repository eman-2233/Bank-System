#pragma once
#include<iostream>
#include<string>
#include<vector>
#include"ClsString.h";
#include"ClsPerson.h";
#include <fstream>
using namespace std;

class clsBankClient:public ClsPerson
{
private:

	enum Mode
	{
		EmptyMode = 0,
		UpdateMode = 1,
		AddNew = 2

	};
	Mode _Mode;
	string _AccountNumber;
	string _PinCode;
	float _AccountBalance;
	bool MarkOfDelete = false;
	static clsBankClient _ConvertLineToClientObject(string Line, string Seperator = "#//#")
	{
		vector<string> vClientData;
		vClientData = clsString::Split(Line, Seperator);

		return clsBankClient(Mode::UpdateMode, vClientData[0], vClientData[1], vClientData[2],
			vClientData[3], vClientData[4], vClientData[5], stod(vClientData[6]));

	}
	static clsBankClient _GetEmptyClientObject()
	{
		return clsBankClient(Mode::EmptyMode, "", "", "", "", "", "", 0);
	}
	static string _ConvertClientObjectToLine(clsBankClient Client, string Seperator = "#//#")
	{

		string stClientRecord = "";
		stClientRecord += Client.FirstName + Seperator;
		stClientRecord += Client.LastName + Seperator;
		stClientRecord += Client.Email + Seperator;
		stClientRecord += Client.Phone + Seperator;
		stClientRecord += Client.AccountNumber() + Seperator;
		stClientRecord += Client.PinCode + Seperator;
		stClientRecord += to_string(Client.AccountBalance);

		return stClientRecord;

	}
	static  vector <clsBankClient> _LoadClientsDataFromFile()
	{

		vector <clsBankClient> vClients;

		fstream MyFile;
		MyFile.open("Clients.txt", ios::in);//read Mode

		if (MyFile.is_open())
		{

			string Line;


			while (getline(MyFile, Line))
			{

				clsBankClient Client = _ConvertLineToClientObject(Line);

				vClients.push_back(Client);
			}

			MyFile.close();

		}

		return vClients;

	}

	static void _SaveCleintsDataToFile(vector <clsBankClient> vClients)
	{

		fstream MyFile;
		MyFile.open("Clients.txt", ios::out);//overwrite

		string DataLine;

		if (MyFile.is_open())
		{

			for (clsBankClient C : vClients)
			{
				DataLine = _ConvertClientObjectToLine(C);
				MyFile << DataLine << endl;

			}

			MyFile.close();

		}

	}

	void _Update()
	{
		vector <clsBankClient> _vClients;
		_vClients = _LoadClientsDataFromFile();

		for (clsBankClient &C : _vClients)
		{
			if (C.AccountNumber() == AccountNumber())
			{
				C = *this;
				break;
			}

		}

		_SaveCleintsDataToFile(_vClients);

	}
	void _AddNew() {
		_AddDataLineToFile(_ConvertClientObjectToLine(*this));
	}
	void _AddDataLineToFile(string  stDataLine)
	{
		fstream MyFile;
		MyFile.open("Clients.txt", ios::out | ios::app);

		if (MyFile.is_open())
		{

			MyFile << stDataLine << endl;

			MyFile.close();
		}

	}
public:
	clsBankClient(Mode Mode, string FirstName, string LastName, string Email, string Phone,
		string AccountNumber, string PinCode, float AccountBalance) :ClsPerson(FirstName,
			LastName, Email, Phone) {

		_Mode = Mode;
		_AccountBalance = AccountBalance;
		_AccountNumber = AccountNumber;
		_PinCode = PinCode;
	}
	string AccountNumber()
	{
		return _AccountNumber;
	}
	void SetPinCode(string PinCode)
	{
		_PinCode = PinCode;
	}
	string GetPinCode()
	{
		return _PinCode;

	}
	__declspec(property(get = GetPinCode, put = SetPinCode))string PinCode;
	void SetAccountBalance(float AccountBalance)
	{
		_AccountBalance = AccountBalance;
	}
	float GetAccountBalance()
	{
		return _AccountBalance;
	}
	__declspec(property(get = GetAccountBalance, put = SetAccountBalance))float AccountBalance;
	bool IsEmpty()
	{
		return (_Mode == Mode::EmptyMode);
	}
	void print() {
		cout << "\nClient Card:";
		cout << "\n___________________\n";
		cout << "FirstName           " << FirstName << endl;
		cout << "LastName            " << LastName << endl;
		cout << "Email               " << Email << endl;
		cout << "Phone               " << Phone << endl;
		cout << "AccountNumber       " << _AccountNumber << endl;
		cout << "PinCode             " << _PinCode << endl;
		cout << "AccountBalance      " << _AccountBalance << endl;
		cout << "___________________\n";
	}
	static clsBankClient Find(string AccountNumber)
	{


		fstream MyFile;
		MyFile.open("Clients.txt", ios::in | ios::out | ios::app);//read Mode

		if (MyFile.is_open())
		{
			string Line;
			while (getline(MyFile, Line))
			{
				clsBankClient Client = _ConvertLineToClientObject(Line);
				if (Client.AccountNumber() == AccountNumber)
				{
					MyFile.close();
					return Client;
				}

			}

			MyFile.close();

		}

		return _GetEmptyClientObject();
	}


	static bool IsClientExist(string AccountNumber)
	{
		clsBankClient Client1 = clsBankClient::Find(AccountNumber);
		return (!Client1.IsEmpty());

	}
	enum enSaveResults{ svFaildEmptyObject = 0, svSucceeded = 1, svFaildAccountNumberExists=2,
	};
	enSaveResults Save()
	{
		switch (_Mode)
		{
		case Mode::EmptyMode:
		{
			if (IsEmpty()) {
				return enSaveResults::svFaildEmptyObject;
			}
		}
		case clsBankClient::UpdateMode:
		{

			_Update();
			return enSaveResults::svSucceeded;

			break;
		}
		case clsBankClient::AddNew:
		{

			if (clsBankClient::IsClientExist(_AccountNumber))
			{
				return enSaveResults::svFaildAccountNumberExists;
			}
			else
			{
				_AddNew();

				//We need to set the msode to update after add new
				_Mode = Mode::UpdateMode;
				return enSaveResults::svSucceeded;
			}

			break;
		}

		}
		
	}

	static clsBankClient GetAddNewClientOject(string AccountNumber)
	{
		return clsBankClient(Mode::AddNew, "", "", "", "", AccountNumber, "", 0);
	}
	bool Delete()
	{
		vector<clsBankClient> _Vclient;
		_Vclient = _LoadClientsDataFromFile();
		for (clsBankClient& C : _Vclient)
		{
			if (C.AccountNumber() == _AccountNumber)
			{
				C.MarkOfDelete = true;
				break;
			}


		}
		_SaveCleintsDataToFile(_Vclient);
		*this = _GetEmptyClientObject();
		return true;
	}
	static vector <clsBankClient> GetClientList()
	{
		return clsBankClient::_LoadClientsDataFromFile();
	}
	static double GetTotalBalance()
	{
		vector<clsBankClient> Vclients = clsBankClient::GetClientList();
		double TotalBalance = 0;
		for (clsBankClient c : Vclients)

		{
			TotalBalance += c.AccountBalance;
		}
		return TotalBalance;
	}
};

