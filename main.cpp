#include <map>
#include <cstdio>
#include <QFile>

using namespace std;

#define BUFSIZE 3

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		printf("%s <inputfile>\n", argv[0]);
		return -1;
	}
	QFile in(argv[1]);
	if (!in.open(QIODevice::ReadOnly))
	{
		printf("ERROR: inputfile not openable\n");
		return -1;
	}
	unsigned int bufsize = 0;
	char buf[BUFSIZE+1];
	for (int i=0; i<BUFSIZE+1; i++)
	{
		buf[i] = '\0';
	}
	map<string, unsigned long> m;
	unsigned long treated = 0;
	unsigned long toBeTreated = in.size();
	while (treated < toBeTreated)
	{
		unsigned int readSize = 1024;
		if (treated+readSize > toBeTreated)
		{
			readSize = toBeTreated-treated;
		}
		char readBuf[readSize];
		if (in.read(readBuf, readSize) != readSize)
		{
			printf("ERROR: inputfile not readable\n");
			return -1;
		}
		for (unsigned int i=0; i<readSize; i++)
		{
			if (bufsize < BUFSIZE)
			{
				buf[bufsize] = readBuf[i];
				bufsize++;
			}
			else
			{
				for (unsigned int j=0; j<bufsize; j++)
				{
					if (j+1 < bufsize)
					{
						buf[j] = buf[j+1];
					}
					else
					{
						buf[j] = readBuf[i];
					}
				}
			}
			if (bufsize == BUFSIZE)
			{
				char key[bufsize+1];
				for (unsigned int j=0; j<bufsize+1; j++)
				{
					key[j] = '\0';
				}
				strncpy(key, buf, bufsize);
				string s(key);
				m[s] = m[s]+1;
			}
		}
		treated += readSize;
	}
	in.close();
	for (auto it=m.begin(); it!=m.end(); ++it)
    {
		printf("%s\t%.2f %%\n", it->first.c_str(), (float)(it->second*100)/toBeTreated);
	}
	return 0;
}
