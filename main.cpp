#include <iostream>
#include <string>
#include <map>
#include <fstream>

// Include necessary headers for the services and products
#include "soa.hpp"
#include "products.hpp"
#include "bondalgoexecutionservice.hpp"
#include "bondalgostreamingservice.hpp"
#include "executionservice.hpp"
#include "guiservice.hpp"
#include "historicaldataservice.hpp"
#include "inquiryservice.hpp"
#include "marketdataservice.hpp"
#include "positionservice.hpp"
#include "pricingservice.hpp"
#include "riskservice.hpp"
#include "streamingservice.hpp"
#include "tradebookingservice.hpp"

int main()
{
    // Log the start of the program
    std::cout << TimeStamp() << " Initializing the trading system..." << std::endl;

    // Log the initialization of services
    std::cout << TimeStamp() << " Initializing all services..." << std::endl;

    // Initialize all required services
    PricingService<Bond> bondPricingService;
    TradeBookingService<Bond> bondTradeBookingService;
    PositionService<Bond> bondPositionService;
    RiskService<Bond> bondRiskService;
    MarketDataService<Bond> bondMarketDataService;
    AlgoExecutionService<Bond> bondAlgoExecutionService;
    AlgoStreamingService<Bond> bondAlgoStreamingService;
    GUIService<Bond> bondGuiService;
    ExecutionService<Bond> bondExecutionService;
    StreamingService<Bond> bondStreamingService;
    InquiryService<Bond> bondInquiryService;

    // Initialize historical data services
    HistoricalDataService<Position<Bond>> bondHistoricalPositionService(POSITION);
    HistoricalDataService<PV01<Bond>> bondHistoricalRiskService(RISK);
    HistoricalDataService<ExecutionOrder<Bond>> bondHistoricalExecutionService(EXECUTION);
    HistoricalDataService<PriceStream<Bond>> bondHistoricalStreamingService(STREAMING);
    HistoricalDataService<Inquiry<Bond>> bondHistoricalInquiryService(INQUIRY);

    std::cout << TimeStamp() << " All services successfully initialized." << std::endl;

    // Link services using ServiceListeners
    std::cout << TimeStamp() << " Linking all services..." << std::endl;

    bondPricingService.AddListener(bondAlgoStreamingService.GetListener());
    bondPricingService.AddListener(bondGuiService.GetListener());
    bondAlgoStreamingService.AddListener(bondStreamingService.GetListener());
    bondStreamingService.AddListener(bondHistoricalStreamingService.GetListener());
    bondMarketDataService.AddListener(bondAlgoExecutionService.GetListener());
    bondAlgoExecutionService.AddListener(bondExecutionService.GetListener());
    bondExecutionService.AddListener(bondTradeBookingService.GetListener());
    bondExecutionService.AddListener(bondHistoricalExecutionService.GetListener());
    bondTradeBookingService.AddListener(bondPositionService.GetListener());
    bondPositionService.AddListener(bondRiskService.GetListener());
    bondPositionService.AddListener(bondHistoricalPositionService.GetListener());
    bondRiskService.AddListener(bondHistoricalRiskService.GetListener());
    bondInquiryService.AddListener(bondHistoricalInquiryService.GetListener());

    std::cout << TimeStamp() << " All services successfully linked." << std::endl;

    // Process pricing data
    std::cout << TimeStamp() << " Processing bond price data..." << std::endl;
    std::ifstream priceDataFile("prices.txt");
    bondPricingService.GetConnector()->Subscribe(priceDataFile);
    std::cout << TimeStamp() << " Bond price data processed successfully." << std::endl;

    // Process trade data
    std::cout << TimeStamp() << " Processing bond trade data..." << std::endl;
    std::ifstream tradeDataFile("trades.txt");
    bondTradeBookingService.GetConnector()->Subscribe(tradeDataFile);
    std::cout << TimeStamp() << " Bond trade data processed successfully." << std::endl;

    // Process market data
    std::cout << TimeStamp() << " Processing bond market data..." << std::endl;
    std::ifstream marketDataFile("marketdata.txt");
    bondMarketDataService.GetConnector()->Subscribe(marketDataFile);
    std::cout << TimeStamp() << " Bond market data processed successfully." << std::endl;

    // Process inquiry data
    std::cout << TimeStamp() << " Processing bond inquiry data..." << std::endl;
    std::ifstream inquiryDataFile("inquiries.txt");
    bondInquiryService.GetConnector()->Subscribe(inquiryDataFile);
    std::cout << TimeStamp() << " Bond inquiry data processed successfully." << std::endl;

    // Log the end of the program
    std::cout << TimeStamp() << " Shutting down the trading system..." << std::endl;
    std::cout << TimeStamp() << " Trading system shut down successfully." << std::endl;

    // Pause the system (Windows-specific)
    system("pause");
    return 0;
}
