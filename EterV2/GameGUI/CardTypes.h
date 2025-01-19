#pragma once
#include <stdint.h>

enum class CardType : uint16_t {
	bCombat, 
	bExplosion,
	bHole,

	mBurn,
	mBurnRowOrColumn,
	mBury,
	mHole,
	mBlowAway,
	mBlowEter,
	mBoat,
	mBoatRowOrColumn,

	pControllerExplosion,
	pDestruction,
	pFlame,
	pFire,
	pAsh,
	pSpark,
	pSquall,
	pGale,
	pHurricane,
	pGust,
	pMirrage,
	pStorm,
	pTide,
	pMist,
	pWave,
	pWhirlpool,
	pBlizzard,
	pWaterfall,
	pSupport,
	pEarthquake,
	pCrumble,
	pBorder,
	pAvalanche,
	pRock
};


inline std::string getCardPath(CardType type) {
	switch (type)
	{
	case CardType::bCombat:
		break;
	case CardType::bExplosion:
		break;
	case CardType::bHole:
		break;
	case CardType::mBurn:
		break;
	case CardType::mBurnRowOrColumn:
		break;
	case CardType::mBury:
		break;
	case CardType::mHole:
		break;
	case CardType::mBlowAway:
		break;
	case CardType::mBlowEter:
		break;
	case CardType::mBoat:
		break;
	case CardType::mBoatRowOrColumn:
		break;
	case CardType::pControllerExplosion:
		break;
	case CardType::pDestruction:
		break;
	case CardType::pFlame:
		break;
	case CardType::pFire:
		break;
	case CardType::pAsh:
		break;
	case CardType::pSpark:
		break;
	case CardType::pSquall:
		break;
	case CardType::pGale:
		break;
	case CardType::pHurricane:
		break;
	case CardType::pGust:
		break;
	case CardType::pMirrage:
		break;
	case CardType::pStorm:
		break;
	case CardType::pTide:
		break;
	case CardType::pMist:
		break;
	case CardType::pWave:
		break;
	case CardType::pWhirlpool:
		break;
	case CardType::pBlizzard:
		break;
	case CardType::pWaterfall:
		break;
	case CardType::pSupport:
		break;
	case CardType::pEarthquake:
		break;
	case CardType::pCrumble:
		break;
	case CardType::pBorder:
		break;
	case CardType::pAvalanche:
		break;
	case CardType::pRock:
		break;
	default:
		break;
	}
}