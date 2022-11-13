#include "yunit.h"
#include <string.h>
#include <iostream>
#include <malloc.h>
#include <time.h>

using namespace std;

YAssert::YAssert(YTest* const test):_actual(),_expected() {
	if (this->_test = test) {
		this->_output = test->output();
	}
	else this->_output = 0;
	this->_message = 0;


	//this->_expected.intValue = expected;
	this->_nextAssert = 0;
	this->_isPass = -1;
	this->_isPrinted = false;
	this->_actual.pointerValue = 0;
	this->_expected.pointerValue = 0;
}

YAssert& YAssert::message(const char* message, ...) {
	if (message == 0) return *this;
	size_t len = strlen(message);
	this->_message = (char*)malloc(len+1);
	if (this->_message == 0) throw "malloc failed";
	memcpy((void*)this->_message, message, len);
	*(char*)(this->_message + len) = 0;
	if (this->_output) {
		this->_output->assert(this);
	}
	return *this;
}
YAssert::~YAssert() {
	free((void*)this->_message);
	this->_message = 0;
}

YUnitCounter::YUnitCounter() {
	this->_failCount = this->_okCount = this->_totalCount = 0;
	this->_startTime = time(NULL);
	this->_endTime = 0;
}
void YUnitCounter::complete() {
	this->_endTime= time(NULL);
}

YTest::YTest(YUnit* unit, const char* name):YUnitCounter() {
	if (this->_unit = unit) {
		this->_output = unit->output();
	}
	else this->_output = 0;
	size_t len = strlen(name);
	this->_name = (char*)malloc(len+1);
	if (this->_name == 0) throw "malloc failed";
	memcpy((void*)this->_name, name, len);
	*(char*)(this->_name+ len) = 0;
	this->_firstAssert = this->_lastAssert = 0;
	this->_nextTest = 0;
	unit->output()->testBegin(name);
	
}


YAssert* YTest::appendAssert() {
	YAssert* assert = new YAssert(this);
	if (this->_firstAssert == 0) {
		this->_firstAssert = this->_lastAssert = assert;
	}
	else {
		this->_lastAssert->_nextAssert = assert;
		this->_lastAssert = assert;
	}
	return assert;
};


void YTest::complete() {
	YUnitCounter::complete();
	YAssert* assert = this->_firstAssert;
	int count = 0;
	while (assert) {
		this->_totalCount++;
		if (assert->isPass()) this->_okCount++;
		else this->_failCount++;
		assert = assert->_nextAssert;
	}
	if(this->_output)this->output()->testEnd(this);
}

YTest::~YTest() {
	free((void*)this->_name);
	this->_name = 0;
	YAssert* a = this->_firstAssert;
	while (a != 0) {
		YAssert* next = a->_nextAssert;
		delete a;
		a = next;
	}
	this->_firstAssert = this->_lastAssert = 0;
}

YUnit::YUnit(const char* name, YUnitOutput* output){
	this->_output = output;
	size_t len = strlen(name);
	this->_name = (char*)malloc(len+1);
	memcpy((void*)this->_name, name, len);
	*(char*)(this->_name + len) = 0;
	this->_firstTest = this->_lastTest = 0;
	if (output) {
		output->unitBegin(name);
	}
}
YTest* YUnit::appendTest(const char* name) {
	YTest* test = new YTest(this, name);
	if (this->_firstTest == 0) {
		this->_firstTest = this->_lastTest = test;
	}
	else {
		this->_lastTest->_nextTest = test;
		this->_lastTest = test;
	}
	return test;
}

void YUnit::complete() {
	YUnitCounter::complete();
	YTest* test = this->_firstTest;
	int count = 0;
	while (test) {
		this->_totalCount++;
		if (test->totalCount() == test->okCount()) this->_okCount++;
		else this->_failCount++;
		test = test->_nextTest;
	}
	if (this->_output) {
		this->_output->unitEnd(this);
	}
}
YUnit::~YUnit() {
	free((void*)this->_name);
	this->_name = 0;
	YTest* a = this->_firstTest;
	while (a != 0) {
		YTest* next = a->_nextTest;
		delete a;
		a = next;
	}
	this->_firstTest = this->_lastTest = 0;
}

void YUnitDefaultOutput::assert(YAssert* assert) {
	if (assert->isPass()) {
		cout << "\t\t+ \033[32;1m[OK]\033[0m ";	
	}
	else {
		cout << "\t\t+ \033[32;1m[FAIL]\033[0m ";
	}
	const char* msg = assert->message();
	if (msg) cout << msg<<endl;
	else {
		cout << "actual value is not match then expected value"<<endl;
	}
}


void YUnitDefaultOutput::testBegin(const char* name) {
	cout << "\t\033[36;1m" << name << endl <<"\t----------\033[0m" << endl;
}

void YUnitDefaultOutput::testEnd(YTest* test) {
	cout << endl << "\t>> ";
	if (test->okCount() == test->totalCount()) {
		
		cout << "\033[32;1m[OK]\033[0m";
		cout << "\033[36;1m"<<test->name()<<"\033[0m";
		cout << ": total=" << test->totalCount() << ", ellapsed=" << test->ellapsed() << endl << endl;
	}
	else {
		cout << "\031[32;1m[FAIL]\033[0m";
		cout << "\t\033[35;1m"<<test->name()<<"\033[0m";
		cout << ": total=" << test->totalCount() << ", fail=" << test->failCount() << ", ellapsed=" << test->ellapsed() << endl << endl;
	}
	
}

void YUnitDefaultOutput::unitBegin(const char* name) {
	cout << "\033[34;1m" << name << endl << "====================\033[0m" << endl << endl;
}

void YUnitDefaultOutput::unitEnd(YUnit* test) {
	cout << endl << "> ";
	if (test->okCount() == test->totalCount()) {
		cout << "\033[32;1m[OK]\033[0m";
		cout << "\033[34;1m"<<test->name()<<"\033[0m";
		cout <<": total=" << test->totalCount() << ", ellapsed=" << test->ellapsed()  << endl << endl;
	}
	else {
		cout << "\031[32;1m[FAIL]\033[0m";
		cout << "\033[33;1m"<<test->name()<<": \033[0m";
		cout << ": total=" << test->totalCount() << ", fail=" << test->failCount() << ", ellapsed=" << test->ellapsed() << endl << endl;
	}

}
YUnitDefaultOutput defaultOutputInst;
extern YUnitDefaultOutput* defaultOutput = &defaultOutputInst;

void unit(const char* name, UnitFn testsStatements) {
	YUnit subscribe(name, (YUnitOutput*)defaultOutput);
	testsStatements(subscribe);
	//subscribe.complete();
}

void testTest() {
	unit("test-unit", [](YUnit& subscribe) {
		subscribe("test-test", [](YTest& test) {
			test("test-expect", [](YAssert& expect) {
				expect(1).toBe(1).message("1 equal 1");
				});
			});
		});
}