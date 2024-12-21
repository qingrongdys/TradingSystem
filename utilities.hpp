/**
* utilities.hpp
* Utility functions
*/
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <string>
#include <chrono>
#include "products.hpp"

#include <random> // For modern random number generation

// Generate uniformly distributed random variables between 0 to 1.
std::vector<double> GenerateUniform(long count, long seed = 0)
{
    static constexpr long modulus = 2147483647;
    static constexpr long multiplier = 39373;
    static constexpr long quotient = modulus / multiplier;
    static constexpr long remainder = modulus % multiplier;

    if (seed == 0) seed = std::chrono::system_clock::now().time_since_epoch().count() % modulus;
    std::vector<double> randomValues;
    for (long i = 0; i < count; ++i)
    {
        long k = seed / quotient;
        seed = multiplier * (seed - k * quotient) - k * remainder;
        if (seed < 0) seed += modulus;
        randomValues.push_back(static_cast<double>(seed) / modulus);
    }
    return randomValues;
}

// Get Bond object for US Treasury bonds based on CUSIP.
Bond GetBond(const std::string& cusip)
{
    static const std::map<std::string, Bond> bondMap = {
        {"9128283H1", Bond("9128283H1", CUSIP, "US2Y", 0.01750, from_string("2019/11/30"))},
        {"9128283L2", Bond("9128283L2", CUSIP, "US3Y", 0.01875, from_string("2020/12/15"))},
        {"912828M80", Bond("912828M80", CUSIP, "US5Y", 0.02000, from_string("2022/11/30"))},
        {"9128283J7", Bond("9128283J7", CUSIP, "US7Y", 0.02125, from_string("2024/11/30"))},
        {"9128283F5", Bond("9128283F5", CUSIP, "US10Y", 0.02250, from_string("2027/12/15"))},
        {"912810RZ3", Bond("912810RZ3", CUSIP, "US30Y", 0.02750, from_string("2047/12/15"))}
    };

    auto it = bondMap.find(cusip);
    return (it != bondMap.end()) ? it->second : Bond();
}

// Get PV01 value for US Treasury bonds based on CUSIP.
double GetPV01Value(const std::string& cusip)
{
    static const std::map<std::string, double> pv01Map = {
        {"9128283H1", 0.01948992},
        {"9128283L2", 0.02865304},
        {"912828M80", 0.04581119},
        {"9128283J7", 0.06127718},
        {"9128283F5", 0.08161449},
        {"912810RZ3", 0.15013155}
    };

    auto it = pv01Map.find(cusip);
    return (it != pv01Map.end()) ? it->second : 0.0;
}

// Convert fractional price to numerical price.
double ConvertPrice(const std::string& fractionalPrice)
{
    size_t hyphenPos = fractionalPrice.find('-');
    size_t plusPos = fractionalPrice.find('+');

    std::string hundredPart = fractionalPrice.substr(0, hyphenPos);
    std::string thirtySecondPart = fractionalPrice.substr(hyphenPos + 1, 2);
    std::string eighthPart = (plusPos != std::string::npos) ? "4" : fractionalPrice.substr(hyphenPos + 3, 1);

    double hundred = std::stod(hundredPart);
    double thirtySecond = std::stod(thirtySecondPart);
    double eighth = std::stod(eighthPart);

    return hundred + thirtySecond / 32.0 + eighth / 256.0;
}

// Convert numerical price to fractional price.
std::string ConvertPrice(double numericalPrice)
{
    int hundredPart = static_cast<int>(numericalPrice);
    int totalThirtySeconds = static_cast<int>((numericalPrice - hundredPart) * 256);
    int thirtySecondPart = totalThirtySeconds / 8;
    int eighthPart = totalThirtySeconds % 8;

    std::string fractionalPrice = std::to_string(hundredPart) + "-" +
        (thirtySecondPart < 10 ? "0" : "") + std::to_string(thirtySecondPart);

    fractionalPrice += (eighthPart == 4) ? "+" : std::to_string(eighthPart);
    return fractionalPrice;
}

// Output Time Stamp with millisecond precision.
std::string TimeStamp()
{
    auto now = std::chrono::system_clock::now();
    auto timeT = std::chrono::system_clock::to_time_t(now);
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    char timeBuffer[24];
    strftime(timeBuffer, 24, "%F %T", std::localtime(&timeT));

    return std::string(timeBuffer) + "." + std::to_string(milliseconds.count());
}

// Get the millisecond count of current time.
long GetMillisecond()
{
    auto now = std::chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

// Generate random IDs.
std::string GenerateId()
{
    static const std::string characters = "1234567890QWERTYUIOPASDFERMDJGKCVBNM";
    std::string id;

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<size_t> distribution(0, characters.size() - 1);

    for (size_t i = 0; i < 12; ++i)
    {
        id += characters[distribution(generator)];
    }

    return id;
}

#endif // UTILITIES_HPP
