#ifndef __SRC_TREE_WRAPPER_HPP__
#define __SRC_TREE_WRAPPER_HPP__

#include <string>
#include <vector>
#include <memory>

class SubscriptionTree;

class TreeWrapper {
    std::shared_ptr<SubscriptionTree> m_tree;

public:
    explicit TreeWrapper();

    bool addWrapper(const std::string& clientID, const std::string& endpoint);
    bool removeWrapper(const std::string& clientID, const std::string& endpoint);

    std::vector<std::string> getSubscriptionsWrapper(const std::string& clientID);
    std::vector<std::string> getClientsWrapper(const std::string& endpoint);

    size_t getNodeCountWrapper();
};

#endif
