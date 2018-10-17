#include <iostream>
#include <string>

using namespace std;

class Mediator;
class Observer;

class Base {
public:
	void Set(Mediator* _mediator);
	void Set(Observer* _observer);

	void Send(const string& msg);
	void Receive(const string& msg);

	void Notify(const string& msg);

protected:
	Mediator* mediator;
	Observer* observer;
};

class A : public Base {
public:
	void PrintA() {
		cout << "printA\n";
	}
};

class B : public Base {
public:
	void PrintB() {
		cout << "printB\n";
	}
};

class Proxy
{
public:
	Proxy(A* _object, B* other = NULL) : object(_object) { }

	static const string ClassName() {
		return "Proxy";
	}

	void Print() {
		cout << "Proxy::Print()" << endl;
		object->PrintA();
	}

private:
	A* object;
};

class Mediator {
public:
	Mediator(A* _first, B* _second) : first(_first), second(_second) {}

	static const string ClassName() {
		return "Mediator";
	}

	void Send(const string& msg, void* from) {
		cout << "Mediator::Send(): " << msg << endl;
		if (from == first) {
			second->Notify(msg);
		}
		else {
			first->Notify(msg);
		}
	}

private:
	A* first;
	B* second;
};

class Observer {
public:
	Observer(A* first = NULL, B* second = NULL) {}

	static const string ClassName() {
		return "Observer";
	}

	void Handle(const string& msg)
	{
		cout << "Observer::Handle(): " << msg << endl;
	}
};

void Base::Set(Mediator* _mediator) {
	mediator = _mediator;
}

void Base::Set(Observer* _observer) {
	observer = _observer;
}

void Base::Send(const string& msg) {
	if (mediator == NULL)
		return;

	cout << "Base::Send(): " << msg << endl;
	mediator->Send(msg, this);

}

void Base::Receive(const string& msg) {
	cout << "Base::Receive(): " << msg << endl;

}

void Base::Notify(const string& msg) {
	if (observer == NULL)
		return;

	cout << "Base::Notify(): " << msg << endl;
	observer->Handle(msg);

}

template <class T>
class C {
public:
	static void* CreateInstance(A* a = NULL, B* b = NULL) {
		auto Pattern = new T(a, b);

		if (Pattern->ClassName() == "Proxy" && a != NULL) {
			delete Pattern;
			return new Proxy(a);
		}
		else if (Pattern->ClassName() == "Mediator" && a != NULL && b != NULL) {
			delete Pattern;
			Mediator* mediator = new Mediator(a, b);
			a->Set(mediator);
			b->Set(mediator);
			return mediator;
		}
		else if (Pattern->ClassName() == "Observer") {
			delete Pattern;
			Observer* observer = new Observer();
			a ? a->Set(observer) : false;
			b ? b->Set(observer) : false;
			return observer;
		}
		return NULL;
	}
	
private:
	C();
};

int main()
{
	A* a = new A();
	B* b = new B();

	Proxy* proxy = (Proxy*)C<Proxy>::CreateInstance(a, b);
	Mediator* mediator = (Mediator*)C<Mediator>::CreateInstance(a, b);
	Observer* observer = (Observer*)C<Observer>::CreateInstance(NULL, b);

	string delimeter = "\n=========================\n";

	proxy->Print();
	cout << delimeter;

	a->Send("Request to B");
	b->Send("Respond to A");
	cout << delimeter;

	a->Notify("Notification for observer");
	b->Notify("Notification for observer");

	system("pause");
}