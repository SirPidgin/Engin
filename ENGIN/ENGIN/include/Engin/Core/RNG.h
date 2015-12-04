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
			unsigned int getRandomNumberFromSeed(); //returns a large unsigned int from seed
			void discardNext(int amount = 1);
			unsigned int getMax();
			unsigned int getMin();

			//Random Pseudorandom Number Generation
			//Returns only relatively small values (RAND_MAX)
			int getRandomSInt();
			int getRandomUInt();
			int getRandomInt(int min, int max);
			float getRandomFloat(float min, float max);

		private:
			void initialize();
			unsigned int seed;
			std::mt19937 MTEngine;
		};
	}
}