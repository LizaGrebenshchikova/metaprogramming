#include <fstream>
#include <cstring>
#include <iostream>

using namespace std;

struct NullType {};

// TypeList

template <typename ...>
struct TypeList;

template <typename H, typename ...T>
struct TypeList<H, T...> {
	using Head = H;
	using Tail = TypeList<T...>;
};

template <typename ...>
struct TypeList 
{
	using Head = NullType;
	using Tail = NullType;
};

using EmptyList = TypeList<>;

// Reader

template <typename List>
void ReadNextLine(char** buf, ifstream& fin)
{
	using H = typename List::Head;
	using T = typename List::Tail;

	H val;
	fin >> val;

	int len = *buf == NULL ? 0 : strlen(*buf);
	*buf = (char*)realloc(*buf, len + sizeof(H));
	(*buf)[len] = '\0';

	*((H*)(*buf + len)) = val;
	ReadNextLine<T>(buf, fin);
}

template <>
void ReadNextLine<EmptyList>(char** buf, ifstream& fin) {}

template <typename List>
class Reader 
{
public:
	Reader(string fname) : fin(fname) {}

	~Reader() 
	{
		fin.close();
	}

	void* readNextLine() 
	{
		if (fin.eof()) 
		{
			return NULL;
		}

		void* buf = NULL;
		ReadNextLine<List>((char**)&buf, fin);
		return buf;
	}

private:
	ifstream fin;
};

int main() 
{
	using List = TypeList<int, double, char>;
	Reader<List> reader("Text.txt");
	void* buf = reader.readNextLine();
	while (buf != NULL) 
	{
		cout << buf << endl;
		free(buf);
		buf = reader.readNextLine();
	}
	
	char c;
	cin >> c;
}