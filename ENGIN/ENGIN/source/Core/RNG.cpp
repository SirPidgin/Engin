//#include <stdlib.h>
//#include <time.h>
//
//#include "Engin\Core\RNG.h"
//
//namespace Engin
//{
//	namespace Core
//	{
//		RNG::RNG() : seedMod(1)
//		{
//			initialize();
//			seed = getRandomUInt();
//		}
//		RNG::RNG(ullongint initSeed) : seed(initSeed), seedMod(1)
//		{
//			initialize();
//		}
//
//
//		void RNG::setSeed(ullongint newSeed)
//		{
//			seed = newSeed;
//		}
//
//
//		ullongint RNG::getSeed()
//		{
//			return seed;
//		}
//
//
//		void RNG::initialize()
//		{
//			srand(time(NULL));
//		}
//
//
//		int RNG::getRandomSInt()
//		{
//			return (rand() % (RAND_MAX / 2 + RAND_MAX / 2)) - RAND_MAX / 2;
//		}
//
//
//		int RNG::getRandomUInt()
//		{
//			return rand();
//		}
//
//
//		int RNG::getRandomInt(int min, int max)
//		{
//			if (min < 0)
//				return (rand() % (abs(min) + max)) - abs(min);
//			else
//				return rand() % (max - min) + min;
//		}
//
//
//		float RNG::getRandomFloat(float min, float max)
//		{
//			return ((max - min)*(static_cast<float>(rand()) / static_cast<float>(RAND_MAX))) - min;
//		}
//	}
//}