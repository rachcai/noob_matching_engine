#include "order.h"
#include <vector>

namespace order {

std::vector<OrderEvent> OrderBook::addOrder(const Order& order) {
    OrderEvent event;
    event.type = OrderEvent::Type::Added;
    event.orderId = order.id;
    // TODO: update once matching logic is implemented
    event.filledQuantity = 0;
    event.remainingQuantity = order.quantity;

    // Update order book
    PriceLevel& level = (order.side == Side::Bid) ? bids[order.price] : asks[order.price];
    level.orders.push_back(order);
    level.totalQuantity += order.quantity;

    orderIndex[order.id] = OrderLocation{order.side, order.price, std::prev(level.orders.end())};
    return {event};
}

std::vector<OrderEvent> OrderBook::removeOrder(uint64_t orderId){
    OrderEvent event;
    event.type = OrderEvent::Type::Cancelled;
    event.orderId = orderId;
    // TODO: update once matching logic is implemented
    event.filledQuantity = 0;
    event.remainingQuantity = 0;

    // Check if order exists
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end()) {
        event.type = OrderEvent::Type::Rejected;
        return {event};
    }
    OrderLocation location = it->second;
    PriceLevel& level = (location.side == Side::Bid) ? bids[location.price] : asks[location.price];
    level.totalQuantity -= location.iterator->quantity;
    level.orders.erase(location.iterator);
    if (level.orders.empty()) {
        if (location.side == Side::Bid) {
            bids.erase(location.price);
        } else {
            asks.erase(location.price);
        }
    }
    orderIndex.erase(it);
    return {event};
}

std::vector<OrderEvent> OrderBook::modifyOrder(uint64_t orderId, uint64_t newQuantity, double newPrice) {
    // Check if order exists
    auto it = orderIndex.find(orderId);
    if (it == orderIndex.end()) {
        return {OrderEvent{OrderEvent::Type::Rejected, orderId, 0, 0}};
    }
    OrderLocation location = it->second;

    Order newOrder = *(it->second.iterator);
    newOrder.price = newPrice;
    newOrder.quantity = newQuantity;
    newOrder.remainingQuantity = newQuantity;

    auto events = removeOrder(orderId);
    auto addEvents = addOrder(newOrder);
    events.insert(events.end(), addEvents.begin(), addEvents.end());
    return events;
}

}