#ifndef RESERVATION_HPP_INCLUDE
#define RESERVATION_HPP_INCLUDE

#include <json.hpp>
#include <string>

#include "datetime.hpp"

class Reservation {
public:
    Reservation(int userId, int numOfBeds, date::year_month_day checkIn, date::year_month_day checkOut);

    void modify(int numOfBeds);

    int getUserId() const;
    int getNumOfBeds() const;
    date::year_month_day getCheckOut() const;

    bool hasConflict(date::year_month_day date) const;
    bool isExpired(date::year_month_day date) const;
    bool canBeCancelled(date::year_month_day date) const;

    bool operator==(const Reservation& other) const;
    bool operator!=(const Reservation& other) const;

    nlohmann::json toJson() const;

private:
    int userId_;
    int numOfBeds_;
    date::year_month_day checkIn_, checkOut_;
};

#endif // RESERVATION_HPP_INCLUDE
