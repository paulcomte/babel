#include "LoggingScene.hpp"
#include "../ClientError.hpp"
#include "../utils.hpp"
#include "MainScene.hpp"

#include "../network/Transporter.hpp"
#include "../network/QtSocket.hpp"
#include "../../protocol/packets/LoginPacket.hpp"

#include <QLineEdit>
#include <iostream>

using namespace babel;

LoggingScene::LoggingScene(std::shared_ptr<ClientManager> clientManager) {
    if (clientManager == nullptr)
        throw ClientError("Whilst initializing LoggingScene: ClientManager cannot be null !");
    this->_clientManager = clientManager;
    this->_initLayouts();
    this->_initWidgets();
    this->_placeWidgets();
    this->clear();
}

LoggingScene::~LoggingScene() {
    this->_loggingButton.reset();
    this->_usernameField.reset();
    this->_serverField.reset();
    this->_portField.reset();
    this->_errorMessage.reset();
    this->_topLayout.reset();
    this->_layout.reset();
    this->_widget.reset();
}

std::string LoggingScene::getName() {
    return "Logging menu";
}

void LoggingScene::display() {
    this->_widget->show();
}

void LoggingScene::clear() {
    this->_widget->hide();
}

// Refresh the MainScene
// Mainly used when window size changed by example
// Or any variable that might have been shown on screen was updated.
void LoggingScene::refresh() {
}

std::shared_ptr<SceneManager> LoggingScene::getSceneManager() {
    return (this->_clientManager->sceneManager);
}

void LoggingScene::_loggingButtonClicked() {
    if (this->_loggingAction) {
        return;
    }

    this->_loggingAction = true;
    std::string hostname = this->_serverField->getValue();
    unsigned int port = std::stoi(this->_portField->getValue());

    this->_clientManager->transporter = std::make_shared<Transporter>(this->_clientManager->eventManager, std::unique_ptr<Socket>(new QtSocket(hostname, port)));

    if (this->_clientManager->transporter->awaitingConnection()) {
        LoginPacket packet(this->_usernameField->getValue());
        this->_clientManager->transporter->sendMessage(packet.serialize());
    } else {
        std::cerr << "[DEBUG] Could not connect to server" << std::endl;
        // this->getSceneManager()->setScene(new MainScene(this->_clientManager));
        // TODO Connection packet to server
    }
}

void LoggingScene::_initLayouts() {
    this->_layout = std::unique_ptr<QBoxLayout>(new QVBoxLayout());
    this->_topLayout = std::unique_ptr<QBoxLayout>(new QHBoxLayout());
}

void LoggingScene::_initWidgets() {
    this->_widget = std::shared_ptr<QWidget>(new QWidget(this->getSceneManager()->getWidget().get()));

    this->_loggingButton = std::unique_ptr<QPushButton>(new QPushButton("Login"));
    this->_loggingButton->setGeometry(0, 0, 200, 100);
   
    this->_usernameField = std::unique_ptr<NamedTextField>(new NamedTextField("Username", this->_widget));
    this->_serverField = std::unique_ptr<NamedTextField>(new NamedTextField("Server", this->_widget));
    this->_serverField->setValue("127.0.0.1");
    this->_portField = std::unique_ptr<NamedTextField>(new NamedTextField("Port", this->_widget, 0.7));
    this->_portField->setValue("8080");
}

void LoggingScene::_placeWidgets() {
    this->_topLayout->addLayout(this->_usernameField->getLayout());
    this->_topLayout->addLayout(this->_serverField->getLayout());
    this->_topLayout->addLayout(this->_portField->getLayout());
    this->_layout->addLayout(this->_topLayout.get());
    this->_layout->addWidget(this->_loggingButton.get());

    // Set the layout for the parent widget
    this->_widget->setLayout(this->_layout.get());
    // Set the size of the widget and move it to the center of the parent
    this->_widget->setFixedSize(this->_widget->sizeHint());
    this->_widget->move((this->getSceneManager()->getWidget()->width() - this->_widget->width()) / 2, (this->getSceneManager()->getWidget()->height() - this->_widget->height()) / 2);

    QObject::connect(this->_loggingButton.get(), &QPushButton::clicked, [=]() {
       this->_loggingButtonClicked(); 
    });
}
