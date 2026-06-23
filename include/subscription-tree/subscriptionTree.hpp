#ifndef __SRC_SUBSCRIPTION_TREE_HPP__
#define __SRC_SUBSCRIPTION_TREE_HPP__

#include <memory>
#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class SubscriptionTree {
protected:
    std::unordered_map<std::string, std::unordered_set<std::string>> m_clientsToEndpoints;
    std::unordered_map<std::string, std::unordered_set<std::string>> m_endpointsToClients;
    
    std::unordered_map<std::string, std::vector<std::string>> m_topicLevelsMap;

    bool validateTopicLevel(const std::string& topicLevel);
    bool validateEndpoint(const std::string& endpoint, std::vector<std::string>& topicLevels);
    bool isMatched(const std::vector<std::string>& endpoint, size_t endpointIndex,
                   const std::vector<std::string>& target, size_t targetIndex);
    
public:
    explicit SubscriptionTree();
    bool addSubscription(const std::string& clientID, const std::string& endpoint);
    bool removeSubscription(const std::string& clientID, const std::string& endpoint);
    std::vector<std::string> getClientSubscriptions(const std::string& clientID);
    std::vector<std::string> getSubscribedClients(const std::string& endpoint);
    size_t countTreeNode();
};
#endif
