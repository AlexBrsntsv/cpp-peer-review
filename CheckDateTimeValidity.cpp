
int CurrMonthDayMax(const DateTime& dt) {
    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = { 31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return month_lengths[dt.month - 1];
}

template<typename T>
void CheckValidity(std::string_view name, T value, T min, T max) {
    if (value < min) throw domain_error(name + " is too small"s);
    if (max < value) throw domain_error(name + " is too big"s);
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckValidity("year"sv, dt.year, 1, 9999);
    CheckValidity("month"sv, dt.month, 1, 12);
    CheckValidity("day"sv, dt.day, 1, CurrMonthDayMax(dt));
    CheckValidity("hour"sv, dt.hour, 0, 23);
    CheckValidity("minute"sv, dt.minute, 0, 59);
    CheckValidity("second"sv, dt.seconds, 0, 59);
}