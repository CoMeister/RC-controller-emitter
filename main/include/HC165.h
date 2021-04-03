#ifndef HC165_h
#define HC165_h

class HC165
{
private:
	char serialOut; //QH
	char load;		//SHLD
	char clk;		//Clock
	char numBerOfChip;

public:
	HC165();
	HC165(char _ser, char _load, char _clk, char _nbrChip);
	~HC165();

	int readByte();
};
#endif
