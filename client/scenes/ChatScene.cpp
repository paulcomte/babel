#include "ChatScene.hpp"
#include "../ClientError.hpp"
#include <iostream>

using namespace babel;

ChatScene::ChatScene(std::shared_ptr<ClientManager> clientManager) {
    if (clientManager == nullptr)
        throw ClientError("Whilst initializing ContactScene: ClientManager cannot be null !");
    this->_clientManager = clientManager;
    this->_initWidgets();
    this->_initLayouts();
    this->_placeWidgets();
    this->clear();
}

ChatScene::~ChatScene() {
    this->_messages.clear();
    this->_messagesLayout.reset();
    this->_scrollArea.reset();
    this->_parent->setParent(nullptr);
    this->_parent.reset();
}

std::string ChatScene::getName() {
    return "Chat menu";
}

void ChatScene::display() {
    this->_parent->show();
    this->_scrollArea->show();
}

void ChatScene::clear() {
    this->_parent->hide();
}

// Refresh the MainScene
// Mainly used when window size changed by example
// Or any variable that might have been shown on screen was updated.
void ChatScene::refresh() {
    this->_parent->repaint();
    // TODO refresh clients
}

std::shared_ptr<SceneManager> ChatScene::getSceneManager() {
    return (this->_clientManager->sceneManager);
}

std::shared_ptr<QWidget> ChatScene::getWidget() {
    return (this->_scrollArea);
}

void ChatScene::_initLayouts() {
    this->_messagesLayout = std::shared_ptr<QVBoxLayout>(new QVBoxLayout());
}

void ChatScene::_initWidgets() {
    this->_scrollArea = std::shared_ptr<QScrollArea>(new QScrollArea());
    this->_parent = std::shared_ptr<QWidget>(new QWidget(this->_scrollArea.get()));

    auto groupedMessages = this->_groupMessagesByTime(this->_clientManager->self, this->_clientManager->getChatting());

    for (auto messages: groupedMessages) {
        std::shared_ptr<Client> client = messages[0]->getAuthor();
        std::shared_ptr<MessageBox> message(new MessageBox(client, messages));
        this->_messages.push_back(message);
    }
}

void ChatScene::_placeWidgets() {
    this->_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->_scrollArea->setWidget(this->_parent.get());
    this->_scrollArea->setWidgetResizable(true);
    this->_parent->setFixedWidth(700);
    this->_scrollArea->setFixedHeight(555);
    this->_messagesLayout->setSpacing(10);

    for (std::shared_ptr<MessageBox> message: this->_messages) {
        this->_messagesLayout->addLayout(message->getLayout().get());
    }
    this->_scrollArea->setLayout(this->_messagesLayout.get());
}

std::vector<std::vector<std::shared_ptr<Message>>> ChatScene::_groupMessagesByTime(std::shared_ptr<Client> client1, std::shared_ptr<Client> client2) {
    // combine the messages from both clients into a single vector
    std::vector<std::shared_ptr<Message>> allMessages = {};
    allMessages.reserve(client1->getMessages().size() + client2->getMessages().size());
    for (const auto message: client1->getMessages())
        allMessages.emplace_back(message);
    for (const auto message: client2->getMessages())
        allMessages.emplace_back(message);

    // sort the messages by timestamp
    std::sort(allMessages.begin(), allMessages.end(),
              [](const std::shared_ptr<Message> a, const std::shared_ptr<Message> b) { return a->getTimestamp() < b->getTimestamp(); });

    // group the messages by time
    std::vector<std::vector<std::shared_ptr<Message>>> messages = {};
    std::shared_ptr<Message> lastMessage = nullptr;
    for (const auto& message : allMessages) {
        if (messages.empty() || message->getAuthor() != lastMessage->getAuthor() || message->getTimestamp() - lastMessage->getTimestamp() > 3) {
            // create a new vector if this is the first message or if there's a time gap
            messages.emplace_back();
        }
        lastMessage = message;
        messages.back().push_back(message);
    }

    return (messages);
}