// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"
#include <cstring>
#include <algorithm>

#define BITS_IN_ONE_MEM (sizeof(TELEM) * 8)

TBitField::TBitField(int len)
{
	if (len < 0)
	{
		throw "Wrong length";
	}
	BitLen = len;
	TELEM k = len % BITS_IN_ONE_MEM > 0 ? 1 : 0;
	MemLen = len / BITS_IN_ONE_MEM + k;
	pMem = new TELEM[MemLen];
	memset(pMem,0,sizeof(TELEM)*MemLen);
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen=bf.BitLen;
	MemLen=bf.MemLen;
	pMem=new TELEM[MemLen];
	for (TELEM i = 0 ; i < MemLen ; i++)
	{
		pMem[i]=bf.pMem[i];
	}
}

TBitField::~TBitField()
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{

 	return n/BITS_IN_ONE_MEM;
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if(n < 0 || n > BitLen) throw "Whong bit for get_mask";
	return ((TELEM)1 << (n % BITS_IN_ONE_MEM));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n > BitLen))
	throw "Out of range diapason";
	TELEM i = GetMemIndex(n);
	TELEM j = GetMemMask(n);
	pMem[i] = (pMem[i] | j);
}

void TBitField::ClrBit(const int n) // очистить бит
{
	TELEM i = GetMemIndex(n);
	TELEM j = ~GetMemMask(n);
	pMem[i] = (pMem[i] & j);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	TELEM i = GetMemIndex(n);
	TELEM j = GetMemMask(n);
	return int((pMem[i] & j) !=0);
}

// битовые операции

TBitField& TBitField::operator=(const TBitField & bf) // присваивание
{	
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		delete[] pMem;
		pMem = new TELEM[MemLen];
		for (TELEM i = 0 ; i < MemLen ; i++)
		{
			 pMem[i]=bf.pMem[i];
		}
	return *this;
}

int TBitField::operator==(const TBitField & bf) const // сравнение
{
	if (MemLen != bf.MemLen) return 0;
	for(TELEM i = 0 ; i < MemLen ; i++)
	{
		if(pMem[i] != bf.pMem[i]) return 0;
	}
	return 1;
}

int TBitField::operator!=(const TBitField & bf) const // сравнение
{
	if (BitLen != bf.BitLen) return 1;
	for(TELEM i = 0 ; i < BitLen ; i++)
	{
		if(this->GetBit(i) != bf.GetBit(i)) return 1;
	}
	return 0;
}

TBitField TBitField::operator|(const TBitField & bf) // операция "или"
{	
	TELEM size=max(BitLen, bf.BitLen);
	TBitField out(size);

	for(TELEM i = 0; i < out.MemLen ; i++)
	{
		if(i < MemLen && i < bf.MemLen)
			out.pMem[i] = pMem[i] | bf.pMem[i];
		else
		{
			if(i < MemLen && i >= bf.MemLen) out.pMem[i] = pMem[i];
			else
			if(i >= MemLen && i < bf.MemLen) out.pMem[i] = bf.pMem[i];
		}
	}
	return out;
}

TBitField TBitField::operator&(const TBitField & bf) // операция "и"  
{
	TELEM size=max(BitLen, bf.BitLen);
	TBitField out(size);
	for(TELEM i = 0; i < out.MemLen ; i++)
	{
		if((i <= MemLen) && (i <= bf.MemLen))
			 out.pMem[i]=pMem[i] & bf.pMem[i];
	}
	/*for(TELEM i = 0; i < out.BitLen ; i++)
	{
		if((i <= BitLen) && (i <= bf.BitLen))
			 if((this->GetBit(i)) & (bf.GetBit(i))) out.SetBit(i);
	}*/
	return out;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField out(BitLen);
		for(TELEM i = 0 ; i < BitLen ; i++)
		if((this->GetBit(i))) out.ClrBit(i);
		else out.SetBit(i); 
	return out;
}

// ввод/вывод

istream& operator>>(istream & istr, TBitField & bf) // ввод
{
	TELEM g=3;
	cout<<"Input -1 for end input"<<endl;
	cin>>g;
	while(g!=-1)
	{
		bf.SetBit(g);
		cin>>g;
	}

  return istr;
}

ostream& operator<<(ostream & ostr, const TBitField & bf) // вывод
{
	for(TELEM i = 0 ; i < bf.BitLen ; i++)
	{
		if(bf.GetBit(i)) cout<<i<<", ";
	}

	return ostr;
}
