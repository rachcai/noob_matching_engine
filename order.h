#include <cstdint>
#include <list>
#include <map>
#include <unordered_map>
#include <variant>
#include <vector>

namespace order {

enum class Side { Bid, Ask };
enum class OrderType { Limit, Market, IOC };

struct Order {
    uint64_t id;
    Side side;
    OrderType type;
    double price;      // use int in production (price * 100 to avoid float issues)
    uint64_t quantity;
    uint64_t remainingQuantity;
    uint64_t timestamp;  // or sequence number
};

struct PriceLevel {
    double price;
    uint64_t totalQuantity;
    std::list<Order> orders; // doubly linked list
};

struct OrderLocation {
    Side side;
    double price;
    std::list<Order>::iterator iterator; // direct pointer into the list
};

struct OrderEvent {
    enum class Type { 
        Added, 
        Filled, 
        PartiallyFilled, 
        Cancelled, 
        Rejected 
    };
    Type type;
    uint64_t orderId;
    uint64_t filledQuantity;
    uint64_t remainingQuantity;
};

class OrderBook {
public:
    std::vector<OrderEvent> addOrder(const Order& order);
    std::vector<OrderEvent> removeOrder(uint64_t orderId);
    std::vector<OrderEvent> modifyOrder(uint64_t orderId, uint64_t newQuantity, double newPrice);

private:
    // Bids: highest price first
    std::map<double, PriceLevel, std::greater<double>> bids;
    
    // Asks: lowest price first  
    std::map<double, PriceLevel, std::less<double>> asks;
    
    // For O(1) order lookup by ID (needed for cancel/modify)
    std::unordered_map<uint64_t, OrderLocation> orderIndex;
};

struct TradeEvent {
    uint64_t incomingOrderId;
    uint64_t restingOrderId;
    double price;
    uint64_t quantity;
};

} // namespace order