#pragma once

#include <random>


namespace Engin
{
	namespace Core
	{
		class RNG
		{
		public:
			RNG();
			RNG(unsigned int initSeed);
			
			//Pseudorandom Number Generation = Same seed always returns the same "random" numbers in the same order			
			void setSeed(unsigned int newSeed);
			unsigned int getSeed();
			void resetRandomSeedEngine();
			unsigned int getRandomNumberFromSeed();
			void discardNext(int amount = 1);
			unsigned int getMax();
			unsigned int getMin();

			//Random Pseudorandom Number Generation
			//Returns only relatively small values (RAND_MAX)
			int getRandomSInt();
			int getRandomUInt();
			int getRandomInt(int min, int max);
			float getRandomFloat(float min, float max); //6 decimal accuracy

		private:
			void initialize();
			float getRandomDecimals();
			unsigned int seed;
			std::mt19937 MTEngine;
		};
	}
}