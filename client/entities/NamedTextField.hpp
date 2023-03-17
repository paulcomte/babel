#pragma once

#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QWidget>

namespace babel {
    class NamedTextField {
        public:
            NamedTextField(std::string name, std::shared_ptr<QWidget> parent);
            NamedTextField(std::string name, std::shared_ptr<QWidget> parent, double modifier);
            ~NamedTextField();

            QHBoxLayout *getLayout();

        private:
            std::shared_ptr<QWidget> _parent = nullptr;
            std::unique_ptr<QLineEdit> _input = nullptr;
            std::unique_ptr<QLabel> _label = nullptr;
            std::unique_ptr<QHBoxLayout> _layout = nullptr;
    };
}