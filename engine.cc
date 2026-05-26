#include "engine.h"
#include "order.h"
#include <vector>

namespace engine {

std::vector<order::OrderEvent> MatchingEngine::submitOrder(const order::Order& order) {
    std::vector<order::OrderEvent> events = orderBook.addOrder(order);
    return events;
}

std::vector<order::OrderEvent> MatchingEngine::cancelOrder(uint64_t orderId) {
    return orderBook.removeOrder(orderId);
}

std::vector<order::OrderEvent> MatchingEngine::modifyOrder(uint64_t orderId, uint64_t newQuantity, double newPrice) {
    return orderBook.modifyOrder(orderId, newQuantity, newPrice);
}

} // namespace engine