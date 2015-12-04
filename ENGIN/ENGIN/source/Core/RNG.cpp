#include <stdlib.h>
#include <time.h>

#include <iostream> //FOR ERRORCHECKING!

#include "Engin\Core\RNG.h"

namespace Engin
{
	namespace Core
	{
		RNG::RNG()
		{
			seed = getRandomUInt();
			initialize();
		}
		RNG::RNG(unsigned int initSeed) : seed(initSeed)
		{
			seed = initSeed;
			initialize();
		}
		void RNG::initialize()
		{
			srand(time(NULL));
			MTEngine = std::mt19937(seed);
		}

		//Seed-Based Pseudo RNG
		void RNG::setSeed(unsigned int newSeed)
		{
			seed = newSeed;
			MTEngine.seed(seed);
		}

		unsigned int RNG::getSeed()
		{
			return seed;
		}

		unsigned int RNG::getRandomNumberFromSeed()
		{
			return MTEngine();
		}

		void RNG::discardNext(int amount)
		{
			MTEngine.discard(amount);
		}

		unsigned int RNG::getMax()
		{
			return MTEngine.max();
		}

		unsigned int RNG::getMin()
		{
			return MTEngine.min();
		}

		void RNG::resetRandomSeedEngine()
		{
			MTEngine.seed(seed);
		}


		//Time-based RNG
		int RNG::getRandomSInt()
		{
			return (rand() % (RAND_MAX / 2 + RAND_MAX / 2)) - RAND_MAX / 2;
		}

		int RNG::getRandomUInt()
		{
			return rand();
		}

		int RNG::getRandomInt(int min, int max)
		{
			if (min > max)
			{
				std::cout << "MAX value is smaller than MIN!" << std::endl;
				return NULL;
			}
			if (min == max)
				return min;
			else if (min < 0)
				return (rand() % (abs(min) + (max + 1))) - abs(min);
			else
				return rand() % (max - min) + min;
		}

		float RNG::getRandomFloat(float min, float max)
		{
			if (min > max)
			{
				std::cout << "MAX value is smaller than MIN!" << std::endl;
				return NULL;
			}
			float random = ((float) getRandomUInt() / (float) RAND_MAX);
			float range = max - min;
			return (random*range) + min;
		}
	}
}