#include <subscription-tree/subscriptionTree.hpp>
#include "subscription-tree/treeWrapper.hpp"

TreeWrapper::TreeWrapper():
    m_tree(std::make_shared<SubscriptionTree>()) { }

bool TreeWrapper::addWrapper(const std::string& clientID, const std::string& endpoint) {
    return m_tree->addSubscription(clientID, endpoint);
}

bool TreeWrapper::removeWrapper(const std::string& clientID, const std::string& endpoint) {
    return m_tree->removeSubscription(clientID, endpoint);
}

std::vector<std::string> TreeWrapper::getSubscriptionsWrapper(const std::string& clientID) {
    return m_tree->getClientSubscriptions(clientID);
}

std::vector<std::string> TreeWrapper::getClientsWrapper(const std::string& endpoint) {
    return m_tree->getSubscribedClients(endpoint);
}

size_t TreeWrapper::getNodeCountWrapper() {
    return m_tree->countTreeNode();
}
