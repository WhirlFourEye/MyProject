#include "OrderBook.h"
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>


OrderBook::OrderBook()
{
}


OrderBook::~OrderBook()
{
}

bool OrderBook::addNewOrder(Order now)// add a new order to order_list
{
	if (now.side == '1') {
		auto it = upper_bound(buy_order.begin(), buy_order.end(), now);
		buy_order.insert(it, now);
	}
	if (now.side == '2') {
		auto it = upper_bound(sell_order.begin(), sell_order.end(), now);
		sell_order.insert(it, now);
	}
	return true;
}

Order OrderBook::delOrder(string id)
{
	for (int i = 0; i < buy_order.size(); i++)
		// find order
		if (buy_order[i].id == id) {
			buy_order[i].status = 4;
			Order ans = buy_order[i];
			buy_order.erase(buy_order.begin() + i);
			return ans;
		}
	for (int i = 0; i < sell_order.size(); i++)
		// find order
		if (sell_order[i].id == id) {
			sell_order[i].status = 4;
			Order ans = sell_order[i];
			sell_order.erase(sell_order.begin() + i);
			return ans;
		}
	return Order(0.0, '9', 0, id, -2);
}

void OrderBook::update(Order now)
{
	for (int i = 0; i < buy_order.size(); i++)
		// find order
		if (buy_order[i].id == now.id) {
			buy_order[i] = now;
		}
	for (int i = 0; i < sell_order.size(); i++)
		// find order
		if (sell_order[i].id == now.id) {
			sell_order[i] = now;
		}
}

pair<Order, Order> OrderBook::fill()// this->first == buy, this->second == sell
{
	pair<Order, Order> ans;
	for (int j = 0; j < sell_order.size(); j++) {
		for (int i = buy_order.size() - 1; i >= 0; i--) {
			if (sell_order[j].price <= buy_order[i].price) {
				// fill order
				int match = min(sell_order[j].quantity, buy_order[i].quantity);
				sell_order[j].quantity -= match;
				buy_order[i].quantity -= match;
				// update status
				sell_order[j].quantity ? sell_order[j].status = 1 : sell_order[j].status = 2;
				buy_order[i].quantity ? buy_order[i].status = 1 : buy_order[i].status = 2;
				// get return
				ans = make_pair(buy_order[i], sell_order[j]);
				// update order list
				if (!sell_order[j].quantity) {
					sell_order.erase(sell_order.begin() + j);
				}
				if (!buy_order[i].quantity) {
					buy_order.erase(buy_order.begin() + i);
				}
				return ans;
			}
		}
	}
	return ans;
}

int OrderBook::getBuySize() const
{
	return buy_order.size();
}

int OrderBook::getSellSize() const
{
	return sell_order.size();
}

string OrderBook::toString() const
{
	stringstream ss;
	for (auto x : buy_order) {
		ss << x << endl;
	}
	for (auto x : sell_order) {
		ss << x << endl;
	}
	return ss.str();
}

string OrderBook::show() const
{
	stringstream ss;
	ss << "BUY ORDERS\t\tSELL ORDERS" << endl
		<< "SHARES\tPRICE\t\tSHARES\tPRICE" << endl;
	if (sell_order.size() >= buy_order.size())
		for (int i = 0; i < min(5, (int)sell_order.size()); i++) {
			int x = buy_order.size() - 1 - i;
			if (x >= 0) ss << buy_order[x].toString();
			ss << "\t\t" << sell_order[i].toString();
			ss << endl;
		}
	else
		for (int i = 0; i < min(5, (int)buy_order.size()); i++) {
			int x = buy_order.size() - 1 - i;
			if (x >= 0) ss << buy_order[x].toString();
			if (i < sell_order.size()) ss << "\t\t" << sell_order[i].toString();
			ss << endl;
		}
	if ((int)buy_order.size() - min(5, (int)buy_order.size()) > 0) ss << "("
		<< (int)buy_order.size() - min(5, (int)buy_order.size())
		<< " more)\t";
	if ((int)sell_order.size() - min(5, (int)buy_order.size()) > 0) ss << "("
		<< (int)sell_order.size() - min(5, (int)buy_order.size())
		<< " more)";
	ss << endl;
	return ss.str();
}

ostream & operator << (ostream & os, const Order & order) {
	return os << "price: " << order.price << endl
		<< "side: " << order.side << endl
		<< "status: " << order.status << endl
		<< "quantity: " << order.quantity << endl
		<< "id: " << order.id << endl;
}

ostream & operator<< (ostream & os, const OrderBook & book)
{
	return os << book.toString();
}

string Order::toString() const
{
	stringstream ss;
	ss << quantity << "\t" << price;
	return ss.str();
}
