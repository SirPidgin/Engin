#pragma once

typedef unsigned long long int ullongint;


namespace Engin
{
	namespace Core
	{
		class RNG
		{
		public:
			RNG();
			RNG(ullongint initSeed);

			void setSeed(ullongint newSeed);
			ullongint getSeed();
			void resetSeed();

			//Pseudorandom Number Generation = Same seed always returns the same "random" numbers in the same order
			//(?)
			template<typename type> type getRandomNumberFromSeed();
			template<typename type> type getRandomNumberFromSeed(type min, type max);

			//Random Pseudorandom Number Generation
				//Returns only relatively small values (RAND_MAX)
			int getRandomSInt();
			int getRandomUInt();
			int getRandomInt(int min, int max);
				//Floats are very inaccurate with large min and/or max values
			float getRandomFloat(float min, float max);	//absolute values of min and max should not be larger than RAND_MAX/2 to be accurate

		private:
			void initialize();
			ullongint seed;
		};


		//Template Methods
		
		template<typename type>
		type RNG::getRandomNumberFromSeed()
		{
			//Mersenne Twister Engine
			//mt19937
			//Xorshift?
		}


		template<typename type>
		type RNG::getRandomNumberFromSeed(type min, type max)
		{

		}
	}
}