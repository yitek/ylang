#pragma once
#ifndef __YUNIT_H__
#define __YUNIT_H__


#ifdef __cplusplus 
extern "C" {
#endif
	

#ifdef __cplusplus 
} // end extern "C"
#endif
class YUnit;
class YTest;
class YAssert;
class YUnitOutput;

typedef union TYUnitValue {
	char members[0];
	char buffer[0];
	char byteValue;
	int intValue;
	bool boolValue;
	float floatValue;
	double doubleValue;
	void* pointerValue;
} YUnitValue;




class YAssert {

private:
	friend YTest;
	YUnitValue _expected;
	YUnitValue _actual;
	YAssert* _nextAssert;
	const char* _message;
	int _isPass;
	bool _isPrinted;
	YTest* _test;
	YUnitOutput* _output;
public:
	YAssert(YTest* const test);
	
	bool isPass() {
		return this->_isPass == 1;
	};
	YAssert& message(const char* message, ...);
	const char* message() { return this->_message; };

	YAssert& operator()(int expect) {
		this->_expected.intValue = expect;
		return *this;
	};


	YAssert& toBe(int val) {
		this->_actual.intValue = val;
		this->_isPass = this->_expected.intValue == val ? 1 : 0;
		return *this;
	};
	YAssert& toBe(void* val) {
		this->_actual.pointerValue = val;
		this->_isPass = this->_expected.pointerValue == val ? 1 : 0;
		return *this;
	};
	YAssert& toBe(float val) {
		this->_actual.floatValue = val;
		this->_isPass = this->_expected.floatValue == val ? 1 : 0;
		return *this;
	};
	YAssert& toBe(double val) {
		this->_actual.doubleValue = val;
		this->_isPass = this->_expected.doubleValue == val ? 1 : 0;
		return *this;
	};
	~YAssert();
};


typedef void (*ExpectsFn)(YAssert&);

class YUnitCounter {
private:

	long long _startTime;
	long long _endTime;
protected:
	int _okCount;
	int _failCount;
	int _totalCount;
public:
	YUnitCounter();
	int okCount() { return this->_okCount; };
	int failCount() { return this->_failCount; };
	int totalCount() { return this->_totalCount; };
	int ellapsed() { return this->_endTime == 0 ? -1 : (int)(this->_endTime - this->_startTime); };
	virtual void complete();

};

class YTest :public YUnitCounter{
private:
	const char* _name;
	YUnitOutput* _output;
protected:
	
	YAssert* _firstAssert;
	YAssert* _lastAssert;
	YTest* _nextTest;
	YUnit* _unit;
	friend YUnit;
public:
	YTest(YUnit* unit, const char* name);
	
	YAssert* appendAssert();

	const char* name() { return this->_name; };
	YUnitOutput* output() { return this->_output; };

	void operator()(const char* name, ExpectsFn assertsStatement) {
		YAssert* expect = this->appendAssert();
		assertsStatement(*expect);
		this->complete();
	};

	virtual void complete();

	~YTest();
};


typedef void(*TestsFn)(YTest& test);
typedef void(*TestsWithAssertsFn)(YTest& test,YAssert& expect);

class YUnit:public YUnitCounter {
private:
	const char* _name;
	YTest* _firstTest;
	YTest* _lastTest;
	YUnitOutput* _output;

public:
	YUnit(const char* name, YUnitOutput* output);

	YUnitOutput* output() { return this->_output; };
	const char* name() { return this->_name; }
	YTest* appendTest(const char* name);

	void operator()(const char* name, TestsFn testsStatement) {
		YTest* test = this->appendTest(name);
		testsStatement(*test);
		this->complete();
	};
	virtual void complete();
	
	~YUnit();
};

class YUnitOutput {
private:

public:
	virtual void unitBegin(const char* name) = 0;
	virtual void unitEnd(YUnit* unit) = 0;
	virtual void testBegin(const char* name) = 0;
	virtual void testEnd(YTest* test) = 0;
	virtual void assert(YAssert* assert) = 0;
};

class YUnitDefaultOutput : public YUnitOutput {
public:
	YUnitDefaultOutput() {}
	virtual void unitBegin(const char* name);
	virtual void unitEnd(YUnit* unit);
	virtual void testBegin(const char* name);
	virtual void testEnd(YTest* test);
	virtual void assert(YAssert* assert);
	~YUnitDefaultOutput() {};
};
extern YUnitDefaultOutput* defaultOutput;

typedef void(*UnitFn)(YUnit& unit);
typedef void(*UnitWithTestFn)(YUnit& unit,void* context);

void unit(const char* name, UnitFn testsStatements);



void testTest();


#endif // !__HDEF_H__

