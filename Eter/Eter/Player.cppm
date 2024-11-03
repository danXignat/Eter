export module Player;
import <string_view>;
import CardDemo;
import<string>;
import<set>;
export class Player {
public:
	Player() =default;
	
	std::string_view getName() const;
	const std::multiset<Card>& getCards()const;

	void addCard(const Card& card);
	void printCard() const;
	bool hasCard() const;
	bool eraseCard(const Card& card);



private:
	std::string_view m_name;
	std::multiset<Card>m_cards;

};