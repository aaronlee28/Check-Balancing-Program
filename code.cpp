#include <iostream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
using namespace std;

class Money
{
	long all_cent;

public:
	//Money(int newDollars = 0, int newCents = 0) : dollar(newDollars), cent(newCents){;}
	Money(long dollar, int cent);
	Money(long dollar);
	Money();

	Money& operator +=(const Money& rhs);
	friend Money operator +(const Money& amount1, const Money& amount2);
	friend Money operator -(const Money& amount1, const Money& amount2);
	friend istream& operator >>(istream& ins, Money& amount);
	friend ostream& operator <<(ostream& outs, const Money& amount);
	friend bool operator <(const Money& lhs, const Money& rhs);
};

class Check
{
	int number;
	Money amount;
	bool cashed;

public:
	Check(int number, long dollar, int cent, bool cashed);
	Check(int number, long dollar, bool cashed);
	Check();
	friend istream& operator >>(istream& ins, Check& check);
	friend ostream& operator <<(ostream& outs, const Check& check);
	void addCashedCheck();
	Money getAmount() const;
	bool getCashed() const;
};

int digit_to_int(char c);
Money getCheckedCash(const Check checks[], int numCheck);
Money getUncheckedCash(const Check checks[], int numCheck);
Money getTotalDeposits(const Money deposits[], int numDeposit);
void getCheckCashedList(Check checks[], int numCheck);
void getCheckUncashList(Check checks[], int numCheck);

int main()
{
	Money startingBalance;
	cout << "Enter the old balance:($##.##)" << endl;
	cin >> startingBalance;
	cout << endl;

	Money totalDeposits, addDeposit, clearedCheck,
		unclearedCheck, bankVersion, yourVersion, difference;
	addDeposit = startingBalance + totalDeposits;
	int numCheck, numDeposit;

	cout << "Enter the number of checks:" << endl;
	cin >> numCheck;
	cout << "\n";
	cout << "Enter the amount of each checks:(# $##.## y/n)" << endl;
	Check* checks = new Check[numCheck];
	for (int i = 0; i < numCheck; ++i) {
		cin >> checks[i];
	}

	cout << "\n";
	cout << "Enter the amount of deposit you are going to make:" << endl;
	cin >> numDeposit;
	cout << "\n";
	cout << "Enter the amount of each deposits:($##.##)" << endl;
	Money* deposits = new Money[numDeposit];

	for (int i = 0; i < numDeposit; ++i) {
		cin >> deposits[i];
	}

	totalDeposits = getTotalDeposits(deposits, numDeposit);
	cout << "\n";
	cout << "==============================" << endl;
	cout << "Total checks cashed: " << getCheckedCash(checks, numCheck) << endl;
	cout << "Total deposits: " << totalDeposits << endl;

	clearedCheck = getCheckedCash(checks, numCheck);
	unclearedCheck = getUncheckedCash(checks, numCheck);
	bankVersion = addDeposit - clearedCheck;
	yourVersion = bankVersion - unclearedCheck;
	difference = bankVersion - yourVersion;

	cout << "The Bank Version of the Balance: " << bankVersion << endl;
	cout << "Your Version of the Balance: " << yourVersion << endl;
	cout << "The difference between the bank version and your version is: " << difference << endl;
	cout << "\n";

	getCheckCashedList(checks, numCheck);
	cout << "\n";
	getCheckUncashList(checks, numCheck);
	cout << "==============================" << endl;

	delete[] deposits;
	delete[] checks;

	return 0;
}
/*Money Functions*/
Money::Money(long dollar, int cent)
{
	if (dollar * cent < 0) {
		cout << "Illegal values for dollars and cents.";
		exit(1);
	}
	all_cent = dollar * 100 + cent;
}
Money::Money(long dollar) :all_cent(dollar * 100) {}
Money::Money() : all_cent(0) {}
Money operator + (const Money& amount1, const Money& amount2) {
	Money temp;
	temp.all_cent = amount1.all_cent + amount2.all_cent;
	return temp;
}
Money operator -(const Money& amount1, const Money& amount2) {
	Money temp;
	temp.all_cent = amount1.all_cent - amount2.all_cent;
	return temp;
}
Money& Money::operator +=(const Money& rhs) {
	this->all_cent += rhs.all_cent;
	return *this;
}
istream& operator >>(istream& ins, Money& amount) {
	char one_char, decimal_point, digit1, digit2;
	long dollar;
	int cent;
	bool negative;

	ins >> one_char;

	if (one_char == '-') {
		negative = true;
		ins >> one_char;
	}
	else {
		negative = false;
	}
	ins >> dollar >> decimal_point >> digit1 >> digit2;
	if (one_char != '$' || decimal_point != '.' || !isdigit(digit1) || !isdigit(digit2) || dollar < 0) {
		cout << "Illegal form for money input \n";
		exit(1);
	}
	cent = digit_to_int(digit1) * 10 + digit_to_int(digit2);
	amount.all_cent = dollar * 100 + cent;
	if (negative)
	{
		amount.all_cent = -amount.all_cent;
	}
	return ins;
}
ostream& operator <<(ostream& outs, const Money& amount) {
	long positive_cent, dollar, cent;
	positive_cent = labs(amount.all_cent);
	dollar = positive_cent / 100;
	cent = positive_cent % 100;

	if (amount.all_cent < 0) {
		outs << "-$" << dollar << '.';
	}
	else {
		outs << "$" << dollar << '.';
	}
	if (cent < 10) {
		outs << cent;
	}
	outs << cent;
	return outs;
}

/*Check Functions: Constructors*/
Check::Check(int number, long dollar, int cent, bool cashed) : number(number), amount(Money(dollar, cent)), cashed(cashed) {}
Check::Check(int number, long dollar, bool cashed) : number(number), amount(Money(dollar)), cashed(cashed) {}
Check::Check() : number(0), amount(Money()), cashed(false) {}

/*Check Functions*/
istream& operator >>(istream& ins, Check& check) {
	string cashed;

	ins >> check.number >> check.amount >> cashed;

	if (cashed == "Y" || cashed == "y" || cashed == "yes" || cashed == "Yes") {
		check.cashed = true;
	}
	else if (cashed == "N" || cashed == "n" || cashed == "no" || cashed == "No") {
		check.cashed = false;
	}
	else {
		cout << "error";
		exit(1);
	}
	return ins;
}
ostream& operator <<(ostream& outs, const Check& check) {
	outs << check.number << ' ' << check.amount << ' ';
	if (check.cashed) {
		outs << "Cashed";
	}
	else {
		outs << "Uncashed";
	}
	return outs;
}
bool operator <(const Money& lhs, const Money& rhs) {
	return lhs.all_cent < rhs.all_cent;
}

/*Functions*/
Money Check::getAmount() const {
	return amount;
}
bool Check::getCashed() const {
	return cashed;
}
int digit_to_int(char c) {
	return(static_cast<int>(c) - static_cast<int>('0'));
}
Money getCheckedCash(const Check checks[], int numCheck) {
	Money temp;
	for (int i = 0; i < numCheck; ++i) {
		if (checks[i].getCashed()) {
			temp += checks[i].getAmount();
		}
	}
	return temp;
}
Money getUncheckedCash(const Check checks[], int numCheck) {
	Money temp;
	for (int i = 0; i < numCheck; ++i) {
		if (!checks[i].getCashed()) {
			temp += checks[i].getAmount();
		}
	}
	return temp;
}
Money getTotalDeposits(const Money deposits[], int numDeposit) {
	Money temp;
	for (int i = 0; i < numDeposit; ++i) {
		temp += deposits[i];
	}
	return temp;
}
void getCheckCashedList(Check checks[], int numCheck) {
	Check temp;
	cout << "List of checks cashed: " << endl; a
		for (int i = 0; i < numCheck; ++i) {
			for (int j = i + 1; j < numCheck; j++) {
				if (checks[j].getAmount() < checks[i].getAmount()) {
					temp = checks[i];
					checks[i] = checks[j];
					checks[j] = temp;
				}
			}
		}
	for (int i = 0; i < numCheck; ++i) {
		if (checks[i].getCashed()) {
			cout << checks[i] << endl;
		}
	}
}
void getCheckUncashList(Check checks[], int numCheck) {
	Check temp;
	cout << "List of checks cashed: " << endl;
	for (int i = 0; i < numCheck; ++i) {
		for (int j = i + 1; j < numCheck; j++) {
			if (checks[j].getAmount() < checks[i].getAmount()) {
				temp = checks[i];
				checks[i] = checks[j];
				checks[j] = temp;
			}
		}
	}
	for (int i = 0; i < numCheck; ++i) {
		if (!checks[i].getCashed()) {
			cout << checks[i] << endl;
		}
	}
}