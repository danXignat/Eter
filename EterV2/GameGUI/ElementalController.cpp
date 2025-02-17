#include "ElementalController.h"

ElementalController::ElementalController(QObject* parent, base::ElementalService& service, GameView* view) : QObject{ parent },
m_service{ service },
m_view{ view } {

   /* base::MageTypeAbility red_ability{ m_service.getMageAbility(color::ColorType::RED) };
    base::MageTypeAbility blue_ability{ m_service.getMageAbility(color::ColorType::BLUE) };

    QString red_description{ QString(base::mapMageTypeAbilities.at(red_ability).data()) };
    QString blue_description{ QString(base::mapMageTypeAbilities.at(blue_ability).data()) };

    view->drawMages(
        red_ability, red_description,
        blue_ability, blue_description
    );*/

    //connect(view, &GameView::applyMageCard, this, &ElementalController::onMageCardApply);
}

//void ElementalController::onMageCardApply(MageCard* card) {
//    card->setUsed(true);
//
//    selectMage(card);
//}
//
//void ElementalController::selectMage(MageCard* card) {
//    std::vector<std::function<void(color::ColorType)>> choices{
//        [this](color::ColorType color) { this->handleFireFront(color); },
//        [this](color::ColorType color) { this->handleFireBack(color); },
//        [this](color::ColorType color) { this->handleEarthFront(color); },
//        [this](color::ColorType color) { this->handleEarthBack(color); },
//        [this](color::ColorType color) { this->handleAirFront(color); },
//        [this](color::ColorType color) { this->handleAirBack(color); },
//        [this](color::ColorType color) { this->handleWaterFront(color); },
//        [this](color::ColorType color) { this->handleWaterBack(color); }
//    };
//
//    int index{ static_cast<int>(card->getTypeAbility()) };
//    choices[index](card->getColor());
//}