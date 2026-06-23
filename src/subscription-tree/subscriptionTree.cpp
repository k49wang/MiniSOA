#include <iostream>
#include <sstream>
#include <cctype>
#include <stdio.h>
#include <iostream>
#include <subscription-tree/subscriptionTree.hpp>


SubscriptionTree::SubscriptionTree() { }

bool SubscriptionTree::validateTopicLevel(const std::string& topicLevel) {
    int len = topicLevel.length();

    int countWildCard = 0;
    for (const char& ch : topicLevel) {
        if (ch == '+' || ch == '#') {
            countWildCard += 1;
        } else if (!std::isalnum(ch)) {
            return false;
        }
    }
    if (countWildCard > 0) return len == 1;
    return true;
}

bool SubscriptionTree::validateEndpoint(const std::string& endpoint, std::vector<std::string>& topicLevels) {
    if (endpoint.empty() || endpoint[0] == '/') return false;
    std::istringstream iss{endpoint};
    std::string topic;
    const char sep = '/';
    size_t countEmpty = 0;
    size_t countWildCard = 0;
    size_t countNonEmpty = 0;
    while (std::getline(iss, topic, sep)) {
        if (!validateTopicLevel(topic)) return false;
        if (topic.empty()) countEmpty += 1;
        if (topic[0] == '+' || topic[0] == '#') countWildCard += 1;
        if (!topic.empty() && topic[0] != '+' && topic[0] != '#') countNonEmpty += 1;
        topicLevels.emplace_back(topic);
    }
    if (countEmpty == topicLevels.size() || countWildCard == topicLevels.size()) return false;
    if (countNonEmpty == 0) return false;
    if (endpoint[endpoint.size() - 1] == '/') topicLevels.emplace_back("");
    return true;
}

bool SubscriptionTree::addSubscription(const std::string& clientID, const std::string& endpoint) {
    std::vector<std::string> topicLevels;
    if (m_topicLevelsMap.find(endpoint) == m_topicLevelsMap.end()) {
        if (!validateEndpoint(endpoint, topicLevels)) return false;
        m_topicLevelsMap[endpoint] = topicLevels;
    } else {
        topicLevels = m_topicLevelsMap[endpoint];
    }
    if (m_clientsToEndpoints.find(clientID) != m_clientsToEndpoints.end() &&
        m_clientsToEndpoints[clientID].find(endpoint) != m_clientsToEndpoints[clientID].end()) {
        return false;
    }
    if (m_endpointsToClients.find(endpoint) != m_endpointsToClients.end() &&
        m_endpointsToClients[endpoint].find(clientID) != m_endpointsToClients[endpoint].end()) {
        return false;
    }
    m_clientsToEndpoints[clientID].emplace(endpoint);
    m_endpointsToClients[endpoint].emplace(clientID);
    return true;
}

bool SubscriptionTree::removeSubscription(const std::string& clientID, const std::string& endpoint) {
    std::vector<std::string> topicLevels;
    if (m_topicLevelsMap.find(endpoint) == m_topicLevelsMap.end()) {
        if (!validateEndpoint(endpoint, topicLevels)) return false;
        m_topicLevelsMap[endpoint] = topicLevels;
    } else {
        topicLevels = m_topicLevelsMap[endpoint];
    }

    if (m_clientsToEndpoints.find(clientID) == m_clientsToEndpoints.end() &&
        m_clientsToEndpoints[clientID].find(endpoint) == m_clientsToEndpoints[clientID].end()) {
        return false;
    }
    if (m_endpointsToClients.find(endpoint) == m_endpointsToClients.end() &&
        m_endpointsToClients[endpoint].find(clientID) == m_endpointsToClients[endpoint].end()) {
        return false;
    }
    m_clientsToEndpoints[clientID].erase(endpoint);
    if (m_clientsToEndpoints[clientID].empty()) m_clientsToEndpoints.erase(clientID);
    m_endpointsToClients[endpoint].erase(clientID);
    if (m_endpointsToClients[endpoint].empty()) m_endpointsToClients.erase(endpoint);
    return true;
}

std::vector<std::string> SubscriptionTree::getClientSubscriptions(const std::string& clientID) {
    std::vector<std::string> subscriptions;
    if (m_clientsToEndpoints.find(clientID) != m_clientsToEndpoints.end()) {
        for (const auto& subscription : m_clientsToEndpoints[clientID]) {
            subscriptions.emplace_back(subscription);
        }
    }
    return subscriptions;
}

bool SubscriptionTree::isMatched(const std::vector<std::string>& endpoint, size_t endpointIndex,
                                 const std::vector<std::string>& target, size_t targetIndex) {
    if (endpointIndex == endpoint.size() && targetIndex == target.size()) return true;
    if (endpointIndex >= endpoint.size() || targetIndex >= target.size()) return false;
    if (endpoint[endpointIndex] == "+") {
        return isMatched(endpoint, endpointIndex + 1, target, targetIndex + 1);
    } else if (endpoint[endpointIndex] == "#") {
        if (target.size() - targetIndex < 2) return false;
        for (size_t i = targetIndex + 2; i <= target.size(); i++) {
            if (isMatched(endpoint, endpointIndex + 1, target, i)) return true;
        }
        return false;
    } else if (endpoint[endpointIndex] != target[targetIndex]) {
        return false;
    } else {
        return isMatched(endpoint, endpointIndex + 1, target, targetIndex + 1);
    }
}

std::vector<std::string> SubscriptionTree::getSubscribedClients(const std::string& endpoint) {
    std::vector<std::string> clients;
    std::vector<std::string> topicLevels;
    if (!validateEndpoint(endpoint, topicLevels)) return clients;
    for (const auto& topicLevel : topicLevels) {
        if (topicLevel == "+" || topicLevel == "#") return clients;
    }

    std::vector<std::string> matchedEndpoints;
    for (const auto& item : m_topicLevelsMap) {
        const auto& curr = item.second;
        if (isMatched(curr, 0, topicLevels, 0)) {
            matchedEndpoints.emplace_back(item.first);
        }
    }
    std::unordered_set<std::string> temp;
    for (auto& matched : matchedEndpoints) {
        if (m_endpointsToClients.find(matched) != m_endpointsToClients.end()) {
            for (auto& client : m_endpointsToClients[matched]) {
                temp.emplace(client);
            }
        }
    }
    for (auto& client : temp) clients.emplace_back(client);
    return clients;
}

size_t SubscriptionTree::countTreeNode() {
    return m_endpointsToClients.size();
}
