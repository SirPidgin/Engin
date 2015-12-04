#include <stdlib.h>
#include <time.h>

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
			if (min == 0 && max == 0)
				return 0;
			else if (min < 0)
				return (rand() % (abs(min) + (max + 1))) - abs(min);
			else
				return rand() % (max - min) + min;
		}

		float RNG::getRandomFloat(float min, float max)
		{
			static int value;
			value = getRandomInt((int) min, (int) max);
			if (value >= max)
			{
				value = max - 1;
			}
			else if (value <= min)
			{
				value = min + 1;
			}
			return value + getRandomDecimals();
		}

		float RNG::getRandomDecimals()
		{
			static int value;
			value = getRandomInt(-10000, 10000);
			return 0.0001 * value;
		}
	}
}