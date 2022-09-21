﻿/* 
 * Copyright (C) 2022 zhou xuan, Email: zhouxuan6676@gmail.com
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License. 
 * You may obtain a copy of the License at * 
 * http://www.apache.org/licenses/LICENSE-2.0 * 
 * Unless required by applicable law or agreed to in writing, software 
 * distributed under the License is distributed on an "AS IS" BASIS, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 * See the License for the specific language governing permissions and 
 * limitations under the License. 
 */

#include "glacier_float.h"

#include <iostream>


template<int32_t FractionNumType>
class GFixedType32
{
public:

	explicit inline constexpr GFixedType32(int32_t raw)
		: rawInt32(raw)
	{

	}

	explicit inline constexpr GFixedType32(uint32_t a, uint32_t b, uint32_t c) :
		rawInt32(int32_t((a << FractionNumType) | ((((uint64_t)b) << FractionNumType) / c)))
	{

	}

	static inline constexpr int32_t GetTypeNumber()
	{
		return FractionNumType;
	}

	inline constexpr GFixedType32 operator +(GFixedType32 b) const
	{
		return GFixedType32(rawInt32 + b.rawInt32);
	}
	inline constexpr GFixedType32 operator +=(GFixedType32 b)
	{
		*this = *this + b;
		return *this;
	}


	inline constexpr GFixedType32 operator -() const
	{
		return GFixedType32(-rawInt32);
	}

	inline constexpr GFixedType32 operator -(GFixedType32 b) const
	{
		return GFixedType32(rawInt32 - b.rawInt32);
	}

	inline constexpr GFixedType32 operator *(GFixedType32 b) const
	{
		return GFixedType32(((int64_t)rawInt32 * (int64_t)b.rawInt32) >> FractionNumType);
	}

	inline constexpr GFixedType32 operator /(GFixedType32 b) const
	{
		return GFixedType32(((((int64_t)rawInt32)<<FractionNumType )/ (int64_t)b.rawInt32));
	}

	static inline constexpr GFixedType32 FromGFloat( const GFloat Value)
	{
		int32_t exp = Value.getexponent() - 127 + GFixedType32<FractionNumType>::GetTypeNumber();

		if (exp >= 0)
		{
			return GFixedType32(Value.getfraction() << exp);
		}
		else
		{
			return GFixedType32(Value.getfraction() >> -exp);
		}
	}

	inline constexpr GFloat ToGFloat() const
	{
		return GFloat::Nomalize((int64_t)rawInt32, uint8_t(127 - GFixedType32<FractionNumType>::GetTypeNumber()));
	}

	int32_t rawInt32;
};

typedef GFixedType32<8>  GFixed08;
typedef GFixedType32<16> GFixed16;
typedef GFixedType32<29> GFixed29;
typedef GFixedType32<30> GFixed30;



int32_t GFloat::ms_SinCosTable[GFloat::ms_TriCount * 2] = {
0, 1073741824, 26350944, 1073418368, 52686016, 1072448384, 78989344, 1070832384,
105245104, 1068571392, 131437456, 1065666816, 157550624, 1062120192, 183568928, 1057933824,
209476640, 1053110144, 235258144, 1047652096, 260897984, 1041563136, 286380672, 1034846592,
311690752, 1027506816, 336813184, 1019548032, 361732736, 1010975232, 386434368, 1001793408,
410903232, 992008064, 435124544, 981625216, 459083776, 970651136, 482766464, 959092224,
506158400, 946955648, 529245440, 934248704, 552013568, 920979072, 574449280, 907154560,
596539008, 892783616, 618269312, 877874944, 639627264, 862437504, 660599936, 846480512,
681174656, 830013568, 701339008, 813046784, 721080960, 795590144, 740388480, 777654400,
759250048, 759250048, 777654400, 740388480, 795590272, 721080832, 813046784, 701338880,
830013568, 681174528, 846480512, 660599808, 862437504, 639627264, 877874944, 618269312,
892783744, 596538880, 907154560, 574449280, 920979072, 552013568, 934248704, 529245376,
946955776, 506158272, 959092224, 482766464, 970651136, 459083776, 981625216, 435124480,
992008064, 410903168, 1001793408, 386434240, 1010975232, 361732672, 1019548160, 336813120,
1027506816, 311690752, 1034846592, 286380608, 1041563136, 260897920, 1047652096, 235258144,
1053110144, 209476544, 1057933824, 183568896, 1062120192, 157550656, 1065666816, 131437408,
1068571392, 105245088, 1070832384, 78989264, 1072448384, 52685984, 1073418368, 26350844,
1073741824, -47, 1073418368, -26350940, 1072448384, -52686088, 1070832384, -78989360,
1068571392, -105245200, 1065666688, -131437504, 1062120192, -157550752, 1057933824, -183568992,
1053110144, -209476640, 1047652096, -235258240, 1041563136, -260898016, 1034846592, -286380736,
1027506816, -311690880, 1019548032, -336813184, 1010975232, -361732800, 1001793408, -386434368,
992008064, -410903296, 981625216, -435124608, 970651136, -459083776, 959092224, -482766528,
946955648, -506158528, 934248832, -529245376, 920979072, -552013696, 907154560, -574449408,
892783616, -596539136, 877874944, -618269312, 862437504, -639627392, 846480384, -660600064,
830013696, -681174656, 813046784, -701339008, 795590144, -721081088, 777654272, -740388736,
759250048, -759250176, 740388480, -777654528, 721080832, -795590400, 701339008, -813046784,
681174528, -830013696, 660599808, -846480640, 639627008, -862437632, 618269312, -877875072,
596538880, -892783744, 574449152, -907154688, 552013568, -920979072, 529245312, -934248832,
506158272, -946955904, 482766528, -959092352, 459083712, -970651136, 435124416, -981625344,
410903040, -992008192, 386434368, -1001793408, 361732672, -1010975360, 336813056, -1019548160,
311690816, -1027506944, 286380608, -1034846720, 260897856, -1041563136, 235257984, -1047652224,
209476608, -1053110272, 183568832, -1057933824, 157550496, -1062120192, 131437488, -1065666816,
105245056, -1068571520, 78989216, -1070832512, 52685808, -1072448512, 26350924, -1073418496,
-94, -1073741824, -26351112, -1073418496, -52686000, -1072448512, -78989408, -1070832512,
-105245248, -1068571520, -131437680, -1065666816, -157550688, -1062120192, -183569056, -1057933824,
-209476832, -1053110144, -235258176, -1047652224, -260898048, -1041563136, -286380800, -1034846720,
-311691008, -1027506816, -336813248, -1019548160, -361732864, -1010975232, -386434560, -1001793408,
-410903232, -992008064, -435124672, -981625216, -459083968, -970651136, -482766720, -959092224,
-506158464, -946955776, -529245504, -934248704, -552013824, -920979072, -574449408, -907154688,
-596539136, -892783616, -618269568, -877874944, -639627264, -862437504, -660599936, -846480512,
-681174784, -830013568, -701339136, -813046656, -721080960, -795590272, -740388608, -777654400,
-759250304, -759250048, -777654400, -740388608, -795590272, -721080960, -813046784, -701339136,
-830013696, -681174656, -846480640, -660599936, -862437632, -639627136, -877875200, -618269184,
-892783872, -596538752, -907154816, -574449024, -920979072, -552013696, -934248832, -529245440,
-946955776, -506158336, -959092352, -482766400, -970651264, -459083648, -981625344, -435124288,
-992008320, -410902912, -1001793408, -386434432, -1010975232, -361732736, -1019548160, -336813184,
-1027506944, -311690688, -1034846720, -286380480, -1041563264, -260897696, -1047652352, -235257824,
-1053110144, -209476704, -1057933824, -183568928, -1062120192, -157550592, -1065666816, -131437312,
-1068571520, -105244880, -1070832512, -78989056, -1072448512, -52685640, -1073418496, -26351008,
-1073741824, 12, -1073418496, 26351028, -1072448512, 52686176, -1070832512, 78989584,
-1068571520, 105245408, -1065666816, 131437840, -1062120192, 157550592, -1057933824, 183568960,
-1053110144, 209476736, -1047652224, 235258336, -1041563136, 260898208, -1034846592, 286380928,
-1027506816, 311691136, -1019548160, 336813120, -1010975232, 361732736, -1001793408, 386434432,
-992008064, 410903360, -981625216, 435124800, -970651008, 459084096, -959092096, 482766848,
-946955776, 506158336, -934248832, 529245440, -920979072, 552013696, -907154560, 574449408,
-892783616, 596539136, -877874816, 618269568, -862437632, 639627136, -846480512, 660599808,
-830013696, 681174656, -813046784, 701339008, -795590144, 721081088, -777654272, 740388736,
-759249920, 759250304, -740388608, 777654272, -721080960, 795590144, -701339008, 813046784,
-681174528, 830013696, -660599808, 846480640, -639627008, 862437632, -618269056, 877875200,
-596539136, 892783616, -574449408, 907154560, -552013568, 920979072, -529245312, 934248832,
-506158208, 946955776, -482766208, 959092352, -459083456, 970651264, -435124608, 981625216,
-410903232, 992008064, -386434304, 1001793408, -361732608, 1010975232, -336812992, 1019548160,
-311690496, 1027506944, -286380288, 1034846720, -260898048, 1041563136, -235258144, 1047652096,
-209476544, 1053110144, -183568768, 1057933824, -157550400, 1062120192, -131437136, 1065666816,
-105244704, 1068571520, -78989392, 1070832384, -52685976, 1072448384, -26350832, 1073418368
};
GFloat GFloat::ms_TanTable[GFloat::ms_TriCount];

void GFloat::Init()
{
	for( int32_t i = 0; i <GFloat::ms_TriCount; ++i ) // for test
	{
		float fvalue = float(i) * 3.141592653f * 2 / GFloat::ms_TriCount;

		float fSin = sinf(fvalue);  // to do use Gfloat compute Sin
		float fCos = cosf(fvalue);
		float fTan = tanf(fvalue);
	
		GFloat GSin = GFloat::FromFloat(fSin);
		GFloat GCos = GFloat::FromFloat(fCos);

		GFixed30 SinValue = GFixed30::FromGFloat( GSin );
		GFixed30 CosValue = GFixed30::FromGFloat(GCos);

	//	std::cout << SinValue.rawInt32 << ", " << CosValue.rawInt32<< ", " ;

	//	if ((i + 1) % 4 == 0)
		//	std::cout << std::endl;

		//ms_SinCosTable[i*2] = SinValue.rawInt32;
		//ms_SinCosTable[i*2+1] = CosValue.rawInt32;

		ms_TanTable[i] = FromFloat( fTan ) ;
	}
}

static inline int32_t Sin_Table(const GFloat value, GFixed30& Delta)
{
	int32_t exp = value.getexponent() - 127;

	if (-64 >= exp || exp >= 64)
	{
		Delta = GFixed30(0,0,1);
		return 0;
	}

	int64_t TRaw = value.getfraction() * (int64_t)GFixed30(0, 159154943, 1000000000).rawInt32;

	int64_t NewRaw = exp >= 0 ? TRaw << exp : TRaw >> -exp;

	GFixed30   F30Fraction = GFixed30((NewRaw) & 0x3FFFFFFF);

	int32_t TWhole = F30Fraction.rawInt32 >> (30 - GFloat::ms_TriTableBit);

	GFixed30 ttTest = GFixed30(TWhole << (30 - GFloat::ms_TriTableBit));

	constexpr GFixed30 C_quaterPi(0, 785398164, 1000000000);

	GFixed30 delta_1 =  GFixed30(F30Fraction.rawInt32 & ((1 << (30 - GFloat::ms_TriTableBit)) - 1));

	//GFixed30 F30Fraction_Test = ttTest + delta_1;

	Delta = C_quaterPi * delta_1;
	Delta.rawInt32 = Delta.rawInt32 << 3;

	int32_t nWhole = (TWhole) & (GFloat::ms_TriCount - 1);

	return nWhole;
}

GFloat GFloat::Sin(const GFloat value) 
{
	GFixed30 F30Delte(0);

	int32_t nWhole = Sin_Table(value, F30Delte );

	GFixed30 TableSin = GFixed30(ms_SinCosTable[nWhole * 2]);
	GFixed30 TableCos = GFixed30(ms_SinCosTable[nWhole * 2 + 1]);

	GFixed30 FixedResult = TableSin + F30Delte * ( TableCos - TableSin * GFixed30(F30Delte.rawInt32 >> 1));

	return FixedResult.ToGFloat();
}

GFloat GFloat::Cos(const GFloat value)
{ 
	GFixed30 F30Delte(0);

	int32_t nWhole = Sin_Table(value, F30Delte);

	GFixed30 TableSin = GFixed30(ms_SinCosTable[nWhole * 2]);
	GFixed30 TableCos = GFixed30(ms_SinCosTable[nWhole * 2 + 1]);

	GFixed30 FixedResult = TableCos - F30Delte * (TableSin + TableCos * GFixed30(F30Delte.rawInt32 >> 1));
	return FixedResult.ToGFloat();
}
void GFloat::SinCos(const GFloat value, GFloat& OutSin, GFloat& OutCos)
{
	GFixed30 F30Delte(0);

	int32_t nWhole = Sin_Table(value, F30Delte);
	GFixed30 TableSin = GFixed30(ms_SinCosTable[nWhole * 2]);
	GFixed30 TableCos = GFixed30(ms_SinCosTable[nWhole * 2 + 1]);

    OutSin = (TableSin + F30Delte * ( TableCos - TableSin * GFixed30(F30Delte.rawInt32 >> 1))).ToGFloat();
    OutCos = (TableCos - F30Delte * ( TableSin + TableCos * GFixed30(F30Delte.rawInt32 >> 1))).ToGFloat();
}
GFloat GFloat::ASin(const GFloat value)
{
	constexpr GFloat TOne =  GFloat::FromRaw32( One().rawint32 - 0x1000  );
    if( value > TOne) 
	{
		return Pi_Half();
	}
	else if (value <-TOne )
    {
        return -Pi_Half();
    }
    else
    {
		GFixed30 FValue =  GFixed30::FromGFloat(value);

		int32_t nLow = -ms_TriCount >> 2;
		int32_t nHigh = ms_TriCount >> 2;

		int32_t nMiddle = 0;

		while ( (nLow != nHigh) && (nLow < (nHigh-1)))
		{
			nMiddle = (nLow + nHigh) >>1;

			if( ms_SinCosTable[(nMiddle&(ms_TriCount-1))*2] <= FValue.rawInt32 )
			{
				nLow = nMiddle;
			}
			else
			{
				nHigh = nMiddle;
			}
		}
		GFixed30 F30Pi_half(1,570796327,1000000000 );

		GFixed30 TSin( ms_SinCosTable[(nMiddle & (ms_TriCount - 1)) * 2]);
		GFixed30 TCos( ms_SinCosTable[(nMiddle & (ms_TriCount - 1)) * 2+1]);


		int64_t Traw = F30Pi_half.rawInt32 * (int64_t)nLow;

		GFixed30 AlignResult = GFixed30( (int32_t)(Traw >> (ms_TriTableBit - 2) ));

		GFixed30 Delta = FValue - AlignResult;

		GFixed30 TDelta = (TCos * Delta) / TSin;
		GFixed30 TResult = AlignResult;
		//if( nLow > 48)
		//	TResult = AlignResult + (TSin * Delta) / TCos;

		if(nLow < 0)
		{
		//	TResult = AlignResult + (TSin * Delta) / TCos;
		}

		return TResult.ToGFloat();
    }
}
GFloat GFloat::ACos(const GFloat value)
{
	return Pi_Half() - ASin(value);
}
GFloat GFloat::Tan(const GFloat value) 
{
	GFloat TSin;
	GFloat TCos; 
    SinCos(value, TSin, TCos);
    return TSin / TCos;
}
GFloat GFloat::ATan(const GFloat value)
{
	if( -One() <= value && value <= One() )
	{
		GFixed29 x1 =  GFixed29::FromGFloat(value);
		GFixed29 x2 = x1 * x1;

		return( x1 *(GFixed29(1,0,2) - x2 *( GFixed29(0,1,3) - x2 * GFixed29(0,1,5) ) )).ToGFloat();
	}
	else if(value > One() )
	{
		return Pi_Half() - ATan(One() / value);
	}
	else
	{
		return  - ATan(One() / value) - Pi_Half();
	}

}
GFloat GFloat::ATan2(const GFloat y, const GFloat x)
{
	if( x > Zero() )
	{
		GFloat value = y / x;
		return ATan(value);
	}
	else if( x < Zero() )
	{
		if( y > Zero() )
		{
			return Pi() + ATan(y / x);
		}
		else
		{
			return ATan(y / x) - Pi();
		}
	}
	else
	{
		return Zero();
	} 
}  


GFloat GFloat::Exp(const GFloat value)
{ 
	if(value >= Zero())
	{
		GFloat fraction;
		int32_t nWhole =  value.GetWhole(fraction);

		GFloat Result = One();
		for( int i = 0; i < (nWhole); ++i )
		{
			Result = Result * e();
		}

		//GFloat fraction = value;

		//GFixed29 x1 =      GFixed29::FromGFloat(     fraction   ) ;
		GFixed29 x1 = GFixed29::FromGFloat(fraction);


		return Result * ( GFixed29(1,0,2) + x1 *( GFixed29(1,0,2) +  x1 *( GFixed29(0,1,2) + x1 * ( GFixed29(0,1,6) + x1 * ( GFixed29(0,1,24) + x1 * GFixed29(0,1,120)) ) ))).ToGFloat();
	}
	else
	{
		return One() / Exp(-value);
	}
}
GFloat GFloat::Log(const GFloat value)
{ 
	if( value <= Zero() )
	{
		return Zero();
	}
	else if (value < Two())
	{
		GFixed29 x1 = GFixed29::FromGFloat(value - One());

		return ( x1 * ( GFixed29(1,0, 2) + x1 * ( -GFixed29(0,1, 2) + x1 *( GFixed29(0,1, 3) - x1 *( GFixed29(0,1, 4) - x1 * GFixed29(0,1, 5)))))).ToGFloat();
	}
	else
	{
		return One() + Log(value * e_Inv());
	}
}
GFloat GFloat::Pow(const GFloat x, const GFloat y)
{
	if (x <= Zero())
		return Zero();

    return Exp(y * Log(x));
}



template<int64_t FractionNumType>
class GFixedType64
{
public:

	explicit inline constexpr GFixedType64(int64_t raw)
		: rawInt64(raw)
	{

	}

	explicit inline constexpr GFixedType64(uint32_t a, uint32_t b, uint32_t c) :
		rawInt64(int64_t((a << FractionNumType) | ((((uint64_t)b) << FractionNumType) / c)))
	{

	}

	static inline constexpr int64_t GetTypeNumber()
	{
		return FractionNumType;
	}

	inline constexpr GFixedType64 operator +(GFixedType64 b) const
	{
		return GFixedType64(rawInt64 + b.rawInt64);
	}
	inline constexpr GFixedType64 operator +=(GFixedType64 b)
	{
		*this = *this + b;
		return *this;
	}


	inline constexpr GFixedType64 operator -() const
	{
		return GFixedType64(-rawInt64);
	}

	inline constexpr GFixedType64 operator -(GFixedType64 b) const
	{
		return GFixedType64(rawInt64 - b.rawInt64);
	}

	inline constexpr GFixedType64 operator *(GFixedType64 b) const
	{
		return GFixedType64(((int64_t)rawInt64 * (int64_t)b.rawInt64) >> FractionNumType);
	}

	static inline constexpr GFixedType64 FromGFloat(const GFloat Value)
	{
		int32_t exp = Value.getexponent() - 127 + GFixedType64<FractionNumType>::GetTypeNumber();

		if (exp >= 0)
		{
			return GFixedType64( ((int64_t)Value.getfraction()) << exp);
		}
		else
		{
			return GFixedType64(( (int64_t)Value.getfraction() )>> -exp);
		}
	}

	inline constexpr GFloat ToGFloat() const
	{
		return GFloat::Nomalize((int64_t)rawInt64, uint8_t(127 - GFixedType64<FractionNumType>::GetTypeNumber()));
	}

	int64_t rawInt64;
};

GFloat GFloat::InvSqrt(const GFloat value )
{
    if (value.rawint32 <= 0)
        return Zero();

	GFixed30 Fixed30(value.getfraction_NoShift() );

	int32_t exp = value.getexponent() - 127 + 22;
	GFixed30  Start(0);
	if(exp & 0x1)
	{
		Fixed30.rawInt32 >>= 1;
		exp += 1;
		Start =  GFixed30(1, 171, 1000);
	}
	else
	{
		
		Start = GFixed30(0, 822, 1000); //GFixed30::FromGFloat(start);
	}

	Fixed30.rawInt32 >>= 1;

	constexpr GFixed30 F1_5(1, 1, 2);

	Start = Start * (F1_5 - (Fixed30 * Start) * Start);
	Start = Start * (F1_5 - (Fixed30 * Start) * Start);
	Start = Start * (F1_5 - (Fixed30 * Start) * Start);

	GFloat TResult = GFloat::Nomalize(Start.rawInt32, uint8_t(127 - GFixed30::GetTypeNumber() - (exp >> 1)));

	return TResult;
}




