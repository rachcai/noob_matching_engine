#include <vector>
#include <optional>
#include "order.h"

namespace engine {

class MatchingEngine {
public:
    order::OrderBook orderBook;
    // Core interface — everything goes through here
    std::vector<order::OrderEvent> submitOrder(const order::Order& order);
    std::vector<order::OrderEvent> cancelOrder(uint64_t orderId);
    std::vector<order::OrderEvent> modifyOrder(uint64_t orderId, uint64_t newQuantity, double newPrice);
};

} // namespace engine